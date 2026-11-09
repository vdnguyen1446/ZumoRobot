/*****************************************************************************
 *	    File: USART0Library.h
 *  Lab Name: (Needed when using UART0Library.c
 *    Author: Dr. Greg Nordstrom
 *   Created: 3/3/19
 *  Modified: 
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This header file defines three simple UART0 (comm port) functions for
 * initializing and using the UART0 hardware in the ATmega128A processor.
 * Before using the comm port to send/receive data, you must initialize
 * the hardware by providing the desired baud rate, number of data bits,
 * number of stop bits, and the type of parity.
 *
 * NOTE: This library only works with the default UART0 port on the
 * ReadyAVR. However, UART1 is the default for uploading code and
 * debugging, and that requires JP2 to have jumpers on PD3 and PD2.
 * It's best to leave UART1 as the default for uploading and debugging,
 * and using UART0 as an extra, independent UART by connecting
 * external RS232 signals DIRECTLY to pin PE0 (Rx) and PE1 (Tx). This
 * arrangement allows for development via Atmel Studio as well as for
 * controlling RS232 devices via UART0 (e.g. a Bluetooth HC-05 module).
 *
 * Recommended use is 9600 baud, 8 data bits, 1 stop bit, no parity.
 * This example uses all three functions in the UART0Library:
 *
 *     #include "UART0Library.h"  // Requires libUART0Library.a be a part
 *                                 // of your project. Place the file in
 *                                 // your project directory and add it to
 *                                 // the project via Libraries | Add Library
 *                                 // in the Solution Explorer panel).
 *
 *     #define BaudRate 9600
 *     #define DataBits 8
 *     #define StopBits 1
 *     #define NoParity 0
 *
 *     int main(void)
 *     {
 *     	  unsigned char c;
 *
 *     	  // initialize the UART0
 *     	  uart0_init(BaudRate, DataBits, StopBits, NoParity);
 *
 *        // transmit a value (0x41) to the comm port
 *        c = 0x41;
 *        uart0_tx(c);
 *
 *        // receive a byte from the comm port
 *        c = uart0_rx();
 *     }
 *
 */ 


#ifndef UART0LIBRARY_H_
#define UART0LIBRARY_H_

// Set default oscillator frequency for ATmega128A (7.3728 MHz)
#define F_OSC 7372800

void uart0_init(long unsigned int baudrate, unsigned char databits, unsigned char stopbits, unsigned char parity);
void uart0_tx(unsigned char data);
unsigned char uart0_rx(void);

#endif /* UART0LIBRARY_H_ */