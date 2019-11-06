#include <xc.h>
#include "hal.h"


void pin_config(HAL_PIN pin, uint8_t cfg)
{
	uint8_t  mask  = 1<<__HAL_PIN(pin);
	uint8_t  imask = ~mask;
	switch(__HAL_PORT(pin)){
		#ifdef PORTA
		case HAL_PORTA:
			#ifdef WPUA
			if (cfg&HAL_PIN_PULLUP) WPUA |= mask;
			else WPUA &= imask;
			#endif
			#ifdef ODCONA
			if (cfg&HAL_PIN_OPENDRAIN) ODCONA |= mask;
			else ODCONA &= imask;
			#endif
			#ifdef TRISA
			if (cfg&HAL_PIN_TRISTATE) TRISA |= mask;
			else TRISA &= imask;
			#endif
			#ifdef ANSELA
			if (cfg&HAL_PIN_ANALOG) ANSELA |= mask;
			else ANSELA &= imask;
			#endif
			#ifdef SLRCONA
			if (cfg&HAL_PIN_SLEWRATE) SLRCONA |= mask;
			else SLRCONA &= imask;
			#endif
			#ifdef LATA
			if (cfg&HAL_PIN_LATCH) LATA |= mask;
			else LATA &= imask;
			#endif
			break;
		#endif
		#ifdef PORTB
		case HAL_PORTB:
			#ifdef WPUB
			if (cfg&HAL_PIN_PULLUP) WPUB |= mask;
			else WPUB &= imask;
			#endif
			#ifdef ODCONB
			if (cfg&HAL_PIN_OPENDRAIN) ODCONB |= mask;
			else ODCONB &= imask;
			#endif
			#ifdef TRISB
			if (cfg&HAL_PIN_TRISTATE) TRISB |= mask;
			else TRISB &= imask;
			#endif
			#ifdef ANSELB
			if (cfg&HAL_PIN_ANALOG) ANSELB |= mask;
			else ANSELB &= imask;
			#endif
			#ifdef SLRCONB
			if (cfg&HAL_PIN_SLEWRATE) SLRCONB |= mask;
			else SLRCONB &= imask;
			#endif
			#ifdef LATB
			if (cfg&HAL_PIN_LATCH) LATB |= mask;
			else LATB &= imask;
			#endif
			break;
		#endif
		#ifdef PORTC
		case HAL_PORTC:
			#ifdef WPUC
			if (cfg&HAL_PIN_PULLUP) WPUC |= mask;
			else WPUC &= imask;
			#endif
			#ifdef ODCONC
			if (cfg&HAL_PIN_OPENDRAIN) ODCONC |= mask;
			else ODCONC &= imask;
			#endif
			#ifdef TRISC
			if (cfg&HAL_PIN_TRISTATE) TRISC |= mask;
			else TRISC &= imask;
			#endif
			#ifdef ANSELC
			if (cfg&HAL_PIN_ANALOG) ANSELC |= mask;
			else ANSELC &= imask;
			#endif
			#ifdef SLRCONC
			if (cfg&HAL_PIN_SLEWRATE) SLRCONC |= mask;
			else SLRCONC &= imask;
			#endif
			#ifdef LATC
			if (cfg&HAL_PIN_LATCH) LATC |= mask;
			else LATC &= imask;
			#endif
			break;
		#endif
		default:
			break;
	}
}


