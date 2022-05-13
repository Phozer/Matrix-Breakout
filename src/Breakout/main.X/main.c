/*
 * File:   main.c
 * Author: sven_stocker
 *
 * Created on 25. M?rz 2022, 15:49
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
#include "SPI.h"
#include "DIGI_DOT_BOOSTER.h"
#include "gamelogic.h"


#define LEFT    0x01
#define RIGHT  0x00

// *** Globale Variablen ***

//*** Funktionsprototypen ***
//void posEdgeDetection(void);
void initINTR(void);
void interrupt ISR(void);


// *** Main-Routine ***
void main(void) {
  
	// *** Port Initialisierung ***
	// ** TRISx **      0 = Output, 1 = Input
    TRISBbits.TRISB0 = 0;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;  

	// ** ANSELx **      0 = Digital I/O, 1 = Analog I/0
    ANSELBbits.ANSB0 = 0;
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;    
    
    initINTR();
    SPI_init();
    SPI_write_DDB(0);
    __delay_ms(1000);
    RB0 = 0;                //Reset
    __delay_ms(100);
    RB0 = 1;                //Reset off
    __delay_ms(100);
    booster_init();
    __delay_ms(30);
    booster_rgbOrder(2, 3 ,1);        //Standardwert für WS2812
    initalizeGame();

	while (1) {
		ballMove();
	}
}


void initINTR(void) {
	IOCAPbits.IOCAP0 = 1;       //IOC Port B pin 5 positive Flanke getriggert
    IOCAPbits.IOCAP1 = 1;
	INTCONbits.IOCIE = 1;       //IOC enable bit
	INTCONbits.GIE = 1;         //Enable Interrupt
}

void interrupt ISR(void) {
	if (IOCAF0) {				//Interrupt Flag vom IOC pin
		__delay_ms(1);         //Entprellen des Schalters
		if (RA0 == 1) {
			barMove(RIGHT);
		}
		IOCAF0 = 0;
	}
    if(IOCAF1){
        __delay_ms(1);
        if(RA1 == 1){
            barMove(LEFT);
        }
        IOCAF1 = 0;
    }
}
