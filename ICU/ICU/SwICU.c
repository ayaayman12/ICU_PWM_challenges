/*
 * SwICU.c
 *
 * Created: 1/29/2020 1:53:53 PM
 *  Author: A_ayman
 */ 
#include "SwICU.h"
#include "timers.h"

void SwICU_Init(EN_SwICU_Edge_t a_en_inputCaptureEdge)
 {
     timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_256,0,0,T0_INTERRUPT_NORMAL);
	  SwICU_SetCfgEdge(a_en_inputCaptureEdge);
	  gpioPinDirection(INT_2_GPIO,INT_2_BIT,INPUT);
 }
void SwICU_SetCfgEdge(EN_SwICU_Edge_t a_en_inputCaptureEdgeedge)
 {
	  SwICU_Disable();
	  MCUCSR&=~ISC2;
	  MCUCSR|=a_en_inputCaptureEdgeedge; //set edge;
	  GIFR&=~INTF2;
	  SwICU_Enable();
	 
 }
void SwICU_Read(volatile uint8_t * a_pu8_capt)
 {
	  *( a_pu8_capt)=timer0Read();
 }
 
void SwICU_Start()
 {
	 timer0Start();
 }
void SwICU_Stop()
 {
	  timer0Stop();
 }

void SwICU_Enable()
 {
	  sei();
	  GICR|=INT2;
	 
	  
 }
void SwICU_Disable()
 {
	 
	  GICR&=~(INT2);
	   
 }