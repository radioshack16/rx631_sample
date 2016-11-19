//
// PORT functions.
//
#ifndef __PORT__
#define __PORT__

#define     PORT_LED1           (PORTA.PODR.BIT.B0)
#define     PORT_TP1_P12_CN13   (PORT1.PODR.BIT.B2)

void    PORT_init(void);

#endif
