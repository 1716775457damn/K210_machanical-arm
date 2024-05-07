#ifndef _car_H_
#define _car_H_
#include "sys.h"
#define  qian  1
#define  hou   2
#define  right 1
#define  left  2
#define  L PAin(11)
#define  M PBin(0)
#define  R PAin(6)

void My_EXTI_Init(void);
#define  L1 PCin(13)
#define  M1 PCin(14)
#define  R1 PCin(15)

#define  zhang PBin(2)
void tail_trail_init(void);
void head_trail_init(void);
void move(u8 direction,float duty);

void turn(u8 direction, float duty);
void car_stop(void);

void tail_xunji(void);

void head_xunji(void);

void bizhang_init(void);
char bizhangAction(void);

void test(short x,short y,short z,short h);


#endif


