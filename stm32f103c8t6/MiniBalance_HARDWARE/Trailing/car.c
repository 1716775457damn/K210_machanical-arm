  #include "car.h"
	 #include "control.h"	
/*
#define  qian  1
#define  hou   2
#define  right 1
#define  left  2

*/
/**********
��������void trail_gpio_init()
���ܣ� β��Ѱ��ģ���IO�ڳ�ʼ��
PA����PA11 PB0



************/
void tail_trail_init()
{
	
		RCC->APB2ENR|=1<<2;
		RCC->APB2ENR|=1<<3;
		GPIOA->CRL&=0XF0FFFFFF;
		GPIOA->CRL|=0x04000000;
  	//GPIOA->CRL&=0XFF0FFFFF;
		//GPIOA->CRL|=0x00400000;
	  GPIOA->CRH&=0XFFFF0FFF;
		GPIOA->CRH|=0X00004000;
		GPIOB->CRL&=0XFFFFFFF0;
    GPIOB->CRL|=0X00000004;
	

}


/**********
��������void head_gpio_init()
���ܣ� ͷ��Ѱ��ģ���IO�ڳ�ʼ��
Pc13��Pc14 Pc15


************/
void head_trail_init()
{

GPIO_InitTypeDef GPIO_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE ); 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13  |
       GPIO_Pin_14 |
       GPIO_Pin_15  ;
GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOC, &GPIO_InitStructure);
PWR_BackupAccessCmd(ENABLE);
RCC_LSEConfig(RCC_LSE_OFF);
PWR_BackupAccessCmd(DISABLE);
	 /*	RCC->APB2ENR|=1<<4;
		RCC->APB2ENR|=1<<0;
	  GPIOC->CRH&=0X000FFFFF;
		GPIOC->CRH|=0x44400000;
	
		PWR_BackupAccessCmd(ENABLE);//�����޸�RTC �ͺ󱸼Ĵ���
	  RCC_LSEConfig(RCC_LSE_OFF);//�ر��ⲿ�����ⲿʱ���źŹ��� ��PC13 PC14 PC15 �ſ��Ե���ͨIO�á�
	  BKP_TamperPinCmd(DISABLE);//�ر����ּ�⹦�ܣ�Ҳ���� PC13��Ҳ���Ե���ͨIO ʹ��
	  PWR_BackupAccessCmd(DISABLE);//��ֹ�޸ĺ󱸼Ĵ���
*/


}
/**********
��������void bizhang_init()
���ܣ� ����
PB12


************/
void bizhang_init()
{

GPIO_InitTypeDef GPIO_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOB, &GPIO_InitStructure);

}


/**********************************************
��������void tail_xunji()
���ܣ�  ��С��Ѱ��
������
����ˣ�PA11 PB0 PA6

#define  L PAin(11)
#define  M PBin(0)
#define  R PAin(6)

**********************************************/

char TailFlag=0;
char ZeroSpeechBf[]={0x7B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7B,0x7D};
char TailoldFlag=0;
char HeadoldFlag=0;
char SpeechBuff[11];

/**********************************************
��������void car_stop()
���ܣ�  ��С��ֹͣ


**********************************************/
void car_stop()
{

    output_pwm(3,0);
	  output_pwm(4,0);
    output_pwm(1,0);
	  output_pwm(2,0);

	
}
void SpeechSet(short x,short y,short z,short h)//
{
	char crc=0;
	memset(SpeechBuff,0,11);
	SpeechBuff[0]=0x7B;
	SpeechBuff[10]=0x7d;
	
	SpeechBuff[1]=x>>8;
	SpeechBuff[2]=x;
	SpeechBuff[3]=y>>8;
	SpeechBuff[4]=y;
	SpeechBuff[5]=z>>8;
	SpeechBuff[6]=z;
	SpeechBuff[7]=h>>8;
	SpeechBuff[8]=h;
	crc = Check_Sum(SpeechBuff,9);
 	SpeechBuff[9]= crc;
}
void test(short x,short y,short z,short h)
{
	  int i=0;
		SpeechSet(x,y,z,h);
		for(i=0;i<11;i++)
			usart2_sendchar(SpeechBuff[i]);	

}
void tail_xunji()
{
				
			if(L==1&&M==0&&R==1)
			{
		    	move(hou,0.003);		
			//	printf("MMMM");
			}
			
		else 	if(L==0&&R==1)
			{
	      
					turn(right,0.005);
			//	printf("LLL");
	
			}
	
		else	if(L==1&&R==0)
	
			{
	        turn(left,0.005);
			//	printf("RRR");
			}
			else	if(L==1&&M==1&&R==1)
					{					
							move(hou,0.003);
					//	  printf("000");
					}
		else	if((L==0&&M==0&&R==0)||(L==0&&M==0&&R==1)||(L==1&&M==0&&R==0))
			{	
					printf("t111\n"); 
					//TAIL_flag=1; 
					car_stop();
									
			}
			
	
			
}

/**********************************************
��������void head_xunji()
���ܣ�  ��С��Ѱ��
������
����ˣ�Pc13 Pc14  Pc15

#define  L1 PCin(13)
#define  M1 PCin(14)
#define  R1 PCin(15)

**********************************************/
char HeadFlag=0;

void head_xunji()
{

    if(L1==1&&M1==0&&R1==1)
			{
		    	move(qian,0.003);		
			}
			
		else if(L1==0&&R1==1)
			{
	      
					turn(left,0.005);
	
			}
	
			else if(L1==1&&R1==0)
			{
	       /* output_pwm(1,0);
	        output_pwm(2,0);
					output_pwm(4,0);
					output_pwm(3,1);
				*/
				turn(right,0.005);
			}
		else if(L1==1&&M1==1&&R1==1)
				{
						move(qian,0.003);
						
				}
				else if((L1==0&&M1==0&&R1==0)||(L1==0&&M1==0&&R1==1)||(L1==1&&M1==0&&R1==0))
					{
							
							 car_stop();
							 printf("h111\n");
										   	
					}
	    
		
}

/**********
��������void move(u8 direction ,float duty)
���ܣ�  С��ǰ������
������u8 direction��qian  1
                     hou   2


ǰ����IN1 :1
      IN2 :0
			IN3 :1
      IN4 :0
output_pwm(u8 chnl,float duty)

************/

void move(u8 direction,float duty)
{

	
	if(direction==qian)
	{
	 output_pwm(1,duty);
	 output_pwm(2,0);
   output_pwm(3,duty);
	 output_pwm(4,0);
	
	}
	
   if(direction==hou)
	{
	output_pwm(1,0);
	 output_pwm(2,duty);
   output_pwm(3,0);
   output_pwm(4,duty);
	
	
	}
	
}
/**********************************************
��������void turn(u8 direction,float duty)
���ܣ�  ��С��ת��
������  u8 direction,u8 float dut

**********************************************/
void turn(u8 direction, float duty)
	
{
	
		if(direction==left)

		{
		output_pwm(1,0);
	  output_pwm(2,duty/2);
    output_pwm(4,0);
	  output_pwm(3,duty);
		
	
		}
		if(direction==right)
		{
		
		output_pwm(2,0);
    output_pwm(1,duty/2);
	  
		output_pwm(3,0);
	  output_pwm(4,duty);
			
		}
	

}

char bizhangAction()
{
	char sta=0;
	if(zhang==0)
	{
		sta=1;
		if(sta==1)
		{
			car_stop();
			sta=2;
			
		}
	
	
	}
		return  sta;
}