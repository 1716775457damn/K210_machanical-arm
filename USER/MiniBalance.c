#include "stm32f10x.h"
#include "control.h"
#include "sys.h"
#include "math.h"

/**************************************************************************

**************************************************************************/
u8 Flag_Way=0,Flag_Show=0;                 //æ ‡å¿—ä½ï¼Œå¯èƒ½ç”¨äºæ§åˆ¶æµç¨‹æˆ–æ˜¾ç¤º
int Servo1,Servo2,Servo3,Servo4,Servo5;     //å­˜è´®æ¯ä¸ªèˆµæœºçš„pwmå€¼
float Velocity1,Velocity2,Velocity3,Velocity4,Velocity5,Velocity6;     //ç”¨äºå­˜å‚¨é€Ÿåº¦å˜é‡ï¼Œå¯èƒ½ç”¨äºPIDæ§åˆ¶
float Position1=750,Position2=750,Position3=750,Position4=750,Position5=750,Position6=750;    //æœºæ¢°è‡‚å…³èŠ‚çš„å½“å‰ä½ç½®ã€‚
float Speed=3;                   //æ§åˆ¶æœºæ¢°è‡‚çš„ç§»åŠ¨é€Ÿåº¦
int Voltage;                      //ç”µæºç”µå‹å€¼
#define Pi 3.1416
u8 delay_50,delay_flag,Bi_zhang=0,PID_Send,Flash_Send; //ÑÓÊ±ºÍµ÷²ÎµÈ±äÁ¿
float Balance_Kp=350,Balance_Kd=0,Velocity_Kp=70,Velocity_Ki=0.7;    //PIDæ§åˆ¶å™¨å‚æ•°
u16 PID_Parameter[10],Flash_Parameter[10];  //FlashÏà¹ØÊı×é 
float Target_X=750,Target_Y=750	,Target_Z=750;   //ç›®æ ‡ä½ç½®çš„åæ ‡                  
//float Target1=680,Target2=730,Target3=770,Target4=730,Target5=450,Target6=800;     //µç»ú³õÊ¼Öµ
float Target1=732,Target2=765,Target3=545,Target4=490,Target5=750,Target6=3000;     //æœºæ¢°è‡‚å„å…³èŠ‚çš„ç›®æ ‡PWMå€¼Öµ
float TargetX,TargetY,TargetZ,Target_Beta=0,Target_Alpha=0,Target_Gamma=0;     					//µç»úÄ¿±êÖµ
float	Position_KP=6,Position_KI=0,Position_KD=3;  //Î»ÖÃ¿ØÖÆPID²ÎÊı
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;

u8 zhua_flag=0;
u8 testcmd[]={0xFD,0x00,0x0B ,0x01 ,0x01, 0xD3, 0xD0 ,0xBA ,0xA6 ,0xC0, 0xAC, 0xBB ,0xF8, 0xC6};


int main(void)
  { 
			int i=0,J=0,K=0;
			//	u8 temp1;
			MY_NVIC_PriorityGroupConfig(2); //ä¸­æ–­ä¼˜å…ˆçº§åˆ†ç»„é…ç½®
			delay_init();	    	            //=====ÑÓÊ±º¯Êı³õÊ¼»¯	
			uart_init(9600);	               //  k210
			uart3_init(9600);                 //OPENMV 
			Time2_pwm_init();
			car_stop();
			//=====´®¿Ú3³õÊ¼»¯
			JTAG_Set(SWD_ENABLE);           //=====´ò¿ªSWD½Ó¿Ú ¿ÉÒÔÀûÓÃÖ÷°åµÄSWD½Ó¿Úµ÷ÊÔ

			delay_ms(1000);                  //=====ÑÓÊ±µÈ´ı

			delay_ms(100);                  //=====ÑÓÊ±µÈ´ı

			tail_trail_init();
			head_trail_init();
			bizhang_init();	
		
			delay_ms(1000);  
			TIM2_Int_Init(99,7199);         //åˆå§‹åŒ–å®šæ—¶å™¨ä¸­æ–­ï¼Œå¯èƒ½ç”¨äºå®šæœŸæ›´æ–°æœºæ¢°è‡‚çŠ¶æ€
	  	
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

