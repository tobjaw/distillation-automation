#include "stepper.h"

#include <avr/io.h>
#include "compat.h"
#include "api.h"


#define	STEPPIN PL3            //Define Step pin
#define DIRPIN PL0             //Define Direction pin
#define STEPENABLE PL6              //Define Enable pin

void inline doStep(int pulswidth){
	PORTL |=(1<<STEPPIN);
	_sleep(pulswidth);
	PORTL &=~(1<<STEPPIN);
	_sleep(pulswidth);
}

void rotateStepper(int steps, rotDirection direction, int pulswidth){
	
	if(direction == CW){
		PORTL |= (1<<DIRPIN); 
	} else {
		PORTL &= ~(1<<DIRPIN);
	}
	
	for(int y=0; y<steps; y++)
	{
		doStep(pulswidth);
	}
}

void initStepper(void){
	
	DDRL |= (1<<STEPPIN)|(1<<DIRPIN)|(1<<STEPENABLE);     // Configure PORTD5, PORTD6, PORTD7 as output
	PORTL &= ~(1<<STEPENABLE);
}




void steppertest(void)
{
	initStepper();
	
	for(int j = 0; j<6; j++){
		for(int i=0;i<50;i++){
			rotateStepper(5,CCW,2);
			_sleep(100);
			rotateStepper(1,CW,2);
			_sleep(100);
		}
	}
}
