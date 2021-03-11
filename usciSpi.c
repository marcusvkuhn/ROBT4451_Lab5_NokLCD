/*
 * usciSpi.c
 *
 * create a proper file header for the C module
 */

#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>

#include "usciSpi.h"
#include "usciUart.h"


// create a function header that describes the function and how to use it. Provide an example function call.
void usciB1SpiInit(unsigned char spiMST, unsigned int sclkDiv, unsigned char sclkMode, unsigned char spiLoopBack){
	
	// call usciB1SpiClkDiv
	usciB1SpiClkDiv(sclkDiv);

    UCB1CTL1 |= UCSWRST;                        // **Put state machine in USCI reset while you intitialize it**

	// configure the control registers using the input arguments. See user manual, lecture notes and programmers model
	UCB1CTL0 |= (sclkMode << 6)      // set clock mode sclkMode.1 = UCCKPH, sclkMode.0 = UCCKPL
	         + (spiMST << 3)        // 0 = slave, 1 = master
	         + UCMSB
	         + UCSYNC;              // Sync == 1, SPI mode

	UCB1CTL1 |= UCSSEL__SMCLK;
	
	if(spiLoopBack)
	    UCB1STAT = UCLISTEN;

	// configure the SPI B1 pins with PxSEL register
	P4SEL |= SIMO_B1 + SOMI_B1 + SCLK_B1;

	// configure P6.0 to be output (SS)
	P6DIR |= SS_B1;

	UCB1CTL1 &= ~UCSWRST;                     	// **Initialize USCI state machine**  take it out of reset
}


// provide function header
// this function is complete. Understand what it is doing.  Call it when SCLKDIV needs to be changed in Lab.
void usciB1SpiClkDiv(unsigned int sclkDiv){

    UCB1CTL1 |= UCSWRST;                        // you always need to put state machine into reset when configuring USC module

    UCB1BR0 = (sclkDiv&0xFF);                   // 2
    UCB1BR1 = (sclkDiv>>8);                     //

    UCB1CTL1 &= ~UCSWRST;                       // **Initialize USCI state machine**
}


// when the TXBUFFER is ready load it.    txByte-->TXBUFFER
// provide a function header
void usciB1SpiPutChar(char txByte) {
    while (!(UCB1IFG & UCTXIFG)); // is this efficient ?
        UCB1TXBUF = txByte;  // if TXBUFF ready then transmit a byte by writing to it
}

int usciB1SpiTxBuffer(int* buffer, int buffLen){
    volatile int i = 0;

    P6OUT &= ~SS_B1;     //assert SS

    for(i = 0; (i < buffLen) && (*buffer != NULL_CHAR) ; i++){
        usciB1SpiPutChar(*buffer++);
    }
    P6OUT |= SS_B1;       //de-assert SS immediately after the last byte has been transmitted

    //usciB1SpiPutChar('\n');             // move terminal to next line
    return i;
}

//---- atoi on each byte in rxString and store in buffer
void numStringToInt(char* rxString, int* rxBuffer){
    volatile int i;
    for(i = 0; rxString[i] != NULL_CHAR ; i++){
        rxBuffer[i] = (int)(rxString[i]) - 48;
    }
}


#pragma vector=USCI_B1_VECTOR
__interrupt void usciB1SpiIsr(void) {
	
// UCB1IV interrupt handler. __even_in_range will optimize the C code so efficient jumps are implemented.
  switch(__even_in_range(UCB1IV,4)) // this will clear the current highest priority flag. TXIFG or RXIFG.
  {
  	  case 0: break;                          	// Vector 0 - no interrupt
  	  case 2:                                 	// Vector 2 - RXIFG. Highest priority
		// process RXIFG
  		
  		  break;

  	  case 4:									// Vector 4 - TXIFG
  		
		// process TXIFG (careful)
		
  		  break;

  	  default: break; 
  }
}
