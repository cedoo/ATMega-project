#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>
#include "HD44780.h"
#include <stdlib.h>
#include <stdio.h>

#define ADCIN PC5   //definicja ADCIN (wejœcie ADC)

int main(void)
{
	

	char znaki[]= {32,4,13,31,13,4,32,32,16,24,24,31,31,24,24,16,32,32,16,31,31,16,32,32,0};
	int adc, adc1;
	int x=0, licznik=0, pozycja_przeszkody1=0, pozycja_2przeszkody1=-7, punkty=0, wyswietlac=1;
	char pozycja_przeszkody;
	char pozycja_2przeszkody;
	char punktychar[3];
	_delay_ms(100);
	LCD_Initalize();
	LCD_WriteCommand(HD44780_CGRAM_SET);
	LCD_WriteText(znaki);
	LCD_WriteCommand(HD44780_DDRAM_SET);
	ADCSRA =   (1<<ADEN) //ADEN: ADC Enable (uruchomienie przetwornika)
	
	//ustawienie preskalera, preskaler=8, poniewa¿ 1MHz/8=125kHz
	|(1<<ADPS0)
	|(1<<ADPS1)
	|(0<<ADPS2);
	
	ADMUX  = (1<<REFS1) | (0<<REFS0) |  (1<<MUX2) | (1<<MUX0); //Napiêcie odniesienia Reserved, ADC5 - Pin 5 
	DDRC &=~ (1<<ADCIN); //Ustawienie Wejœcia ADC
	
	ADCSRA |= (1<<ADSC); //ADSC: uruchomienie pojedynczej konwersji
	while(ADCSRA & (1<<ADSC)); //czeka na zakoñczenie konwersji
	adc=ADC;
	srand(adc);
	pozycja_przeszkody=(rand()%0x02);
	pozycja_2przeszkody=(rand()%0x02);
	for(;;)//pêtla g³ówna
	{	
		LCD_WriteText("Przekrec w lewo ");
		LCD_GoTo(0,1);
		LCD_WriteText("aby rozpoczac");
		_delay_ms(1000);
		while(!(ADC==0))
		{
			ADCSRA |= (1<<ADSC); //ADSC: uruchomienie pojedynczej konwersji
			while(ADCSRA & (1<<ADSC)); //czeka na zakoñczenie konwersji
		}
		wyswietlac=1;
		pozycja_przeszkody1=0;
		pozycja_2przeszkody1=-7,
		licznik=0;
		punkty=0;
		LCD_Clear();
		_delay_ms(500);
		while(wyswietlac==1){
			ADCSRA |= (1<<ADSC); //ADSC: uruchomienie pojedynczej konwersji
			while(ADCSRA & (1<<ADSC)); //czeka na zakoñczenie konwersji									
			adc1=ADC;
			if(adc1-adc>200)
			{
				x=1;
				adc=ADC;
				LCD_GoTo(0,0);
				LCD_WriteText("   ");
			}
			else
			{
				if(adc-adc1>200)
				{
					x=0;
					adc=ADC;
					LCD_GoTo(0,1);
					LCD_WriteText("   ");
				}
			}
			_delay_ms(10);
			licznik++;
			if(licznik>15)
			{
				licznik=0;
				pozycja_2przeszkody1++,
				pozycja_przeszkody1++;
				if(pozycja_przeszkody1>15)
				{
					LCD_GoTo(0,pozycja_przeszkody);
					LCD_WriteText(" ");
					pozycja_przeszkody=(rand()%0x02);
					pozycja_przeszkody1=0;
					punkty++;
				}
				if(pozycja_2przeszkody1>15)
				{
					LCD_GoTo(0,pozycja_2przeszkody);
					LCD_WriteText(" ");
					pozycja_2przeszkody=(rand()%0x02);
					pozycja_2przeszkody1=0;
					punkty++;
				}
			}
			if((x==pozycja_przeszkody && pozycja_przeszkody1>13)||(x==pozycja_2przeszkody && pozycja_2przeszkody1>13))
			{
				LCD_Clear();
				LCD_WriteText("Twoj wynik:");
				LCD_GoTo(0,1);
				sprintf(punktychar, "%i", punkty);
				LCD_WriteText(punktychar);
				_delay_ms(2000);
				wyswietlac=0;
				LCD_Clear();
			}
// WYSWIETLANIE -----------------------------------------------------------------------------------	
			if(wyswietlac==1)
			{		
				LCD_GoTo(15-pozycja_przeszkody1,pozycja_przeszkody);
				LCD_WriteData(0);
				LCD_WriteText(" ");
				LCD_GoTo(15-pozycja_2przeszkody1,pozycja_2przeszkody);
				LCD_WriteData(0);
				LCD_WriteText(" ");
				if(x==0)
				{
					LCD_GoTo(0,0);
					LCD_WriteData(2);
					LCD_WriteData(1);
				}
				if(x==1)
				{
					LCD_GoTo(0,1);
					LCD_WriteData(2);
					LCD_WriteData(1);
				}
			}
		}
	}
}

