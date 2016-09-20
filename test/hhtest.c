#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#define UBAUD F_CPU / 16 / 9600 - 1

#include "../include/dhtxx.h"

void cominit( )
{
	UBRR0 = UBAUD; //Baud rate
	UCSR0B = ( 1 << RXEN0 ) | ( 1 << TXEN0 ); //Enable receiver and transmitter
	UCSR0C = ( 3 << UCSZ00 ); //8 bit of data, 1 stop
	DDRB |= ( 1 << 0 );
	PORTB |= ( 1 << 0 );
}

void comsend( const char *text )
{
	unsigned int i = 0;
	while ( text[i] )
	{
		while ( !( UCSR0A & ( 1 << UDRE0 ) ) );
		UDR0 = text[i++];
	}
}

int main( )
{
	char buff[128];
	unsigned char  ec;
	int temp, humid;

	cominit( );

	DDRD |= ( 1 << 4 ) | ( 1 << 2 );
	while( 1 )
	{
		for ( ec = 0; ec < 4; ec++ )
			_delay_ms( 1000 );

		dhtxxconvert( DHTXX_DHT22, &PORTC, &DDRC, &PINC, ( 1 << 0 ) );
		PORTD |= ( 1 << 2 );
		_delay_ms( 1000 );
		ec = dhtxxread( DHTXX_DHT22, &PORTC, &DDRC, &PINC, ( 1 << 0 ), &temp, &humid );
		PORTD &= ~( 1 << 2 );

		PORTD |= ( 1 << 4 );
		sprintf( buff, "EC %d, T: %d, H: %d\n\r", ec, temp, humid );
		comsend( buff );
		PORTD &= ~( 1 << 4 );
	}

	return 0;
}
