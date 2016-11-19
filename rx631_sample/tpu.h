//
// TPU functions.
//
#ifndef __TPU__
#define __TPU__

#define     TPU0_N_PERIOD       48000               // Period = N*(1/48MHz);    48000 -->1ms
#define     TPU0_N_PERIOD_HALF  (TPU0_N_PERIOD>>1)  // *(1/2)

#define     TPU3_UNIT_SEC       (1024.0/48e6)       // =21.3[us]=(1/(48MHz/1024))=PRESCALE/PLCK
                                                    // max: x65535 = 1.398[s]

void    TPU_init(void);

void    TPU_stop_ch     (int ch);
void    TPU_clear_ch    (int ch);
void    TPU_start_ch    (int ch);
void    TPU_restart_ch  (int ch);
int     TPU_count_ch    (int ch);

int     TPU_ch0_if_half_passed(void);
double  TPU_ch3_unit_in_sec(void);

#endif
