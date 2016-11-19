-------- PROJECT GENERATOR --------
PROJECT NAME :	rx631_sample
PROJECT DIRECTORY :	C:\hewwork\rx631_sample\rx631_sample
CPU SERIES :	RX600
CPU TYPE :	RX63N
TOOLCHAIN NAME :	Renesas RX Standard Toolchain
TOOLCHAIN VERSION :	1.2.1.0
GENERATION FILES :
    C:\hewwork\rx631_sample\rx631_sample\dbsct.c
        Setting of B,R Section
    C:\hewwork\rx631_sample\rx631_sample\typedefine.h
        Aliases of Integer Type
    C:\hewwork\rx631_sample\rx631_sample\sbrk.c
        Program of sbrk
    C:\hewwork\rx631_sample\rx631_sample\iodefine.h
        Definition of I/O Register
    C:\hewwork\rx631_sample\rx631_sample\intprg.c
        Interrupt Program
    C:\hewwork\rx631_sample\rx631_sample\vecttbl.c
        Initialize of Vector Table
    C:\hewwork\rx631_sample\rx631_sample\vect.h
        Definition of Vector
    C:\hewwork\rx631_sample\rx631_sample\resetprg.c
        Reset Program
    C:\hewwork\rx631_sample\rx631_sample\hwsetup.c
        Hardware Setup file
    C:\hewwork\rx631_sample\rx631_sample\rx631_sample.c
        Main Program
    C:\hewwork\rx631_sample\rx631_sample\sbrk.h
        Header file of sbrk file
    C:\hewwork\rx631_sample\rx631_sample\stacksct.h
        Setting of Stack area
START ADDRESS OF SECTION :
 H'1000	B_1,R_1,B_2,R_2,B,R,SU,SI
 H'FFFF8000	PResetPRG
 H'FFFF8100	C_1,C_2,C,C$*,D_1,D_2,D,P,PIntPRG,W*,L
 H'FFFFFFD0	FIXEDVECT

* When the user program is executed,
* the interrupt mask has been masked.
* 
* Program start 0xFFFF8000.
* RAM start 0x1000.

DATE & TIME : 2016/10/22 17:50:11
