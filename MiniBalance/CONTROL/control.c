#include "control.h"	
#include "oled.h"
#include "sys.h"
 /**************************************************************************

**************************************************************************/
int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
u8 Flag_Target;
u32 Flash_R_Count;
u8 color;
u8 init_flag=0;
u8 head_flag=0;//车头寻迹模块灯全灭
u8 TAIL_flag=0;//尾部寻迹模块灯全灭
u8 end_ok;//完成抓取或者放置，并且回到初始位置
u8 H_xun,T_xun;//头寻迹和尾巴寻迹
u8 T_anx;
int Voltage_Temp,Voltage_Count,Voltage_All;
float Last_TargetX,Last_TargetY,Last_TargetZ;
float temp_radian[5];
float RecyclableTarget[]={765,723,1016,1094};
float KitchenTarget[]={649,706,1016,1094};
float OtherTarget[]={582,623,949,1094};
float HarmfulTarget[]={855,584,906,1104};

u8 count_time;
u8 bi_flag=0;
u8 bi_step=0;
#define l0 0.105f
#define l1 0.10f
#define l2 0.135f

#define L2 0.105f
#define L3 0.10f
#define L4 0.135f

//#define  Grasp 750  //夹紧的舵机5数值
//#define   Unstuck 250//松开的舵机5数值
//#define l1 0.078f
//#define l2 0.055f

#define Ratio 5.06f
float temp_alpha=-2.355;
u8 step=0;
int count=0;

/**************************************************************************
函数功能：计算发送的数据校验位
入口参数：
返回  值：检验位
**************************************************************************/
 char Check_Sum( char *Count_Number,unsigned char size)
{
	 char check_sum=0,k;
	for(k=0;k< size;k++)
	{
	check_sum=check_sum^Count_Number[k];
	}
	
	return check_sum;
}

/**************************************************************************

函数功能：float数据类型的绝对值
入口参数：float num					
返回  值：绝对值

**************************************************************************/

float abss(float num) 
{
		if(num>=0)
			return num;
		else
			return -num;


}

/**************************************************************************

函数功能：用正解验算逆解的正确性
入口参数：float x,float y,float z:x,y,z坐标
					float theta1,theta3,theta2,theta4：各个关节的旋转角度(注意：是弧度值不是角度值)
					
返回  值：　１：正确
						０：错误

**************************************************************************/
int is_right_solution(float x,float y,float z,float theta1,float theta2,float theta3,float theta4)
{
	float x1,y1,z1,d;
	
	d=l0*cos(theta2)+l1*cos(theta2+theta3)+l2*cos(theta2+theta3+theta4);
	z1=l0*sin(theta2)+l1*sin(theta2+theta3)+l2*sin(theta2+theta3+theta4);
	x1=d*sin(theta1);
	y1=d*cos(theta1);
	
	if(abss(x1-x)>1e-2||abss(y1-y)>1e-2||abss(z1-z)>1e-2)
		return -1;
	
	
	
	 return 1;
	

}
/**************************************************************************

函数功能：判断角度合法性
入口参数：float theta:角度
          u8 joint_num：关节序号
theta1(原beta)取值范围：-0.587PI到0.359PI
theta2(原theta1):(5/180)*PI--(166/180)*PI
theta3(原theta2):-PI/2----(77/180)*PI)
theta4(原theta3):-7PI/18-----PI/2

返回值：true false

**************************************************************************/
int joint_angle_legal(float theta,u8 joint_num)
{
	int temp;
	switch(joint_num)
	{
		case 1:if(theta>=(-0.5*PI)&&theta<=0.5*PI) temp= 1;else temp= -1;break;
		case 2:if(theta>=((0)*PI)&&theta<=PI) temp= 2;else temp= -2;		break;		
		case 3:if(theta>=(-PI/2)&&theta<=(0.5)*PI) temp= 3;else temp= -3;break;
		case 4:if(theta>=(-0.5*PI)&&theta<=PI/2) temp= 1;else temp= -1;break;
	}
	return temp;
}

