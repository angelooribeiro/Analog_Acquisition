/*
 * parse_s.c
 *
 * Created: 29/03/2016 09:55:58
 *  Author: angel
 */ 

#include "parse.h"

volatile unsigned char temp_TCCR1B;

void parse_s(void)		//parse for the letter s
{
			
	unsigned char temp = str_read();
	switch(temp)
	{
		
		/*stop sampling when the buffer is full*/
		case 'r':
		case 'R':
			temp = str_read();
			if(temp == '\0')
			{
				flag_cycle_complet = 0;		/*if a cycle was complet*/
				adc_buff_start = 0;			/*restart the buffer*/
				flag_sampling_run = 1;		
				TCCR1B |= temp_TCCR1B;	/*switch on the timer*/
				TIMSK1 |= (1 << OCIE1B);					/*activates interrupt for match with B value*/
			}
			if(temp != '\0')
				serial_error();
		break;
				
		/*sampling period*/
		case 'p':
		case 'P':
			set_sampling_period();
		break;
				
		/*ends after a signal to stop*/
		case 'c':
		case 'C':
			temp = str_read();
			if(temp == '\0')
			{
				flag_cycle_complet = 0;		/*if a cycle was complet*/
				adc_buff_start = 0;			/*for multiple read*/
				flag_sampling_cycle = 1;
				TCCR1B |= temp_TCCR1B; /*switch on ADC source*/
				TIMSK1 |= (1 << OCIE1B);					/*activates interrupt for match with B value*/
			}
			if(temp != '\0')
				serial_error();
		break;
				
		default:
			serial_error();
		break;
	}
}

/*Define the sampling Period*/
void set_sampling_period(void)
{
	unsigned char control_digit1 = str_read();			//get 0x20
	unsigned char temp_units = parse_number();			//get 1,2,3 time units -> ys, ms, s
	unsigned char control_digit2 = str_read_last();			//get 0x20
	unsigned int samp_period = parse_number();			//get the time of sampling
	unsigned char control_digit3 = str_read();			//get '\0'
	
	/*defines samp_period max = 0x3E8 = d1000*/
	if( (temp_units < 0x00) || (temp_units > 0x03) || (samp_period < 0x00) || (samp_period > 0x3E8) || (control_digit1 != 0x20) || 
		(control_digit2 != 0x20) || (control_digit3 != '\0') )			//the addr of adc ports goes to 0 to 7
		serial_writestr("parametros errados\n");										//error message

	if( (temp_units >= 0x00) && (temp_units <= 0x03) && (samp_period >= 0x00) && (samp_period <= 0x3E8) && (control_digit1 == 0x20) &&
	(control_digit2 == 0x20) && (control_digit3 == '\0') )			//the addr of adc ports goes to 0 to 7
	{
		secounds_dec = 0;			//reset to secound
		old_secounds_dec = 0;
		
		if(temp_units == 1) /*ys*/
		{
			if(samp_period >= 100)
			{
				TCCR1B &= ~0x07;			/*reset the prescaler and stop*/
				TCNT1 = 0;				/*counter of the timer to zero*/	
				OCR1B  =  (samp_period * 2) ; 
				temp_TCCR1B = 0x02;			/*makes 0.5 ys*/
			}
			if(samp_period < 100)
				serial_writestr("too low sampling period\n");
		}/*ys*/
		
		if(temp_units == 2) /*ms*/
		{
			TCCR1B &= ~0x07;			/*reset the prescaler and stop*/
			TCNT1 = 0;				/*counter of the timer to zero*/
			OCR1B = ((int)((samp_period*65535)/1048));
			temp_TCCR1B = 0x04;	/* fclock/256 -> makes 16ys each machine cycle*/
		}
		
		if(temp_units == 3) /*s*/
		{
			TCCR1B &= ~0x07;
			TCNT1 = 0;
			secounds_dec = samp_period;
			old_secounds_dec = samp_period;
			OCR1B = 15625;	/*makes 1 secound*/
			temp_TCCR1B = 0x05;	/*fclock/1024 -> makes 64ys*/
		}
	}
}