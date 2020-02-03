/*
 * timers.c
 *
 * Created: 1/20/2020 10:53:02 PM
 *  Author: A_ayman
 */ 

#include "timers.h"
#include "gpio.h"
#include "led.h"
#include "SwICU.h"


typedef struct
{
	uint64_t delay_counts;
	uint16_t OV_count;
	uint8_t timer_init_value;
	uint8_t timer1_init_value;
	
}delay_parameters_t;
volatile uint8_t Gu8_dutyCycle=0;
uint16_t  gu16_Timer1_Prescaler=0;
uint16_t  gu16_Timer1_Value=0;
volatile static  double interrupt_OV_time = 0;
volatile uint8_t Timer_initial_value;

volatile uint8_t ov_counts=0;
uint8_t prescal=0;
volatile double Ton=0;
volatile static double  Toff=0;
volatile double pwm_period=0;
volatile uint8_t   mission_complete=0;
volatile uint8_t flag=0;
volatile uint8_t xvz=0;
void timer0Init(En_timer0Mode_t en_mode,En_timer0OC_t en_OC0,En_timer0perscaler_t en_prescal, uint8_t u8_initialValue, uint8_t u8_outputCompare, En_timer0Interrupt_t en_interruptMask)
{
	TCCR0|=0x80;
	TCCR0|=en_mode;
	TCCR0|=en_OC0;
	prescal=en_prescal;
	//re_prescal=en_prescal;
	TCNT0=u8_initialValue;
	OCR0=u8_outputCompare;
	TIMSK|=en_interruptMask;
}

void timer0Set(uint8_t u8_value)
{
	TCNT0=u8_value; 
}

uint8_t timer0Read(void)
{
	volatile uint8_t timer0_Read= TCNT0;
	 return timer0_Read;
}
void timer0Start(void)
{
	TCCR0|=prescal;
}

void timer0Stop(void)
{
	TCCR0&=~(0x07);
	TCCR0|=T0_NO_CLOCK;
	
}
	
void timer0DelayMs(uint16_t u16_delay_in_ms)
{
	/* this function based on prescaller =8 and fcpu=8Mhz*/
	/* timer freq 1 MHZ */
	/* each tick =1us */
	uint16_t i=0;
	uint16_t j=0;
	delay_parameters_t timer_delay ;
	switch (prescal)
	{
		case (T0_PRESCALER_8):
		timer_delay.delay_counts=1000; //it will count 2000 ticks to get 1 msec
		timer_delay.OV_count=3;
		timer_delay.timer_init_value=24;
		break;
		default:
		break;
	}	
	timer0Start();
	for (i=0;i<u16_delay_in_ms;i++)
	{
		TCNT0=timer_delay.timer_init_value;	
		for( j=0;j<=timer_delay.OV_count;j++)
		{
			//while((TIFR>>TOV0)&1==0);
			while((TIFR & 1)!=1);
			TIFR|=0x01;
		}
	}
	timer0Stop();
}

void timer0DelayUs(uint32_t u32_delay_in_us)
{
	/* this function based on prescaller =8 and fcpu=8 Mhz*/
	/* timer freq 1 MHZ */
	/* each tick =1 us */
	/* this function take around 11.84 us for calling and context switching so we will not be able to get the exact delay for values less than 12 us*/
	uint16_t j=0;
	delay_parameters_t timer_delay ;
	timer0Start();
	if (u32_delay_in_us<15)
	{
		
	}
	else if (20<=u32_delay_in_us&&u32_delay_in_us<=256)
	{
		TCNT0=272-u32_delay_in_us;
		while((TIFR & 1)!=1);
		TIFR|=0x01;
	}
	
	
	else
	{
		switch  (prescal)
		{
			case (T0_PRESCALER_8):
			timer_delay.delay_counts=(u32_delay_in_us-15);
			timer_delay.OV_count=(timer_delay.delay_counts)/256;
			timer_delay.timer_init_value=255-((timer_delay.delay_counts)%256);
			break;
			default:
			break;
		}
		TCNT0=timer_delay.timer_init_value;
		for( j=0;j<=timer_delay.OV_count;j++)
		{
			//while((TIFR>>TOV0)&1==0);
			while(!(TIFR & 1));
			TIFR|=0x01;
		}
	}		
}



