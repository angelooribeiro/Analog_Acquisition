/*
 * GccApplication1.c
 *
 * Created: 20/02/2016 09:23:40
 * Author : angel
 */ 
#include "header.h"

char version[60] = "Version 1.0 Atmega2560 - Grupo 4 LPI-II DEI UM2016\n";			//program version
volatile char received_command[30];							//command to be executed 
volatile char last_command[30];								//stores the last command executed with success 
volatile char parse_index;
void save_command();										//save the last command executed with success 
char str_read_last(void);

volatile int i = 0;

//used to read the received_command array
char str_read(void)
{
	char temp_parse = '\0';
	temp_parse = received_command[parse_index];
	if(received_command[parse_index]!='\0')
	parse_index ++;
	
	return temp_parse;
}//end function

//to read a character that as been redden 
char str_read_last(void)
{
	char temp_parse = '\0';
	temp_parse = received_command[parse_index-1];
	return temp_parse;
} //end function

//saves the last comand executed with success
void save_command()
{
	int i = 0;
	while(received_command[i] != '\0')				//while is not the terminator character
	{
		last_command[i] = received_command[i];		//copies character by character
		i++;
	}
	last_command[i] = '\0';							//inserts the terminator character
	
}//end function 


unsigned int *p = (unsigned int*) (0x22);

volatile int j = 0;
ISR(TIMER0_OVF_vect)
{
	/*pointer for the DDRA*/
	/*test if is a + if it is activate direct pwm, if isn't activates reverse pwm*/
	if(j==20)
	{
		(*p) |= (1 << 0);
		j=0;
	}
	
	if(j==10) (*p) &= ~(1 << 0);
	j++;
	
}

int main(void)
{
	serial_configuration();		//configuration of serial port
	adc_configuration();
	config_timer1();
	sei();
	
	
		/*default buffer sizes values*/
		adc_buffer_size = 10;				/*default buffer size*/
		adc_buff_end = 0;					/*reset to buffer index's*/
		adc_buff_start = 0;
		flag_cycle_complet = 1;
		secounds_dec = 0;
	/*#######################################################     test       #############################################*/
		TCCR0A |= (1<<COM0A1) ;		/*set OC0A on Compare Match*/
		/*Clear OC0A on Compare Match, set OC0A at BOTTOM (non-inverting mode) */
		TCCR0B |= (1<<CS02) | (1<<CS00);
		/*FOC0A: Force Output Compare A*/
		TIMSK0 |= (1<<TOIE0);
		/*OCIE0A: Timer/Counter0 Output Compare Match A Interrupt Enable*/
		DDRA = 1;
		

		
	/*#######################################################     test       #############################################*/	
	
	while (1)
	{
		/*if there is data to be treated*/
		if(!flag_cycle_complet)
		{
			data_adc();
		}
	}
		
}


/*copyright RIBEIRO_ANGELO*/

