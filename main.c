#include <xc.h>
#include "config.h"
#include "pwm.h"
#include "clock.h"


void __interrupt() isr()
{
	clock_isr();
}


inline void setup()
{
	// Инициализация периферии
	pwm_init();
	clock_init();
	// Режим сна - IDLE
	IDLEN = 1;
	// Разрешаем прерывания от периферии
	PEIE  = 1;
}


void main()
{
	di();
	setup();
	ei();

	// ШИМ: Ton=20мсек, Toff=10мсек
	pwm_set(20,10);

	for(;;){
		// На нечетных секундах ШИМ запускаем, на четных останавливаем
		uint8_t sec = (uint8_t) clock_sec();
		if (sec&0x01) pwm_start();
		else pwm_stop();
		// Спим до след. события
		SLEEP();
	}
}
