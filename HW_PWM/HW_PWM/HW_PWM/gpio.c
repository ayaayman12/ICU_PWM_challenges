/*
 * gpio.c
 *
 * Created: 1/18/2020 12:42:37 AM
 *  Author: A_ayman
 * Latest version.
 */ 
#include "gpio.h"
#include "registers.h"

/* This function determines the PORTx Direction either INPUT or OUTPUT by assigning that value to the PORTx DIR*/
void gpioPortDirection(uint8_t u8_port, uint8_t u8_direction)
{
	 switch(u8_port)
	 {
		 case GPIOA: PORTA_DIR = u8_direction;break;
		 case GPIOB: PORTB_DIR = u8_direction;break;
		 case GPIOC: PORTC_DIR = u8_direction;break;
		 case GPIOD: PORTD_DIR = u8_direction;break;
		 default: break;
	 }
}

/* This function assign the PORTx Data with the desired value */

void gpioPortWrite(uint8_t u8_port, uint8_t u8_value)
{
	switch(u8_port)
	{
		case GPIOA: PORTA_DATA = u8_value;break;
		case GPIOB: PORTB_DATA = u8_value;break;
		case GPIOC: PORTC_DATA = u8_value;break;
		case GPIOD: PORTD_DATA = u8_value;break;
		default:break;
	}
}

/* This function toggling the PORTx DATA by XORing the value of the PORTx DATA with 0xff */

void gpioPortToggle(uint8_t u8_port)
{
	switch(u8_port)
	{
		case GPIOA: PORTA_DATA ^= (0xff);break;
		case GPIOB: PORTB_DATA ^= (0xff);break;
		case GPIOC: PORTC_DATA ^= (0xff);break;
		case GPIOD: PORTD_DATA ^= (0xff);break;
		default: break;
	}
}

/* This function only get the stored value in the PORTx PIN and returns it */

uint8_t gpioPortRead(uint8_t u8_port)
{
	uint8_t port_value=-1;
	switch(u8_port)
	{
		case GPIOA: port_value = PORTA_PIN;break;
		case GPIOB: port_value = PORTB_PIN;break;
		case GPIOC: port_value = PORTC_PIN;break;
		case GPIOD: port_value = PORTD_PIN;break;
		default: break;
	}
	return port_value;
}

/* This function determine the direction of a specific Pin or pins by ANDing the PORTx Direction with NOT Pins Mask if the Required Direction is INPUT */
/* and ORing the PORTx Direction with Pins Mask if the Required Direction is OUTPUT */

void gpioPinDirection(uint8_t u8_port, uint8_t u8_pins, uint8_t u8_direction)
{
	if( u8_direction == INPUT)
	{
		switch(u8_port)
		{
			case GPIOA: PORTA_DIR &= ~(u8_pins);break;
			case GPIOB: PORTB_DIR &= ~(u8_pins);break;
			case GPIOC: PORTC_DIR &= ~(u8_pins);break;
			case GPIOD: PORTD_DIR &= ~(u8_pins);break;
			default: break;
		}
	}
	
	else if( u8_direction == OUTPUT)
	{
		switch(u8_port)
		{
			case GPIOA: PORTA_DIR |= (u8_pins);break;
			case GPIOB: PORTB_DIR |= (u8_pins);break;
			case GPIOC: PORTC_DIR |= (u8_pins);break;
			case GPIOD: PORTD_DIR |= (u8_pins);break;
			default: break;
		}
	}
}

/* This function based on clearing the required Pins int the port by ANDing the PORTx DATA with NOT Pin mask*/
/* After that we make sure of the value by ANDing the Pin Mask with the desired value*/
/* Finally ORing PORTx DATA with the value*/
void gpioPinWrite(uint8_t u8_port, uint8_t u8_pins, uint8_t u8_value)
{
	uint8_t temp_reg;
	switch(u8_port)
	{
		case GPIOA:
		{
			 temp_reg = PORTA_DATA & ~(u8_pins);
			 u8_value = (u8_pins & u8_value);
			 PORTA_DATA = temp_reg |u8_value ;
			 break;
		}
		case GPIOB:
		{ 
			temp_reg = PORTB_DATA & ~(u8_pins);
			u8_value = (u8_pins & u8_value);
			PORTB_DATA = temp_reg | ( u8_value);
			break;
		}
		case GPIOC:
		{
			temp_reg = PORTC_DATA & ~(u8_pins);
			u8_value = (u8_pins & u8_value);
			PORTC_DATA = temp_reg | ( u8_value);
			break;
		}
		case GPIOD:
		{
			temp_reg = PORTD_DATA & ~(u8_pins);
			u8_value =(u8_pins & u8_value);
			PORTD_DATA = temp_reg | (u8_value);
			break;
		}
		default: break;
	}	
}
/* When you XORing PORTx DATA value withe the required Pin to toggle, that changes only the value of the required Pin*/
void gpioPinToggle(uint8_t u8_port, uint8_t u8_pins)
{
	switch(u8_port)
	{
		case GPIOA: PORTA_DATA ^= u8_pins;break;
		case GPIOB: PORTB_DATA ^= u8_pins;break;
		case GPIOC: PORTC_DATA ^= u8_pins;break;
		case GPIOD: PORTD_DATA ^= u8_pins;break;
		default: break;
	}
}

/* When you anding PORTx PIN value withe the required Pin to read it returns either 0 or pin number*/
/* we consider 0 as Low and any other value as High*/
uint8_t gpioPinRead(uint8_t u8_port, uint8_t u8_pin)
{
	uint8_t u8_pin_value=-1;
	switch(u8_port)
	{
		case GPIOA: u8_pin_value = (PORTA_PIN & u8_pin);break;
		case GPIOB: u8_pin_value = (PORTB_PIN & u8_pin);break;
		case GPIOC: u8_pin_value = (PORTC_PIN & u8_pin);break;
		case GPIOD: u8_pin_value = (PORTD_PIN & u8_pin);break;
		default:break;
	}
	if(u8_pin_value == 0)
		return LOW;
	else
		return HIGH;
}
