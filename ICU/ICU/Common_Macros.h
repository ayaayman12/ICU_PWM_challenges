/*
 * Common_Macros.h
 *
 * Created: 1/18/2020 12:47:50 AM
 *  Author: A_ayman
 */ 


#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

#define SET_BIT(Reg,Bit_No) (Reg|=(1<<Bit_No))
#define CLEAR_BIT(Reg,Bit_No) (Reg&=~(1<<Bit_No))
#define TOGGLE_BIT(Reg,Bit_No) (Reg^=(1<<Bit_No))
#define GET_BIT(Reg,Bit_No) (Reg>>Bit_No)&1)




#endif /* COMMON_MACROS_H_ */