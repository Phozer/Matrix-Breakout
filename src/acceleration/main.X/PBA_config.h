
//	/******************** Compiler-Includes, Clock Definiton **********************************/
#include <xc.h>
#define _XTAL_FREQ 8000000

/******************** Einbinden der Bibliotheken*******************************/
	#include <stdio.h>				/**<IO-functions (printf, scanf,...)*/
	#include <string.h>				/**<String-functions*/
	#include <stdint.h>				/**<standard integer types*/

/******************** Helper Makros *******************************************/
/**
 * @brief Löscht ein bestimmtes Bit in einem Byte.
 * @param reg Datenbyte
 * @param bitNumber Bitnummer
 */
    #define BIT_CLEAR( reg, bitNumber )	((reg) &= ~(1 << (bitNumber)))

/**
 * @brief Setzt ein bestimmtes Bit in einem Byte.
 * @param reg Datenbyte
 * @param bitNumber Bitnummer
 */
    #define BIT_SET( reg, bitNumber )		((reg) |= (1 << (bitNumber)))

/**
 * @brief Wechselt den Zustand ein bestimmtes Bits in einem Byte.
 * @param reg Datenbyte
 * @param bitNumber Bitnummer
 */
    #define BIT_TOGGLE( reg, bitNumber )	((reg) ^= (1 << (bitNumber)))

/**
 * @brief Testet ein bestimmtes Bit in einem Byte und retourniert Zustand.
 * @param reg Datenbyte
 * @param bitNumber Bitnummer
 */
    #define BIT_TEST( reg, bitNumber )	(((reg) & (1 << (bitNumber)))>0)

/******************** Prototypen Helper-Funktionen ****************************/

void DelayMS(uint16_t timeMS);
void DelayUS(uint16_t timeUS);
uint16_t BinToBcd(uint8_t value);
void Beep(uint16_t freqHZ, uint16_t durationMS);
uint8_t MirrorByte(uint8_t value);
uint8_t BitSum(uint8_t value);
uint8_t BitNumb(uint8_t value);

/*Display-Pins*/
#define 	LCD_D4 			LATE2
#define 	LCD_D5 			LATA2
#define 	LCD_D6 			LATA4
#define 	LCD_D7 			LATA5
#define 	LCD_RW 			LATC0
#define 	LCD_EN 			LATC1
#define 	LCD_RS 			LATC2

/*Defines für Curson-Funktion (dispcurs(mode);)*/
/**
 * @name Cursor Modi
 * Defintion der verschiedenen Cursor-Modi
 * @see lcd_curs
 * @{
 */
#define CURSOR_ON 			0x06
#define CURSOR_OFF 			0x04
#define CURSOR_OFF_BLINK 	0x05
#define CURSOR_ON_BLINK 	0x07
/**
 * @}
 */

/**
 * @brief	Betriebsspannung LCD
 */
typedef enum  
{
	V_3V3,	/**< 3.3V--> PBA6*/
	V_5V	/**< 5V --> ältere Boards*/
}displayVoltage_t;

/*Makros für das Senden eines Befehl*/
#define LCD_SEND_COMMAND(Command)	LCD_Send(Command)
/*Makros für das Senden eines Datenbytes (Zeichen)*/
#define LCD_SEND_DATA(value)		LCD_RS=1;LCD_Send(value);LCD_RS=0

void LCD_Send(uint8_t value);

void LCD_Clear(void);

void LCD_Home(void);

void LCD_ShiftLeft(uint8_t numPos);

void LCD_ShiftRight(uint8_t numPos);

void LCD_GotoXY(uint8_t posX, uint8_t posY);

void LCD_Putc(uint8_t ch);

void LCD_Puts(uint8_t *p_string);

void LCD_PutsRom(const uint8_t *p_romString);

void LCD_SetCursorMode(uint8_t cursorMode);


void LCD_Init(displayVoltage_t displayVoltage);





/**
 * @}
 */



	void putch(uint8_t c);
	