/**************************************************************************

函数功能：产生a角的一个随机角度数(非弧度)
入口参数：         
theta1(原beta)取值范围：-0.587PI到0.359PI
theta2(原theta1):(5/180)*PI--(166/180)*PI
theta3(原theta2):-PI/2----PI/2

theta4(原theta3):-PI/2-----PI/2
a:(根据实际情况得出取值范围)：-3PI/4---PI/2 (-135---90度)


**************************************************************************/
u32 random1()
{
		u32 temp;
		//srand(time(0));
		temp=rand()%226-135;
		return temp;

}

float angle_to_radian(u32 angle)//角度转弧度
{
	float temp;
	temp=angle*PI/180;
	return temp;

}

float radian_to_angle(float radian)
{
	float angle;
	angle=radian*180/PI;	
	return angle;
	
}

/**************************************************************************

函数功能：数学模型求逆解(优化算法)
入口参数：float x,float y,float z:x,y,z坐标
theta1
theta2:
theta3:
theta4:
a:(根据实际情况得出取值范围)


**************************************************************************/
u16 ID_Count[7] = {0};

int  inserve_kinematics(float wx,float wy,float wz)
{
	float distance,theta1,theta2,theta3,theta4,d;
	float b,z,cos_theta3,sin_theta3;
	float k1,k2;	
	distance=sqrt(wx*wx+wy*wy+wz*wz);
	d=sqrt(wx*wx+wy*wy);
	
	if(distance>(L2+L3+L4))
	{
			printf("too long\n");
			return -1;
	}
	if((wx==0)&&(wy==0))
	{
	     theta1=0;
	}
	else
	{	
				theta1=asin(wx/d);
		
				if(joint_angle_legal(theta1,1)<0)
				{				
					return -2;			
				}
	}			
				temp_alpha=temp_alpha+0.02;
				
				if(temp_alpha>=1.57)
				{
				//	printf("error\n");
					start_flag=0;
		
					temp_alpha=-2.355;
					return -1;
				
				}
				temp_radian[1]=temp_alpha;
			//	printf("temp:%f\n",temp_alpha);
		
				b=d-L4*cos(temp_alpha);
				z=wz-L4*sin(temp_alpha);
				cos_theta3=(z*z+b*b-L2*L2-L3*L3)/(2*L2*L3);
				if(abss(cos_theta3)>1)
					return -3;
				sin_theta3=sqrt(1-cos_theta3*cos_theta3);
				theta3=atan2(-sin_theta3,cos_theta3);
				if(joint_angle_legal(theta3,3)<0)
				{
					
					return -4 ;
				
				}
				temp_radian[3]=theta3;
				k1=L2+L3*cos(theta3);
				k2=L3*sin(theta3);
				theta2=atan2(z,b)-atan2(k2,k1);
				theta4=temp_alpha-theta3-theta2;
				temp_radian[2]=theta2;
				temp_radian[4]=theta4;
				if((joint_angle_legal(theta2,2)<0)||(joint_angle_legal(theta4,4)<0))
				{
					
					return -2 ;
				
				}
	
       
			if(is_right_solution(TargetX,TargetY,TargetZ,theta1,theta2,theta3,theta4)<0)
				{
				
						return -6;
				
				}

			  Target1   = 1545-(radian_to_angle(theta1))*7.407;   //作用到输出  底座
				Target2= 7.407*radian_to_angle(theta2)+900;
	      Target3   = 1500-(radian_to_angle(theta3))*7.407;
	      Target4   = 1500-(radian_to_angle(theta4))*7.407;
				ID_Count[6] = (u16)Target1;
				ID_Count[5] = (u16)Target2;
				ID_Count[4] = (u16)Target3;
				ID_Count[3] = (u16)Target4;
				ID_Count[2] = 1500;
				ID_Count[1] = 1313;
				Target_Alpha=temp_alpha;
				
			//	printf("1:%f.4  2:%.4f 3:%.4f 4:%.4f a:%.4f\n",radian_to_angle(theta1),radian_to_angle(theta2),radian_to_angle(theta3),radian_to_angle(theta4),radian_to_angle(Target_Alpha));
			//	printf("t1:%.4f t2:%.4f t3:%.4f t4:%.4f\n",Target1,Target2,Target3,Target4);
				
	     return 4;


}

