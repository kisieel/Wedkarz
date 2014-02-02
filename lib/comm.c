#include "comm.h"

uint8_t SYG_ID[] = {0xFF, 0, 0, 0};
void parse_buffer(uint8_t *bufcontents, uint8_t length)
{
	for(int i = 0; i < length; i++)
	{
		if(bufcontents[i] == 0xFF && (i + 5) < length)
		{

			if(PAIRING && bufcontents[i+4] == 0x0C)
			{
				SYG_ID[1] = bufcontents[i+1];
				SYG_ID[2] = bufcontents[i+2];
				SYG_ID[3] = bufcontents[i+3]; 
				led_set(6, 1);
				led_push();
				_delay_ms(100);
				led_set(6, 0);
				led_push();
				_delay_ms(100);
				led_set(6, 1);
				led_push();
				_delay_ms(100);
				led_set(6, 0);
				led_push();
				comm_wywolanie = 2;
				PAIRING = 0;
				return;
			}

			if(bufcontents[i+1] != SYG_ID[1] || bufcontents[i+2] != SYG_ID[2] ||
				bufcontents[i+3] != SYG_ID[3] )
				return;


			switch(bufcontents[i+4])
			{
				case 0x0A:

				break;

				case 0x0B:

				if(bufcontents[i+5] == 0x01)
				{
					ANTI_THEFT = 1;
				}
				else if(bufcontents[i+5] == 0x02)
				{
					ANTI_THEFT = 0;
					THEFT_ALARM = 2;
				}

				break;

				case 0x0C:
					//comm_wywolanie = 1;
				break;
			}


			return;
		}
	}
}
void send_packet(uint8_t id, uint8_t param)
{

}