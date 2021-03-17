#include <msp430.h>
#include "usciSpi.h"
#include "usciUart.h"
#include "nok5110LCD.h"


/**
 * main client file for nokia 5110 LCD display interface
 */


int main(void){
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    // activate the SCE  chip select
    P4OUT &= ~SCE;
    P4DIR |= SCE + DAT_CMD;

    P8OUT |= BIT1;
    P8DIR |= BIT1;

	usciB1SpiInit(1,2,0x02,0);
	nokLcdInit();

//    nokLcdSetPixel(5,5);
//    nokLcdSetPixel(5,6);
//    nokLcdSetPixel(6,5);
//    nokLcdSetPixel(1,1);
//    nokLcdSetPixel(10,10);
    nokLcdDrawScrnLine(5, 5, 'V');
    nokLcdDrawScrnLine(0, 0, 'H');
    nokLcdDrawScrnLine(10, 43, 'V');

    // debugging loop
	while (1){
	    //nokLcdSetPixel(5,5);
	}

	return 0;
}
