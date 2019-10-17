#include <xc.h>
#include "pwm.h"


// Размер 1мкс в тактах
#if _XTAL_FREQ%4000000!=0
	#error "Invalid _XTAL_FREQ"
#endif
#define PWM_STEP_1US  (_XTAL_FREQ/4000000)


struct {
	uint16_t  ton;
	uint16_t  toff;
} pwm;


inline void pwm_init()
{
	// Подаем питание на периферию
	TMR1MD = 0;
	CCP1MD = 0;
	CCP2MD = 0;
	CLC1MD = 0;
	/* Настраиваем таймер. Тактирование FOSC/4 */
	T1CON  = 0x04;
	T1GCON = 0x00;
	/* Настраиваем CCP1. Сравнение по Toff */
	CCP1CON = 0x0A;
	CCP1IE  = 0;
	CCPTMRSbits.C1TSEL = 0x01;
	/* Настраиваем CCP2. Сравнение по Ton+Toff, сброс таймера */
	CCP2CON = 0x0B;
	CCP2IE  = 0;
	CCPTMRSbits.C2TSEL = 0x01;
	/* Настраиваем CLC1. SR-триигер, фронт по CCP1, спад по CCP2 */
	CLC1CON  = 0x03;
	CLC1SEL0 = 0x0C; // CCP1
	CLC1SEL1 = 0x0D; // CCP2
	CLC1GLS0 = 0x02;
	CLC1GLS1 = 0x00;
	CLC1GLS2 = 0x08;
	CLC1GLS3 = 0x00;
	CLC1POL  = 0x00;
	CLC1IE   = 0x00;
	/* Параметры по-умолчанию. Ton=1us, Toff=1us */
	pwm.ton  = PWM_STEP_1US;
	pwm.toff = PWM_STEP_1US;
}

void pwm_start()
{
	// ШИМ уже запущен
	if (TMR1ON) return;
	// Настраиваем CCP
	CCPR1 = pwm.toff;
	CCPR2 = pwm.ton+pwm.toff;
	// Настраиваем таймер за 1 такт до CCP2
	TMR1  = CCPR2-1;
	// Запускаем CCP/CLC
	CCP1EN = 1;
	CCP2EN = 1;
	LC1EN  = 1;
	// Запускаем таймер
	TMR1ON = 1;
}

void pwm_stop()
{
	// Останавливаем периферию
	LC1EN  = 0;
	TMR1ON = 0;
	CCP1EN = 0;
	CCP2EN = 0;
}


void pwm_set(uint16_t ton, uint16_t toff)
{
	// Расчет новых значений
	pwm.ton  = ton*PWM_STEP_1US;
	pwm.toff = toff*PWM_STEP_1US;
	// ШИМ запущен
	if (TMR1ON){
		// Останавливаем таймер и CLC
		TMR1ON = 0;
		LC1EN  = 0;
		// Настраиваем CCP, таймер
		CCPR1  = pwm.toff;
		CCPR2  = pwm.ton+pwm.toff;
		TMR1   = CCPR2-1;
		// Запускаем таймер и CLC
		LC1EN  = 1;
		TMR1ON = 1;
	}
}
