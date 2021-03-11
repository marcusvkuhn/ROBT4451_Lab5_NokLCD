/*
 * usciSpi.h
 *
 *  Created on: Mar 2, 2021
 *      Author: Raymond
 */

#ifndef USCISPI_H_
#define USCISPI_H_

#define SIMO_B1 BIT1
#define SOMI_B1 BIT2
#define SCLK_B1 BIT3

#define SS_B1   BIT0

#define BUFFER_SZ 100

//------

void usciB1SpiInit(unsigned char spiMST, unsigned int sclkDiv, unsigned char sclkMode, unsigned char spiLoopBack);
void usciB1SpiClkDiv(unsigned int sclkDiv);
void usciB1SpiPutChar(char txByte);
int usciB1SpiTxBuffer(int* buffer, int buffLen);
void numStringToInt(char* rxString, int* rxBuffer);





#endif /* USCISPI_H_ */
