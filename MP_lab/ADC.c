/*
 * File:   main2_2.c
 * Author: Cherry
 *
 * Created on 3 กุมภาพันธ์ 2565, 14:07 น.
 */


#include <xc.h>
#include <avr/io.h>

int ADCval = 0 ;
int main(void)
{
	//set pin/port
	DDRB = 0xFF; // set PORTB output (ADCH)
	DDRD = 0xFF;  // set PORTD output (ADCL)
    
    //set register ADC
    ADMUX |= (1<<REFS0) ; //Vref = AVcc , MUX[3:0]=0000 :ADC0
    ADCSRA |= (1<<ADEN); // ADEN = 1 ready to start , no prescale
    
    while(1)
    {
        ADCSRA |= (1<<ADSC); // start polling
        //while( ADCSRA & (1<<ADSC)); // if ADSC = 0 mean have value
        
        //output
        PORTB = ADCH; 
        PORTD = ADCL;
        
        ADCval = ADCL;
        ADCval = (ADCH<<8) + ADCval ; 
    }
}

//รับสัญญาณแอนะล็อกที่ขา Analog input 0 ด้วย ADC 