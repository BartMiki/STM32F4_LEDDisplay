/**
  ******************************************************************************
  * @file    main.c
  * @author  Bartosz Mikulski
  * @nick	 BartMiki
  * @brief   Created in Open Workbench with STD_Periph library
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


int main(void)
{

	// ---- Enabling GPIO ----

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// ---- OutPut pins PBxx configuration ----
	GPIO_InitTypeDef LEDs;
	LEDs.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	LEDs.GPIO_Mode = GPIO_Mode_OUT;
	LEDs.GPIO_OType = GPIO_OType_PP;
	LEDs.GPIO_Speed = GPIO_Speed_100MHz;
	LEDs.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &LEDs);

	// ---- Timer set to count 1Hz ( 1s ) ----
	TIM_TimeBaseInitTypeDef Timer;
	Timer.TIM_Period = 9999;
	Timer.TIM_Prescaler = 8399;
	Timer.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &Timer);

	// ---- Enabling Timer ----
	TIM_Cmd(TIM2,ENABLE);

	/* ---- How to connect board to LED Display ----
	 *
	 * PB12 and PB13 are used for enabling displays
	 *
	 * LED Segment | PBxx
	 * 			 a - 0
	 * 			 b - 1,
	 * 			 c - 2,
	 * 			 d - 4,
	 * 			 e - 5,
	 * 			 f - 7,
	 * 			 g - 8,
	 * 			 h - 11
	*/

	// ---- Table of Numbers ----
	uint16_t numbers [] = {
		/* -0- */	GPIO_Pin_8,
		/* -1- */	GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8,
		/* -2- */	GPIO_Pin_2 | GPIO_Pin_7,
		/* -3- */	GPIO_Pin_5 | GPIO_Pin_7,
		/* -4- */	GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5,
		/* -5- */	GPIO_Pin_1 | GPIO_Pin_5,
		/* -6- */	GPIO_Pin_1,
		/* -7- */	GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8,
		/* -8- */	(uint16_t)0,
		/* -9- */	GPIO_Pin_5
	};

	unsigned int i = 0;
	unsigned int counter;

	// ---- Displays enabling ----
	GPIO_ToggleBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);

	// ---- Infinite loop ----
	for(;;){
		// ---- Reading Period's content ----
		counter = TIM2->CNT;

		// ---- Dot flickering (0.5s) ---
		if(counter<5000){
			GPIO_SetBits(GPIOB, GPIO_Pin_11);
		}
		else{
			GPIO_ResetBits(GPIOB, GPIO_Pin_11);
		}

		// ---- Number changing ----
		if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)){
			for(i%=10;;){
				GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8);
				GPIO_SetBits(GPIOB,numbers[i]);
				i++;
				break;
				}
			TIM_ClearFlag(TIM2,TIM_FLAG_Update);
			}
	}
}

