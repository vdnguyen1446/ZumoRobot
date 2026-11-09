/*****************************************************************************
 *	    File: Lab10.c
 *  Lab Name: RS232/BT Communication Tester
 *    Author: Dr. Greg Nordstrom & Prof. John Hutton
 *   Created: 12/30/17
 *  Modified: 3/28/19, 1/17/21, 11/13/2023
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server by reading a character
 * from a UART Terminal and reflecting that character to the BT UART
 * device.
 *
 * This program depends on the UARTLibrary used in previous labs, but
 * students are required to create and use their own UARTLibrary.c file,
 * but must use the UARTLibrary.h file that was given to them in Lab07.
 *
 * NOTE:  Since we skipped Lab08 this year (2023-Fall), I'm providing 
 * the USARTLibrary.a file from Lab07.
 *
 * In other words, their routines must conform to the prototypes they
 * have previously used.
 *
 * NB: The ReadyAVR-64 board uses UART1 as the default for programming,
 * freeing uart1 for external RS232 traffic (e.g. like adding the Bluetooth
 * transceivers in Lab09).
 *
 *   UART1 for communication to a Putty Terminal
 *   UART0 for communication across the paired BT channel.
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define END_STRING  '\0'
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>  // for sprintf functionality
#include <avr/interrupt.h> 
#include "UARTLibrary.h"
#include "UART0Library.h"

//#define BAUDRATE    19200 // standard values from 1200 to 115200
//#define DATABITS        7 // 5-8
//#define STOPBITS        1 // 1,2
//#define PARITY          0 // 0=none, 1=odd, 2=even

#define BAUDRATE   9600 // standard values from 1200 to 115200
#define DATABITS      8 // 5-8
#define STOPBITS      1 // 1,2
#define PARITY        0 // 0=none, 1=odd, 2=even


volatile uint8_t data_ready = 0;
volatile char rcvdChar;
char text[40];

void send_string(const char *cp) {
	while(*cp != END_STRING) {
		uart_tx(*cp++);
	}
}

int main(void) {
	
	const char resetMsg[] = "\r\nRS232/BT Test Reflector.\r\n\0";
	
	
	// Port Initialization
	
	//Up/Forward Joystick
	DDRB &= ~(1 << DDB3);
	DDRD &= ~(1 << DDD0);
	PORTB |= (1 << PORTB3);
	
	//Down/Back Joystick
	DDRB &= ~(1 << DDB1);
	DDRD &= ~(1 << DDD1);
	PORTB |= (1 << PORTB1);
	
	//Right/Right Joystick
	DDRB &= ~(1 << DDB0);
	DDRD &= ~(1 << DDD2);
	PORTB |= (1 << PORTB0);
	
	//Left/Left Joystick
	DDRB &= ~(1 << DDB2);
	DDRD &= ~(1 << DDD3);
	PORTB |= (1 << PORTB2);

	
	// Interrupt Enable Block
	// Using your Lab05 code, you will need to update these
	// lines from 0x00 to have the appropriate mask.
	EICRA &= (0<< ISC01) & (0<<ISC00);
	EIMSK |= (1<<INT0) | (1<<INT1) | (1<<INT2) | (1<<INT3);  //
	// Enable Global Interrupts
	sei();
	
	
	
	
	
	
	// For simplicity, use the same setup information for both ports.
	// Init UART1 for debug information to Putty
    uart_init(BAUDRATE,DATABITS,STOPBITS,PARITY);
	
	
	// Init UART0 for BT/UART path
	uart0_init(BAUDRATE,DATABITS,STOPBITS,PARITY);
	
	
	send_string(resetMsg);
	
	while(data_ready != 1)
	{
		
		switch(rcvdChar)
		{
			case CR:
				// send back both CR and LF (makes PuTTY work without having to assume CRs w/ LFs)
				uart_tx(CR);	// echo char back to sender
				uart_tx(LF);	// echo char back to sender
				break;					
			default:
				// Echo normal character back to terminal
				uart_tx(rcvdChar);
				// Send character to BT Device (Zumo)
				uart0_tx(rcvdChar);
		}
		data_ready = 0;
	}
	return 0;
}



// This is the required format of an ISR routine for a
// Hardware Pin Interupt.  Format is "PIN#_vect" where
// '#' is the number of the interupt...  PD0->0, PD1->1, etc


ISR(INT0_vect){
	rcvdChar = 'w';
	data_ready = 1;
	
}

ISR(INT1_vect){
	rcvdChar = 's';
	data_ready = 1;
	
}

ISR(INT2_vect){
	rcvdChar = 'd';
	data_ready = 1;
	
}

ISR(INT3_vect){
	rcvdChar = 'a';
	data_ready = 1;
	
}