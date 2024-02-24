#define F_CPU 1000000UL								
#include <avr/io.h>									
#include <stdio.h>									
#include <util/delay.h>								
#include <avr/interrupt.h>


void servo_init(void);
void stop_servo(void);
void INT0_init(void);
void INT1_init(void);
void move_gate(void);


volatile char gate_open = 0;

// IR-1
ISR(INT0_vect) {
	//move_gate();
	//servo_init();
	
	if(OCR1A == 180){
		OCR1A = 65;
	}
	else {
		OCR1A = 180;
	}
	
	_delay_ms(500);
	//gate_open = 1;
	//stop_servo();
}

// IR-2
ISR(INT1_vect) {
	//move_gate();	
	//servo_init();
	
	if(OCR1A == 180){
		OCR1A = 65;
	}
	else {
		OCR1A = 180;
	}
	_delay_ms(500);
	//stop_servo();
	//gate_open = 0;
}


int main(void)
{
	INT0_init();
	INT1_init();
	OCR1A = 180;
	servo_init();
	
	
	sei();

	while(1);

}

void servo_init(){
	DDRD |= (1<<PD5);								/* Make OC1A pin as output */
	TCNT1 = 0;										/* Set timer1 count zero */
	ICR1 = 2499;									/* Set TOP count for timer1 in ICR1 register */
	TCCR1A |= (1<<WGM11)|(1<<COM1A1);				/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/8 */
	TCCR1B |= (1<<WGM12)|(1<<WGM13)|(1<<CS11);
}

void stop_servo(){
	// set clock select bits to 0
	TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12));
}


void INT0_init(){
	//set as input
	DDRD &= ~(1<<PD2);
	
	GICR |= (1<<INT0);
	// trigger on rising edge
	MCUCR |= (1<<ISC01)|(1<<ISC00);
}

void INT1_init(){
	//set as input
	DDRD &= ~(1<<PD3);
	
	GICR |= (1<<INT1);
	// trigger on falling edge
	MCUCR |= (1<<ISC11)|(1<<ISC10);
}

void move_gate(){
	
	if(gate_open == 0) {
		OCR1A = 65;		
		_delay_ms(1000);
		gate_open = 1;
		// stop_servo();
		return;		
	}
	else {
		OCR1A = 180;
		_delay_ms(1000);
		gate_open = 0;
		// stop_servo();
		return;
	}
	
}


