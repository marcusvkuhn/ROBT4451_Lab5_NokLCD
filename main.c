#include <msp430.h>
#include "usciSpi.h"
#include "usciUart.h"
#include "nok5110LCD.h"
#include "cmdNok5110LCD.h"


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

	usciB1SpiInit(1,1,0x02,0);
	nokLcdInit();
    usciA1UartInit();

    CMD nok5110Cmds[MAX_CMDS]; //this is an array of vnh7070Cmds of type CMD
    initNok5110Cmds(nok5110Cmds);

    char rxString[50];
    unsigned char errorMsg[] = "Error!";


    int cmdIndex;
        do{
            usciA1UartGets(rxString);
            cmdIndex = parseCmd(nok5110Cmds, rxString);
            if (cmdIndex != -1){
                if (cmdIndex != QUIT_IDX){
                    //displayParsing(scaraCmds, cmdIndex);
                    executeCMD(nok5110Cmds, cmdIndex);
                }
            }
            else
                usciA1UartTxString("Invalid command.");
        } while (cmdIndex != QUIT_IDX);


//	nokLcdDrawLine(0,0,5,5);
//    nokLcdDrawLine(5,5,0,0);
//    nokLcdDrawLine(10,0,0,10);
//    nokLcdDrawLine(10,10,0,0);
//
//    nokLcdSetPixel(5,5);
//    nokLcdSetPixel(5,6);
//    nokLcdSetPixel(6,5);
//    nokLcdSetPixel(1,1);
//    nokLcdSetPixel(10,10);
//    nokLcdDrawScrnLine(5, 5, 'V');
//    nokLcdDrawScrnLine(0, 0, 'H');
//    nokLcdDrawScrnLine(10, 43, 'V');

    // debugging loop
//	while (1){
//	    //nokLcdSetPixel(5,5);
//	}

	return 0;
}
