#include<avr/io.h>

#include<avr/interrupt.h>

#include<util/delay.h>

#include"adc.h"

volatile uint16_t CollisionSensorValue=0,SeatBeltFlag=0, DriverPresence=0;


void pininit()

{
    DDRB|=(1<<PB2);//OUPUT3 //Air Bag Deploy ****

    DDRC&=~(1<<PC3);//Collision Detection Sensor Input //Pressure sensor =pot ****

    DDRD&=~(1<<PD2);//pin D2 used as input (driver is present/not)

    DDRD&=~(1<<PD3);//pin D3 used as input(seat_belt indication) ******

    PORTD|=(1<<PD2);//making D2 pin with high voltage(5v)/1

    PORTD|=(1<<PD3);//making D3 pin with high voltage(5v)/1

    PORTD|=(1<<PD0);//making D0 pin with high voltage(5v)/1


    EICRA|=(1<<ISC00);//for any logic changes in INT0 jump to isr

    EICRA|=(1<<ISC10);//for any logic changes in INT1  jump to isr


    EIMSK|=(1<<INT0);// selecting interrupt 0(INT0)

    EIMSK|=(1<<INT1);//selecting interrupt 1(INT1)

    sei();

    SREG |=(1<<7);         //global interrupt register

    initADC();

}

int main()

{

    pininit();
    _delay_ms(100);

    while(1)

    {
            CollisionSensorValue=ReadADC(3);
            _delay_ms(100);


            if(SeatBeltFlag==1 && DriverPresence==1 && CollisionSensorValue>100)

            {

                 if (ReadADC(4) > 128)
                   {
                          PORTB |=(1<<PB5);
                          _delay_ms(100);
                   }
                PORTB|=(1<<PB2);//DeployAirbagON

                _delay_ms(100);

            }
            else
            {
              PORTB&=~(1<<PB2); //DeployAirbagOFF
                _delay_ms(200);

    }
    }
}


ISR(INT0_vect)

{

    DriverPresence=1;

}

ISR(INT1_vect)

{
    SeatBeltFlag=1;

}

ADC PROGRAM:
//ADC PROGRAM:

#include<avr/io.h>

#include<avr/interrupt.h>

uint16_t sixteen=0;



uint8_t eight=0;



void initADC()
{

    ADMUX=(1 << REFS0);

    ADCSRA |=(1 << ADEN)|(7<< ADPS0);

}



uint16_t ReadADC(uint8_t ch)

{

    ADMUX&=0xf8;

    ch=ch&0b00000111;

    ADMUX |=ch;

    ADCSRA|=(1 << ADSC);

    while(!(ADCSRA& (1 << ADIF)));

     ADCSRA |=(1 << ADIF);

eight=ADCL;

sixteen=(ADCH<<8)|(eight);
return(eight);

}







