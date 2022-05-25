#include <avr/io.h>
#define F_CPU 16000000UL //16MHz.

int main()
 { 
   // set port d5
   DDRD |= (1<<PD5);              //Fast PWM output   //t OC0B pin
   
   // set Fast PWM
   OCR0A = 40 ;  //Top Value 40
   OCR0B = 20;     	// Duty cycle 50 %
   TCCR0A |= (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);	//Non-Inverting Fast PWM mode 7 and clear OCR0A when match
   TCCR0B |= (1<<WGM02) | (1<<CS00);	//No-Prescalar
   TCNT0 = 0x00;

   while (1);

   return 0;
 }

//สร้างสัญญาณ PWM 