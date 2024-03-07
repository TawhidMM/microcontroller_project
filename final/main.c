#define F_CPU 1000000UL

#define D4 eS_PORTB0
#define D5 eS_PORTB1
#define D6 eS_PORTB2
#define D7 eS_PORTB3
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"


void INT0_init();
void INT1_init();
void servo_init();
void uart_init();
void UART_send(uint8_t data);
void stop_servo();
int slot_full();
void set_message(const char* msg);
void start_timer0();
void stop_timer0();
void start_timer2();
void stop_timer2();


#define WELCOME_MSG "WELCOME         "
#define BYE_MSG		"GOOD BYE        "
#define SWIP_MSG    "SWIPE YOUR CARD "
#define DEFAULT_MSG "DIGITAL PARKING "
#define DENY_MSG    "ACCESS DENIED   "

#define	GATE_OPEN 0xF0
#define	ACCESS_DENIED 0xE0
#define	SEND_ENTER_SMS 0xD0
#define	SEND_LEAVE_SMS 0xC0
#define	RECEIVE_SMS 0xB0

#define	ENTER_REQUEST 0x80
#define	LEAVE_REQUEST 0xC0
#define	NONE 0xFF
#define SLOT1 0
#define SLOT2 1

volatile uint8_t gate_open = 0;
volatile uint8_t slot1 = 0;
volatile uint8_t slot2 = 0;
volatile uint8_t request = 0xF0;
volatile uint8_t ath_status;
volatile uint16_t timer0_count = 0;
volatile uint16_t timer2_count = 0;
volatile uint8_t amount = 0;

char buffer[16];
char slot_buffer[16];
uint8_t slot_times[2];
uint8_t cars[2];
uint8_t recent_entered;
uint8_t recent_emptied = NONE;



void show_message(){
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String(buffer);
}

void show_slot_status() {
	sprintf(slot_buffer, "slot1:%d slot2:%d ", slot1, slot2);
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String(slot_buffer);
}

ISR(USART_RXC_vect) {
	// Read the received byte
	ath_status = UDR;
	
	if(ath_status == GATE_OPEN){
		PORTB |= (request);
		if(request == ENTER_REQUEST) {
			PORTA = 1;
			OCR1A = 300;  // 65, 180
			gate_open = 1;
			set_message(WELCOME_MSG);
			// resetting debugging port
			PORTA = 0;
		}
		else if(request == LEAVE_REQUEST){
			OCR1A = 300;  // 65, 180
			gate_open = 1;
			set_message(BYE_MSG);
			recent_emptied = NONE;
		}
	}
// 	else if(ath_status==0){
// 		set_message(DENY_MSG);
// 	}
}

ISR(TIMER0_OVF_vect){
	timer0_count++;
	
	//// for measuring 3 secs
	//if(timer0_count == 46) {
		//set_message(SWIP_MSG);
		//timer0_count = 0;
		//stop_timer0();
	//}	
}
ISR(TIMER2_OVF_vect){
	timer2_count++;
}


ISR(INT0_vect) {
	if(gate_open) {
		OCR1A = 180;
		gate_open = 0;
		set_message(SWIP_MSG);
		request = 0xF0;
		recent_emptied = NONE;
	}
	else {
		set_message(SWIP_MSG);
		request = ENTER_REQUEST;
	}
	
}


ISR(INT1_vect) {
	if(!gate_open) {
		
		if(recent_emptied!=NONE) {
			set_message("precessing bill ");
			//show_message();
			
			_delay_ms(500); 
			
			sprintf(buffer,"PAY BILL: %d $    ", 85);
			//show_message();
			
			// send sms receive command
			UART_send(RECEIVE_SMS);
			// send the amount
			UART_send(amount);
			
			// for debugging 
			PORTA = amount;
			
			recent_emptied=NONE;
			request = LEAVE_REQUEST;
			
		}
		//set_message(SWIP_MSG);
		//request = LEAVE_REQUEST;
		//PORTA |= (1<<PA1);
	}
	else {
		OCR1A = 180;
		gate_open = 0;
		set_message(DEFAULT_MSG);
		request = 0xF0;
		PORTA = 0;
	}
}

