/*
 * data_transfer.c
 *
 * Created: 3/6/2024 6:01:55 PM
 * Author : tawhi
 */ 

#define F_CPU 1000000 

#include <avr/io.h>
#include <util/delay.h>


#define	GATE_OPEN 0xF0
#define	ACCESS_DENIED 0xE0
#define	SEND_ENTER_SMS 0xD0
#define	SEND_LEAVE_SMS 0xC0
#define	RECEIVE_SMS 0xB0


void uart_init()
{
	UCSRA = 0b00000010;
	UCSRB = 0b00011000;
	UCSRC = 0b10000110;
	
	UBRRH = 0;
	UBRRL = 12;
}

void uart_send(const char data){
	while ((UCSRA & (1<<UDRE)) == 0x00);
	UDR = data;
}

int main(void)
{
    DDRB = 0xFF;
	PORTB = 0x00;
	
	uart_init();
	_delay_ms(5000);
	
// 	uart_send('A');
// 	_delay_ms(1000);
// 	PORTB = 0x01;
// 	
// 	uart_send('Y');
// 	_delay_ms(1000);
// 	PORTB = 0x02;
// 	
// 	uart_send('P');
// 	
// 	PORTB = 0xFF;
	
    while (1) 
    {
		uart_send('Y');
    }
}

