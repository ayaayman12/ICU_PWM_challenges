/*
 * HwPWM.c
 *
 * Created: 2/1/2020 2:40:20 AM
 *  Author: A_ayman
 */ 

#include "HwPWM.h"
void HwPWMInit(void)
{
	 gpioPinDirection(GPIOD,BIT5,OUTPUT); //set oc1 as output
	 gpioPinDirection(GPIOD,BIT4,OUTPUT);
	 timer1Init(T1_PhaseCorrect_MODE_ICR1_TOP ,T1_OC1A_CLEAR|T1_OC1B_CLEAR,T1_PRESCALER_256, 0, 0,0,0, T1_POLLING);

}

void HwPWMSetDuty(uint8_t a_u8_duty, uint32_t a_u32_frequency)
{
	 uint32_t OCR1A_counts=0;
	 switch (a_u32_frequency)
	 {
		 /*We Get PWM_PERIOD in sec from the given frequency, As time =(1/frequency)*/
		 case (T0_FERQ_50) :
		 {
			   ICR1 =1250;
				OCR1A_counts=12.5*a_u8_duty;
			   break;
		 }
		 case (T0_FERQ_100): ICR1 =10000; break;
// 		 case (T0_FERQ_200): pwm_period=0.005;  break;
	 }
	 
	
	 OCR1A=OCR1A_counts;
	 OCR1B=OCR1A_counts;

	 
	 //(a_u8_duty*20000)/100.0
	 timer1Start();
	 
}
En_timer0perscaler_t timer_pwm_prescaler;
// void HwPWMInit(void)
// {
// 	 gpioPinDirection(GPIOB,BIT3,OUTPUT); //set oc0 as output
// 	 timer0Init(T0_PhaseCorrect_MODE,T0_OC0_SET,T0_PRESCALER_64,0,0,T0_INTERRUPT_NORMAL);
// }
/* 
/** this function based on F_CPU =16000000 
*/
//  void HwPWMSetDuty(uint8_t a_u8_duty, uint32_t a_u32_frequency)
 //  {
 // 	  uint8_t timer_freq=0;
 // 	  OCR0=a_u8_duty*2.56;
 // 	  timer_freq=(a_u32_frequency*256.0);
 // 	  switch (a_u32_frequency)
 // 	  {
 // 		  case (T0_FERQ_1) :
 // 		  {
 // 				pwm_period=0.01;
 // 				timer_pwm_prescaler=
 // 				
 // 				break;
 // 		  }
 // 		  
 // 		  case (T0_FERQ_100): pwm_period=0.01; break;
 // 		  case (T0_FERQ_200): pwm_period=0.005;  break;
 // 	  }
 // 	  
 //  }