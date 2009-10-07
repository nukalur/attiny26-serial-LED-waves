#include <avr/io.h> 
#include <avr/interrupt.h>  
#include "util/delay.h" 
#include "USI_UART.h"
#include "elapsed.h"
#include "softpwm.h"
#include <stdbool.h> 

bool flashstate = 1;
long lastflash;

static unsigned char          mybuffer[UART_RX_BUFFER_SIZE]; 
int rxindex;
volatile static unsigned char red,blue,green,checksum;
int main(void) {

	timer1init();
	PWMInit();
	USI_UART_Initialise_Transmitter(); 
	USI_UART_Initialise_Receiver();
	lastflash = elapsed();

	while(1) {
	
		_delay_ms(10);
	
		if(USI_UART_Data_In_Receive_Buffer()) { 				// if there is data in receive buffer
			rxindex = 0; // reset my buffer rx index
			while(USI_UART_Data_In_Receive_Buffer()) {			// move from rx buffer to my buffer	
				mybuffer[rxindex] = USI_UART_Receive_Byte();	
				rxindex++;										// incr index
			}
		} 														// nothing in rx buffer so we can transmit
		
		_delay_ms(10);
		
		// echo buffer then reset index and process command -- debug
		if(!USI_UART_Data_In_Receive_Buffer() && mybuffer[0]) { // still nothing in buffer, useless check
			rxindex=0;											 // reset rx index
			while(mybuffer[rxindex]) {							 // while we have something to send
				USI_UART_Transmit_Byte(mybuffer[rxindex]);		 // send byte
				_delay_ms(1);
				rxindex++;
				
			}
			rxindex=0; 											//reset rxindex
			
		}
		
		_delay_ms(10);
		
		if(!USI_UART_Data_In_Receive_Buffer() && mybuffer[0]) { // 
			rxindex=0;											 // reset rx index
			while(mybuffer[rxindex]) {							 // while we have rx buffer data
				if(mybuffer[rxindex] == 0xFF) {			// check for start
					red=mybuffer[rxindex+1];
					green=mybuffer[rxindex+2];
					blue=mybuffer[rxindex+3];
					//checksum=mybuffer[rxindex+4];
					//if(checksum == (red^green^blue)) { // good checksum
						setPWM(0,red);
						setPWM(1,green);
						setPWM(2,blue);
						
						USI_UART_Transmit_Byte('o');
						_delay_ms(1);				
						USI_UART_Transmit_Byte('k');
						_delay_ms(1);
						USI_UART_Transmit_Byte('\n');
						
						mybuffer[rxindex]=0x00;
					//} else { // badchecksum
					//	USI_UART_Transmit_Byte('e');
					//	USI_UART_Transmit_Byte('r');
					//	USI_UART_Transmit_Byte('r');
					//	USI_UART_Transmit_Byte('\n');
					//	mybuffer[rxindex] = 0x00;						 // clear byte in buffer
					//	rxindex++; // next index, look for data again
					//}
				} else {
				mybuffer[rxindex] = 0x00;						 // clear byte in buffer
				rxindex++; // next index, look for data again
				}
				
			}
		}
		if(lastflash + 5 < elapsed()) {
			if(flashstate) {
						setPWM(0,red);
						setPWM(1,green);
						setPWM(2,blue);
						flashstate = !flashstate;
			} else {
						setPWM(0,0x00);
						setPWM(1,0x00);
						setPWM(2,0x00);
						flashstate = !flashstate;
			}
		}
	}
	return 1;

 
}
