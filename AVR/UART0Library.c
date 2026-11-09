/*****************************************************************************
 *	    File: UART0Library.c
 *  Lab Name: (Needed for Bluetooth-controlled ZUMO Bots final project, S2019)
 *    Author: Dr. Greg Nordstrom
 *   Created: 3/3/2019
 *  Modified: 
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This is intended to be compiled as an AVR Library Project and
 * used by other programs. After compiling this code, copy the
 * libUART0Library.a and UART0Library.h files to the project directory,
 * then add the library to the target project by R-clicking on the
 * project's Libraries directory and clicking Add Library. Also,
 * the UART0Library.h file must be #included in the project's .c file.
 *
 * NOTE: This library only works with the default UART0 port on the
 * ReadyAVR. However, UART1 is the default for uploading code and
 * debugging, and that requires JP2 to have jumpers on PD3 and PD2.
 * It's best to leave UART1 as the default for uploading and debugging,
 * and using UART0 as an extra, independent UART by connecting 
 * external RS232 signals DIRECTLY to pin PE0 (Rx) and PE1 (Tx). This
 * arrangement allows for development via Atmel Studio as well as for
 * controlling RS232 devices via UART0 (e.g. a Bluetooth HC-05 module).
 */

#include <avr/io.h>
#include "UART0Library.h"

// Use UART0_init to initialize the UART hardware in the AVR processor.
// The function takes the following arguments:
//    - desired baud rate (standard values from 2400 to 115200,
//    - number of databits (5-8),
//    - number of stop bits (1,2),
//    - parity (0=no parity, 1=odd parity, 2=even parity).
// No error checking is performed and the function does not return a value.
//
// For example,
//     uart0_init(9600,8,1,0);
// sets the baudrate to 9600, with 8 data bits, 1 stop bit, and no parity
//
void uart0_init(long unsigned int baudrate, unsigned char databits, unsigned char stopbits, unsigned char parity)
{
    long unsigned int baud_prescale;

	// set baud rate (via UBRR0 high and low registers)
    baud_prescale = (((F_OSC/(baudrate * 16UL)))-1);
	UBRR0H = baud_prescale >> 8;
	UBRR0L = baud_prescale;

    // set various bits of USCR0C for UART0 comm parameters    
    UCSR0C = 0;     // initially clear all UCSR0C bits

    // set databits
    switch(databits) {
        case 5:
            break;  // no changes if 5-bit data
        case 6:
            UCSR0C |= (1 << UCSZ00);
            break;
        case 7:
            UCSR0C |= (1 << UCSZ01);
            break;
        default:  // default to 8 bits
            UCSR0C |= (1 << UCSZ01)|(1 << UCSZ00);
            break;
    }
    
    // set parity
    switch(parity) {
        case 1:     // odd parity
    	    UCSR0C |= (1 << UPM01)|(1 << UPM00);
            break;
        case 2:     // even parity
    	    UCSR0C |= (1 << UPM01);
            break;
        default:
            break;  // default is no parity
    }
    
    // set stopbits
    if(stopbits == 2)
        UCSR0C |= (1 << USBS0);
    	
	// finally, enable receiver and transmitter
	UCSR0B = (1 << RXEN0)|(1 << TXEN0);
}

// This function waits for space in the UART transmit buffer. When space
// is available, the function places an unsigned character value (passed in
// as the function's only argument) in the buffer and returns. No error
// checking is performed and the function does not return a value.
void uart0_tx(unsigned char data)
{
    // wait for empty tx buffer
    while(!(UCSR0A & (1 << UDRE0)));
    
    // put data into UART0's I/O data register (UDR0)
    UDR0 = data;
}

// This function blocks until a character is received in the UART input
// buffer. Once a character arrives, it is passed back to the caller as
// an unsigned character. No error checking is performed.
unsigned char uart0_rx(void)
{
	// wait for received char to arrive
	while (!(UCSR0A & (1 << RXC0)));
	
	// put received char into data variable and return
	return UDR0;
}