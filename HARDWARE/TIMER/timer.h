#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "hctl2020.h"
#include "lcd.h"
#include "control.h"


void TIM2_Int_Init(u32 arr,u32 psc);	//TIM2��ʱ����ʼ��
void TIM2_IRQHandler(void);				//TIM2�жϺ���
void TIM4_Int_Init(u32 arr,u32 psc);
	

#endif
