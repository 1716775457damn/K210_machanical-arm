#include "stm32f10x.h"
#include "control.h"
#include "sys.h"
#include "math.h"

/**************************************************************************

**************************************************************************/
u8 Flag_Way=0,Flag_Show=0;                 //标志位，可能用于控制流程或显示
int Servo1,Servo2,Servo3,Servo4,Servo5;     //存贮每个舵机的pwm值
float Velocity1,Velocity2,Velocity3,Velocity4,Velocity5,Velocity6;     //用于存储速度变量，可能用于PID控制
float Position1=750,Position2=750,Position3=750,Position4=750,Position5=750,Position6=750;    //机械臂关节的当前位置。
float Speed=3;                   //控制机械臂的移动速度
int Voltage;                      //电源电压值
#define Pi 3.1416
u8 delay_50,delay_flag,Bi_zhang=0,PID_Send,Flash_Send; //��ʱ�͵��εȱ���
float Balance_Kp=350,Balance_Kd=0,Velocity_Kp=70,Velocity_Ki=0.7;    //PID控制器参数
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash������� 
float Target_X=750,Target_Y=750	,Target_Z=750;   //目标位置的坐标                  
//float Target1=680,Target2=730,Target3=770,Target4=730,Target5=450,Target6=800;     //�����ʼֵ
float Target1=732,Target2=765,Target3=545,Target4=490,Target5=750,Target6=3000;     //机械臂各关节的目标PWM值ֵ
float TargetX,TargetY,TargetZ,Target_Beta=0,Target_Alpha=0,Target_Gamma=0;     					//���Ŀ��ֵ
float	Position_KP=6,Position_KI=0,Position_KD=3;  //λ�ÿ���PID����
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;

u8 zhua_flag=0;
u8 testcmd[]={0xFD,0x00,0x0B ,0x01 ,0x01, 0xD3, 0xD0 ,0xBA ,0xA6 ,0xC0, 0xAC, 0xBB ,0xF8, 0xC6};


int main(void)
  { 
			int i=0,J=0,K=0;
			//	u8 temp1;
			MY_NVIC_PriorityGroupConfig(2); //中断优先级分组配置
			delay_init();	    	            //=====��ʱ������ʼ��	
			uart_init(9600);	               //  k210
			uart3_init(9600);                 //OPENMV 
			Time2_pwm_init();
			car_stop();
			//=====����3��ʼ��
			JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���

			delay_ms(1000);                  //=====��ʱ�ȴ�

			delay_ms(100);                  //=====��ʱ�ȴ�

			tail_trail_init();
			head_trail_init();
			bizhang_init();	
		
			delay_ms(1000);  
			TIM2_Int_Init(99,7199);         //初始化定时器中断，可能用于定期更新机械臂状态
	  	
    while(1)  
	   {
					// printf("111111111");
					delay_flag=1;	
					delay_50=0;
			 /*
			 if(M ==1)
			 {
				printf("MMM");
			 
			 
			 }
			  if(L ==1)
			 {
				printf("LLL");
			 
			 
			 }
			  if(R ==1)
			 {
				printf("RRR");
			 
			 
			 }
			 */
					//head_xunji();	
					//	 turn(right,0.5);
					// move(hou ,0.5);
					//tail_xunji(); 
					// bizhangAction();
					// uart4_sendchar(0XFF);
					//	move(hou,0.5);
					// test(1800,1800,1800,1800);
					//	 car_stop();
					while(delay_flag);	     				
	   } 
}

