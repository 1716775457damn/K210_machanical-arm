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
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//Flag_Show!=0  使用串口1   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
void usart3_printf(u8 *p)
{
	while(*p!='\0')
	{
  while((USART3->SR&0X40)==0);//Flag_Show!=0  使用串口   
	USART3->DR = *p;
		p++;
	
	}

}

void usart3_sendchar(u8 p)
{

 while((USART3->SR&0X40)==0);//Flag_Show!=0  使用串口   
	USART3->DR = p;

}
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
   //USART 初始化设置
  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//打开空闲中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
void usart1_sendchar(u8 p)
{

 while((USART1->SR&0X40)==0);//Flag_Show!=0  使用串口   
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
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
  
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
   //USART 初始化设置
  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//打开空闲中断
  USART_Cmd(USART2, ENABLE);                    //使能串口1 

}
void usart2_sendchar(u8 p)
{

 while((USART2->SR&0X40)==0);//Flag_Show!=0  使用串口   
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
































			
			
			
