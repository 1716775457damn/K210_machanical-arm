#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************

**************************************************************************/
#define PI 3.14159265

#define FILTERING_TIMES  10
extern	int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
#define  Grasp 2010  //夹紧的舵机5数值
#define  Unstuck 1250//松开的舵机5数值
int EXTI15_10_IRQHandler(void);
u8 Kinematic_Analysis(float x,float y,float Beta,float Alpha);
u8 xyz_Analysis(float x,float y,float z,float Alpha);
void Set_Pwm(float velocity1,float velocity2,float velocity3,float velocity4,float velocity5);
void Key(void);
void Xianfu_Pwm(void);
void Xianfu_Pwm2(void);
u8 Turn_Off( int voltage);
void Get_Angle(u8 way);
int myabs(int a);
void Control(float Step);
u32 random1(void);
float angle_to_radian(u32 angle);//角度转弧度
float Position_PID1 (float Encoder,float Target);
float Position_PID2 (float Encoder,float Target);
float Position_PID3 (float Encoder,float Target);
float Position_PID4 (float Encoder,float Target);
float Position_PID5 (float Encoder,float Target);
float Position_PID6 (float Encoder,float Target);
int  inserve_kinematics(float wx,float wy,float wz);
void count_xyz(void);
u8 success_action(float p1,float p2,float p3,float p4,float T1,float T2,float T3,float T4);
void duoji_init(void);
u8 duoji5_static(float sta);
extern u8 zhua_flag;
char Check_Sum( char *Count_Number,unsigned char size);
void musicFuction(char count);//播放语音
#endif
