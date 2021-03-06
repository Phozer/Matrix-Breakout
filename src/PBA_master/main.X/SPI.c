/*
 * File:   SPI.c
 * Author: sven_stocker
 *
 * Created on 1. April 2022, 15:46
 */

#include "SPI.h"

#define CS_DIGI_DOT_BOOSTER         RB4

void SPI_init(){  
  // this setting selects master mode with frequency fosc/16
  SSPCON1bits.SSPM0 = 1;
  SSPCON1bits.SSPM1 = 0;
  SSPCON1bits.SSPM2 = 0;
  SSPCON1bits.SSPM3 = 0;
  
  // Enable SPI Port
  SSPCON1bits.SSPEN = 1;
  
  // Configure The Clock Polarity & Phase
  SSPCON1bits.CKP = 0;
  SSPSTATbits.CKE = 1; 
  
  // Slew rate control enabled for High Speed mode
  SSPSTATbits.SMP = 0;
  
  // Alternate pin Function Control Register
  APFCON1bits.SDOSEL = 1; //sets SDO on RB5
  APFCON1bits.SDISEL = 1; //sets SDI on RB6
  APFCON1bits.SCKSEL = 1; //sets SCL on RB7
  
  // TRISx 0 = Output, 1 = Input
  TRISBbits.TRISB3 = 0; //CS Gyroscope 
  TRISBbits.TRISB4 = 0; //CS Digi Dot Booster
  TRISBbits.TRISB5 = 0; //SDO -> Output (MOSI)
  TRISBbits.TRISB6 = 1; //SDI -> Input  (MISO)
  TRISBbits.TRISB7 = 0; //SCK -> Output
  
  // ANSELx 0 = Digital I/O, 1 = Analog I/O
  ANSELBbits.ANSB4 = 0;     //Digital I/O
  ANSELBbits.ANSB5 = 0;     //Digital I/O
  ANSELBbits.ANSB6 = 0;     //Digital I/O
  
  //CS is low active
  CS_DIGI_DOT_BOOSTER = 1;      //CS is low active  
}


//Einzelner Befehl senden �ber SPI
void SPI_write_DDB(char data){  
    CS_DIGI_DOT_BOOSTER = 0;
    __delay_ms(5);
    SSPBUF = data;
    while(BF == 0);
    __delay_ms(4);                          //delay f�r DDB
    CS_DIGI_DOT_BOOSTER = 1;   
}

//Array senden �ber SPI
void SPI_write_array_DDB(int array[], int arrayindex){        
    CS_DIGI_DOT_BOOSTER = 0;
    __delay_ms(5);
    for(int i = 0; i < arrayindex + 1; i++){
        int data = array[i];
        SSPBUF = data;
        while(BF == 0);
        }
    CS_DIGI_DOT_BOOSTER = 1;      
}

