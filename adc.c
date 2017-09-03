/*
 * adc.c
 *
 * Created: 07/03/2016 16:32:52
 *  
 */ 

#include "adc.h"


/*from buffer*/
extern volatile int Adc_Buffer[];			/*stores the redden information from Adc*/
extern volatile unsigned int adc_buff_start;		//transmission buffer start index
extern volatile unsigned int adc_buff_end;			//transmission buffer end index
extern volatile unsigned int adc_buffer_size;			//size of the buffer
extern volatile unsigned char flag_cycle_complet;			//flag to declare that sampling cycle maskes one lap
volatile unsigned int secound_tail;

extern volatile unsigned char flag_sampling_cycle;			//flag to determinate if is a sampling cycle
extern volatile unsigned char flag_sampling_run;				//flag to determinate if is a sampling run
extern volatile unsigned char flag_filter_on;	//set to one to declare that filter must be used
extern volatile unsigned char filter_weight;	//weight of the filter
extern volatile unsigned char flag_linear_on;	//set to one to declare that linear mode is on
extern volatile unsigned char secounds_dec;					//when is a sample period with seconds
extern volatile unsigned char old_secounds_dec;				//when is a sample period with seconds

volatile unsigned int sample;
volatile unsigned int temp_sample;
volatile unsigned char read_flag = 0;
volatile unsigned char was_flag_sampling_run=0;
volatile unsigned int temp_sampl;
volatile unsigned char was_flag_sampling_cycle=0;

void config_timer1()
{
		TCCR1A = (1<<COM1B0);  	/*CTC mode*/
		TCCR1B |= (1<<WGM12) ;
}

/*ADC default Configration*/
void adc_configuration()
{
	/*runs in free mode*/
	ADMUX |= (1 << REFS0);	/*default to avcc amd pin0 input*/
 	/*enables adc; adc auto trigger enable;  adc start conversion;  activates the interrupt*/
	 /*fosc/128 -> 8ys take a sample*/
	ADCSRA |= (1 << ADATE)| (1<<ADEN) | (1 << ADIE)| (1<<ADSC) | (1<<ADPS1) | (1<<ADPS0);

}

/*ADC interrupt response*/
ISR(ADC_vect)
{
	read_flag = 1;
	sample = ADC;
	ADCSRA |= (1<<ADIF); /*ACK interrupt flag*/
	read_flag = 0;
}

ISR(TIMER1_COMPB_vect)
{	
	while(read_flag);
	temp_sampl =  sample;
	isr_timer1();
	TCNT1 = 0;	/*reset the value of the counter*/
}

void isr_timer1()
{
	 if(secounds_dec > 0)			/*decrements when operation takes more than 1 second*/
	 	secounds_dec = secounds_dec - 1;
	 
	 else
	 {	
		 if(flag_sampling_run)
		 {
	 		if( ((adc_buff_start+1) > adc_buffer_size) )				/*if full terminates adc conversion*/
	 		{
		 		flag_sampling_run = 0;
		 		TCCR1B &= ~0x07;								/*switch off the timer source*/
				TIMSK1 &= ~(1 << OCIE1B);
				was_flag_sampling_run = 1;
			}
			if( (adc_buff_start+1) <= adc_buffer_size )
			{
				Adc_Buffer[adc_buff_start] = temp_sampl;		/*saves the last result*/
				adc_buff_start++;			/*to save in the next position*/
			}
		} /*end sampling_cycle*/
		
		if(flag_sampling_cycle)		/*sampling cycle mode*/
		{
			Adc_Buffer[adc_buff_start] = temp_sampl;		/*saves the received data*/
			adc_buff_start = ((adc_buff_start+1) & MASk_ADC);			/*to save in the next position*/
			was_flag_sampling_cycle = 1;
		}/*end sampling_cycle*/
	}
 }

void data_adc()
{
	while( (secound_tail == adc_buff_start) );	/*waits for data */
	while(secound_tail != adc_buff_start)			/*while exists data to be redden*/
	{
		temp_sample = Adc_Buffer[secound_tail];
		
		if( (flag_sampling_run == 0) && (flag_sampling_cycle == 0) )
			serial_writestr("Activate ADC\n");
			
		if(flag_sampling_run || was_flag_sampling_run)			/*sampling run mode*/
		{	
				if(flag_filter_on == 1)		filter_function();		/*if filter is active*/
				if(flag_linear_on == 1)		linear_function();		/*if linear is active*/
				/*sends to serial port the last sampled result*/
				serial_write(temp_sample>>8);
				serial_write(temp_sample);
				Adc_Buffer[secound_tail] = temp_sample;		/*saves the last result*/
				(secound_tail > adc_buffer_size) ? was_flag_sampling_run = 0 : secound_tail++;
		} /*ends sampling run*/
	/*###########################################################*/	
		/*sampling cycle mode*/
		if(flag_sampling_cycle || was_flag_sampling_cycle)
		{
			if( secound_tail <= adc_buff_start)
			{
				if(flag_linear_on == 1)		linear_function();			/*if linear is active*/
				if(flag_filter_on == 1)		filter_function();			/*if filter is active*/	
				/*sends to serial port the last sampled result*/
				serial_write(temp_sample>>8);
				serial_write(temp_sample);
				Adc_Buffer[secound_tail] = temp_sample;		/*saves the received data*/
				secound_tail = ((secound_tail+1) & MASk_ADC);			/*to save in the next position*/
			}
			
			if( !flag_sampling_cycle )		/*if its terminated*/
			{
				was_flag_sampling_cycle = 0;
				flag_cycle_complet = 1;			/*terminate all operation*/
			}
		} /*ends sampling cycle */
	} /*end while*/
 } /*end function*/


void set_analog_in(void)
{
	unsigned char control_digit1 = str_read();
	unsigned char selected_pin = parse_number();
	unsigned char control_digit2 = str_read();
	
	if( (selected_pin < 0x00) || (selected_pin > 0x07) || (control_digit1 != 0x20) || (control_digit2 != '\0'))			//the addr of adc ports goes to 0 to 7
		serial_error();										//error message

	if( (selected_pin >= 0x00) && (selected_pin <= 0x07) &&  (control_digit1 == 0x20) && (control_digit2 == '\0') )
	{
		ADCSRA &= ~(1 << ADEN);
		serial_write(selected_pin+0x30);
		ADMUX &= ~(0x1f);	//reset to the last selected port
		ADMUX |= selected_pin;	//select a new port
		ADCSRA |= (1 << ADEN);
		save_command();	
	}
}

void linear_function()
{
	int i=1;
	unsigned int lower_value, lower_reading, upper_value, upper_reading;
	/*applies linear*/
	while(linear_table[i] <= temp_sample) i += 2;  // sets index to upper limit of interval
	lower_value = linear_table[i-3];
	lower_reading = linear_table[i-2];
	upper_value = linear_table[i-1];
	upper_reading = linear_table[i];
	temp_sample = ( (upper_value - lower_value) / (upper_reading - lower_reading) ) * (temp_sample - lower_reading) + lower_value;
}

void filter_function()
{
	temp_sample = (int) (((float)((float)filter_weight)*(float)last_sample * 0.1)  +	((1-((filter_weight)*0.1))*(float)temp_sample));
	last_sample = temp_sample;		/*stores the last filtered value*/
}