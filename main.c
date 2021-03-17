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
    P4DIR |= SCE;

    P8OUT |= BIT1;
    P8DIR |= BIT1;

	usciB1SpiInit(1,2,0x02,0);
	nokLcdInit();

    nokLcdSetPixel(5,5);
    nokLcdSetPixel(50,20);
    nokLcdSetPixel(50,21);
    nokLcdSetPixel(50,22);
    nokLcdSetPixel(50,23);
	//nokLcdDrawScrnLine(5, 5, 'V');
	while (1);

	return 0;
}
