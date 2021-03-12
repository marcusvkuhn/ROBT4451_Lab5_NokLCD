/*************************************************************************************************
 * nok5110LCD.c
 * - C implementation or source file for NOKIA 5110 LCD.
 *
 *  Author: Greg Scutt
 *  Created on: Feb 20th, 2017
 **************************************************************************************************/

// nok5110LCD pin connectivity --> to MSP-EXP430F5529LP EVM.
//  8-LIGHT  	-->  	no connection necessary
//  7-SCLK  	-->  	MS430EVM  P4.3 or UCB1CLK
//  6-DN(MOSI)  -->  	MS430EVM  P4.1 or UCB1SIMO
//  5-D/C'  	-->  	MS430EVM  P4.2. 	Kept as I/O pin !!
//  4-RST'  	-->  	MS430EVM  I/O pin of your choice. See 8.1, 8.2, 12.2 in Nokia Datasheet.
//  3-SCE'  	-->  	MS430EVM  P4.0.  	Kept as I/O pin !!
//  2-GND  		-->  	MS430EVM or supply VSS
//  1-VDD  		-->  	MS430EVM or supply 3V3. Consider controlling it with an I/O pin

#include <msp430.h>
#include "nok5110LCD.h"
#include "usciSpi.h"

// 2-D 84x6 array that stores the current pixelated state of the display.
// remember a byte (8 bits) sets 8 vertical pixels in a column allowing 8x6=48 rows
// note that this array is GLOBAL to this file only. In that way it is protected from access from other functions in files.
// said another way - it is a private global array with local scope to the file in which the defining declaration exists.
// we don't want other functions messing with the shadow RAM. This is the reason for static and for its dec/defn in this .c file
// static here means that it does not have inter-file scope, it is local to this file only. This is a best practice when you want to
// protect data structures from unwanted access by other functions in other files.
static unsigned char currentPixelDisplay[LCD_MAX_COL][LCD_MAX_ROW / LCD_ROW_IN_BANK];


/************************************************************************************
* Function: nokLcdInit
* -
* argument:
*   none
* return: none
* Author: Greg Scutt
* Date: Feb 20th, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void nokLcdInit(void) {
    // power-on RST sequence here.  The display is not powered until this sequence occurs.
    // hold VCC low
    P2OUT &= ~BIT6;
    // hold !RES high
    P2OUT |= BIT3;
    // init PWR RST pins
    P2DIR |= BIT3 + BIT6;
    // power on
    _PWR;
    _RST;

    P4OUT   &=  ~(SCE | DAT_CMD);   // Set DC and CE Low. This should be made a macro.  But is this command necassary? Doesn't nokLcdWrite do it?

    // send initialization sequence to LCD module
    nokLcdWrite(LCD_EXT_INSTR, DC_CMD);
    nokLcdWrite(LCD_SET_OPVOLT, DC_CMD);
    nokLcdWrite(LCD_SET_TEMPCTRL, DC_CMD);
    nokLcdWrite(LCD_SET_SYSBIAS, DC_CMD);
    nokLcdWrite(LCD_BASIC_INSTR, DC_CMD);
    nokLcdWrite(LCD_NORMAL_DISP, DC_CMD);

    nokLcdClear(); // clear the pixel memory and hence the display.
    /* Sometimes necessary since the pixel ram is not defined after a PWR on and RST. The best practice would be to
     always clear it so no residual pixels are set. You will sometimes see random pixels set and I think it is from not clearing the memory
     which must be done manually. Try removing this function to see what happens.*/

}

