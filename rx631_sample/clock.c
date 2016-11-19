//================================================================================
// GR-CITRUS (Akizuki RX631) CPU board
// CPU: RX631
//================================================================================
// Clocks:
//  MAIN-XTAL:  fMAIN                   =  12MHz
//  PLL VCO:    fPLL    = fMAIN * 16    = 192MHz
//  ICLK:       fICLK   = fPLL / 2      =  96MHz
//  PCLKA, B:   fPCLK   = fPLL / 4      =  48MHz
//================================================================================
//
// Kazuyuki Hirooka
//
// 20161105
//
#include "iodefine.h"
#include "hwsetup_rx631.h"
#include "clock.h"

void CLOCK_init(void)
{
    //--------------------------------------------------------------------------------
    // Register names
    //--------------------------------------------------------------------------------
    // BCKCR        External bus clock control register
    // MOSCCR       Main clock oscillator control register
    // PLLCR/2      PLL control register, 2nd
    // PRCR         Protect control register
    // OPCCR        Operating Power Consumption control register
    // SCKCR/2/3    System clock control register, 2nd, 3rd
    // MOFCR        Main clock oscillator force control register
    //--------------------------------------------------------------------------------
    // LOCO         Low -speed On-Chip Oscilator
    // HOCO         High-speed On-Chip Oscilator
    //--------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------
    // It should be running on LOCO(125kHz), LOw-Clock-frequency-Oscilator.
    //--------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------
    // Release register protect
    //--------------------------------------------------------------------------------
    SYSTEM.PRCR.WORD                = (PRCR_PRKEY<<8)+0x0B; // Write-enable all

    //--------------------------------------------------------------------------------
    // Set the P36 and P37 pins for Main Xtal by force
    //--------------------------------------------------------------------------------
	SYSTEM.MOFCR.BYTE               =   0x01;   // Force Main OSC XTAL
    while (SYSTEM.MOFCR.BYTE!=0x01);            // Write verification

    //--------------------------------------------------------------------------------
    // Stop all clocks except LOCO.
    //--------------------------------------------------------------------------------
	SYSTEM.MOSCCR.BYTE              =   0x01;   // Stop Main clock
    while (SYSTEM.MOSCCR.BYTE!=0x01);           // Write verification
    SYSTEM.PLLCR2.BYTE              =   0x01;   // Stop PLL
	SYSTEM.SOSCCR.BYTE              =   0x01;   // Stop Sub clock
    while (SYSTEM.SOSCCR.BYTE!=0x01);           // Write verification
	SYSTEM.ILOCOCR.BYTE             =   0x01;   // Stop Independant Watch Dog Timer clock
	SYSTEM.HOCOCR.BYTE              =   0x01;   // Stop HOCO
	SYSTEM.HOCOPCR.BYTE             =   0x01;   // Power off HOCO

    //--------------------------------------------------------------------------------
    // Operating Power Consumption mode: Fast mode
    //--------------------------------------------------------------------------------
    SYSTEM.OPCCR.BYTE           = 0x00; // Fast mode to allow PLL operation and maximum ICLK(100MHz).

    //--------------------------------------------------------------------------------
    // Set clock dividers, stop some clocks.
    //--------------------------------------------------------------------------------
    // SYSTEM.SCKCR.BIT.FCK         =   2;  // Flash IF clock:      1/4
    // SYSTEM.SCKCR.BIT.ICK         =   1;  // System clock:        1/2
    // SYSTEM.SCKCR.BIT.PSTOP1      =   1;  // Stop BCLK
    // SYSTEM.SCKCR.BIT.PSTOP0      =   1;  // Stop SDCLK, Synchronus DRAM clock
    // SYSTEM.SCKCR.BIT.BCK         =   2;  // Bus clock:           1/4
    // SYSTEM.SCKCR.BIT.PCKA        =   2;  // Peripheral A clock:  1/4
    // SYSTEM.SCKCR.BIT.PCKB        =   2;  // Peripheral B clock:  1/4
    //--- LONG-wise writing seems necessary.
    SYSTEM.SCKCR.LONG               =   0x21C22211UL;       // LSB-8 must be 0x11.
    //---
    // SYSTEM.SCKCR2.BIT.UCK        =   3;  // USB clock:           1/4
    // SYSTEM.SCKCR2.BIT.IEBCK      =   3;  // IEBUS clock:         1/8
    //---
    SYSTEM.SCKCR2.WORD              =   0x0033;
    //---
	SYSTEM.BCKCR.BYTE               =   0x01;   // BCLK output pin freq = BCLK * 1/2

    //--------------------------------------------------------------------------------
    // Set PLL dividers
    //--------------------------------------------------------------------------------
    // PLL input freq       = 12MHz by main Xtal.   Range must be:    4 --  16MHz
    // PLL output(VCO) freq = 192MHz = 12 * 16      Range must be:  104 -- 200MHz
    //---
	// SYSTEM.PLLCR.BIT.STC         =   0x0F;   // Multiplication:  x16
	// SYSTEM.PLLCR.BIT.PLIDIV      =   0x00;   // Input divider:   1/1
    //---
	SYSTEM.PLLCR.WORD               =   0x0F00;

    //--------------------------------------------------------------------------------
    // PLL Wait Control register
    //--------------------------------------------------------------------------------
    SYSTEM.PLLWTCR.BYTE             =   0x0B;   // 1010b, as a typical example

    //--------------------------------------------------------------------------------
    // MAIN Wait Control register
    //--------------------------------------------------------------------------------
    SYSTEM.MOSCWTCR.BYTE            =   0x0D;   // 1101b, as a typical example

    //--------------------------------------------------------------------------------
    // Start main clock
    //--------------------------------------------------------------------------------
	SYSTEM.MOSCCR.BYTE              =   0x00;   // Start Main clock
    while (SYSTEM.MOSCCR.BYTE!=0x00);           // Write verification
    // Wait for the main clock to settle.
    PORT_LED1 = 1;
    for (volatile int i=0;i<700;i++);   // Measured: 110ms at 125kHz.
    PORT_LED1 = 0;

    //--------------------------------------------------------------------------------
    // Start PLL, whose input is main clock.
    //--------------------------------------------------------------------------------
    SYSTEM.PLLCR2.BYTE              =   0x00;   // Start PLL
    //  Wait for the PLL to settle.
    PORT_LED1 = 1;
    for (volatile int i=0;i<700;i++);   // Measured: 110ms at 125kHz.
    PORT_LED1 = 0;

    //--------------------------------------------------------------------------------
    // Switch clock source to PLL, resulting in ICLK=96MHz.
    //--------------------------------------------------------------------------------
    SYSTEM.SCKCR3.WORD              =   0x0400; // clock source select: PLL
                                                // 0x0000;  LOCO
                                                // 0x0100;  HOCO
                                                // 0x0200;  MAIN
                                                // 0x0400;  PLL

    //--------------------------------------------------------------------------------
    // Stop LOCO
    //--------------------------------------------------------------------------------
	SYSTEM.LOCOCR.BYTE              =   0x01;   // Stop LOCO

    //--------------------------------------------------------------------------------
    // Protect register
    //--------------------------------------------------------------------------------
    SYSTEM.PRCR.WORD                = (PRCR_PRKEY<<8)+0x00; // Write-protect all
}
