#include <msp430.h>
#include "usciSpi.h"
#include "usciUart.h"
#include "nok5110LCD.h"


/**
 * main client file for nokia 5110 LCD display interface
 */


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	usciB1SpiInit(1,1,0x02,0);
	nokLcdInit();


	return 0;
}
