#include <xc.h>
#include "config.h"
#include "pwm.h"


inline void setup()
{
	// Инициализация периферии
	pwm_init();
	// Режим сна - IDLE
	IDLEN = 1;
}


void main()
{
	di();
	setup();
	ei();

	pwm_set(20,10);
	pwm_start();



	for(;;){
		// Спим до след. события
		SLEEP();
	}
}
