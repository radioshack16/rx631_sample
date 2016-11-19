//
// DFT/FFT pamameters
//

#ifndef __DFT_FFT_PARAMS__
#define __DFT_FFT_PARAMS__

//#define     FFT_N_MAX    12600    // float,  RX631: RAM 256kB: 00000000hÅ`0003FFFFh
#define     FFT_N_MAX       6400    // double, RX631: RAM 256kB: 00000000hÅ`0003FFFFh
//#define   FFT_N_MAX       4096    // float,  RX621: RAM  96kB: 00000000hÅ`00017FFFh
//#define   FFT_N_MAX       2048    // double, RX621: RAM  96kB: 00000000hÅ`00017FFFh
//#define   FFT_N_MAX        128    // double, RX220: RAM  16kB: 00000000hÅ`00003FFFh
#define     DFT_N_MAX       FFT_N_MAX

#endif