/* this function based on f_cpu= 16 MHZ, NO timer_prescaller, So if you want to change any of them you have to recheck the values*/
void timer0SwPWM(uint8_t u8_dutyCycle,uint8_t u8_frequency)
{	
   Gu8_dutyCycle=u8_dutyCycle;
	switch (u8_frequency)
	{
		 /*We Get PWM_PERIOD in sec from the given frequency, As time =(1/frequency)*/
		 case (T0_FERQ_50) : pwm_period=0.02; break;
		 case (T0_FERQ_100): pwm_period=0.01; break;
		 case (T0_FERQ_200): pwm_period=0.005;  break;
	}
	interrupt_OV_time=(pwm_period/100.0);
	uint16_t interrupt_counts = interrupt_OV_time/(64/16000000.0); 
	Timer_initial_value= 256-interrupt_counts;//200
	timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_64,Timer_initial_value,0,T0_INTERRUPT_NORMAL);
	timer0Start();
	TCNT0=Timer_initial_value;
	gpioPinWrite(T0_PWM_GPIO,T0_PWM_BIT,HIGH);
}

// ISR(TIMER0_OVF_vect)
// {	
// 	 if((ov_counts ) == (Gu8_dutyCycle)) ////
// 	 {
// 	 		gpioPinWrite(T0_PWM_GPIO,T0_PWM_BIT,LOW) ;
// 		   Led_On(LED_0);
// 	 }
// 	 else if(ov_counts ==100)
// 	 {
// 		  ov_counts=0;
// 		  gpioPinWrite(T0_PWM_GPIO,T0_PWM_BIT,HIGH) ;
// 		  Led_Off(LED_0);
// 	 }
// 	 ov_counts++;
// 	 TCNT0=Timer_initial_value;
// }
/////////////////////////////timer1///////////////////////
void timer1Init(En_timer1Mode_t en_mode,En_timer1OC_t en_OC,En_timer1perscaler_t en_prescal, uint16_t u16_initialValue, uint16_t u16_outputCompareA, uint16_t u16_outputCompareB,uint16_t u16_inputCapture, En_timer1Interrupt_t en_interruptMask)
{
	 TCCR1|=en_mode;
	 TCCR1|=en_OC;
    gu16_Timer1_Prescaler=en_prescal;
	 TCNT1=u16_initialValue;
	 OCR1A=u16_outputCompareA;
	 OCR1B=u16_outputCompareB;
	 ICR1=u16_inputCapture;
	 TIMSK|=en_interruptMask;
}
void timer1Set(uint16_t u16_value)
{
	 timer1Stop();
    TCNT1=u16_value;
	 timer1Start();
}

void timer1Start(void)
{
	 TCCR1|=gu16_Timer1_Prescaler;
	 	 
}
void timer1Stop(void)
{
	 TCCR1&=~(0x0007);
	 TCCR1|=T0_NO_CLOCK;
	 
}
uint16_t timer1Read(void)
{
	 timer1Stop();
	 gu16_Timer1_Value=TCNT1;
	 timer1Start();
	 return  gu16_Timer1_Value;	 
}
void timer1DelayMs(uint16_t u16_delay_in_ms)
{
	/* this function based on prescaller =8 and fcpu=16Mhz*/
	/* timer freq 1 MHZ */
	/* each tick =1us */
	uint16_t i=0;
	uint16_t j=0;
	uint16_t timer1_init_value=0;
	delay_parameters_t timer1_delay ;
	switch (gu16_Timer1_Prescaler)
	{
		case (T0_PRESCALER_8):
		timer1_delay.delay_counts=2000; //it will count 2000 ticks to get 1 msec
		timer1_delay.OV_count=0;
		timer1_init_value=63536;
		break;
		default:
		break;
	}
	
	for (i=0;i<u16_delay_in_ms;i++)
	{
	   timer1Stop();
		TCNT1=timer1_init_value;
		timer1Start();
		for( j=0;j<=timer1_delay.OV_count;j++)
		{
			//while((TIFR>>TOV1)&1==0);
			while(((TIFR>>2)&1)!=1);
			TIFR|=0x04;
		}
	}
	timer1Stop();
}
ISR(TIMER0_OVF_vect)
{
	 ov_counts++;
}

ISR(INT2_vect)
{
	 //Led_Toggle(LED_0);
	 
	 switch (flag)
	 {
		  case 0:
		  Led_On(LED_0);
		  SwICU_Start();
		  SwICU_SetCfgEdge(SwICU_EdgeFalling);
		  flag=1;
		  break;
		  
		  case 1:
		  {
				SwICU_Stop();
				 SwICU_SetCfgEdge(SwICU_EdgeRisiging);
				Led_Off(LED_0);
				flag=2;		
				mission_complete=2;
		  }
		  
		  break;
		  
		  
	 }
	 
	 
}