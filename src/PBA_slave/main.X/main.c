/*
 * File:   main.c
 * Author: steigem2
 *
 * Created on 10. Mai 2022, 09:19
 */

/*******************************************************************************
 * Beschreibung:
 * I2C 6D-BS
 * SCL: RB7
 * SDA: RB6
 * UART
 * TX:  C7
 * RX:  C7
 * 
 * Datum        Vers.   Name            Kommentar / ?nderung
 * 10.05.2022   1.0     Marco Steiger   erstellung
 *
 *
 *
 ******************************************************************************/

//*** Config ***
//#pragma config FOSC = HS        // Oscillator Selection (HS Oscillator, 
//// High-speed crystal/resonator connected 
//// between OSC1 and OSC2 pins)
//#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
//#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
//#pragma config MCLRE = ON       // MCLR Pin Function Select 
//// (MCLR/VPP pin function is MCLR)
//#pragma config CP = OFF         // Flash Program Memory Code Protection 
//// (Program memory code protection is disabled)
//#pragma config CPD = OFF        // Data Memory Code Protection 
//// (Data memory code protection is disabled)
//#pragma config BOREN = OFF      // Brown-out Reset Enable 
//// (Brown-out Reset disabled)
//#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function disabled.
//// I/O or oscillator function on the CLKOUT pin)
//#pragma config IESO = ON        // Internal/External Switchover 
//// (Internal/External Switchover mode enabled)
//#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable 
//// (Fail-Safe Clock Monitor is enabled)
//#pragma config WRT = OFF        // Flash Memory Self-Write Protection 
//// (Write protection off)
//#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit 
//// (Vcap functionality is disabled on RA6.)
//#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
//#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack
//// Overflow or Underflow will cause a Reset)
//#pragma config BORV = LO        // Brown-out Reset Voltage Selection 
//// (Brown-out Reset Voltage (Vbor),
//// low trip point selected.)
//#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit 
//// (Low power brown-out is disabled)
//#pragma config LVP = ON         // Low-Voltage Programming Enable 
//// (Low-voltage programming enabled)

// *** Include Files ***
#include <xc.h>                 // Einbindung Compiler XC8 / PIC16F1787
#include "inc/PBA_config.h"     // Einbindung PBA Library

// *** Defines ***
#define _XTAL_FREQ 8000000      // Oszillatorfrequenz PBA6: 8MHz (ohne PLL)
#define LSM330_Addr_Gyro (0xD6) // Adresse des LSM330 Gyroscope
#define LSM330_Addr_Accl (0x32) // Adresse des LSM330 Accelometer
#define I2C_READ        (0x01)  // Adress-LSB muss 1 sein f�rs Lesen

// *** GLOABLE Variabeln ***

// *** FUNKTIONSPROTOTYPEN ***
void LSM330_init();
int16_t LSM330_read(char Addr, char RegAddress);
void UART_WriteAllDebug(int16_t xGyro, int16_t yGyro, int16_t zGyro, int16_t xAccl, int16_t yAccl,int16_t zAccl);

// *** Main-Routine ***
void main(void) {
    // *** Variabeln ***
  int16_t xGyro;
  int16_t yGyro;
  int16_t zGyro;
  
  int16_t xAccl;
  int16_t yAccl;
  int16_t zAccl;  
  
  unsigned char tilt_count = 0;
    // *** INITIALISIERUNG *** 
    // ** TRISx ** 
    TRISBbits.TRISB6 = 1;           //SDA as input
    TRISBbits.TRISB7 = 1;           //SCL as input
    TRISD = 0;                      // PORTD 0 = Output

    // ** ANSELx ***
    ANSELBbits.ANSB6 = 0;           //SDA
    ANSELD = 0;                     // Set PORTD as DIGITAL I/O

    // ** LATx ** 
    LATD = 0b00000000;
    
    LSM330_init();
    UART_Init();
    __delay_ms(500);
    
    while(1){      
      xGyro = LSM330_read(LSM330_Addr_Gyro, 40);
      yGyro = LSM330_read(LSM330_Addr_Gyro, 42);
      zGyro = LSM330_read(LSM330_Addr_Gyro, 44);

      xAccl = LSM330_read(LSM330_Addr_Accl, 40);
      yAccl = LSM330_read(LSM330_Addr_Accl, 42);
      zAccl = LSM330_read(LSM330_Addr_Accl, 44);
      
      //UART_WriteAllDebug(xGyro, yGyro, zGyro, xAccl, yAccl, zAccl);
      
      if(xAccl > 4000){
        if(++tilt_count >= 15){
          LATD = PORTD << 1;
          UART_Putc(0b01);
          tilt_count = 0;
        }
      }
      else if (xAccl < -4000){
        if(++tilt_count >= 15){
          LATD = PORTD >> 1;
          UART_Putc(0b00);
          tilt_count = 0;
        }
      }
      else{
        tilt_count = 0;
      };
      __delay_ms(10);
    }
}

