/*************************************************************************************************
 * usciUart.h
 * - C interface file for MSP430 usci UART A1, A0
 *
 *  Author: Greg Scutt
 *  Created on: March 1, 2017
 *  Modified: Feb 19, 2018
 **************************************************************************************************/

#ifndef USCIUART_H_
#define USCIUART_H_

#define     _UART_A1PSEL    P4SEL |= PM_UCA1TXD | PM_UCA1RXD // use macros for short expressions.

#define     TXD_A1          BIT4            //Transmit Data on P4.4
#define     RXD_A1          BIT5            //Recieve Data on P4.5
#define     NULL_CHAR       '\0'          // null char
#define     NL_CHAR         0x0D          // new line char
#define     BUFF_SZ         100
#define     PER_DELAY       168000  // 80 ms delay

void usciA1UartInit();

void usciA1UartTxChar(char txChar);

void usciA1UartTxString(char* txChar);

int usciA1UartTxBuffer(char* buffer, int buffLen);

char* usciA1UartGets(char* rxString);

char rxBuffer[BUFF_SZ];


#endif /* USCIUART_H_ */
