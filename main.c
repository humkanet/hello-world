#include <xc.h>
#include "config.h"
#include "pwm.h"
#include "clock.h"
#include "button.h"
#include "encoder.h"


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


void button_event(BUTTON *btn, BUTTON_EVENT e)
{
	// Если кнопка зажата вниз - запускаем ШИМ,
	// иначе останавливаем
	if (e==BUTTON_EVENT_DOWN) pwm_start();
	else pwm_stop();
}


void main()
{
	di();
	setup();
	ei();

	// Кнопка, пин RC2
	BUTTON btn = {
		.port  = BUTTON_PORTC,
		.pin   = 2,
		.event = button_event
	};
	button_init(&btn);

	// ШИМ: Ton=20мсек, Toff=10мсек
	pwm_set(20,10);

	for(;;){
		// Считываем системные часы
		uint16_t msec = clock_msec();
		// Обрабатываем кнопку
		button_tick(&btn, msec);		
		// Спим до след. события
		SLEEP();
	}
}
