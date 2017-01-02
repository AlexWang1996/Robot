#include "timer.h"
#include "usart.h"

u8 count = 0;
float ll1[4]={0,0,0,0};
float ll2[4]={0,0,0,0};
float ll3[4]={0,0,0,0};


void TIM2_Int_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM2时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	int16_t l1=0,l2=0,l3=0,temp=0;
	//u8 i = 0;

	
	//printf("中断");
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{	
		if(count%3 == 0)
		{
			l1= hctl2020_getdata_0();
			l2= hctl2020_getdata_1();
			l3= hctl2020_getdata_2();
		}
		else if(count%3 == 1)
		{
			l2= hctl2020_getdata_1();
			l3= hctl2020_getdata_2();
			l1= hctl2020_getdata_0();
		}
		else if(count%3 == 2)
		{
			l3= hctl2020_getdata_2();
			l1= hctl2020_getdata_0();
			l2= hctl2020_getdata_1();
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
	
	
	
	/*
	if(abs(l1-ll1[3])>200)
	{
		l1 = ll1[3] + (ll1[0]+ll1[1]+ll1[2])/3;
	}	 
	for(i = 0; i < 2; i++)
	{
		ll1[i] = ll1[i+1];
	}
	ll1[2] = l1-ll1[3];
	ll1[3] = l1;
	
	
	if(abs(l2-ll2[3])>200)
	{
		l2 = ll2[3] + (ll2[0]+ll2[1]+ll2[2])/3;
	}	 
	for(i = 0; i < 2; i++)
	{
		ll2[i] = ll2[i+1];
	}
	ll2[2] = l2-ll2[3];
	ll2[3] = l2;
	
	
	if(abs(l3-ll3[3])>200)
	{
		l3 = ll3[3] + (ll3[0]+ll3[1]+ll3[2])/3;
	}	 
	for(i = 0; i < 2; i++)
	{
		ll3[i] = ll3[i+1];
	}
	ll3[2] = l3-ll3[3];
	ll3[3] = l3;
	*/
	
	
	robot_zqd.w[2] += l3/4;
	robot_zqd.w[1] += l2/4;
	robot_zqd.w[0] += l1/4;    
	
	/*
	//测试用，通过串口发送里程计数据
	USART_SendData(USART1, 0x11);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	USART_SendData(USART1, (l2>>8)&0xff);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	USART_SendData(USART1, (l2&0xff));
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	*/
	
	
	robot_zqd.v[0] = l1*RAD*BMQ_R;
	robot_zqd.v[1] = l2*RAD*BMQ_R;			//1.0233173f
	robot_zqd.v[2] = l3*RAD*BMQ_R;
	
	
	if(receive2)
	{
		if(USART2_RX_STA&0x8000)
		{
			temp = USART2_RX_BUF[7];
			robot_zqd.theta = ((float)((temp<<8)|USART2_RX_BUF[6]))/32768*180;
			receive2 = 0;
			USART2_RX_STA=0;
			
			robot_zqd.theta = robot_zqd.theta * PI / 180 + robot_zqd.theta_offset;
			
			while(robot_zqd.theta < 0)
				robot_zqd.theta  = robot_zqd.theta + PI + PI;
			
			while (robot_zqd.theta > 2 * PI)
				robot_zqd.theta = robot_zqd.theta - PI - PI;
		}
	}
	
	
	
	get_position();
	
	
	
	
	
	if(count++ == 0)
	{
		LCD_Show_lcj();
	}
	else if(count == 2)
	{
		LCD_Show_v();
	}
	else if(count == 3)
	{
		LCD_Show_V();
	}
	else if(count == 4)
	{
		LCD_Show_position();
	}
	
		
	
}



