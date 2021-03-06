/*************************************************************************************************
 * nok5110LCD.h
 * - C interface file for NOKIA 5110 LCD.
 *
 *  Author: Greg Scutt
 *  Created on: Feb 20th, 2017
 **************************************************************************************************/

// nok5110LCD pin connectivity --> to MSP-EXP430F5529LP EVM.
// Assumes UCB1SI  is used.
//  8-GND  		-->  	MS430 Ground
//  7-LIGHT     -->     P8.1
//  6-VCC  		-->  	MSP430EVM P2.6
//  5-SCLK      -->     MSP430EVM  P4.3 or UCB1CLK
//  4-DN(MOSI)  -->     MSP430EVM  P4.1 or UCB1SIMO
//  3-D/C'      -->     MSP430EVM  P4.2 Kept as I/O pin !!
//  2-SCE'      -->     MSP430EVM  P4.0 chip select     Kept as I/O pin !!
//  1-RST'      -->     MSP430EVM or supply VSS.  Can tie to I/O pin if user wants. data sheet says RESET is necessary .. but?


#ifndef nok5110LCD_H_
#define nok5110LCD_H_

// nok5110 pin --> msp430 PORT4 bit position
#define SCLK  	BIT3
#define DAT_CMD BIT2
#define SIMO 	BIT1
#define	SCE   	BIT0

// constants for cmdType argument passed to the nokLcdWrite function
#define DC_CMD  0	// command control
#define DC_DAT  1	// data control

// NOKIA 5110 LCD row,col MAX
#define LCD_MAX_COL 84  	// 	max # of columns.  84 pixels in x direction
#define LCD_MAX_ROW 48     	// 	max # of row.  48 pixels in y direction

// NOKIA 5110 LCD controller instructions for initialization
#define LCD_NORMAL_DISP      	0x0C // normal display control
#define LCD_EXT_INSTR 			0x21 // enable extended instruction set
#define LCD_BASIC_INSTR      	0x20 // enable basic instruction set
#define LCD_SET_SYSBIAS       	0x13 // set system bias mode
#define LCD_SET_OPVOLT     		0xBC // set operation voltage for contrast ctrl
#define LCD_SET_TEMPCTRL       	0x04 // set coeff 2
#define LCD_SET_YRAM          	0x40 // set Y address of RAM
#define LCD_SET_XRAM          	0x80 // set X address of RAM


#define LCD_ROW_IN_BANK 8 	    // 8 rows in a bank. 6 banks, so  8x6 = 48 rows of pixels. y coordinate

//-- added by me
#define _PWR P2OUT |= BIT6                // power on transistor
#define _RST P2OUT &= ~BIT3; P2OUT |= BIT3 // reset strobe

/************************************************************************************
* Function: nokLcdInit
* -
* argument:
*	none
* return: none
* Author: Greg Scutt
* Date: Feb 20th, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void nokLcdInit(void);

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
unsigned char nokLcdSetPixel(unsigned char xPos, unsigned char yPos);

/************************************************************************************
* Function: nokLcdWrite
* - performs write sequence to send data or command to nokLCD. Calls spiTxByte transmit serially to nokLCD
* argument:
* Arguments: lcdByte - the 8 bit char (data or command) that is written to nokLCD.
* 			 cmdType - 0 - lcdByte is a cmd,   1 - lcdByte is data.
* return: none
* Author: Greg Scutt
* Date: Feb 20th, 2017
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
void nokLcdWrite(char lcdByte, char cmdType);

/************************************************************************************
* Function: nokLcdDrawScrnLine
* - draws either a horizontal or a vertical line on the Nokia display
*
* arguments: x coordinate
*            y coordinate
*            mode - Vertical or Horizontal line
*
* return: 0 if inputs were valid, -1 if not
* Author: Marcus Kuhn
* Date: Mar 11th, 2021
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
int nokLcdDrawScrnLine(int x, int y, char mode);

/************************************************************************************
* Function: nokLcdDrawLine
* - draws a line between two coordinates in the Nokia display using bresenham's line algorithm
*
* arguments: (x0, y0) start coordinate
*            (x1, y1) finish coordinate
*
* return: 0 if inputs were valid, -1 if not
* Author: Marcus Kuhn
* Date: Mar 20th, 2021
* Modified: <date of any mods> usually taken care of by rev control
************************************************************************************/
int nokLcdDrawLine(int x0, int y0, int x1, int y1);

//-- Bresenham's line algorithm for when dx > dy
void plotLineLow(int x0, int y0, int x1, int y1);

//-- Bresenham's line algorithm for when dy > dx
void plotLineHigh(int x0, int y0, int x1, int y1);

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
void nokLcdClear(void);

#endif /* nok5110LCD_H_ */
