/*
 * File:   newmain.c
 * Author: Cherry
 *
 * Created on 3 Feb 2565, 10:48.
 */



#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL // 16MHz

int main(void)
{
	//set pin
	DDRD &= ~(1<<DDD2); // set input PD2
	DDRD |= (1<<DDD5); // set output PD5
	
	// set interrupt 
	EICRA |= (1<<ISC00); // Any logical change on INT0 generates an interrupt request.
	EIMSK |= (1<<INT0);   // External Interrupt Request 0 Enable  , open on INT0
	sei();
    
    //set Timer1
    TCCR1A = 0x00; //normal mode
    TCCR1B |= (1<<CS11); //prescaler 8
    TCCR1C = 0x00;
    TCNT1 = 0x0000; //start timer1
    while(1);
}

ISR(INT0_vect) {
	PORTD ^= (1<<PORTD5); // XOR bit if same 1 = 0 , else = 1
}

// external interrupt และใช้ Timer1 เป็นตัวจับเวลา