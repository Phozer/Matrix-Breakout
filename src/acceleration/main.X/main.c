/*
 * File:   main.c
 * Author: Marco_Steiger
 *
 * Created on 28. March 2022
 */

/*******************************************************************************
 * Beschreibung:
 * UART
 *  Tx      RC6
 *  Rx      RC7
 * 
 * 
 * Datum        Vers.   Name            Kommentar / Änderung
 * 28.03.2022   1.0     Marco Steiger   erstellung
 * 04.04.2022   1.1     Marco Steiger   Implementierung UART
 *
 *
 ******************************************************************************/

//*** Config ***
#pragma config FOSC = HS        // Oscillator Selection (HS Oscillator, 
// High-speed crystal/resonator connected 
// between OSC1 and OSC2 pins)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select 
// (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection 
// (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection 
// (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable 
// (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function disabled.
// I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover 
// (Internal/External Switchover mode enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable 
// (Fail-Safe Clock Monitor is enabled)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection 
// (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit 
// (Vcap functionality is disabled on RA6.)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack
// Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection 
// (Brown-out Reset Voltage (Vbor),
// low trip point selected.)
#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit 
// (Low power brown-out is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable 
// (Low-voltage programming enabled)

// *** Include Files ***
#include <xc.h>                 // Einbindung Compiler XC8 / PIC16F1787
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "UART.h"

// *** Defines ***
#define _XTAL_FREQ 8000000      // Oszillatorfrequenz PBA6: 8MHz (ohne PLL)
#define MAX_LENGTH_UART 16      // Max lenght of a string send or received with UART

// *** GLOABLE Variabeln ***
char arr[3][40]= {"Hello", "wonderful", "world"};

// *** FUNKTIONSPROTOTYPEN ***

// *** Main-Routine ***

void main(void) {
  char* readBuf[MAX_LENGTH_UART];
  char* tmpBuf[8];
  int nRead = 0;

  // *** PORT INITIALISIERUNGEN *** 
  // ** TRISx ** 
  TRISBbits.TRISB1 = 1; // PORTB1 1 = Input
  TRISBbits.TRISB2 = 1; // PORTB2 1 = Input
  TRISBbits.TRISB7 = 0; // PORTB0 0 = Output

  // UART
  TRISCbits.TRISC6 = 0; // 0: C6 is digital output
                        // 1: C6 is digital (tristate) input
  TRISCbits.TRISC7 = 1; // 0: C7 is digital output
                        // 1: C7 is digital (tristate) input

  // ** ANSELx *** 
  ANSELBbits.ANSB1 = 1; // Set RB1 as ANALOG I/O
  ANSELBbits.ANSB2 = 1; // Set RB2 as ANALOG I/O
  // RB7 is default DIGITAL I/O

  // ** LATx ** 
  LATD = 0;

  UARTInit(9600, 1, true);
  
  while (1) {
    
    for (int i = 0; i < 4; i++) {
      UARTSendString(arr[i], MAX_LENGTH_UART);
      UARTSendString("\n\r\0", MAX_LENGTH_UART);
    }
    __delay_ms(100);
//    // Send prompt
//    UARTSendString("Hallo \n\0", MAX_LENGTH_UART);
//    // Read response
//    nRead = UARTReadString(readBuf, MAX_LENGTH_UART);
//    // Send number of bytes received
//    itoa(tmpBuf, nRead, 10);
//    UARTSendString("\n\rReceived \0", MAX_LENGTH_UART);
//    UARTSendString(tmpBuf, MAX_LENGTH_UART);
//    UARTSendString(" bytes: \0", MAX_LENGTH_UART);
//    // Send echo of response.
//    UARTSendString(readBuf, nRead);
//    UARTSendString("\n\r\0", MAX_LENGTH_UART);
  } // end of while
} // end of main