/**************************************************************************
函数功能:ABC模式下计算末端执行器的位置坐标	
float TargetX=0.15,TargetY=-0.15,TargetZ=0.1,Target_Beta=0,Target_Alpha=0,Target_Gamma=0;   
float Target1=660,Target2=730,Target3=750,Target4=730,Target5=450,Target6=800;     //电机目标值
        Target1   = 660-(Beta)*Ratio;   //作用到输出  
        Target2   = 730-(theta1-90)*Ratio;
	      Target3   = 770+(theta2)*Ratio;
	      Target4   = 700+(theta3)*Ratio;  
Y4 = 5.615233763116244*x + 714.1838206873543
 y3 = 5.447043157528549*x + 766.5773232333704
y2 = -5.274503075479348*x + 1182.9972709649717
**************************************************************************/
void count_xyz()
{
		float beta,theta1,theta3,theta2,d,z;
		beta=((750-Target1)/Ratio)*PI/180;
		//theta1=((750-Target2)/Ratio+90)*PI/180;
	  theta1=((Target2-1250)/(-5.56))*PI/180;
		theta2=((Target3-750)/5.56)*PI/180;
		
		
		theta3=((Target4-750)/5.56)*PI/180;
	
		
		
		d=l0*cos(theta1)+l1*cos(theta1+theta2)+l2*cos(theta1+theta2+theta3);
		z=l0*sin(theta1)+l1*sin(theta1+theta2)+l2*sin(theta1+theta2+theta3);
		TargetZ=z;
		TargetX=d*sin(beta);
		TargetY=d*cos(beta);
	  Target_Alpha=(theta1+theta2+theta3);
		



}
/**************************************************************************
函数功能：判断前末端执行器状态是否已经完成动作
sta:  Grasp 750  //夹紧的舵机5数值
      Unstuck 450//松开的舵机5数值
**************************************************************************/
u8 duoji5_static(float sta)
{
			u8 a;
			a=abss(floor(Position5)-floor(sta));
			if(a<=1)
			return 1;
			else
			return 2;
			


}
/**************************************************************************
函数功能：判断前四个舵机是否已经完成动作
cmd 0:抓
cmd 1：放

**************************************************************************/

u8 success_action(float p1,float p2,float p3,float p4,float T1,float T2,float T3,float T4)
{
		u8 a,b,c,d;
	a=abss(floor(p1)-floor(T1));
	b=abss(floor(p2)-floor(T2));
	c=abss(floor(p3)-floor(T3));
	d=abss(floor(p4)-floor(T4));
  if((a<=1)&&(b<=1)&&(c<=1)&&(d<=1))
	{
	   return 1;
	
	}
			   
	else
			return 2;
			
			
		

}

