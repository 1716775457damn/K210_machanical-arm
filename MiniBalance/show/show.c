#include "show.h"
  /**************************************************************************

**************************************************************************/
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void oled_show(void)
{                         
                         // OLED_ShowString(0,0,"K");
	                       // OLED_ShowNumber(5,0,PS2_KEY,2,12);
	
	                        //OLED_ShowString(0,0,"S:");
	                        OLED_ShowNumber(0,0,Speed,2,12);
	
		                      OLED_ShowString(37,0,".");
		                      OLED_ShowString(57,0,"V");
		                      OLED_ShowNumber(25,0,Voltage/100,2,12);
		                      OLED_ShowNumber(44,0,Voltage%100,2,12);
		 if(Voltage%100<10) 	OLED_ShowNumber(31,0,0,2,12);
		      if(Flag_Way==1) OLED_ShowString(70,0,"XYZ");  //坐标控制
		else if(Flag_Way==0)	OLED_ShowString(70,0,"ABC");   //单独控制
	//=============显示电机A的状态=======================//	
	  if( Target1<0)		   // OLED_ShowString(00,10,"-"),
		                      OLED_ShowNumber(0,10,-Target1,4,12);
		else                 //	OLED_ShowString(0,10,"+"),
		                      OLED_ShowNumber(0,10, Target1,4,12); 
		
		if( Position1<0)		 // OLED_ShowString(28,10,"-"),
		                      OLED_ShowNumber(30,10,-Position1,4,12);
		else                 	//OLED_ShowString(28,10,"+"),
		                      OLED_ShowNumber(30,10, Position1,4,12);
		//=============显示电机B的状态=======================//	
		  if( Target2<0)		//  OLED_ShowString(00,20,"-"),
		                      OLED_ShowNumber(0,20,-Target2,4,12);
		else                // 	OLED_ShowString(0,20,"+"),
		                      OLED_ShowNumber(0,20, Target2,4,12); 
		  
		if( Position2<0)		  //OLED_ShowString(60,20,"-"),
		                      OLED_ShowNumber(30,20,-Position2,4,12);
		else                 //	OLED_ShowString(60,20,"+"),
		                      OLED_ShowNumber(30,20, Position2,4,12);
 		//=============显示电机C的状态=======================//	
		  if( Target3<0)		//  OLED_ShowString(00,30,"-"),
		                      OLED_ShowNumber(0,30,-Target3,4,12);
		else                // 	OLED_ShowString(0,30,"+"),
		                      OLED_ShowNumber(0,30, Target3,4,12); 
		  
		if( Position3<0)		 // OLED_ShowString(60,30,"-"),
		                      OLED_ShowNumber(30,30,-Position3,4,12);
		else                 //	OLED_ShowString(60,30,"+"),
		                      OLED_ShowNumber(30,30, Position3,4,12);	
		//=============显示电机D的状态=======================//	
		  if( Target4<0)	  	//OLED_ShowString(00,40,"-"),
		                      OLED_ShowNumber(0,40,-Target4,4,12);
		else                 	//OLED_ShowString(0,40,"+"),
		                      OLED_ShowNumber(0,40, Target4,4,12); 
		
		if( Position4<0)		 // OLED_ShowString(60,40,"-"),
		                      OLED_ShowNumber(30,40,-Position4,4,12);
		else                 //	OLED_ShowString(60,40,"+"),
		                      OLED_ShowNumber(30,40, Position4,4,12);
			//=============显示电机D的状态=======================//	
    if( Target5<0)	    	//OLED_ShowString(00,50,"-"),
		                      OLED_ShowNumber(0,50,-Target5,4,12);
		else                // 	OLED_ShowString(0,50,"+"),
		                      OLED_ShowNumber(0,50, Target5,4,12); 
		
		if( Position5<0)		 // OLED_ShowString(60,50,"-"),
		                      OLED_ShowNumber(30,50,-Position5,4,12);
		else                // 	OLED_ShowString(60,50,"+"),
		                       OLED_ShowNumber(30,50, Position5,4,12);
		
		
		                       OLED_ShowString(60,10,"X:");
		                       OLED_ShowString(60,24,"Y:");
		                       OLED_ShowString(60,38,"Z:");
		                        OLED_ShowString(60,52,"a:");
													 //Show_float_num(88,24,TargetY,3,12);
		                        show_float_num1(78,24,TargetY);
		                       show_float_num1(78,38,TargetZ);
													 show_float_num1(78,10, TargetX);
		                       show_float_num1(78,52, Target_Alpha);

		//=============刷新=======================//
		OLED_Refresh_Gram();	
	}
/**************************************************************************
函数功能：向APP发送数据
入口参数：无
返回  值：无

**************************************************************************/
void APP_Show(void)
{    
		static u8 flag;
	  int app_2,app_3,app_4;
		app_4=(Voltage-1110)*2/3;		if(app_4<0)app_4=0;if(app_4>100)app_4=100;   //对电压数据进行处理
//    app_3=Moto1/50; if(app_3<0)app_3=-app_3;			                           //对编码器数据就行数据处理便于图形化
//		app_2=Moto2/50;  if(app_2<0)app_2=-app_2;
	  flag=!flag;
	if(PID_Send==1)//发送PID参数
	{
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)(Balance_Kp*100),(int)(Balance_Kd*100),(int)(Velocity_Kp*100),(int)(Velocity_Ki*100),0,0,0,0,0);//打印到APP上面	
		PID_Send=0;	
	}	
   else	if(flag==0)// 
   printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,(u8)app_4,0);//打印到APP上面
	 else
	 printf("{B%d:%d:%d:%d:%d}$",(int)Position1,(int)Position2,(int)Position3,(int)Position4,(int)Position5);//打印到APP上面 显示波形
}
/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无

**************************************************************************/
void DataScope(void)
{   
    Vol=(float)Voltage/100;
		DataScope_Get_Channel_Data(Position1, 1 );   //显示角度 单位：度（°）
		DataScope_Get_Channel_Data(Position2, 2 );   //显示超声波测量的距离 单位：CM 
		DataScope_Get_Channel_Data(Position3, 3 );   //显示电池电压 单位：V
		DataScope_Get_Channel_Data(Position4, 4 );   
		DataScope_Get_Channel_Data(Position5, 5 ); //用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0, 7 );
//		DataScope_Get_Channel_Data( 0, 8 ); 
//		DataScope_Get_Channel_Data(0, 9 );  
//		DataScope_Get_Channel_Data( 0 , 10);
		Send_Count = DataScope_Data_Generate(5);
		for( i = 0 ; i < Send_Count; i++) 
		{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
		}
}
