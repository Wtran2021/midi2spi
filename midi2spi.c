/*
*******************************************************
* Program Name:
*  lab3_01_midiToSpi.c
*
* Purpose:
*       to output the note number of a MIDI msg with SPI
*
*
* Test environment:
*   - Bare metal C via Atmel Studio 7
*   - ATtiny817 Xplained Pro development board
*
* Authors: Joshua Teixeira, Wilson Tran
*
* Course: ECE231, UMass Amherst
*
* Usage:
*     1. Run the program in Atmel Studio 7.
*     2. Observe note number output with logic analyzer
*
* Revision history:
*      1.0 2019-04-26, Joshua Teixeira & Wilson Tran
*           Initial Version.
* Note: 2019-04-28 - tested and ready for demo
*
* References:
*       UIAs and datasheets
*
*******************************************************
*/

#include <avr/io.h>
#include <stdio.h>

void SPI_MasterInit(void) // Pg 172, info sourced from Pg 171
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<DDB3)|(1<<DDB5)|(1<<DDB2); // DDB2 Turns on chip select, also declares master/slave
										  // Changed DDB_SPI to DDRB
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData) // Pg 172, code unchanged 
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
}

void USART_Init(unsigned int ubrr) // Pg 185 
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00); // Select character size (011) pg 203 on the 328 datasheet, took out USBS0
	}
	
unsigned char USART_Receive( void ) // Pg 189 changed UCSRnA to UCSR0A -- RXCn to RXC0 told by UIA
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) ) //Changed n to 0
	;
	/* Get and return received data from buffer */
	return UDR0;
}

int main (void){
	SPI_MasterInit();
	USART_Init(31); // [16Mhz / (16*31250)] - 1 = 31
	while(1){
		char cData = USART_Receive();
		if(cData != 0x80 && cData != 0x90 && cData != 0x7F){
			SPI_MasterTransmit(cData);
		}
		
	}
}