void pin_set(HAL_PIN pin, uint8_t cfg, uint8_t val)
{
	uint8_t  mask  = 1<<__HAL_PIN(pin);
	if (!val) mask = ~mask;
	switch(__HAL_PORT(pin)){
		#ifdef PORTA
		case HAL_PORTA:
			#ifdef WPUA
			if (cfg&HAL_PIN_PULLUP){
				if (val) WPUA |= mask;
				else WPUA &= mask;
			}
			#endif
			#ifdef ODCONA
			if (cfg&HAL_PIN_OPENDRAIN){
				if (val) ODCONA |= mask;
				else ODCONA &= mask;
			}
			#endif
			#ifdef TRISA
			if (cfg&HAL_PIN_TRISTATE){
				if (val) TRISA |= mask;
				else TRISA &= mask;
			}
			#endif
			#ifdef ANSELA
			if (cfg&HAL_PIN_ANALOG){
				if (val) ANSELA |= mask;
				else ANSELA &= mask;
			}
			#endif
			#ifdef SLRCONA
			if (cfg&HAL_PIN_SLEWRATE){
				if (val) SLRCONA |= mask;
				else SLRCONA &= mask;
			}
			#endif
			#ifdef LATA
			if (cfg&HAL_PIN_LATCH){
				if (val) LATA |= mask;
				else LATA &= mask;
			}
			#endif
			break;
		#endif
		#ifdef PORTB
		case HAL_PORTB:
			#ifdef WPUB
			if (cfg&HAL_PIN_PULLUP){
				if (val) WPUB |= mask;
				else WPUB &= mask;
			}
			#endif
			#ifdef ODCONB
			if (cfg&HAL_PIN_OPENDRAIN){
				if (val) ODCONB |= mask;
				else ODCONB &= mask;
			}
			#endif
			#ifdef TRISB
			if (cfg&HAL_PIN_TRISTATE){
				if (val) TRISB |= mask;
				else TRISB &= mask;
			}
			#endif
			#ifdef ANSELB
			if (cfg&HAL_PIN_ANALOG){
				if (val) ANSELB |= mask;
				else ANSELB &= mask;
			}
			#endif
			#ifdef SLRCONB
			if (cfg&HAL_PIN_SLEWRATE){
				if (val) SLRCONB |= mask;
				else SLRCONB &= mask;
			}
			#endif
			#ifdef LATB
			if (cfg&HAL_PIN_LATCH){
				if (val) LATB |= mask;
				else LATB &= mask;
			}
			#endif
			break;
		#endif
		#ifdef PORTC
		case HAL_PORTC:
			#ifdef WPUC
			if (cfg&HAL_PIN_PULLUP){
				if (val) WPUC |= mask;
				else WPUC &= mask;
			}
			#endif
			#ifdef ODCONC
			if (cfg&HAL_PIN_OPENDRAIN){
				if (val) ODCONC |= mask;
				else ODCONC &= mask;
			}
			#endif
			#ifdef TRISC
			if (cfg&HAL_PIN_TRISTATE){
				if (val) TRISC |= mask;
				else TRISC &= mask;
			}
			#endif
			#ifdef ANSELC
			if (cfg&HAL_PIN_ANALOG){
				if (val) ANSELC |= mask;
				else ANSELC &= mask;
			}
			#endif
			#ifdef SLRCONC
			if (cfg&HAL_PIN_SLEWRATE){
				if (val) SLRCONC |= mask;
				else SLRCONC &= mask;
			}
			#endif
			#ifdef LATC
			if (cfg&HAL_PIN_LATCH){
				if (val) LATC |= mask;
				else LATC &= mask;
			}
			#endif
			break;
		#endif
		default:
			break;
	}
}


void pin_write(HAL_PIN pin, uint8_t val)
{
	uint8_t  mask  = 1<<__HAL_PIN(pin);
	if (!val) mask = ~mask;
	switch(__HAL_PORT(pin)){
		#ifdef PORTA
		case HAL_PORTA:
			if (val) PORTA |= mask;
			else PORTA &= mask;
			break;
		#endif
		#ifdef PORTB
		case HAL_PORTB:
			if (val) PORTB |= mask;
			else PORTB &= mask;
			break;
		#endif
		#ifdef PORTC
		case HAL_PORTC:
			if (val) PORTC |= mask;
			else PORTC &= mask;
			break;
		#endif
		default:
			break;
	}
}


uint8_t pin_read(HAL_PIN pin)
{
	uint8_t  mask = 1<<__HAL_PIN(pin);
	uint8_t  val;
	switch(__HAL_PORT(pin)){
		#ifdef PORTA
		case HAL_PORTA:
			val = PORTA&mask;
			break;
		#endif
		#ifdef PORTB
		case HAL_PORTB:
			val = PORTB&mask;
			break;
		#endif
		#ifdef PORTC
		case HAL_PORTC:
			val = PORTC&mask;
			break;
		#endif
		default:
			val = 0x00;
	}
	if (val) val = 0x01;
	return val;
}