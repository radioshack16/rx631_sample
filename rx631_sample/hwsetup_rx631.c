//----------------------------------------
// GR-CITRUS (Akizuki RX631) CPU board
// CPU chip:
// ROM 2MB
// RAM 256KB
//----------------------------------------
//
// Kazuyuki Hirooka
//
// 20161101
//
#include "iodefine.h"
#include "hwsetup_rx631.h"

/*
// ??? TO BE MODIFIED TO USE IN RX631
//
// RX220

//============================================================
// ADC
//============================================================
// Data:                12bit
// Ch count:            12
//                      4 of 16ch are not assigned for 64-pin chip.
//---
// AD Data Register:    ADDR0-ADDR15
//---
// Data Alignment:      right aligned in 16bit
// ADC start trigger:   TRG0EN(MTU0.TGRE compare match),
//                      1ms period
// Mode:                single scan
// Interrupt:           generate interrupt request S12ADI0
//                      on the end of scan
//---
// registers not used:
//  ADRD:   AD Self Diagnose Data register
//  ADOCDR: AD Internal Reference Data Register
//---
// LQFP64:
//  AN00:   pin60:  (PortXX)
//  AN01:   pin58:  (PortXX)
//  AN02:   pin57:  (PortXX)
//  AN03:   pin56:  (PortXX)
//  AN04:   pin55:  (PortXX)
//  AN05:   NA
//  AN06:   pin53:  (PortXX)
//  AN07:   NA
//  AN08:   pin51:  (PortXX)
//  AN09:   pin50:  (PortXX)
//  AN10:   pin49:  (PortXX)
//  AN11:   pin48:  (PortXX)
//  AN12:   pin47:  (PortXX)
//  AN13:   pin46:  (PortXX)
//  AN14:   NA
//  AN15:   NA
//------------------------------------------------------------
void ADC_init(void) {
    int n;

    MSTP_S12AD = 0;     // Release module stop

    //------------------------------------------------------------
    // AD Control (Scan) Register
    //------------------------------------------------------------
    S12AD.ADCSR.BIT.ADST    = 0;    // AD Start bit: 0: ADC stop, 1: ADC start
                                    // Stop while register setting.
    S12AD.ADCSR.BIT.DBLANS  = 0;    // Don't care in single scan.
    S12AD.ADCSR.BIT.GBADIE  = 0;    // Disable interrupt on the end of group B scan
    S12AD.ADCSR.BIT.DBLE    = 0;    // Not double trigger mode
    S12AD.ADCSR.BIT.EXTRG   = 0;    // Select synchronous trigger by MTU, ELC for ADC start
    S12AD.ADCSR.BIT.TRGE    = 0;    // Disable trigger for ADC start. Enable later.
    S12AD.ADCSR.BIT.ADIE    = 1;    // Enable Interrupt S12ADI0 on the end of scan
    S12AD.ADCSR.BIT.ADCS    = 0;    // Scan mode: 0:*single scan, 1: group scan, 2: continuous scan
    // S12AD.ADCSR.BIT.ADST         // AD Start bit: auto set/reset by the ADC circuit.
    //------------------------------------------------------------

    //------------------------------------------------------------
    // ADANSA: AD Analog channel select A
    //------------------------------------------------------------
    S12AD.ADANSA.WORD  = 0xffff;    //  Enable all 16ch, except:
    S12AD.ADANSA.BIT.ANSA5  = 0;    //  Disable
    S12AD.ADANSA.BIT.ANSA7  = 0;    //  Disable
    S12AD.ADANSA.BIT.ANSA14 = 0;    //  Disable
    S12AD.ADANSA.BIT.ANSA15 = 0;    //  Disable
    //------------------------------------------------------------

    //------------------------------------------------------------
    // ADANSB: AD Analog channel select B
    //------------------------------------------------------------
    S12AD.ADANSB.WORD = 0x0000;     // Not used in single scan.
    //------------------------------------------------------------

    //------------------------------------------------------------
    // ADADS: AD Add-mode channel select
    //------------------------------------------------------------
    S12AD.ADADS.WORD = 0x0000;      // Disable add-mode for all ch.
    //------------------------------------------------------------

    //------------------------------------------------------------
    // ADADC: AD Add count
    //------------------------------------------------------------
    S12AD.ADADC.BIT.ADC = 0;        // No addition.
    //------------------------------------------------------------

    //------------------------------------------------------------
    // ADCER: AD Control Extention Register
    //------------------------------------------------------------
    S12AD.ADCER.BIT.ADRFMT  = 0;    // AD Data Register Format:     right aligned
    S12AD.ADCER.BIT.ACE     = 1;    // Auto Clear Enable:           Enable
    S12AD.ADCER.BIT.DIAGVAL = 2;    // Self diagnose voltage value: (Vref x 1/2) (Not used)
    S12AD.ADCER.BIT.DIAGLD  = 1;    // Self diagnose voltage:       fixed (Not used)
    S12AD.ADCER.BIT.DIAGM   = 0;    // Self diagnose:               Disable
    //------------------------------------------------------------

    //------------------------------------------------------------
    // ADSTRGR: AD Start Trigger select Register
    //------------------------------------------------------------
    S12AD.ADSTRGR.BIT.TRSA  = 4;    // TRG0EN (MTU0/TRGE compare match)
    S12AD.ADSTRGR.BIT.TRSB  = 0;    // Group B trigger:             ADTRG0# (Not used)
    //------------------------------------------------------------

    //------------------------------------------------------------
    // ADEXICR: AD Extended Input Control Register
    //------------------------------------------------------------
    S12AD.ADEXICR.BIT.OCSAD = 0;    // Addtion mode of internal Vref:   No
    S12AD.ADEXICR.BIT.TSS   = 0;    // (Although RX220 does not have TSS bit.)
    S12AD.ADEXICR.BIT.OCS   = 0;    // AD convert internal Vref:        No  // Must be 0 for single scan mode.
    //------------------------------------------------------------

    //------------------------------------------------------------
    // ADSSTR: AD Sampling State Register
    //------------------------------------------------------------
    // Necessary:
    //  12 <= n <= 255
    //  T>0.4us  ;Sampling time T=n*(1/fclk)=n*50ns     // fclk=20MHz   ???
    //------------------------------------------------------------
    // n=255, T=12.75us     // Tentative
    //------------------------------------------------------------
    n=255;
    S12AD.ADSSTR0   = n;    // AN000
    //
    S12AD.ADSSTRL   = n;    // AN008--AN015
    S12AD.ADSSTRT   = n;    // (Although RX220 does not have Temperature sensor.)
    S12AD.ADSSTRO   = n;    // internal vref
    //
    S12AD.ADSSTR1   = n;    // AN001
    S12AD.ADSSTR2   = n;    // AN002
    S12AD.ADSSTR3   = n;    // AN003
    S12AD.ADSSTR4   = n;    // AN004
    S12AD.ADSSTR5   = n;    // AN005
    S12AD.ADSSTR6   = n;    // AN006
    S12AD.ADSSTR7   = n;    // AN007
    //------------------------------------------------------------

    //------------------------------------------------------------
    // ADDISCR: AD Disconnect Detect Control Register
    //------------------------------------------------------------
    S12AD.ADDISCR.BIT.ADNDIS = 0;   // Assist Disconnect Detect: No
    //------------------------------------------------------------

}

void ADC_trigger_enable(int trig_enable) {
    if (trig_enable==1) {
        S12AD.ADCSR.BIT.TRGE  = 1;  // Enable  trigger for ADC start
    } else {
        S12AD.ADCSR.BIT.TRGE  = 0;  // Disable trigger for ADC start
    }
}
*/
