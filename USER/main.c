#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "HCTL2020.h"
#include "timer.h"
#include "lcd.h"
#include "control.h"
#include "pwm.h"
#include "led.h"
//#include "key.h"
#include "beep.h"
#include "remote.h"






int main(void)
{
	u8 key = 0;					//按键值
	u8 chengxu = 0;				//程序选择
	u8 flag=0;
	u8 qiu = 0;				//找球
	u8 zhongquan_case=0;
	u8 sanfen_case=0;
	int16_t time = 0;			//延时
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置系统中断优先级分组2   2位抢占优先 2位响应优先  
		
	delay_init(168);  			//初始化延时函数
	uart_init(9600);	 		//串口初始化为9600
	initall_PWM();				//初始化PWM发生器
 	LED_Init();			    	 //LED端口初始化
	LCD_Init();					//LCD初始化
	LCD_Show_Title();			//液晶屏显示内容初始化
	//KEY_Init();					//按键初始化
	hctl2020_init();			//初始化解码器
	Remote_Init();				//红外遥控初始化
	charge_init();				//弹射充电开关初始化
	shot_init();				//弹射初始化
	xianwei_init();				//限位开关初始化
	BEEP_Init();
	hongwai_init();				//红外开关初始化
	//EXTIX_Init();

	//TIM_SetCompare2(TIM5,280);	//给HCTL2020提供时钟信号，PA0
	TIM_SetCompare2(TIM5,7);
	control_init();				//机器人初始化
	TIM2_Int_Init(100-1,8400-1);//定时读取解码器，时间0.01f
	//TIM7_Int_Init(20000,8400*5-1); // 压哨投球 待完善 120s定时 
	

	while(1)
	{
		key = Remote_Scan();
		flag = 0;
		key = 0;
		chengxu = 0;
		zhongquan_case = 0;
		sanfen_case = 0;
		//选择程序
		while(1)
		{
			LCD_ShowString(30+200,400,200,16,16,"chengxu:");
			key = Remote_Scan();
			//key = KEY_Scan(0);
			switch(key)
			{
				case 0:		//没有按键按下
					
					break;
				case KEY_RIGHT:		//右
					LCD_ShowString(30+200,400,200,16,16,"qiu:    ");
					flag = 1;
					break;
				case KEY_DOWN:		//下
					LCD_ShowString(30+200,400,200,16,16,"chengxu-");
					if(chengxu != 0)
						chengxu--;
					break;
				case KEY_LEFT:		//左
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					chengxu = 0;
					break;
				case KEY_UP:		//上
					LCD_ShowString(30+200,400,200,16,16,"chengxu+");
					chengxu++;
					break;
			}
			
			LCD_ShowNum(30+200+48+8+10,320,chengxu,4,16);
			
			if(flag)
				break;
		}
		
		flag = 0;
		key = 0;
		
		//选择球
		while(1)
		{
			key = Remote_Scan();
			//key = KEY_Scan(0);
			switch(key)
			{
				case 0:		//没有按键按下
					
					break;
				case KEY_RIGHT:		//右
					LCD_ShowString(30+200,400,200,16,16,"zhongquan   ");
					flag = 1;
					break;
				case KEY_DOWN:		//下
					LCD_ShowString(30+200,400,200,16,16,"qiu-");
					if(qiu != 0)
						qiu--;
					break;
				case KEY_LEFT:		//左
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					qiu = 0;
					break;
				case KEY_UP:		//上
					LCD_ShowString(30+200,400,200,16,16,"qiu+");
					qiu++;
					break;

			}
			
			LCD_ShowNum(30+200+48+8+10,340,qiu,4,16);
			
			if(flag)
				break;
		}
		
		flag = 0;
		key = 0;
				while(1)
		{
			key = Remote_Scan();
			switch(key)
			{
				case 0:		//没有按键按下
					
					break;
				case KEY_RIGHT:		//右
					LCD_ShowString(30+200,400,200,16,16,"sanfen   ");
					flag = 1;
					break;
				case KEY_DOWN:		//下
					LCD_ShowString(30+200,400,200,16,16,"zhongquan-");
					if(zhongquan_case != 0)
						zhongquan_case--;
					break;
				case KEY_LEFT:		//左
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					zhongquan_case = 0;
					break;
				case KEY_UP:		//上
					LCD_ShowString(30+200,400,200,16,16,"zhongquan+");
					zhongquan_case++;
					break;
				case KEY_POWER:
					flag = 1;
					chengxu = 99;
					break;
			}
			
			LCD_ShowNum(30+200+48+8+10,360,zhongquan_case,4,16);
			
			if(flag)
				break;
		}
		
		
		flag = 0;
		key = 0;
				while(1)
		{
			key = Remote_Scan();
			switch(key)
			{
				case 0:		//没有按键按下
					
					break;
				case KEY_RIGHT:		//右
					LCD_ShowString(30+200,400,200,16,16,"start   ");
					flag = 1;
					break;
				case KEY_DOWN:		//下
					LCD_ShowString(30+200,400,200,16,16,"sanfen-");
					if(sanfen_case != 0)
						sanfen_case--;
					break;
				case KEY_LEFT:		//左
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					sanfen_case = 0;
					break;
				case KEY_UP:		//上
					LCD_ShowString(30+200,400,200,16,16,"sanfen+");
					sanfen_case++;
					break;
				case KEY_POWER:
					flag = 1;
					chengxu = 99;
					break;
			}
			
			LCD_ShowNum(30+200+48+8+10,380,sanfen_case,4,16);
			
			if(flag)
				break;
		}
		
		
		
		//延时10s
		//循环100次，delay_ms(40000)
		for(time = 0 ;time <20;time++)
			delay_ms(4000);
		//每次程序开始前初始化位置信息
		control_init();						//机器人初始化
		
		
		switch(chengxu)
		{
			case 0:	    	//测试程序
				switch(qiu){
					case 0:
						robot_turnOrigin_stage(30);
						//顺时针180°
						break;
					case 1:
						//机械臂下降
						jixiebi_down();
						LED0 = !LED0;
						break;
					case 2:
						//机械臂上升
						jixiebi_up();
						LED0 = !LED0;
						break;
					case 3:
						//红外测试
						get_hongwai();
						LED0 = !LED0;
						break;
					case 4:
						//弹射测试
						while(1)
						{
							time = 0;
							key = Remote_Scan();
							switch(key)
							{
								case KEY_1:
									charge(1);
									LED0 = 0;
									break;
								case KEY_2:
									charge(0);
									LED0 = 1;
									break;
								case KEY_3:
									charge(0);
									delay_ms(30000);
									GPIO_SetBits(GPIOG,GPIO_Pin_7);
									LED1 = 0;
									delay_ms(10000);
									delay_ms(10000);
									delay_ms(10000);
									GPIO_ResetBits(GPIOG,GPIO_Pin_7);
									LED1 = 1;
									break;
								case KEY_4:
									TIM_SetCompare1(TIM9,1950);		//PE6
									TIM_SetCompare2(TIM9,3990);		//PE5
									break;
								case KEY_5:
									TIM_SetCompare1(TIM9,3990);		//PE6
									TIM_SetCompare2(TIM9,3000);		//PE5
									break;
								case KEY_6:
									TIM_SetCompare1(TIM9,3990);		//PE6
									TIM_SetCompare2(TIM9,3990);		//PE5
								case KEY_7:
									time = 1;
									break;
							}
							if(time)
								break;
						}
						break;
					case 5:
						//视觉测试
					//find_ball_sanfen(1);
						find_ball(qiu);
						break;
					case 6:
						//激光测试
						find_ball_laser();
						break;
					case 7:
						//直线行走
				//	set_motor_vx_vy_w_R(50,10,5);
				//	control1_W(robot_zqd.pwm[0]);
				//	control1_W(robot_zqd.pwm[1]);
				//	control1_W(robot_zqd.pwm[2]);
					
				//	robot_certain_point(3,3,270,0,4.2,0);
					robot_straight_stage(0,4.2,90);
					robot_straight_stage(3,3,270);
				//	robot_straight_ObsAvoidance(0,6.0f,0);
				//		robot_straight_stage(0,1,0);
						break;
					case 8:
						//找篮筐测试
						find_lankuang();
						break;
					case 9:
						remote_control();
						break;
				}		
				
				break;
			case 1:
				//传球第一回合
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//             口     
				charge(1); 
				//robot_straight_stage(0,5.3,70);										//左场
				robot_straight_stage(0,5.4f,290);									//右场
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				charge(1);
				robot_turnOrigin_stage(0);
				delay_ms(30000);
				find_ball_zhongquan();			//雷达找球
				//find_ball(qiu);				//视觉找球
				delay_ms(30000);
				if(robot_zqd.X>0)
					//robot_turnOrigin_stage(90);										//左场
					robot_turnOrigin_stage(270);									//右场
				else
					//robot_turnOrigin_stage(70);										//左场
					robot_turnOrigin_stage(290);									//右场
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				//robot_straight_stage(0.08,-0.02,0);		 			//左场
				robot_straight_stage(0,0,0);									//右场
				break;
			case 2:
				//传球第二回合
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//             口     
				charge(1);
			
				//robot_straight_stage(0,5.0f,0);
				zhongquanpoint(zhongquan_case);
				delay_ms(30000);
				find_ball_sanfen(qiu); // 1是红色篮球 2是蓝色篮球
				//find_ball(qiu);					//视觉找球
				//find_ball_zhongquan();				//激光找球
			  delay_ms(30000);
				//robot_turnOrigin_stage(80);											//左场
				robot_turnOrigin_stage(280);										//右场
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				charge(1);
				//robot_straight_stage(-4.5,2.8,45);							//左场
				//robot_straight_stage(4.5,2.8,315);              //右场
				sanfenpoint(sanfen_case,zhongquan_case);
				delay_ms(30000);
				 //蓝色篮球
				//find_ball_zhongquan();				//雷达找球
				if(qiu == 1)						//视觉找球
						qiu = 2;
				else
						qiu = 1;
				//find_ball(qiu);
				find_ball_sanfen(qiu);
				//find_ball_zhongquan();
				delay_ms(30000);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y-1,0);//退后一米 
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				robot_straight_stage(0,robot_zqd.Y,0);
				robot_straight_stage(0.05f,-0.40,0);//原来 0 0 0 
				break;
			case 3:
				//传球第三回合 
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//                  口     
				charge(1);
				set_motor_vx_vy_w(0,400,0);
				control1_W(robot_zqd.pwm[0]);
				control2_W(robot_zqd.pwm[1]);
				control3_W(robot_zqd.pwm[2]);
				delay_ms(20000);
				//robot_straight_stage(-3.9,2,45);										//左场
				robot_straight_stage(3.9,2,315);									//右场
			//robot_straight_ObsAvoidance(3.9,2,315)
			//sanfenpoint(sanfen_case,100);
				delay_ms(30000);
				find_ball_sanfen(qiu);
				//find_ball_zhongquan();			//雷达找球
				//find_ball(qiu);				//视觉找球
				delay_ms(30000);
				//sanfenpoint(sanfen_case,100);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y-1,0);
				//robot_straight_ObsAvoidance(robot_zqd.X,robot_zqd.Y-1,0);
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				charge(1);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y+2,0);
				//sanfenpoint(sanfen_case,100);
				//robot_straight_stage(8.925f,3.0f,0); //进入三分线内定点 
				//robot_straight_stage(-11,3,180);									//左场
				robot_straight_stage(10.5,3,180);										//右场   
				//robot_certain_point(10.75f,0.8f,180,11,3,180)
				delay_ms(30000);
				find_ball_dixian();			//雷达找球
				//if(qiu == 1)					//视觉找球
					//qiu = 2;
				//else
					//qiu = 1;
				//find_ball(qiu);
				//find_ball_sanfen(qiu);
				delay_ms(30000);
				//robot_turnOrigin_stage(330);											//左场
				//robot_turnOrigin_stage(30);												//右场
				robot_straight_stage(robot_zqd.X,robot_zqd.Y-0.5,30);
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				robot_zqd.theta_offset = -0.05f;
				robot_straight_stage(0,robot_zqd.Y,0);
				delay_ms(30000);
				robot_straight_stage(0,-0.3,0);
				break;
			case 4:
				//投篮第一回合
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//             口     
				charge(1);
				//robot_straight_stage(-8,7.5,90);										//左场
				robot_straight_stage(8,7.5,270);										//右场
			
			//	robot_certain_point(8,7.5,270,3.5,6.5,0)
				delay_ms(30000);
				find_lankuang();
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				charge(1);
				zhongquanpoint(zhongquan_case);
				//robot_straight_stage(0,5.2,0);
				//robot_straight_ObsAvoidance(0,5.2,0);
				delay_ms(30000);
				find_ball_sanfen(qiu);
				//find_ball_zhongquan();			//雷达找球
				//find_ball(qiu);				//视觉找球
				delay_ms(30000);
				//robot_straight_stage(-8,7,90);
				//左场
				zhongquanpoint(zhongquan_case);
				robot_straight_stage(8,7,270);										//右场
				//robot_straight_ObsAvoidance(8,7,270);
				delay_ms(30000);
				find_lankuang(); 
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				robot_straight_stage(0,2,0);
				//robot_straight_stage(0.1,0,0);										//左场
				robot_straight_stage(-0.1,0,0);									//右场
				break;
			case 5:
				//投篮第二回合
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//             口     
				charge(1);
			//robot_straight_stage(0,5.0f,0);
			//robot_straight_ObsAvoidance(0,5.0f,0);
				zhongquanpoint(zhongquan_case);
				delay_ms(30000);
				find_ball_sanfen(qiu);
				//find_ball(qiu);					//视觉找球
				//find_ball_zhongquan();				//激光找球
				delay_ms(30000);
				//robot_straight_stage(-8,7,90);											//左场
				sanfenpoint(sanfen_case,zhongquan_case);
				robot_straight_stage(8,7,270);										//右场
				//robot_straight_ObsAvoidance(8,7,270);
				delay_ms(30000);
				find_lankuang();
				delay_ms(30000);
				if(down_shot_up())
					break;
				charge(1);
				delay_ms(30000);
				//robot_straight_stage(-4.5,2.8,45);										//左场
				sanfenpoint(sanfen_case,100);
				robot_straight_stage(4.5,2.8,315);									//右场
				//robot_straight_ObsAvoidance(4.5,2.8,315);
				delay_ms(30000);
				if(qiu == 3)						
					qiu = 4;
				else
					qiu = 3;
				find_ball_sanfen(qiu);
				delay_ms(30000);
				robot_straight_stage(8.925f,3.0f,0);  //进入三分线内点
				//robot_straight_stage(7.5,4.9,180);
				//sanfenpoint(sanfen_case,100);
				//robot_straight_stage(-8,7,90);											//左场
				robot_straight_stage(8,7,270);										//右场
				
				delay_ms(30000);
				find_lankuang();
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				robot_straight_stage(0,2,0);
				robot_straight_stage(0,0,0);
				break;
			case 6:
				//投篮第三回合
				// --------------- --------------
				//|     |         |       |      |
				//|  -            |         -    |
				//|-              |            - |
				// --------------- --------------
				//                  口     
				charge(1);
				set_motor_vx_vy_w(0,400,0);
				control1_W(robot_zqd.pwm[0]);
				control2_W(robot_zqd.pwm[1]);
				control3_W(robot_zqd.pwm[2]);
				delay_ms(20000);
				//robot_straight_ObsVoidance(-4.0f,2.2f,45);									//左场
				robot_straight_ObsAvoidance(4.0f,2.2f,315);										//右场
				delay_ms(30000);
				find_ball_sanfen(qiu);					//视觉找球
				delay_ms(30000);
				//robot_straight_stage(8.925f,3.0f,0);   //进入三分线内
				//robot_straight_stage(-8,7,90);										//左场
				robot_straight_stage(8,7,270);											//右场
				delay_ms(30000);
				find_lankuang();
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				charge(1);
				//robot_straight_stage(-11,3,180);									//左场
				robot_straight_stage(11,3,180);										//右场
				delay_ms(30000);
				find_ball_dixian();			//雷达找球
				//if(qiu == 3)					//视觉找球
					//qiu = 4;
				//else
					//qiu = 3;
				//find_ball_sanfen(qiu);
				//find_ball(qiu);
				delay_ms(30000);
				//robot_straight_stage(-8,7,90);											//左场
				robot_straight_stage(8,7,270);												//右场
				delay_ms(30000);
				find_lankuang();
				delay_ms(30000);
				if(down_shot_up())
					break;
				delay_ms(30000);
				robot_straight_stage(0,3,0);
				delay_ms(30000);
				robot_straight_stage(0,-0.2,0);
				break;
			case 7:
				//测试
				robot_straight_stage(0,1,0);
				break;
			case 8:
				//charge(1);
				//while(!Remote_Scan());
				//down_shot_up();
				robot_turnOrigin_stage(90);
				robot_straight_ObsAvoidance(-7,0,90);
				break;
			case 9:
				//第三回合测试程序
				charge(1);
				set_motor_vx_vy_w(0,400,0);
				control1_W(robot_zqd.pwm[0]);
				control2_W(robot_zqd.pwm[1]);
				control3_W(robot_zqd.pwm[2]);
				delay_ms(20000);
				//robot_straight_stage(-3.9,2,45);										//左场
				robot_straight_stage(3.9,2,315);										//右场
				delay_ms(30000);
				//find_ball_zhongquan( );			//雷达找球
				//find_ball(qiu);				//视觉找球
				delay_ms(30000);
				robot_straight_stage(robot_zqd.X,robot_zqd.Y-1,0);
				delay_ms(30000);
				//if(down_shot_up())
				//	break;
				delay_ms(30000);
				charge(1);
				//robot_straight_stage(-11,3,180);									//左场
				robot_straight_stage(11,3,180);										//右场
				delay_ms(30000);
				//find_ball_dixian();			//雷达找球
				//if(qiu == 1)					//视觉找球
				//	qiu = 2;
				//else
				//	qiu = 1;
				//find_ball(qiu);
				delay_ms(30000);
				//robot_turnOrigin_stage(330);											//左场
				robot_turnOrigin_stage(30);												//右场
				delay_ms(30000);
				//if(down_shot_up())
				//	break;
				delay_ms(30000);
				robot_zqd.theta_offset = -0.05f;
				robot_straight_stage(0,robot_zqd.Y,0);
				delay_ms(30000);
				robot_straight_stage(0,0,0);
				break;
		}

		
		
	}
	
}

