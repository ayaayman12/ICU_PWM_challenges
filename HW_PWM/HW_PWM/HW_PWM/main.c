/*
 * HW_PWM.c
 *
 * Created: 2/1/2020 2:38:01 AM
 * Author : A_ayman
 */ 

#include "gpio.h"
#include "led.h"
#include "HwPWM.h"
#include "registers.h"
#include "interrupts.h"
#include "std_types.h"
#include "SwICU.h"
#include "timers.h"
#include "util/delay.h"
#include "dcMotor.h"

int main(void)
{
	 
	 sei();
	 SwICU_Init(SwICU_EdgeRisiging);
	 Led_Init(LED_0);
	 Led_Init(LED_1);
	 Led_Init(LED_2);
	 Led_Init(LED_3);
	 gpioPinDirection(GPIOA,BIT0,OUTPUT);
	 uint16_t time_calc =0;
	 uint16_t Ov_flow_ticks =0;
	 uint16_t Ticks_Number =0;
	 uint8_t Distanc=0;
    /* Replace with your application code */
	 HwPWMInit();
	 MotorDC_Init(MOT_1);
	 MotorDC_Init(MOT_2);
	 MotorDC_Dir(MOT_1, FORWARD);
	 MotorDC_Dir(MOT_2, FORWARD);
	 HwPWMSetDuty(50,50);	
	 
    while (1) 
    {
		  	  
		  gpioPinWrite(GPIOA,BIT0,HIGH);
        _delay_ms(1);
		  gpioPinWrite(GPIOA,BIT0,LOW);
		  while(mission_complete!=2);
		  Ticks_Number=(TCNT0);
		  time_calc=Ticks_Number*16;
		  Distanc=((time_calc)/58);
		 
		  if (Distanc>=15)
		  {
			  Distanc=15;
		  }
		  if (Distanc<=5)
		  {
			   MotorDC_Dir(MOT_1, STOP);
				MotorDC_Dir(MOT_2, STOP);
				HwPWMSetDuty(0,50);		
				 _delay_ms(1000);

		  }
		 else
		 {
			   MotorDC_Dir(MOT_1, FORWARD);
			   MotorDC_Dir(MOT_2, FORWARD);
			   HwPWMSetDuty(50,50);
			  
		 }
		  gpioPinWrite(GPIOB,BIT4|BIT5|BIT6|BIT7,(Distanc<<4));
		  TCNT0=0;
		  _delay_ms(100);
		  mission_complete=0;
 		  flag=0;

		  
    }
}

