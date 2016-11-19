# rx631_sample
Small sample C program as a HEW project for Akizuki GR-CITRUS, RX631, board
------
# Functions
## DFT/FFT and self test
On power up, DFT and FFT functions are executed and processing time is shown via serial port.  
 - FFT function spec:  
  - Input:          real only  
  - Output:         0 to Nuquist frequency  
  - Precision:      double(8 or 4byte)  
    (Renesas tool HEW can handle double type as either 8 or 4 byte.)  
  - Number of input data supported: **N=4(2^p)(3^q)(5^r)(7^s)**  
    (4<=N<=6400 and p, q, r and s is an integer >= 0. N can be extended to 12600, if double is set to be handled as 4byte)
  - Desimation type: decimation-in-time  
  - Radix:          mixed radix  
      - intermediate stage: 8, 7, 5, 4, 3, 2  
      - terminal stage:     16, 8, 5, 4, 3, 2  
  - Implementation: recursively implemented according to the recursive nature of the FFT algorithm
  - performance summary
    ![FFT(real) performance summary](./FFT_real_performance_RX631_RX621_RX220.png)
    For further details, see [20161117_FFT_performance_RX631_RX621_RX220.pdf](20161117_FFT_performance_RX631_RX621_RX220.pdf).
 - For other RX621 or RX220 development board versions, see projects: [https://github.com/radioshack16/rx621_sample](https://github.com/radioshack16/rx621_sample), [https://github.com/radioshack16/rx220_sample](https://github.com/radioshack16/rx220_sample).
------
# License: MIT License  
- See the [LICENSE.txt](LICENSE.txt) file for details.

-----
Blog : <http://solar-club.jp/member/radioshack16/>
-----
