//
// SCI
// RX631, GR-CITRUS
//
// 20161105
//
#include    "iodefine.h"

#include    <stdio.h>
#include    <stdarg.h>

#include    "hwsetup_rx631.h"
#include    "sci.h"

static  char    vsbuff[SCI_BUF_LEN];    /* 文字列展開用バッファ(必要なら増やす) */

//============================================================
// SCI
//============================================================
// GR-CITRUS
// +-----+-----------+
// |SCI0 | Board pin |
// +-----+-----------+
// |TXD0 | CN1-1     |
// |RXD0 | CN1-2     |
// +-----+-----------+
//------------------------------------------------------------

void SCI_init(void) {
    //------------------------------------------------------------
    // Release module to run
    //------------------------------------------------------------
    SYSTEM.PRCR.WORD    = (PRCR_PRKEY<<8)+0x0B; // Release register protect
        MSTP_SCI0       = 0;                    // 0: release, 1: stop
    SYSTEM.PRCR.WORD    = (PRCR_PRKEY<<8)+0x00; // Protect register

    //------------------------------------------------------------
    // Set Pins for SCI1 than Port2-0/1
    //------------------------------------------------------------
    // Caution: Port2-0/1 should have been set as input.
    //---
    // Release protect
    MPC.PWPR.BIT.B0WI   = 0;    // Release PFSWE-bit protect
    MPC.PWPR.BIT.PFSWE  = 1;
        // Set pins
        MPC.P20PFS.BIT.PSEL =   0x0A;   // TXD0
        MPC.P20PFS.BIT.ISEL =   0;      // Not use for IRQn input
        MPC.P21PFS.BIT.PSEL =   0x0A;   // RXD0
        MPC.P21PFS.BIT.ISEL =   0;      // Not use for IRQn input
    // Protect
    MPC.PWPR.BIT.PFSWE  = 0;
    MPC.PWPR.BIT.B0WI   = 1;    // Protect PFSWE-bit

    //------------------------------------------------------------
    // SCI0: Serial Control Register
    //------------------------------------------------------------
    SCI0.SCR.BIT.CKE    = 0;    // Clock Enable:                        use INTERNAL(*1) baud rate generator
    SCI0.SCR.BIT.TEIE   = 0;    // Transmit End Interrupt Enable:       Disable
    SCI0.SCR.BIT.MPIE   = 0;    // Multi-Processor Interrupt Enable:    Normal
    SCI0.SCR.BIT.RE     = 0;    // Receive Enable:                      Disable
    SCI0.SCR.BIT.TE     = 0;    // Transmit Enale:                      Disable     // Enable later.
    SCI0.SCR.BIT.RIE    = 0;    // Receive Interrupt Enable:            Disable
    SCI0.SCR.BIT.TIE    = 0;    // Transmit Interrupt Enable:           Disable
    //------------------------------------------------------------

    //------------------------------------------------------------
    // SCI0: Seaial Moder Register
    //------------------------------------------------------------
    SCI0.SMR.BIT.CKS    = 0;    // Clock select:        PCLK/1 clock (n=0; BRR)
    SCI0.SMR.BIT.MP     = 0;    // Multi Processor Communication:   Prohibit
    SCI0.SMR.BIT.STOP   = 0;    // Stop bit length:     1 bit
    SCI0.SMR.BIT.PM     = 0;    // Parity mode:         Even parity
    SCI0.SMR.BIT.PE     = 0;    // Parity Enable;       Paritny none
    SCI0.SMR.BIT.CHR    = 0;    // Character length:    8 bit
    SCI0.SMR.BIT.CM     = 0;    // Communication mode:  Asynchronous
    //------------------------------------------------------------

    //------------------------------------------------------------
    // SCI0: Smart Card Mode Register
    //------------------------------------------------------------
    SCI0.SCMR.BIT.SMIF  = 0;    // Smart Card Interface mode select:    Serial Communication
    SCI0.SCMR.BIT.SINV  = 0;    // Data Invert bit:     not invert, the same as TDR/RDR
    SCI0.SCMR.BIT.SDIR  = 0;    // Direction:           LSB first
    SCI0.SCMR.BIT.BCP2  = 1;    // default
    //------------------------------------------------------------

    //------------------------------------------------------------
    // SCI0: Bit Rate Register
    //------------------------------------------------------------
    SCI0.BRR = SCI_BRR_9600baud_48MHz;
    //------------------------------------------------------------

    //------------------------------------------------------------
    // SCI0: Noise filter register
    //------------------------------------------------------------
    SCI0.SNFR.BIT.NFCS  = 0;    // 0 only allowed for asynchronous serial communication.

    //------------------------------------------------------------
    // SCI0: Serial Control Register, again.
    //------------------------------------------------------------
    SCI0.SCR.BIT.TE     = 1;    // Transmit Enale:  Enable

    //------------------------------------------------------------
    // Port0: Port Mode Register
    //------------------------------------------------------------
    PORT2.PMR.BIT.B0    = 1;    // SCI0/Tx
    PORT2.PMR.BIT.B1    = 1;    // SCI0/Rx
}

//--------------------------------------------------------------------------------
void SCIput(char c)
//--------------------------------------------------------------------------------
{
    //------------------------------------------------------------
    // SSR: Serial Status Register
    // TDR: Transmit Data Register
    //------------------------------------------------------------

    while(SCI0.SSR.BIT.TEND!=1);    // Wait until transmit ends.
    SCI0.TDR = c;
    while(SCI0.SSR.BIT.TEND!=0);    // Wait until transmit starts.
}

static  int     len = 0;
//--------------------------------------------------------------------------------
// SCIprintf
//--------------------------------------------------------------------------------
void SCIprintf(char *fmt, ...)
{
int     i;
va_list arg;

    va_start(arg, fmt);
    *vsbuff = '\0';
    vsprintf(vsbuff, fmt, arg);
    va_end(arg);

    for(i=0;i<SCI_BUF_LEN;i++) {
        if(vsbuff[i] == 0) break;
        if(vsbuff[i] == '\n') { // if LF,
            SCIput('\r');       // insert CR before LF for Windows.
            len++;
        }
        SCIput(vsbuff[i]);
        len++;
    }
}

int SCIprintf_len(void)
{
    SCIprintf("SCIprintf_len=%d\n", len);
    return  len;
}

