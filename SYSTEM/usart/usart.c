#include "sys.h"
#include "usart.h"	
 #include "control.h"	
char rebuff[30];
char buff[30];
u8 j=0;
u8 reflag=0;
  /**************************************************************************

**************************************************************************/
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//Flag_Show!=0  ʹ�ô���1   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
void usart3_printf(u8 *p)
{
	while(*p!='\0')
	{
  while((USART3->SR&0X40)==0);//Flag_Show!=0  ʹ�ô���   
	USART3->DR = *p;
		p++;
	
	}

}

void usart3_sendchar(u8 p)
{

 while((USART3->SR&0X40)==0);//Flag_Show!=0  ʹ�ô���   
	USART3->DR = p;

}
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
   //USART ��ʼ������
  //Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//�򿪿����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}
void usart1_sendchar(u8 p)
{

 while((USART1->SR&0X40)==0);//Flag_Show!=0  ʹ�ô���   
	USART1->DR = p;

}
u32 RXCENT=0;
char  RXBuff[RXMAX]={0};
u8  ch;
int RXFLAG=0;


void USART1_IRQHandler(void)
{
	
	   u8 clear;
	  u8 re=0;
					 
					 if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
					 {
						 re = USART1->DR;
					 RXBuff[RXCENT++]=re;
						 
				  //  usart1_sendchar(re);
						if(RXCENT>RXMAX)
							RXCENT=0;
							
					 
					 }
					else if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)
					 {
					    clear=USART1->SR;
					    clear=USART1->DR;
					 	  RXFLAG=1;
					 }
	
	
}
				
void uart2_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOAʱ��
  
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART_RX	  GPIOA.3��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
   //USART ��ʼ������
  //Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�򿪿����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 

}
void usart2_sendchar(u8 p)
{

 while((USART2->SR&0X40)==0);//Flag_Show!=0  ʹ�ô���   
	USART2->DR = p;

}

u32 RXCENT_2=0;
char  RXBuff_2[30]={0};
void USART2_IRQHandler(void)
{
	
	   u8 clear;
			u8 re=0;
		 
		 if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
		 {
				re=USART2->DR;
		  // usart1_sendchar(re);
			 RXBuff_2[RXCENT_2]=re;
			 /*
			 if(re==0x7B || RXCENT_2>0)		 RXCENT_2++;else RXCENT_2=0;
			 if(RXCENT_2==5)
			 {
				 RXCENT_2=0;
					if(RXBuff_2[4]==0x7D)
					{
						 if(Check_Sum(RXBuff_2,3)==RXBuff_2[3])
						 {
								PS2_KEY = RXBuff_2[2];
								Flag_Way =  RXBuff_2[1];
						//	  usart1_sendchar(0xff);
						 }

						//usart1_sendchar(0xff);	  
					}
			 } */
		 }
		else if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET)
		 {
				clear=USART2->SR;
				clear=USART2->DR;
		 }
}
































			
			
			
