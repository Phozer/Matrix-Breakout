/*
 * File:   UART.h
 * Author: Marco_Steiger
 *
 * Created on 4. April 2022, 10:51
 * 
 * Library for UART communication with a 8-bit PIC microcontroller. 
 */

#ifndef UART_H
#define	UART_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

/****** CHANGE VALUES BELOW ******/
// System clock frequency
#define _XTAL_FREQ  8000000

// Define the pins used
#define UART_TRIS_RX     TRISCbits.TRISC7
#define UART_TRIS_TX     TRISCbits.TRISC6

/**
 * Initialize the UART module with given baud rate and speed mode
 * @param baud_rate
 * @param BRGH speed mode (high speed or low speed)
 * @param Select Alternate Pins 
 */
void UARTInit(const uint32_t baud_rate, const uint8_t BRGH, const bool alternate);

/**
 * Send a character
 * @param c character to send
 */
void UARTSendChar(const char c);

/**
 * Send a string, until '\0' is read, up to max_length characters
 * @param str
 * @param max_length
 */
void UARTSendString(const char* str, const uint8_t max_length);

/**
 * Determines whether a new data is available to read
 * @return 
 */
uint8_t UARTDataReady();

/**
 * Read one character
 * @return  byte read
 */
char UARTReadChar();

/**
 * Read a string, until we read '\0', up to max_length characters.
 * @param buf   pre-allocated buffer
 * @param max_length
 * @return number of bytes read
 */
uint8_t UARTReadString(char *buf, uint8_t max_length);

#endif	/* UART_H */

