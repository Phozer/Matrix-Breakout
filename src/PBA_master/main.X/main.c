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
#include "inc/PBA_config.h"
#include "SPI.h"
#include "DIGI_DOT_BOOSTER.h"
#include "gamelogic.h"

/********************* Defines / Makros / Konstanten ****************************************/
#define UART_BUFFERSIZE         (16)            /**< Anzahl Zeichen die maximal eingegeben werden k?nnen*/
#define LEFT    0x01
#define RIGHT   0x00
#define RESET_DIGI_DOT_BOOSTER      RB0
/********************* Funktionsprototypen **************************************/
inline void ISR_UartRx(void);

/********************* Globale Daten ********************************************/
volatile uint8_t uartHasReceived_g = 0;             /**< Flag UART-String empfangen*/
volatile uint8_t uartBuffer_g[UART_BUFFERSIZE];     /**< UART-String*/



// *** Main-Routine ***
void main(void) {
    // *** Port Initialisierung ***
	// ** TRISx **      0 = Output, 1 = Input
    TRISBbits.TRISB0 = 0;   //RB0 as Output
    
    // ** ANSELx **      0 = Digital I/O, 1 = Analog I/0
    ANSELBbits.ANSB0 = 0;   //Digital I/O
    
    // Initialisieren der Hardware
    PBA_Init();     
    UART_Init();
  
    //Initalisierung SPI
    SPI_init();
    SPI_write_DDB(0);
    __delay_ms(1000);
    
    //Reset des DIGI DOT BOOSTER
    RESET_DIGI_DOT_BOOSTER = 0;                //Reset on
    __delay_ms(100);
    RESET_DIGI_DOT_BOOSTER = 1;                //Reset off
    __delay_ms(100);
    
    //Initialisierung des DIGI DOT BOOSTER
    booster_init();
    __delay_ms(30);
    booster_rgbOrder(2, 3 ,1);        //Standardwert f???r WS2812
    
    //Initialisierung des Spieles
    initializeGame();
    
    //Initialiserung des Interrupts f???r UART
    INTCONbits.PEIE = 1;                                //Peripherie-Interrupts akivieren
    PIE1bits.RCIE = 1;                                  //Receive Interrupt einschalten
    INTCONbits.GIE = 1;                                 //Globale Interrupts akivieren
   
    
    if(0 != INT_AddUartCallbackFnc(ISR_UartRx)){}        //UART-Interruptroutine festelegen
    
    //Endlos Loop
	while (1) {
		ballMove();
	}
}

/**
 * @brief UART Empfangsinterrupt
 * Liest Zeichen ein, bis Carriage return empfangen
 */
inline void  ISR_UartRx(void)
{
    uint8_t c;                              //Aktuell eingelesenes Zeichen
    while (PIR1bits.RCIF)                   //Zeichen im Buffer
    {
        c = UART_Getc();                    //Zeichen einlesen        
        
        if(c == 0){
            barMove(RIGHT);
        }
        else if(c == 1){
            barMove(LEFT);        
        }
        PIR1bits.RCIF = 0;
    }
}