/**************************************************************************
函数功能：d舵机回到初始化位置	未包括末端执行器 (回到初始位置)
**************************************************************************/
void duoji_init()
{
 
Target1=750;Target2=1040;Target3=1040;Target4=300;


}
/**************************************************************************
函数功能：判断机械臂是否为初始位置
        返回值：1：回归成功
								0：未回归
**************************************************************************/
u8 judge_init(float p1,float p2,float p3,float p4)
{
   u8 a,b,c,d;
		
	a=abss(floor(p1)-750);
	b=abss(floor(p2)-1040);
	c=abss(floor(p3)-1040);
	d=abss(floor(p4)-300);
  if((a<=1)&&(b<=1)&&(c<=1)&&(d<=1))
	{
	   return 1;
	  
	}
			   
	else
		return 0;
			
   
}
/**************************************************************************
函数功能：抓取
参数：u32 q：0：抓
							1：放
**************************************************************************/
u8 zhua_and_fang(u32 q)
{
			u8 temp;
	   // judge_init(Position1,Position2,Position3,Position4);
	
			if(zhua_flag==1)
			{
			if(success_action(Position1,Position2,Position3,Position4,Target1,Target2,Target3,Target4)==1)
			{
			if(q==0)
			{
				//printf("init6\n");
			Target5=Grasp;
				
			init_flag=1;
			temp=1;
			}
			if(q==1)
			{
			Target5=Unstuck;
			
				//printf("init5\n");
				init_flag=1;
				temp=2;
			}
			
		//	zhua_flag=0;
		  
			}
				
			}

     return temp;
		


}
/**************************************************************************
函数功能：抓或者放后回到初始位置
参数：sta :为0为抓取
					为1 为放
**************************************************************************/
void return_init(u8 sta)
{
				
     if(zhua_and_fang(sta))
		 {
			if (init_flag==1)
			{
		  //	printf("bug1\n");
			if((duoji5_static(Grasp)==1&&sta==0)||(duoji5_static(Unstuck)==1&&sta==1))
			{
	
			Target1=750;Target2=1040;Target3=1040;Target4=300;
				if(judge_init(Position1,Position2,Position3,Position4))
				{
		      	init_flag=0;
			      
					  end_ok=1;
						
				}
			}
		}			
				
		

	    }

}
/**************************************************************************
函数功能：实现对色块的搬运
float RecyclableTarget[]={899,933,1216,1233};
float KitchenTarget[]={666,1033,1216,1250};
float OtherTarget[]={1216,850,1250,980};
float HarmfulTarget[]={1083,816,1233,930};
**************************************************************************/
void color_fenlei(int color1)
{
	
	switch(color1)
	{
		case 2: Target1=RecyclableTarget[0];Target2=RecyclableTarget[1];Target3=RecyclableTarget[2];Target4=RecyclableTarget[3];break;
	  case 3: Target1=KitchenTarget[0];Target2=KitchenTarget[1];Target3=KitchenTarget[2];Target4=KitchenTarget[3];break;
	  case 1: Target1=HarmfulTarget[0];Target2=HarmfulTarget[1];Target3=HarmfulTarget[2];Target4=HarmfulTarget[3];break;
		case 4: Target1=OtherTarget[0];Target2=OtherTarget[1];Target3=OtherTarget[2];Target4=OtherTarget[3];break;
		default:Target1=OtherTarget[0];Target2=OtherTarget[1];Target3=OtherTarget[2];Target4=OtherTarget[3];break;
	}
	




}

/**************************************************************************
函数功能：定时中断函数 所有的控制代码都在这里面		 
**************************************************************************/
int datalen;
int dataIdx=0;
float re_x,re_y,re_z;
int r_color;
u8 start_flag;
extern char TailFlag;
char stepcount=0;

u8 dogcmd[]={0x55,0xAA,0X11,0X00,0X21,0X31};

u8 music_Harmful[]={0xFD, 0x00, 0x0F, 0x01, 0x01, 0xBA, 0xDA, 0xC1, 0xFA, 0xBD, 0xAD, 0xB9, 0xFE, 0xB6, 0xFB, 0xB1, 0xF5, 0xF7  };//黑龙江哈尔滨
u8 music_Recyc[]={0xFD, 0x00, 0x0B, 0x01, 0x01, 0xBC, 0xAA, 0xC1, 0xD6, 0xB3, 0xA4, 0xB4, 0xBA, 0xEE   };//吉林长春
u8 music_Kitchen[]={0xFD, 0x00, 0x0B, 0x01, 0x01, 0xC1, 0xC9, 0xC4, 0xFE, 0xC9, 0xF2, 0xD1, 0xF4, 0xDA  };//辽宁沈阳
u8 music_other[]={0xFD, 0x00, 0x0B, 0x01, 0x01, 0xC1, 0xC9, 0xC4, 0xFE, 0xB4, 0xF3, 0xC1, 0xAC, 0xEE };//辽宁大连



int musiccount=0;
char GarbageStype=0;
char singleStep=0;
int singnalCount=0;

void Action_Group(char group)
{
	
	char SendCmd[7];
	int i =0;
	SendCmd[0] = 0x55;
	SendCmd[1] = 0x55;
	SendCmd[2] = 0x05;
	SendCmd[3] = 0x06;
	SendCmd[4] = group;
	SendCmd[5] = 0x01;
	SendCmd[6] = 0x00;
	for(i = 0; i<7;i++)
		usart3_sendchar(SendCmd[i]);
	
}
void Action_Singnal(char num,u16 count)
{

		
	char SendCmd[10];
	int i =0;
	SendCmd[0] = 0x55;
	SendCmd[1] = 0x55;
	SendCmd[2] = 0x08;
	SendCmd[3] = 0x03;
	SendCmd[4] = 0x01;
	SendCmd[5] = 0x00;
	SendCmd[6] = 0x00;
	SendCmd[7] = num;
	SendCmd[8] = count & 0x00ff;
	SendCmd[9] = (count >>8 )& 0x00ff;

	for(i = 0; i<10;i++)
		usart3_sendchar(SendCmd[i]);



}
void U16T0Byte(u16 CountNum,u8 *Hight,u8 *Low)
{
	*Hight = (CountNum >> 8) & 0x00ff;
	*Low = (CountNum ) & 0x00ff;
	
}
u16 tempcount[]={0,983,1747,1680,1029,1836,1275};

