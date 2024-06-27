#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL


unsigned char Sev_seg[10]={0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09};
int valADC, Ttime, Gtime, Rtime, Ytime ,BalanceTime;
int number = 0;
int n0,n1;
int tenth=0, sec=0, sec1=0, sec2=0;//


#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

// Timer 0 output compare interrupt service routine
ISR(TIMER0_COMP_vect)
{
	tenth++;
	if (tenth == 10) {
		tenth = 0;
		sec2--;
		sec1--;
	}
}


unsigned int read_adc(unsigned char adc_input)
{
	ADMUX=adc_input | ADC_VREF_TYPE;
	_delay_us(10);
	ADCSRA|=(1<<ADSC);

	while ((ADCSRA & (1<<ADIF))==0);

	ADCSRA|=(1<<ADIF);
	return ADCW;
}



int main(void)
{

	// Port A initialization
	DDRA=0xfe;
	PORTA=0x00;

	// Port B initialization
	DDRB=0x3f;
	PORTB=0x00;

	// Port C initialization
	DDRC = 0xff;
	PORTC= 0x00;

	// Port D initialization
	DDRD= 0xf0;
	PORTD= 0x00;
	
	// ADC initialization:
	// ADC Clock frequency: 125.000 kHz
	// ADC Voltage Reference: AVCC pin
	// ADC Auto Trigger Source: disconnected
	ADMUX=ADC_VREF_TYPE;
	ADCSRA=0x83;
	
	// Timer/Counter 0 initialization:
	// Clock source: System Clock
	// Clock value: 0.977 kHz
	// Mode: CTC top=OCR0
	// OC0 output: Disconnected
	// Timer Period: 58.62 ms
	TCCR0=0x0d;
	TCNT0=0x00;
	OCR0=0x3a;
	
	
	TIMSK=0x02;

	
	sei();
	PORTB = 0x00;


	
	while (1)
	{
		valADC = read_adc(0);

		if(valADC > 1200)
		valADC = 1200;
		if(valADC < 200)
		valADC = 200;
		Ttime = valADC / 10;
		BalanceTime = 52;
		Gtime = (Ttime * 50) / 100;
		Ytime = (Ttime * 1) / 100;
		Rtime = (Ttime * 48) / 100;
		number = Gtime + Rtime;
		Gtime = (number * BalanceTime) / 100;
		Rtime = (number * (100 - BalanceTime)) / 100;
		sec1 = Gtime;
		sec2 = Rtime;
		tenth = 0;
		PORTB = 0x00;
		while(sec1>0){
			if (sec2 <= 0)
			{
				n1 =  (sec1/10) % 10;
				n0 = sec1 % 10;
				PORTB = 0x11;
				PORTD = 0x80;
				PORTC = Sev_seg[n0];
				_delay_ms(10);
				PORTD = 0x40;
				PORTC = Sev_seg[n1];
				_delay_ms(10);
				PORTD = 0x20;
				PORTA = 0x03;
				_delay_ms(10);
				PORTD = 0x10;
				PORTA = 0x03;
				_delay_ms(10);
				continue;
				
				
			}
			n1 =  (sec1/10) % 10;
			n0 = sec1 % 10;
			PORTB = 0x21;
			PORTD = 0x80;
			PORTC = Sev_seg[n0];
			_delay_ms(10);
			PORTD = 0x40;
			PORTC = Sev_seg[n1];
			_delay_ms(10);
			
			
			n1 =  (sec2/10) % 10;
			n0 = sec2 % 10;
			PORTD = 0x20;
			PORTA = Sev_seg[n0];
			_delay_ms(10);
			PORTD = 0x10;
			PORTA = Sev_seg[n1];
			_delay_ms(10);
			
		}
		
		
		
		
		sec1 = Gtime;
		tenth = 0;
		sec2 = Rtime;
		while(sec1>0){
			if (sec2 <= 0)
			{
				n1 =  (sec1/10) % 10;
				n0 = sec1 % 10;
				PORTB = 0x0A;
				PORTD = 0x20;
				PORTA = Sev_seg[n0];
				_delay_ms(10);
				PORTD = 0x10;
				PORTA = Sev_seg[n1];
				_delay_ms(10);
				
				PORTD = 0x80;
				PORTC = 0x03;
				_delay_ms(10);
				PORTD = 0x40;
				PORTC = 0x03;
				_delay_ms(10);
				continue;
			}
			
			
			n1 =  (sec2/10) % 10;
			n0 = sec2 % 10;
			PORTB = 0x0C;
			PORTD = 0x80;
			PORTC = Sev_seg[n0];
			_delay_ms(10);
			PORTD = 0x40;
			PORTC = Sev_seg[n1];
			_delay_ms(10);
			
			n1 =  (sec1/10) % 10;
			n0 = sec1 % 10;
			PORTD = 0x20;
			PORTA = Sev_seg[n0];
			_delay_ms(10);
			PORTD = 0x10;
			PORTA = Sev_seg[n1];
			_delay_ms(10);
			
			
		}
		


	}

}

