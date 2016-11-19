//----------------------------------------
// GR-CITRUS (Akizuki RX631) CPU board
// CPU chip:
//----------------------------------------
//
// Kazuyuki Hirooka
//
// 201611
//
#include "iodefine.h"
#include "hwsetup_rx631.h"
#include "dac.h"

//======================================================================
// DAC  10bit, ch1
//======================================================================
// There is no DAC-ch0 on 100pin-RX631.
void DAC_init(void)
{
    //------------------------------------------------------------
    // Release DAC module to run
    //------------------------------------------------------------
    SYSTEM.PRCR.WORD    = (PRCR_PRKEY<<8)+0x0B; // Release register protect
        MSTP_DA         = 0;                    // 0: release, 1: stop
    SYSTEM.PRCR.WORD    = (PRCR_PRKEY<<8)+0x00; // Protect register

    //------------------------------------------------------------
    // Set Pin for DAC1 than Port0-5
    //------------------------------------------------------------
    // Caution: The port should have been set as input.
    //---
    // Release protect
    MPC.PWPR.BIT.B0WI   = 0;    // Release PFSWE-bit protect
    MPC.PWPR.BIT.PFSWE  = 1;
        // Set pin
        MPC.P05PFS.BIT.PSEL =   0;      // Reserved
        MPC.P05PFS.BIT.ISEL =   0;      // Reserved
        MPC.P05PFS.BIT.ASEL =   1;      // Use as an analog pin, DAC1.
    // Protect
    MPC.PWPR.BIT.PFSWE  = 0;
    MPC.PWPR.BIT.B0WI   = 1;    // Protect PFSWE-bit

    //------------------------------------------------------------
    // DAC Data Position Register
    //------------------------------------------------------------
    DA.DADPR.BIT.DPSEL  = 0;    // Right aligned

    //------------------------------------------------------------
    // DAC Control Register
    //------------------------------------------------------------
    DA.DACR.BIT.DAE     = 0;    // Control ch0, 1 separately
    DA.DACR.BIT.DAOE0   = 0;    // Output enable ch0: disable
    DA.DACR.BIT.DAOE1   = 1;    // Output enable ch1: ENABLE

    //------------------------------------------------------------
    // DAADSCR: DA/AD Synchronization Control Register
    //------------------------------------------------------------
    DA.DAADSCR.BIT.DAADST   = 0;    // DAC not synchronous to ADC

    //------------------------------------------------------------
    // DADR: DA Data Register
    //------------------------------------------------------------
    DA.DADR0    = 0;    // ch0
    DA.DADR1    = 0;    // ch1
}
