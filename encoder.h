#ifndef _ENCODER_H
#define _ENCODER_H

/*
	�������

	������� �������� ������ ����� ����. ��������� ����� ���� ���� � ��������
	������������� �������� ENCODER_SAMPLE_RATE. � �������� ������� ������������
	Timer4
*/


#include <stdint.h>


#define ENCODER_PORT_A  A
#define ENCODER_PIN_A   0
#define ENCODER_PORT_B  A
#define ENCODER_PIN_B   1


#define ENCODER_SAMPLE_RATE  1500


inline void    encoder_isr(void);
inline void    encoder_init(void);
inline void    encoder_reset(void);
inline int8_t  encoder_read(void);

#endif