/**
 * @brief LSM330 init
 */
void LSM330_init(void)
{
    I2C_Init();                             // I2C initialisieren
    I2C_Start();                            // Bus übernehmen
    I2C_WriteByte(LSM330_Addr_Gyro);        // Start I2C Transmission
    I2C_WriteByte(0x20);                    // Select control register 1
    I2C_WriteByte(0x2B);                    // X, Y and Z axis enabled, power on mode, output data rate 95 Hz
    I2C_Stop();                             // Bus freigeben
    
    I2C_Start();                            // Bus übernehmen
    I2C_WriteByte(LSM330_Addr_Accl);        // Start I2C Transmission
    I2C_WriteByte(0x20);                    // Select control register 1
    I2C_WriteByte(0x67);                    // X, Y and Z axis enabled, power on mode, output data rate 100 Hz
    I2C_Stop();                             // Bus freigeben
    __delay_ms(300);
}

/**
 * @brief LSM330 read
 */
int16_t LSM330_read(char Addr, char RegAddress)
{
    uint8_t dataL;
    int16_t dataH;
    int16_t data;
    I2C_Start();                            // Bus übernehmen
    I2C_WriteByte(Addr);                    // Start I2C Transmission
    I2C_WriteByte(RegAddress);              // Select register
    I2C_Restart();                          //Bus erneut �bernemen (Restart)
    I2C_WriteByte(Addr+I2C_READ);           // Adresse (+ read-Anweisung)
    dataL=I2C_ReadByte(0);                  // Low-Byte Register auslesen
    I2C_Stop();                             // Bus freigeben
    
    I2C_Start();
    I2C_WriteByte(Addr);                    // Start I2C Transmission
    I2C_WriteByte(RegAddress+1);            // Select register
    I2C_Restart();                          // Bus erneut übernemen (Restart)
    I2C_WriteByte(Addr+I2C_READ);           // Adresse (+ read-Anweisung)*/
    dataH=I2C_ReadByte(0);                  // High-Byte Register auslesen
    I2C_Stop();                             // Bus freigeben
    data = dataH *256 + dataL;              // HB und LB zusammenführen
    return(data);
}

/**
 * @brief Prints out all Values over UART to view in terminal
 */
void UART_WriteAllDebug(int16_t xGyro, int16_t yGyro, int16_t zGyro, int16_t xAccl, int16_t yAccl,int16_t zAccl)
{
  char str[12];
  UART_PutsRom("xGyro: ");
  sprintf(str, "%d", xGyro);
  UART_WriteLine(str);
  UART_PutsRom("yGyro: ");
  sprintf(str, "%d", yGyro);
  UART_WriteLine(str);
  UART_PutsRom("zGyro: ");
  sprintf(str, "%d", zGyro);
  UART_WriteLine(str);
  
  UART_PutsRom("xAccl: ");
  sprintf(str, "%d", xAccl);
  UART_WriteLine(str);
  UART_PutsRom("yAccl: ");
  sprintf(str, "%d", yAccl);
  UART_WriteLine(str);
  UART_PutsRom("zGyro: ");
  sprintf(str, "%d", zAccl);
  UART_WriteLine(str);
  UART_WriteLineRom("\n");
}
