#include <xc.h>
#include "main.h"
#include "config.h"
#include "pwm.h"
#include "clock.h"
#include "button.h"
#include "encoder.h"
#include "pins.h"


#define FLAG_PWM_TON_CHANGED   0x01
#define FLAG_PWM_TOFF_CHANGED  0x02


struct {
	uint8_t  pwm_ton;
	uint8_t  pwm_toff;
	uint8_t  flags;
} run = {
	.pwm_ton  = PWM_TON_MIN,
	.pwm_toff = PWM_TOFF_MAX,
	.flags    = 0x00
};


void __interrupt() isr()
{
	clock_isr();
	encoder_isr();
}


inline void setup()
{
	// Инициализация периферии
	pwm_init();
	clock_init();
	encoder_init();
	button_init();
	// Режим сна - IDLE
	IDLEN = 1;
	// Разрешаем прерывания от периферии
	PEIE  = 1;
}


void button_callback(BUTTON *btn, BUTTON_EVENT e)
{
	// Если кнопка зажата вниз - запускаем ШИМ,
	// иначе останавливаем
	if (e==BUTTON_EVENT_DOWN) pwm_start();
	else pwm_stop();
}


void encoder1_callback(int8_t cnt)
{
	uint8_t  d;
	int8_t   inc = cnt;
	// Определяем отступ от границ
	if (cnt>0){
		d = PWM_TON_MAX-run.pwm_ton;
	}
	else{
		d   = run.pwm_ton-PWM_TON_MIN;
		inc = -inc;
	}		
	// Расчитывае новое значение
	if (d){
		// Выбираем максимально возможное изменение
		if (inc>d) inc = d;
		// Изменяем значение
		if (cnt<0) inc = -inc;
		run.pwm_ton += inc;
		// Флажок изменения параметров ШИМ
		run.flags |= FLAG_PWM_TON_CHANGED;		
	}
}


void encoder2_callback(int8_t cnt)
{
	uint8_t  d;
	int8_t   inc = cnt;
	// Определяем отступ от границ
	if (cnt>0){
		d = PWM_TOFF_MAX-run.pwm_toff;
	}
	else{
		d   = run.pwm_toff-PWM_TOFF_MIN;
		inc = -inc;
	}		
	// Расчитывае новое значение
	if (d){
		// Выбираем максимально возможное изменение
		if (inc>d) inc = d;
		// Изменяем значение
		if (cnt<0) inc = -inc;
		run.pwm_toff += inc;
		// Флажок изменения параметров ШИМ
		run.flags |= FLAG_PWM_TOFF_CHANGED;		
	}
}


void main()
{
	di();
	setup();
	ei();

	// Кнопка, пин RC2
	BUTTON btn = {
		.pin      = HAL_MK_PIN(BUTTON_PORT, BUTTON_PIN),
		.callback = button_callback
	};
	button_append(&btn);

	// Энкодер1, изменение Ton ШИМ
	ENCODER enc1 = {
		.pina     = HAL_MK_PIN(ENCODER1_PORTA, ENCODER1_PINA),
		.pinb     = HAL_MK_PIN(ENCODER1_PORTB, ENCODER1_PINB),
		.callback = encoder1_callback
	};
	// Энкодер2, изменение Toff ШИМ
	ENCODER enc2 = {
		.pina     = HAL_MK_PIN(ENCODER2_PORTA, ENCODER2_PINA),
		.pinb     = HAL_MK_PIN(ENCODER2_PORTB, ENCODER2_PINB),
		.callback = encoder2_callback
	};
	encoder_append(&enc1);
	encoder_append(&enc2);

	// Устанавливаем параметры ШИМ по-умолчанию
	pwm_set(run.pwm_ton, run.pwm_toff);

	for(;;){
		// Считываем системные часы
		uint16_t msec = clock_msec();
		// Обрабатываем кнопки
		button_tick(msec);
		// Обрабатываем энкодеры
		encoder_tick();
		// Применяем новые параметры ШИМ
		if (run.flags&(FLAG_PWM_TON_CHANGED|FLAG_PWM_TOFF_CHANGED)){
			// Устанавливаем новые параметры ШИМ
			pwm_set(run.pwm_ton, run.pwm_toff);
			// Убираем флаги изменения параметров ШИМ
			run.flags &= ~(FLAG_PWM_TON_CHANGED|FLAG_PWM_TOFF_CHANGED);
		}
		// Спим до след. события
		SLEEP();
	}
}
