#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"
#include "lib/led.h"
#include "lib/pot.h"
#include "lib/speaker.h"
#include "lib/button.h"

volatile unsigned int x_prev[3], x[3], x_prev2[3];
volatile uint8_t *bufcontents;

ISR(INT1_vect)
{	
}

void read_config()
{
	COLOR = eeprom_read_byte((uint8_t*)0);
    BRIGHTNESS = eeprom_read_byte((uint8_t*)8);
    TIME = eeprom_read_byte((uint8_t*)16);

    if(COLOR > 6 || COLOR < 1)
    	COLOR = 1;

    if(BRIGHTNESS > 6 || BRIGHTNESS < 1)
    	BRIGHTNESS = 6;

    led_brightness_to_power();

     if(TIME> 6 || TIME < 1)
    	TIME = 1;
}

void read_silent_values()
{
	x[0] = adc[POT1]/200; // volume or color 
	x[1] = adc[POT2]/200; // freq or brigthness
	x[2] = adc[POT3]/200; // sensivity
	
	VOL = 4 - x[0];
	SPK_FREQ = x[1];
	SENSIVITY = x[2];
	
	x_prev[0] = x[0];
	x_prev[1] = x[1];
	x_prev[2] = x[2];
	
}

volatile int adc_diff = 0;
 void normal_mode()
 {
		adc_diff = adc[POT1]- x_prev2[0];
		if(adc_diff<0)adc_diff*=-1;
		
		if (x[0] != x_prev[0] && adc_diff > 50) 
		{
			wake_up_after_sleep(1);
			VOL = 4 - x[0];
			x_prev[0] = x[0];
			x_prev2[0] = adc[POT1];		
			led_bar(VOL + 2, COLOR, 1);			
			play_speaker(100, 0);
		}

		adc_diff = adc[POT2]- x_prev2[1];
		if(adc_diff<0)adc_diff*=-1;
		
		if (x[1] != x_prev[1]&& adc_diff > 50)
		{				
			wake_up_after_sleep(1);
			SPK_FREQ = x[1];
			x_prev[1] = x[1];	
			x_prev2[1] = adc[POT2];		
			led_bar(6-SPK_FREQ, COLOR, 1);			
			play_speaker(100, 0);	
		}

		adc_diff = adc[POT3]- x_prev2[2];
		if(adc_diff<0)adc_diff*=-1;

		if (x[2] != x_prev[2] && adc_diff > 50)
		{				
			wake_up_after_sleep(1);
			SENSIVITY = x[2];
			x_prev2[2] = adc[POT3];	
			x_prev[2] = x[2];			
			led_bar(SENSIVITY + 1, COLOR, 1);
			play_speaker(100, 0);		
		}
	
}

void config_mode()
{
		adc_diff = adc[POT1]- x_prev2[0];
		if(adc_diff<0)adc_diff*=-1;
		
		if (x[0] != x_prev[0] && adc_diff > 50) 
		{		
			COLOR = x[0]+1;
			led_bar(6, COLOR, 1);
			x_prev[0] = x[0];	
			x_prev2[0] = adc[POT1];
		}

		adc_diff = adc[POT2]- x_prev2[1];
		if(adc_diff<0)adc_diff*=-1;
		
		if (x[1] != x_prev[1]&& adc_diff > 50) 
		{				
			TIME = 6-x[1];
			led_bar(6-x[1], COLOR, 1);
			x_prev[1] = x[1];			
			x_prev2[1] = adc[POT2];	
		}

		adc_diff = adc[POT3]- x_prev2[2];
		if(adc_diff<0)adc_diff*=-1;

		if (x[2] != x_prev[2] && adc_diff > 50) 
		{				
			BRIGHTNESS = 6-x[2];
			led_brightness_to_power();
			led_bar(6, COLOR, 1);
			x_prev[2] = x[2];	
			x_prev2[2] = adc[POT3];			
		}
		
}

int main(void) 
 {  
 	button_init();
	read_config(); 
	pot_init();
	led_init();
	speaker_init();
	_delay_ms(100);
	sei();
	read_silent_values();
	branie_counter = 500;

	while(1)
	{     
		if(GO_TO_POWER_DOWN > 0 && THEFT_ALARM == 0)
		{
			GO_TO_POWER_DOWN = 0;			
			power_down();
		}
		else
		{
			if(STATUS == 5)
			{
				x[0] = adc[POT1]/150; // volume or color
				x[1] = adc[POT2]/200; // freq or brigthness
				x[2] = adc[POT3]/200; // sensivity
				config_mode();
			}
			else
			{
				x[0] = adc[POT1]/200; // volume or color
				x[1] = adc[POT2]/200; // freq or brigthness
				x[2] = adc[POT3]/200; // sensivity
				normal_mode();
			}

		}

	}  
 }

