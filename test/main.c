// #define F_CPU 1000000UL
// #include <avr/io.h>
// #include <stdio.h>
// #include <util/delay.h>
// #include <avr/interrupt.h>
// 
// int main(){
// 	DDRD |= (1<<PD5);
// 	TCNT1 = 0;										/* Set timer1 count zero */
// 	ICR1 = 2499;									/* Set TOP count for timer1 in ICR1 register */
// 	TCCR1A |= (1<<WGM11)|(1<<COM1A1);				/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/8 */
// 	TCCR1B |= (1<<WGM12)|(1<<WGM13)|(1<<CS11);
// 	OCR1A = 185;
// 	
// 	DDRD &= ~((1<<PD3)|(1<<PD2));
// 	
// 	while(1){
// 		while(!(PIND&(1<<PD3)) ) {
// 			if(OCR1A == 125)
// 				OCR1A = 300;
// 			else
// 				OCR1A = 125;
// 				
// 			while(!(PIND&(1<<PD3)));	
// 		}
// 		
// 		while(!(PIND&(1<<PD2)) ) {
// 			if(OCR1A == 125)
// 				OCR1A = 300;
// 			else
// 				OCR1A = 125;
// 			
// 			while(!(PIND&(1<<PD2)));
// 		}
// 	}
// }




//#define F_CPU 1000000UL
//
//#define D4 eS_PORTB0
//#define D5 eS_PORTB1
//#define D6 eS_PORTB2
//#define D7 eS_PORTB3
//#define RS eS_PORTC6
//#define EN eS_PORTC7
//
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
//#include <stdio.h>
//#include "lcd.h"
//char buffer[16];
//char slot_buffer[16];
//
//void move_gate();
//void servo_init();
//void stop_servo();
//int slot_full();
//void delay(int millisec);
//
//volatile int int0 = 0;
//volatile int int1 = 0;
//volatile int intCnt = 0;
//volatile uint8_t gate_open = 0;
//volatile uint8_t slot1 = 0;
//volatile uint8_t slot2 = 0;
//volatile uint8_t triggered_int;
//
//void text(){
	////Lcd4_Clear();
	//
	//if(intCnt == 0){
		//sprintf(buffer, "cse316Project   ");
		//int0 = int1 = 0;
	//}
	//else if(int0 == 1 && int1 == 0){
		//sprintf(buffer, "welcome         ");
	//}
	//else if(int0 == 0 && int1 == 1){
		//sprintf(buffer, "good bye        ");
	//}
	//
//// 	Lcd4_Set_Cursor(2,0);
//// 	Lcd4_Write_String(buffer);
	////_delay_ms(2000);
//}
//void show_message(){
	//Lcd4_Set_Cursor(2,0);
	//Lcd4_Write_String(buffer);
//}
//
//void show_slot_status() {
	//sprintf(slot_buffer, "slot1:%d slot2:%d ", slot1, slot2);
	//Lcd4_Set_Cursor(1,0);
	//Lcd4_Write_String(slot_buffer);
//}
//
//void INT0_init(){
	////set as input
	//DDRD &= ~(1<<PD2);
	//GICR |= (1<<INT0);
	//
	//// trigger on falling edge
//// 	MCUCR |= (1<<ISC01);
//// 	MCUCR &= ~(1<<ISC00);
//}
//ISR(INT0_vect) {
	//triggered_int = 0;
	//intCnt = 1 - intCnt;
	//int0 = 1;
		//
	//text();
	//move_gate();
	//
	//
	////sei();
		//
	//int0 = 0;
//}
//
//void INT1_init(){
	////set as input
	//DDRD &= ~(1<<PD3);
	//GICR |= (1<<INT1);
	//
	//// trigger on falling edge
//// 	MCUCR |= (1<<ISC11);
//// 	MCUCR &= ~(1<<ISC10);
//}
//ISR(INT1_vect) {
	//triggered_int = 1;
	//intCnt = 1 - intCnt;
	//int1 = 1;
	//
	//text();
	//show_message();
	//move_gate();
	//
	//_delay_ms(500);
	////sei();
	//
	//int1 = 0;
//}
//
//int main(void)
//{
	//MCUCSR = (1<<JTD);
	//MCUCSR = (1<<JTD);
	//
	//DDRB = 0xFF;
	//DDRC = 0xFF;
	//DDRC &= ~((1<<PC5)|(1<<PC4)|(1<<PC3));
	//
	//Lcd4_Init();
	//servo_init();
	//INT0_init();
	//INT1_init();
	//
	//text();
	//show_slot_status();
	//
	//PINC |= (1<<PC3);
	//_delay_ms(1000);
	////sei();
	//while(1){
		//
		//if(PINC&(1<<PC3)){
			//OCR1A = 125;  // 65, 180
			//gate_open = 1;
			//PORTB &= ~(1<<PB7);	
			//
			//_delay_ms(5000);
			//
			//PORTB |= (1<<PB7);	
			//OCR1A = 250;  // 65, 180
			//gate_open = 0;
		//}
		//
		//if(PIND&(1<<PD3) || PIND&(1<<PD2)){
			//sei();
		//}
		//
		//if(PINC&(1<<PD5)) {
			//slot1 = 0;
		//}
		//else {
			//slot1 = 1;
		//}
		//
		//if(PINC&(1<<PD4)) {
			//slot2 = 0;
		//}
		//else {
			//slot2 = 1;
		//}
		//
		//show_slot_status();
		//show_message();
	//}
	//cli();
	//
	//return 0;
//}
//
//
//void servo_init(){
	//DDRD |= (1<<PD5);								/* Make OC1A pin as output */
	//TCNT1 = 0;										/* Set timer1 count zero */
	//ICR1 = 2499;									/* Set TOP count for timer1 in ICR1 register */
	//TCCR1A |= (1<<WGM11)|(1<<COM1A1);				/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/8 */
	//TCCR1B |= (1<<WGM12)|(1<<WGM13)|(1<<CS11);
//}
//
//void move_gate() {
	//
	//if(gate_open) {
		//OCR1A = 250;
		//gate_open = 0;
	//}
	//else {
		//OCR1A = 125;  // 65, 180
		//gate_open = 1;
	//}
	//
	//cli();
	//_delay_ms(500);
	//
	//
	////if(!gate_open) {
		////OCR1A = 125;  // 65, 180
		////gate_open = 1;
		////return;
	////}
	//
//}
//
//void stop_servo(){
	//// set clock select bits to 0
	//TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12));
//}
//
//int slot_full(){
	//return slot1 && slot2;
//}
//
//void delay(int millisec) {
	//for (unsigned int i = 0; i < millisec; i++) {
		//for (unsigned int j = 0; j < 1100; j++) {
			//asm volatile("nop");
		//}
	//}
//}
//
//


#define F_CPU 1000000UL


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

void uart_init();
void UART_send(uint8_t data);

volatile uint8_t data;

ISR(USART_RXC_vect) {
	// Read the received byte
	data = UDR;
	UART_send(data);
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


int main(){
	uart_init();
	//UART_send('a');
	
	sei();
	while(1){
		
	}
}