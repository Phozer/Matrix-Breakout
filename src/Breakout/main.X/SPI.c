/*
 * File:   SPI.c
 * Author: sven_stocker
 *
 * Created on 1. April 2022, 15:46
 */

#include "SPI.h"


void SPI_init(){  
  // this setting selects master mode with frequency fosc/4
  SSPCON1bits.SSPM0 = 0;
  SSPCON1bits.SSPM1 = 0;
  SSPCON1bits.SSPM2 = 0;
  SSPCON1bits.SSPM3 = 0;
  // Enable SPI Port
  SSPCON1bits.SSPEN = 1;
  // Configure The Clock Polarity & Phase
  SSPCON1bits.CKP = 0;
  SSPSTATbits.CKE = 0;
  // Slew rate control enabled for High Speed mode
  SSPSTATbits.SMP = 0;
  // Alternate pin Function Control Register
  APFCON1bits.SDOSEL = 1; //sets SDO on RB5
  APFCON1bits.SDISEL = 1; //sets SDI on RB6
  APFCON1bits.SCKSEL = 1; //sets SCL on RB7
  // TRISx 0 = Output, 1 = Input
  TRISB5 = 0; // SDO -> Output
  TRISB6 = 1; // SDI -> Input
  TRISB7 = 0; // SCK -> Output
  // ANSELx 0 = Digital i/o, 1 = Analog i/o
  ANSELBbits.ANSB5 = 0;
  ANSELBbits.ANSB6 = 0;
}


void SPI_write(char data){
  SSPBUF = data;
  while(BF == 0);
}

void SPI_write_array(int array[], int arrayindex){
    for(int i = 0; i < arrayindex+1; i++){
        int data = array[i];
        SSPBUF = data;
        while(BF == 0);
        __delay_ms(500);    //delay fuer Ueberpruefung, muss entfernt werden
    }
}
