//----------------------------------------
// GR-CITRUS (Akizuki RX631) CPU board
// CPU: RX631
//----------------------------------------
//
// Kazuyuki Hirooka
//
// 20161105
//
#include "iodefine.h"
#include "hwsetup_rx631.h"
#include "tpu.h"

//============================================================
// TPUa init
//  PCLK: 48MHz at GR-CITRUS(AKI-RX631)
//    (differ from 20MHz at AKI-RX220)
//============================================================
// ch0:     for 1ms-interrupt
//  input:  PCLK=48MHz xtal clock
//  output: none
//---
// ch3:     for elapsed time measurement
//  input:  PCLK=48MHz xtal clock
//          Prescale=1/1024 ==> T=21.3us
//  output: none
//------------------------------------------------------------
void TPU_init(void)
{
    //------------------------------------------------------------
    // Release module to run
    //------------------------------------------------------------
    SYSTEM.PRCR.WORD                = (PRCR_PRKEY<<8)+0x0B; // Release register protect
        SYSTEM.MSTPCRA.BIT.MSTPA13  = 0;    // Release TPU unit 1 (TPU/0-5)
        SYSTEM.MSTPCRA.BIT.MSTPA12  = 1;    // Stop    TPU unit 2 (TPU/6-11)
    SYSTEM.PRCR.WORD                = (PRCR_PRKEY<<8)+0x00; // Protect register

    //------------------------------------------------------------
    // Stop all channels' counter   // count start  0: stop, 1: start
    //------------------------------------------------------------
    TPUA.TSTR.BYTE  = 0x00;
    TPUB.TSTR.BYTE  = 0x00;
    //------------------------------------------------------------
    // No synchronization among channels
    //------------------------------------------------------------
    TPUA.TSYR.BYTE  = 0x00;
    TPUB.TSYR.BYTE  = 0x00;

    ////////////////////////////////////////////////////////////////////////////////
    // ch0: for 1ms-period ADC
    ////////////////////////////////////////////////////////////////////////////////
    //----------------------------------------
    // Timer Control Register
    //----------------------------------------
    TPU0.TCR.BIT.TPSC = 0;          // Timer count clock / Prescaler select:
                                    //   Internal clock 0:*PCLK/1, 1: PCLK/4, 2: PCLK/16, 3: PCLK/64,
                                    //   External clock 4: TCLKA,  5: TCLKB,  6: TCLKC,   7: TCLKD
    TPU0.TCR.BIT.CKEG = 0;          // Clock edge by:    0:*rising edge, 1: falling edge, 2, 3: both edges
    TPU0.TCR.BIT.CCLR = 1;          // Counter clear by: 1: TGRA compare match
    //----------------------------------------
    // Timer Mode Register
    //----------------------------------------
    TPU0.TMDR.BIT.MD        = 0;    // Mode of operation: 0:*normal, 2: PWM1, etc.
    TPU0.TMDR.BIT.BFA       = 0;    // Mode for TGRA: 0:*normal, 1: TGRC is a buffer of TGRA
    TPU0.TMDR.BIT.BFB       = 0;    // Mode for TGRB: 0:*normal, 1: TGRD is a buffer of TGRB
    TPU0.TMDR.BIT.ICSELB    = 0;    // TGRB Input capture input select
    TPU0.TMDR.BIT.ICSELD    = 0;    // TGRD Input capture input select
    //----------------------------------------
    // Timer I/O Control Register
    //----------------------------------------
    // High
    TPU0.TIORH.BIT.IOA = 0;         // 0: Prohibit ouput from TIOC0B
                                    // 2: Output from TIOC0A: Low at initial, High on compare match
    TPU0.TIORH.BIT.IOB = 0;         // 0: Prohibit ouput from TIOC0B
    // Low
    TPU0.TIORL.BIT.IOC = 0;         // 0: Prohibit ouput from TIOC0C
    TPU0.TIORL.BIT.IOD = 0;         // 0: Prohibit ouput from TIOC0D
    //----------------------------------------
    // Timer Counter Register
    //----------------------------------------
    TPU0.TCNT = 0x0000;
    //----------------------------------------
    // Timer General Register
    //----------------------------------------
    // $$$ SET PERIOD
    TPU0.TGRA = (TPU0_N_PERIOD-1);  // TGRA for compare match
                                    // -1 NECESSARY, since TCNT counts {0, 1, 2, ..., (N-1)}.
    // ==> Compare-match-A can trigger ADC start.
    TPU0.TGRB = 0xFFFF;             // the same as init value
    TPU0.TGRC = 0xFFFF;             // the same as init value
    TPU0.TGRD = 0xFFFF;             // the same as init value

    //----------------------------------------
    // Timer Interrupt Enable Register
    //----------------------------------------
    TPU0.TIER.BYTE  = 0;            // disable all TPU0-related interrupt
                                    // except:
    TPU0.TIER.BIT.TGIEA = 1;        // Interrupt Enable by compare-match-A: 0: disable, 1: enable
    //----------------------------------------
    // TPU0 remains stopped here.
    //----------------------------------------

    ////////////////////////////////////////////////////////////////////////////////
    // ch1, ch2
    ////////////////////////////////////////////////////////////////////////////////
    // Blank

    ////////////////////////////////////////////////////////////////////////////////
    // ch3: for processing time measurement
    ////////////////////////////////////////////////////////////////////////////////
    //----------------------------------------
    // Timer Control Register
    //----------------------------------------
    TPU3.TCR.BIT.TPSC = 5;          // Options differ from TPU0.
                                    // Timer count clock / Prescaler select:
                                    //   Internal clock 0: PCLK/1,   1: PCLK/4, 2: PCLK/16, 3: PCLK/64,
                                    //                  5: PCLK/1024
                                    //                  6: PCLK/ 256
                                    //                  7: PCLK/4096
                                    //   External clock 4
    TPU3.TCR.BIT.CKEG = 0;          // Clock edge by:    0:*rising edge, 1: falling edge, 2, 3: both edges
    TPU3.TCR.BIT.CCLR = 0;          // Counter clear by: 0: free run
    //----------------------------------------
    // Timer Mode Register
    //----------------------------------------
    TPU3.TMDR.BIT.MD  = 0;          // Mode of operation: 0: normal, 2: PWM, 4: phase measure mode1, etc.
    TPU3.TMDR.BIT.BFA = 0;          // Mode for TGRA: 0:*normal, 1: TGRC is a buffer of TGRA
    TPU3.TMDR.BIT.BFB = 0;          // Mode for TGRB: 0:*normal, 1: TGRD is a buffer of TGRB
    //----------------------------------------
    // Timer I/O Control Register
    //----------------------------------------
    // High
    TPU3.TIORH.BIT.IOA = 0;         // 0: Prohibit ouput from TIOC0A
    TPU3.TIORH.BIT.IOB = 0;         // 0: Prohibit ouput from TIOC0B
    // Low
    TPU3.TIORL.BIT.IOC = 0;         // 0: Prohibit ouput from TIOC0C
    TPU3.TIORL.BIT.IOD = 0;         // 0: Prohibit ouput from TIOC0D
    //----------------------------------------
    // Timer Counter Register
    //----------------------------------------
    TPU3.TCNT = 0x0000;
    //----------------------------------------
    // Timer General Register
    //----------------------------------------
    TPU3.TGRA = 0xFFFF;             // the same as init value
    TPU3.TGRB = 0xFFFF;             // the same as init value
    TPU3.TGRC = 0xFFFF;             // the same as init value
    TPU3.TGRD = 0xFFFF;             // the same as init value
    //----------------------------------------
    // Timer Interrupt Enable Register
    //----------------------------------------
    TPU3.TIER.BYTE  = 0;            // disable all TPU3-related interrupt
    //   TIER2 none
    //----------------------------------------
    // TPU3 remains stopped here.
    //----------------------------------------

    ////////////////////////////////////////////////////////////////////////////////
    // ch4, 5
    ////////////////////////////////////////////////////////////////////////////////
    // Blank
}

