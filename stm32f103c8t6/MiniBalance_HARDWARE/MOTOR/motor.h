#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 


void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);
void Time2_pwm_init(void);
void output_pwm(u8 chnl,float duty);
#endif
