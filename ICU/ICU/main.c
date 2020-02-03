/*
 * ICU.c
 *
 * Created: 1/29/2020 1:21:57 PM
 * Author : A_ayman
 */ 

#include "gpio.h"
#include "interrupts.h"
#include "timers.h"
#include "led.h"
#include "SwICU.h"
#include "util/delay.h"

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
	 //gpioPortDirection(GPIOC,OUTPUT);
	 

  while (1) 
    {		
		  gpioPinWrite(GPIOA,BIT0,HIGH);
		  _delay_ms(1);
		  gpioPinWrite(GPIOA,BIT0,LOW);
	     while(mission_complete!=2);
   	 // Ov_flow_ticks =ov_counts*256;
		  Ticks_Number=(TCNT0);
		  time_calc=Ticks_Number*16;
		  Distanc=((time_calc)/58);
     	  if (Distanc>=15)
			{
				 Distanc=15;
			}
	  
		  gpioPinWrite(GPIOB,BIT4|BIT5|BIT6|BIT7,(Distanc<<4));

		 TCNT0=0;
		 _delay_ms(100);
		 mission_complete=0;
		 flag=0;
		 
	   
			 
		  
		  
		  
   }
}