void Action_Six(u16 *ID_Count)
{

	u8 SendCom[24] = {0};
	int i=0;
	SendCom[0] = 0x55;
	SendCom[1] = 0x55;
	SendCom[2] = 0x17;
	SendCom[3] = 0x03;
	SendCom[4] = 0x06;
	SendCom[5] = 0xd0;
	SendCom[6] = 0x07;
	for(i =0;i<6;i++)
	{
		SendCom[i*3+7] = i+1;
		U16T0Byte(ID_Count[i+1],&SendCom[i*3+9],&SendCom[i*3+8]);
	
	}
	for(i =0;i<25;i++)
	 usart3_sendchar(SendCom[i]);

}
void GarbageSorting(int garbage)//执行不同动作组
{
	switch(garbage)
	{
		case 1:
			Action_Group(2);//
			
		break;
		case 2:
			Action_Group(2);//
			
		break;	
	
		case 3:
			Action_Group(2);//
			
		break;	
		case 4:
			Action_Group(2);//
			
		break;	
		default :
			
		break;
	
	}


}
void SendCoint(u8 *p,int ID)
{
	char CountSd[10] ={0};
	int i=0;
	CountSd[0] = 0xA5;
	CountSd[1] = 0x5A;
	
	switch(ID)
	{
		case 1:p[0] =p[0]+1;break;
		case 2:p[1] =p[1]+1;break;
		case 3:p[2] =p[2]+1;break;
		case 4:p[2] =p[2]+1;break;
		
	
	}
	CountSd[2] = p[0];
	CountSd[3] = p[1];
	CountSd[4] = p[2];
	CountSd[5] = p[3];
	CountSd[6] = 0xff;
	for(i = 0;i<7;i++)
		usart1_sendchar(CountSd[i]);

}
int princount=0;
int bicount=0;
u8 CountBuff[4];
int TIM2_IRQHandler(void)
{    
	int temp;
	int i=0,J=0;
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{   
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
		   if(delay_flag==1)
			 {
				 if(++delay_50==5)
				 {
					
					 delay_50=0;
					 delay_flag=0;  //给主函数提供50ms的精准延时

				 }
			 }
						 
			 princount++;
			 if(princount==200)
			 {
				 princount=0;
				 printf("the step is %d\n",step);
			 }
			 
			if(RXFLAG  == 1)
			{   
					dataIdx=0;
					RXBuff[RXCENT] = '\0';
				//  printf(RXBuff);
		  	//printf("the count is %d\n",RXCENT);
				if(CRStrLib_isValidData(RXBuff, "HEAD", "TAIL")==TRUE)
				{
				
					datalen= CRStrLib_strLen(RXBuff);
					CRStrLib_findNextFloat(RXBuff, datalen, &dataIdx, &re_x);
					CRStrLib_findNextFloat(RXBuff, datalen, &dataIdx, &re_y);
					CRStrLib_findNextFloat(RXBuff,datalen, &dataIdx, &re_z);
					CRStrLib_findNextInt(RXBuff,datalen, &dataIdx, &r_color);
				
					RXCENT=0;
					RXFLAG = 0;//接收完成标志位清零
					start_flag=1;
				}
				else
				{
					//printf("2");
					RXCENT=0;
					RXFLAG = 0;//接收完成标志位清零
					//start_flag=1;
				
				
				}
				
			 }
			if(Flag_Way==1)     //进行运动学分析之后的控制
			{
				//printf("hi");
			
			}
			else
			{
					switch(step)	
				{
					case 0:
						   
							
								
											count++;
											if(count<2)
													Action_Group(1);
											 head_flag=1;
											 
											if(count==400)
											{
												count=0;
											 step++;
												
											}
										//	printf("case0");
										
					break;
					case 1:
									if(start_flag==0)
									{
											step=7;
									} 
									else
									{
										start_flag=0;
										step=8;//延时
									
									}
						break;
					
					case 2:
									TargetX=re_x;
									TargetY=re_y;
									TargetZ=re_z;
								//	color=r_color;								
									temp= inserve_kinematics(TargetX,TargetY,TargetZ);
								//	printf("return:%d",temp);
									if(temp==-1)
									{
										 start_flag=0;
										 temp_alpha=-2.355;
										
										 step=1;
									}
									if(temp==4)
									{
										
								  	
										zhua_flag=1;
										temp=0;		
										start_flag=0;
										
										temp_alpha=-2.355;
									  Action_Six(ID_Count);
			
											step=3;
									}
					break;
									
					case 3:
							
											count++;
											if(count==400)
											{
													count=0;
													zhua_flag=0;
								
													Target5=Grasp;
												
													step++;
											//		printf("333\n");
												 
												Action_Singnal(1,Target5);//抓紧
												
											}	
									
			
					break;
					case 4:
												count++;
											if(count==250)
											{
													count=0;	
													step++;
											//		printf("333\n");
											
											}
					break;
					case 5:
							    zhua_flag=1;	 
									count++;
									if(count<2)
									{
										GarbageSorting(GarbageStype);
										
									}
									 // printf("134\n");
								
										if(count==500)
										{
											//Target5=Unstuck;
											Action_Singnal(1,Unstuck);//松开
											//count=0;			
										}
										if(count==650)
										{
											count=0;
											step++;	
										}
																		
						
					
					break;
								
					case 6:
						
									Action_Group(1);  //电机初始值
												
										 count++;
										if(count==250)
										{
										
											count=0;
										//	printf("136\n");
											if(H_xun==1)
												step=11;
											else if(T_xun==1)
												step=7;
											else
												step=7;
												
											
										}
										
					break;
					case 7:		//后退		
							
					     // Action_Group(1);
								if((L==0&&M==0&&R==0))
								{	
									//printf("t111\n");
								
									TAIL_flag=1; 
							
								}
								if(TAIL_flag ==0)
								{
									 tail_xunji();
									
								//	 printf("TXUN\n");
								
								}
									//printf("case7");
								if(TAIL_flag==1)
									{
									//	printf("1111111111111111111111");
										step=11;
										car_stop();
									
									}
									if(start_flag==1)
									{
										start_flag=0;
										car_stop();
										step=8;
									
									}
								 T_xun=1;
								 head_flag=0;
								 H_xun=0;
					break;
					case 8:	
									count++;
								//start_flag=0;
									if(count<30)
									{
										//if(start_flag==1)
											start_flag=0;
										
									}
								 else if((count >30) && (count <130) )
								 {
										if(start_flag==1)
										{
										 
											start_flag=0;
											GarbageStype= r_color;
											
										//	printf("successful\n");
											step=2;
											count=0;
								
										}	
									}
									else if (count >130)	
									{										

											start_flag=0;
											
									//		 printf("successful\n");
											if(H_xun==1)
												step=11;
											else if(T_xun==1)
												step=7;
											else
												step=7;	
											count=0;
										
								
									}			
					break;
					case 9:					
					break;
					case 10:
					break;
									
					case 11://前进
						
							//	if(bi_flag==0)
							//	{
									if((L1==0&&M1==0&&R1==0))
									{
											 head_flag=1;
											// printf("h111\n");		
									}	
									if(head_flag ==0 )
									{
										head_xunji();	
									//	printf("h111\n");
										
									}
									if(head_flag==1)
									{
										 car_stop();
										 step=7;
									}
									if(start_flag==1)
									//if(start_flag==1)
									{
										//GarbageStype= dogcount;
										
										start_flag=0;
										car_stop();
										step=8;
									}

									TAIL_flag=0;
									T_xun=0;
									H_xun=1;
					break;		
					default:break;
				}
			//	printf("step %d,step1%d",step,bi_step);
			}
	
   	}       	
	 return 0;	  
} 