/************************************************************************************
* Function: nokLcdWrite
* - performs write sequence to send data or command to nokLCD. Calls spiPutChar to transmit serially to nokLCD
* argument:
* Arguments: lcdByte - the 8 bit char (data or command) that is written to nokLCD.
* 			 cmdType - 0 - lcdByte is a cmd,   1 - lcdByte is data.
* return: none
* Author: Greg Scutt
* Date: Feb 20th, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void nokLcdWrite(char lcdByte, char cmdType) {

	// ********** complete this function. *************

	// check cmdType and output correct DAT_CMD signal to PORT4 based on it. Use definitions in .h file
    switch(cmdType){
        // -- if its a command, issue a 0
        case DC_CMD:  P4OUT &= ~DAT_CMD;
                      break;
        // -- if its data, issue a 1
        case DC_DAT:  P4OUT |= DAT_CMD;
                      break;

        default: break;
    }
	// activate the SCE  chip select
    P4OUT |= SCE;

    // transmit lcdByte with spiPutChar from Lab 3.  That function must stay in the spi C module.
    usciB1SpiPutChar(lcdByte);

    // wait for SPI transmission to complete. You need to poll an SPI interrupt flag. Which one RXIFG or TXIFG? Understand why.
    while (!(UCB1IFG & UCRXIFG)); // When RXIFG == 1, RXBUF has received a complete character

    // when transmission is complete deactivate the SCE */
    P4OUT &= ~SCE;
}

/************************************************************************************
* Function: nokLcdSetPixel
* -
* argument:
*	xPos - The horizontal pixel location in the domain (0 to 83)
*	yPos - The vertical pixel location in the domain (0 to 47)
*
* return: 0 - pixel was valid and written.  1 - pixel not valid
* Author: Greg Scutt
* Date: Feb 20th, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
unsigned char  nokLcdSetPixel(unsigned char xPos, unsigned char yPos) {
	unsigned char bank; // a bank is a group of 8 rows, selected by 8 bits in a byte

	// verify pixel position is valid
	if ((xPos < LCD_MAX_COL) && (yPos < LCD_MAX_ROW)) {

		// if-else statement avoids costly division
		if (yPos<8) bank = 0;
		else if (yPos<16) bank = 1;
		else if (yPos<24) bank = 2;
		else if (yPos<32) bank = 3;
		else if (yPos<40) bank = 4;
		else if (yPos<48) bank = 5;

		// set the x and y RAM address  corresponding to the desired (x,bank) location. this is a command DC_CMD
		nokLcdWrite(LCD_SET_XRAM | xPos, DC_CMD);
		nokLcdWrite(LCD_SET_YRAM | bank, DC_CMD);

		// update the pixel being set in currentPixelDisplay array
		currentPixelDisplay[xPos][bank] |= BIT0 << (yPos % LCD_ROW_IN_BANK); // i.e if yPos = 7 then BIT0 is left shifted 7 positions to be 0x80. nice mod.
		nokLcdWrite(currentPixelDisplay[xPos][bank], DC_DAT); // write the byte defining a single pixel
		return 0;
	}
	return 1;
}

/************************************************************************************
* Function: nokLcdDrawScrnLine
* - draws either a horizontal or a vertical line on the Nokia display
*
* arguments: x coordinate
*            y coordinate
*
* return: 0 if inputs were valid, -1 if not
* Author: Marcus Kuhn
* Date: Mar 11th, 2021
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
int nokLcdDrawScrnLine(int x, int y){
    //-- tbd
}


/************************************************************************************
* Function: nokLcdClear
* - clears all pixels on LCD diplay. results in blank display.
* argument:
*   none
* return: none
* Author: Greg Scutt
* Date: Feb 20th, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void nokLcdClear(void) {

    unsigned char bank; // bank (group of 8 rows) on which pixel falls
    unsigned char x;    // x coordinate to track columns

    // sweep banks (or group of 8 rows)
    for (bank = 0; bank < 6; bank++) {

        // start in first column of the bank (0, bank)
        nokLcdWrite(LCD_SET_XRAM | 0, DC_CMD);
        // start in current bank
        nokLcdWrite(LCD_SET_YRAM | bank, DC_CMD);

        // sweep columns. X address is auto-incremented by one since V = 0.
        for (x = 0; x < LCD_MAX_COL; x++) {
            currentPixelDisplay[x][bank] = 0;       // update pixel display array to keep pixel state current
            nokLcdWrite(0, DC_DAT);                 // clear all 8 pixels at (x,bank)
        }
    }
}
