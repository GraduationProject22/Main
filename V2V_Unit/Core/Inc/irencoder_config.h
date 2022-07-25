#ifndef __IRENCODER_CONFIG_H__
#define __IRENCODER_CONFIG_H__

/*	Defining the number of slots of the wheel attached to the motor.
	This number represents the number of pulses per one cycle (The resolution).
	Since we are using IR encoder,the resolution is the number of times the IR is blocked or transmitted in one cycle.
	IRENCODER_ONE_CYCLE_PULSES would represent the number of slots in the wheel.
*/
#define IRENCODER_ONE_CYCLE_PULSES		20

/*	Defining the clock frequency of the timer in kHz.
	To calculate the RPM we need two variables; the number of turns and the time took for these turns to occur.
	So, there will be two timer peripherals activated; one will act as timer and the other will act as counter.
	This defining is for the timer.
	There are two APB timer clocks in the clock configuration.
		1) APB2 is for TIM1.
		2) APB1 is for TIM2,TIM3, and TIM4.
	So, make sure which timer peripheral is used as timer. 
	The value defined should be verifying the next formula
			0.8 < IRENCODER_TIMER_CLK
	IRENCODER_TIMER_CLK would define the clock frequency of the timer in kHz.
*/
#define IRENCODER_TIMER_CLK		8000

/*	Defining the fixed time for the RPM calculation in ms.
	This time would represent the fixed time used to sample the counts
	The value of IRENCODER_FIXED_TIME_VALUE should be verifying the next formula.
			0 < (IRENCODER_TIMER_CLK)(IRENCODER_FIXED_TIME_VALUE) <	(65535)^2
	The selection of time depends on the minimum RPM you can measure.
	IRENCODER_FIXED_TIME_VALUE should contain the time of the overflow timer in ms.
*/
#define IRENCODER_FIXED_TIME_VALUE		1000


#endif