/**************************************************************************
函数功能：赋值给PWM寄存器,并且判断转向
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
 void Set_Pwm(float velocity1,float velocity2,float velocity3,float velocity4,float velocity5)
{	
		Position1+=velocity1;		   //速度的积分，得到舵机的位置
		Position2+=velocity2;
		Position3+=velocity3;
		Position4+=velocity4;
		Position5+=velocity5;	
			
	
	 TIM4->CCR1=Position1;    //赋值给STM32的寄存器
	 TIM4->CCR2=Position2;
	 TIM4->CCR3=Position3;
	 TIM4->CCR4=Position4;
	 TIM1->CCR1=Position5;
	 
	
		
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
			if(voltage<650)//电池电压过低  舵机将不能控制
			{	                                                
      temp=1;      
      }
			else
      temp=0;
      return temp;			
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude_H=1250, Amplitude_L=250;       //舵机脉宽极限值
    if(Target1<Amplitude_L)  Target1=Amplitude_L;	
		if(Target1>Amplitude_H)  Target1=Amplitude_H;	
	  if(Target2<250)  Target2=250;	
		if(Target2>1250)  Target2=1250;	
		if(Target3<Amplitude_L)  Target3=Amplitude_L;	
		if(Target3>Amplitude_H)  Target3=Amplitude_H;	
		if(Target4<Amplitude_L)  Target4=Amplitude_L;	
		if(Target4>Amplitude_H)  Target4=Amplitude_H;	
	  if(Target5<650)  Target5=550;	
		if(Target5>1250)  Target5=1250;	
	//	if(Target6<Amplitude_L)  Target6=Amplitude_L;	
	//	if(Target6>Amplitude_H)  Target6=Amplitude_H;		
}
void Xianfu_Pwm2(void)
{	
	  int Amplitude_H=Speed, Amplitude_L=-Speed;  
    if(Velocity1<Amplitude_L)  Velocity1=Amplitude_L;	
		if(Velocity1>Amplitude_H)  Velocity1=Amplitude_H;	
	  if(Velocity2<Amplitude_L)  Velocity2=Amplitude_L;	
		if(Velocity2>Amplitude_H)  Velocity2=Amplitude_H;	
		if(Velocity3<Amplitude_L)  Velocity3=Amplitude_L;	
		if(Velocity3>Amplitude_H)  Velocity3=Amplitude_H;	
		if(Velocity4<Amplitude_L)  Velocity4=Amplitude_L;	
		if(Velocity4>Amplitude_H)  Velocity4=Amplitude_H;	
	  if(Velocity5<Amplitude_L)  Velocity5=Amplitude_L;	
		if(Velocity5>Amplitude_H)  Velocity5=Amplitude_H;	
		if(Velocity6<Amplitude_L)  Velocity6=Amplitude_L;	
		if(Velocity6>Amplitude_H)  Velocity6=Amplitude_H;	
}
/**************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 temp;
	temp=click_N_Double(50);         //双击检测
  if(temp==1)	Flag_Way=!Flag_Way;//单击控制机械臂的状态
 // if(temp==2)	Flag_Show=!Flag_Show;//控制显示状态
}

/**************************************************************************
函数功能：控制机械臂
入口参数：控制目标
返回  值：无
**************************************************************************/
void Control(float Step)
{ 	
		
		if(PS2_KEY==5)     Target1+=Step;     //云台
	 else if(PS2_KEY==6)		 Target1-=Step;
	
	 else if(PS2_KEY==13)			Target2-=Step;    //大臂
	 else if(PS2_KEY==14)			Target2+=Step;
	
	 else if(PS2_KEY==7)		Target3-=Step;   //中间的舵机
	 else if(PS2_KEY==8)		Target3+=Step;   
	
	 else if(PS2_KEY==15)		Target4-=Step;  //小臂
	 else if(PS2_KEY==16)		Target4+=Step;
	 else if(PS2_KEY==12)		Target5-=Step;  //小臂
	 else if(PS2_KEY==10)		Target5+=Step;
	 
	
	
	
		
		//if(Speed<=3)Speed=3;
		//if(Speed>=30)Speed=30;
}
/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
float Position_PID1 (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
float Position_PID2 (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
float Position_PID3 (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
float Position_PID4 (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
float Position_PID5 (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
float Position_PID6 (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}


/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