//////////////////////////////////////////////////////////////
// TPU/ch0, 3, etc.: stop, clear, start, restart
//////////////////////////////////////////////////////////////
void TPU_stop_ch(int ch)
{
    switch(ch) {
        case 0:
            TPUA.TSTR.BIT.CST0 = 0;
            break;
        case 3:
            TPUA.TSTR.BIT.CST3 = 0;
            break;
        default:
            break;
    }
}

void TPU_start_ch(int ch)
{
    switch(ch) {
        case 0:
            TPUA.TSTR.BIT.CST0 = 1;
            break;
        case 3:
            TPUA.TSTR.BIT.CST3 = 1;
            break;
        default:
            break;
    }
}

void TPU_clear_ch(int ch)
{
    switch(ch) {
        case 0:
            TPU0.TCNT = 0x0000;
            break;
        case 3:
            TPU3.TCNT = 0x0000;
            break;
        default:
            break;
    }
}

void TPU_restart_ch(int ch)
{
    TPU_stop_ch(ch);
    TPU_clear_ch(ch);
    TPU_start_ch(ch);
}

int TPU_count_ch(int ch)
{
    int n = 0;
    switch(ch) {
        case 0:
            n = TPU0.TCNT;
            break;
        case 3:
            n = TPU3.TCNT;
            break;
        default:
            break;
    }
    return n;
}

//------------------------------------------------------------
// Check TPU/ch0 if counter half passed?
//------------------------------------------------------------
int TPU_ch0_if_half_passed(void)
{
    return (TPU0.TCNT >= TPU0_N_PERIOD_HALF);
}

//------------------------------------------------------------
// Return ch3 timer counter in second unit.
//------------------------------------------------------------
double  TPU_ch3_unit_in_sec(void)
{
    return TPU3_UNIT_SEC;
}

