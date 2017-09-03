/*
 * parse_buffer.c
 *
 * Created: 28/03/2016 14:42:10
 *  Author: angel
 */ 

#include "parse_buffer.h"

void parse_buffer()
{
	unsigned char temp = str_read();
	
	switch(temp)
	{
		/*Buffer externals [0-5]V*/
		case 'e':
		case 'E':
			read_buffer_external();
		break;
				
		/*analog buffer size*/
		case 'l':
		case 'L':
			set_buffer_size();
		break;
				
		/*Buffer internals [0-255] adc value*/
		case 'i':
		case 'I':
			read_buffer_internal();
		break;
				
		/*Buffer Eng. Units [0-100]ºC*/
		case 'u':
		case 'U':
			read_buffer_eng_units();
		break;
				
		default:
			serial_writestr("default");
			serial_error();
		break;
	}
}


/*Redefines the size of the buffer*/
void set_buffer_size(void)
{
	unsigned char control_digit1 = str_read();
	long int temp_adc_buffer_size = parse_number();
	unsigned char control_digit2 = str_read();
	
	/*buffer size [0-0xFF]   verifies if isn't a valid buffer_size */
	if( (temp_adc_buffer_size < 0)  || (temp_adc_buffer_size > 1024) || (control_digit1 != 0x20) || (control_digit2 != '\0') )
		serial_writestr("out of range!");		/*outputs an error message*/
		
	if( (temp_adc_buffer_size >= 0) && (temp_adc_buffer_size <= 1024) && (control_digit1 == 0x20) && (control_digit2 == '\0') )
	{
		adc_buffer_size = temp_adc_buffer_size;				/*new buffer size*/
		adc_buff_end = 0;					/*reset to buffer index's*/
		adc_buff_start = 0;
		
		//serial_writestr("Modified buffer size to -> ");
		//conv_hexa_ascii(&temp_adc_buffer_size,2);		//convert and print the value
		
		save_command();
		//serial_write('\n');
	}
} /*end set_buffer_size

/*Reads x bytes from buffer in internal units (adc value)*/
void read_buffer_internal(void)
{	
	unsigned char control_digit1 = str_read();
	unsigned char temp_end = 0;						//to determinate how long will be the read operation
	unsigned char control_digit2 = 0;

	if(control_digit1 == '\0')			/*if the user don't specific the size of the operation*/
	{
		temp_end = adc_buffer_size;
		control_digit2 = '\0';			/*to control if everything is right*/
	}
	
	if(control_digit1 == 0x20)		/*if the user specific the size of the operation*/
	{
		temp_end = parse_number();
		control_digit2 = str_read();
	}
	
	if( (control_digit2 != '\0') || (temp_end > adc_buffer_size) )
		serial_error();
	
	/*read operation*/
	if( (control_digit2 == '\0') && (temp_end <= adc_buffer_size) )
	{
		if(flag_cycle_complet)						//if makes one lap in the buffer
			adc_buff_end = ( (adc_buff_start+1));	//reads all the data
	
		if( adc_buff_start == adc_buff_end)						/*if the buffer is empty*/
			serial_writestr("Nothing to print\n");
		
		while ( (adc_buff_start != adc_buff_end) && temp_end )				/*if exist something to transmit*/
		{
			if (adc_buff_end >= adc_buffer_size)			/*if reaches the end*/ 
				adc_buff_end = 0;
			
			conv_hexa_ascii(&Adc_Buffer[adc_buff_end],2);		//convert and print the value
			serial_write('\n');
			adc_buff_end++;	//actualization of index
			temp_end--;
		}
		adc_buff_end = 0;				//allows to read the same sampled values until a new conversion is done
	} /*end read operation*/
}

/*Reads x bytes from buffer in external units (input value)*/
void read_buffer_external(void)
{
	unsigned char control_digit1 = str_read();
	unsigned char temp_end = 0;						//to determinate how long will be the read operation
	unsigned char control_digit2 = 0;

	if(control_digit1 == '\0')			/*if the user don't specific the size of the operation*/
	{
		temp_end = adc_buffer_size;
		control_digit2 = '\0';			/*to control if everything is right*/
	}
		
	if(control_digit1 == 0x20)		/*if the user specific the size of the operation*/
	{
		temp_end = parse_number();
		control_digit2 = str_read();
	}
		
	if( (control_digit2 =! '\0') || (temp_end > adc_buffer_size) )
		serial_error();
	
	/*read operation*/
	if( (control_digit2 == '\0') && (temp_end <= adc_buffer_size) )
	{
		if(flag_cycle_complet)					//if makes one lap in the buffer
			adc_buff_end = (adc_buff_start+1);	//reads all the data
				
		if( adc_buff_start == adc_buff_end)						/*if the buffer is empty*/
			serial_writestr("Nothing to print\n");
	
		while ( (adc_buff_start != adc_buff_end) && temp_end)				/*if exist something to transmit*/
		{
			if(adc_buff_end >= adc_buffer_size)
				adc_buff_end = 0;	
		
			unsigned char* string;
			unsigned long int micro_volt;
			temp_end--;		/*decreases until reach the end*/
			micro_volt = (unsigned long int)(Adc_Buffer[temp_end]) * 4883;
			int i = 0;
		
			do {       /* generate digits in reverse order */
				string[i++] = micro_volt % 10;  /* get next digit */
				micro_volt /= 10;			/* delete it */
			} while (micro_volt);

			while(i > 0){       /* send digits in reverse order */
				i--;
				serial_write(string[i]+48);
			}
			serial_writestr(" uV");
			serial_write('\n');
		}	
		adc_buff_end = 0;				//allows to read the same sampled values until a new conversion is done
	}/*end read operation*/
}

/*Reads x bytes from buffer in engineering units (real value)*/
void read_buffer_eng_units(void)
{
	read_buffer_external();
}