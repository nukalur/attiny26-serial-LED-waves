#include <avr/io.h> 
#include <avr/interrupt.h>  
#include <util/delay.h> 
#include <stdlib.h>
#include "USI_UART.h"
#include "elapsed.h"
#include "softpwm.h"
#include <stdbool.h> 


#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

int map(int x, int in_min, int in_max, int out_min, int out_max);

bool state = 1;
unsigned long lastred,lastgreen,lastblue,last;

static unsigned char          mybuffer[UART_RX_BUFFER_SIZE]; 
int rxindex;
volatile static unsigned char red,blue,green,checksum;
int main(void) {

	timer1init();
	PWMInit();
	USI_UART_Initialise_Receiver();
	lastred = elapsed();

	while(1) {
	
		_delay_ms(10);
	
		if(USI_UART_Data_In_Receive_Buffer()) { 				// if there is data in receive buffer
			rxindex = 0; 										// reset my buffer rx index
			while(USI_UART_Data_In_Receive_Buffer()) {			// move from rx buffer 
				mybuffer[rxindex] = USI_UART_Receive_Byte();	// to my buffer
				rxindex++;										// incr index
			}
		} 														// nothing in rx buffer so we can transmit
		
		if(!USI_UART_Data_In_Receive_Buffer() && mybuffer[0]) { 
			rxindex=0;											 // reset rx index
			while(mybuffer[rxindex]) {							 // while we have rx buffer data
				if(mybuffer[rxindex] == 0xFF) {				// check for start
					red=mybuffer[rxindex+1];
					green=mybuffer[rxindex+2];
					blue=mybuffer[rxindex+3];
					//checksum=mybuffer[rxindex+4];
					//if(checksum == (red^green^blue)) {		// good checksum
						setPWM(0,red);
						setPWM(1,green);
						setPWM(2,blue);
						mybuffer[rxindex]=0x00;
					//} else { // badchecksum
					//	USI_UART_Transmit_Byte('e');
					//	USI_UART_Transmit_Byte('r');
					//	USI_UART_Transmit_Byte('r');
					//	USI_UART_Transmit_Byte('\n');
					//	mybuffer[rxindex] = 0x00;				 // clear byte in buffer
					//	rxindex++; 								 // next index, look for data again
					//}
				} else {
				mybuffer[rxindex] = 0x00;						 // clear byte in buffer
				rxindex++; 										 // next index, look for data again
				}
				
			}
		}
		
		/*
		
		
		if(last + 4 > elapsed()) { // 4 * 2.5 = 10ms yah yah my ms timer is busted
		    USI_UART_Transmit_Byte(13);
			if(fstate) {
						setPWM(0,red);
						setPWM(1,green);
						setPWM(2,blue);
						state = 0;
						last = elapsed();
						
			} else {
						setPWM(0,0x00);
						setPWM(1,0x00);
						setPWM(2,0x00);
						state = 1;
						last = elapsed();
			}
		}
		
		*/
		
		if(lastred + 500 > elapsed()) { 
				static int wavei = 0;
				if(wavei<128) {
					setPWM(0,map(wavei,0,128,0,red));
					wavei++;
				} else if (wavei>127 && wavei<256) {
					setPWM(0,map(256-wavei,0,128,0,red));
					wavei++;
				} else {
					wavei=0;
				}
				lastred = elapsed();				
		}

		if(lastgreen + 750 > elapsed()) { 
				static int wavei = 0;
				if(wavei<64) {
					setPWM(1,map(wavei,0,64,0,green));
					wavei++;
				} else if (wavei>63 && wavei<128) {
					setPWM(1,map(128-wavei,0,64,0,green));
					wavei++;
				} else {
					wavei=0;
				}
				lastgreen = elapsed();	
		}
		
		if(lastblue + 1000 > elapsed()) {
				static int wavei = 0;
				if(wavei<32) {
					setPWM(2,map(wavei,0,32,0,blue));
					wavei++;
				} else if (wavei>31 && wavei<64) {
					setPWM(2,map(64-wavei,0,32,0,blue));
					wavei++;
				} else {
					wavei=0;
				}
				lastblue = elapsed();	
		}
		
	
	}
	return 1;

 
}



int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