int main(void)
{
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRA = 0xFF;
	DDRC &= ~((1<<PC5)|(1<<PC4)|(1<<PC3));
	
	Lcd4_Init();
	uart_init();
	servo_init();
	INT0_init();
	INT1_init();
	TIMSK |= (1<<TOIE0);
	
	set_message(DEFAULT_MSG);
	
	sei();
	
	while(1){
		show_message();
		show_slot_status();
		//PORTA = gate_open; 
		
		if(PINC&(1<<PD5)) {
			slot1 = 0;
			stop_timer0();	
		}
		else {
			slot1 = 1;
			start_timer0();
			
		}
		
		if(PINC&(1<<PD4)) {
			slot2 = 0;
			stop_timer2();
		}
		else {
			slot2 = 1;
			start_timer2();	
			
		}
	}
	cli();
	
	return 0;
}


void INT0_init(){
	//set as input
	DDRD &= ~(1<<PD2);
	GICR |= (1<<INT0);
	
	// trigger on falling edge
	MCUCR |= (1<<ISC01)|(1<<ISC00);
	// 	MCUCR &= ~(1<<ISC00);
}

void INT1_init(){
	//set as input
	DDRD &= ~(1<<PD3);
	GICR |= (1<<INT1);
	
	// trigger on falling edge
	MCUCR |= (1<<ISC11)|(1<<ISC10);
	// 	MCUCR &= ~(1<<ISC10);
}

void uart_init()
{
	UCSRA = 0b00000010;
	UCSRB = 0b11011000;
	UCSRC = 0b10000110;
	
	UBRRH = 0;
	UBRRL = 12;
}

void UART_send(uint8_t data){
	while ((UCSRA & (1<<UDRE)) == 0x00);
	UDR = data;
}


void servo_init(){
	DDRD |= (1<<PD5);								/* Make OC1A pin as output */
	TCNT1 = 0;										/* Set timer1 count zero */
	ICR1 = 2499;									/* Set TOP count for timer1 in ICR1 register */
	TCCR1A |= (1<<WGM11)|(1<<COM1A1);				/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/8 */
	TCCR1B |= (1<<WGM12)|(1<<WGM13)|(1<<CS11);
}

int slot_full(){
	return slot1 && slot2;
}

void set_message(const char* msg){
	sprintf(buffer, msg);
}

void start_timer0() {
	if(timer0_count==0){
		TCCR0 |= (1<<CS02);
		TIMSK |= (1<<TOIE0);
		cars[SLOT1]=recent_entered;
	}
}
void stop_timer0(){
	if(timer0_count!=0){
		TCCR0 = 0;
		TCNT0 = 0;
		slot_times[SLOT1] = timer0_count*0.065;
		timer0_count = 0;
		recent_emptied = SLOT1;
		amount = slot_times[SLOT1];
		sprintf(buffer, "BILL: %d         ", amount);
		
		
// 		UART_send(RECEIVE_SMS);
// 		_delay_ms(500);
// 		// send the amount
// 		UART_send(slot_times[SLOT1]);
// 		//_delay_ms(500);
	}	
}
void start_timer2(){
	if(timer2_count==0){
		TCCR2 |= (1<<CS22)|(1<<CS21);
		TIMSK |= (1<<TOIE2);
		cars[SLOT2]=recent_entered;
	}
	
}
void stop_timer2(){
	if(timer2_count!=0){
		TCCR2 = 0;
		TCCR2 = 0;
		slot_times[SLOT2] = timer2_count*0.065;
		timer2_count = 0;
		recent_emptied = SLOT2;
		amount = slot_times[SLOT2];
		sprintf(buffer, "BILL: %d         ", amount);
		
		
// 		UART_send(RECEIVE_SMS);
// 		_delay_ms(500);
// 		// send the amount
// 		UART_send(slot_times[SLOT2]);
	}
}




