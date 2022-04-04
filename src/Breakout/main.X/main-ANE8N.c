/*
 * File:   main.c
 * Author: sven_stocker
 *
 * Created on 10. Januar 2021, 15:49
 */


 // CONFIG1
#pragma config FOSC = HS        // Oscillator Selection (HS Oscillator, High-speed crystal/
								//resonator connected between OSC1 and OSC2 pins)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code
								//protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection
								//is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or
								// oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External 
								//Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor 
								//is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (Vcap 
								//functionality is disabled on RA6.)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or
								//Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset
								//Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out
								//is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming
								//enabled)

// Include Files
#include <xc.h>
#include "PBA_config.h"

//#define _XTAL_FREQ 8000000          //Frequenz 8 MHz wird im Header File gemacht


// *** Globale Variablen ***
int counter = 0;
int wertADC = 0;
int newVal = 0;
int oldVal = 0;
int pos_edge = 0;
int neg_edge = 0;


//*** Funktionsprototypen ***

//void initADC (void);
void initTimerGross(void);
void initTimerKlein(void);
void initINTR(void);
void interrupt ISR(void);
void AnalogDigitalWandler(void);
void BerechnungBereichA(void);
void BerechnungBereichB(void);
void EdgeDetection(void);
int map(int x, int in_min, int in_max, int out_min, int out_max);

// *** Main-Routine ***
void main(void) {

	// *** Port Initialisierung ***
	// ** TRISx **      0 = Output, 1 = Input

	TRISD = 0;               //PORTD 0 = Output
	TRISBbits.TRISB0 = 1;    //ADC pin
	TRISBbits.TRISB1 = 1;    //Kippschalter
	


	  //    TRISBbits.TRISB6 = 0;
	  //    TRISBbits.TRISB7 = 0;
	  //    TRISBbits.TRISB5 = 0;

		  // ** ANSELx **      0 = Digital I/O, 1 = Analog I/0

	ANSELD = 0;              // Set PORTD as Digital I/O
	ANSELBbits.ANSB0 = 1;    //ADC pin
	ANSELBbits.ANSB1 = 0;    //Kippschalter
  


	  // ** PORTx **

	LATD = 0;
  RB2 = 0;



	//    initADC();    //Initialisierung ADC
	initTimerGross();
  initTimerKlein();
	initINTR();

	while (1) {
    
	}

}


void initADC(void) {

	ADCON0bits.ADRMD = 1;           //10 bit result
	ADCON0bits.CHS = 0b01100;       //Analoger Kanal AN12, entspricht RB0
	ADCON0bits.ADON = 1;            //ADC is enabled
	ADCON1bits.ADFM = 1;            //Resultatformat im 2er Komplement
	ADCON1bits.ADCS = 0b101;        //FOSC/16
	ADCON1bits.ADNREF = 0;          //Vref- = 0 V
	ADCON1bits.ADPREF = 0b00;       //Vref+ = 3.3 V
	ADCON2bits.CHSN = 0b1111;       //ADC Negative reference

}


void initTimerKlein(void) {

	// Timer für 10 Hz bis 100 Hz

	T2CONbits.T2CKPS = 0b11;      //Prescaler 1:64
	T2CONbits.T2OUTPS = 0b0110;   //Postscaler 1:7
	T2CONbits.TMR2ON = 1;         //Timer2 ON bit



}

void initTimerGross(void){
  
  //Timer für 100 Hz bis 1000 Hz
  
  
  
}

void initINTR(void) {

	PIE1bits.TMR2IE = 1;        //TMR2 to PR2 Match Interrupt Enable bit
	INTCONbits.PEIE = 1;        //Peripheral Interrupt Enable bit	
	INTCONbits.GIE = 1;         //Enable Interrupt
}



void AnalogDigitalWandler(void) {

	ADCON0bits.ADGO = 1;
	while (ADCON0bits.ADGO == 1) {};
	wertADC = (ADRESH << 6) | (ADRESL >> 2);   //abspeichern ADC-Wert
  
}

void BerechnungBereichA(void) {
	PR2 = map(wertADC, 0, 255, 223, 22);   //map evtl. anpassen
  
}

void BerechnungBereichB(void) {
	PR2 = map(wertADC, 0, 255, 100, 1000);
}



int map(int x, int in_min, int in_max, int out_min, int out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void EdgeDetection(void) {
	newVal = RB1;
	if (oldVal == 0 && newVal == 1) {
		pos_edge = 1;
	}
	else
		pos_edge = 0;

	if (oldVal == 1 && newVal == 0) {
		neg_edge = 1;
	}

	else
		neg_edge = 1;

	oldVal = newVal;
}


void interrupt ISR(void) {

	if (TMR2IF) {
		
    
    AnalogDigitalWandler();
    BerechnungBereichA();
    initTimerKlein();
    
    

		LATD = ~LATD;
		TMR2IF = 0;       // Clear timer interrupt flag

	}
}

