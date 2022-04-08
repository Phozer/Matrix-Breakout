
#include "PBA_config.h"

void putch(uint8_t c) 
{
	LCD_Putc(c);
}

void DelayMS(uint16_t timeMS)									/*Workaround f�r gr�ssere Delays bei 20MHz Systemtakt*/
{
	for(;timeMS>=35;timeMS-=35)
		__delay_ms(35);
	for(;timeMS>=1;timeMS--)
		__delay_ms(1);	
}

/**
 * @brief Delayroutine f�r Delays im Mikrosekunden-Bereich.
 * Blockierend!
 * @param timeUS Gew�nschtes Delay in us
 */
void DelayUS(uint16_t timeUS)									/*Workaround f�r gr�ssere Delays bei 20MHz Systemtakt*/
{
	for(;timeUS>=100;timeUS-=100)
		__delay_us(100);
	for(;timeUS>=10;timeUS-=10)
		__delay_us(10);	
}

/**
 * @brief Wandelt eine Bin�rzahl (8-Bit) um in BCD-Code.
 * @param value Zahl die gewandelt werden soll
 * @return BCD-Wert (4-Bit H|4-Bit Z|4-Bit E)LSB
 */
uint16_t BinToBcd(uint8_t value)								/*Umwandlung Bin�r zu BCD*/
{
	uint8_t E,Z;												/*dekl. Lokale Variabeln*/
	uint16_t H;													/*dekl. Lokale Variabeln*/
	H=value/100;		 										/*Hunderter definieren*/
	Z=(value-=H*100)/10;             							/*Zehner definieren*/
	E=value-Z*10;												/*Einer definieren*/
	H<<=8;
	Z<<=4;
	return H|Z|E;
}

/**
 * @brief Spiegelt ein Byte.
 * @param value Zu Spiegelndes Byte
 * @return Gespiegelter Wert
 */
uint8_t MirrorByte(uint8_t value)
{
	uint8_t i,rw;
	for(i=rw=0;i<=7;i++)
		if(BIT_TEST(value,i))
			BIT_SET(rw,7-i);
	return rw;
}

/**
 * @brief Z�hlt die Anzahl gesetzter Bits in einer Variable.
 * @param value Zu pr�fender Wert
 * @return Anzahl gesetzter Bits
 */
uint8_t BitSum(uint8_t value)
{
	uint8_t i,Result;
	for(i=Result=0;i<=7;i++)
		Result+=BIT_TEST(value,i);
	return Result;
}

/**
 * @brief Gibt die Bitnummer des letzten gesetzten Bits zur�ck.
 * @param value Zu pr�fendert Wert
 * @return Nummer des letzten gesetzten Bits
 */
uint8_t BitNumb(uint8_t value)
{
	uint8_t i,Result;
	Result=0xFF;
	for(i=0;i<=7;i++)
		Result = BIT_TEST(value,i) ? i : Result;
	return Result;
}
//#endif
/**
 * @}
 */

#define MAX_ADDR 			80

static uint8_t LCD_GetSpecChar(uint8_t ch);

/**
 * @brief Sendet ein Datenbyte (z.B. ein Zeichen) oder einen Befehl ans LCD.
 * 
 * @param value		Das zu �betragende Datenbyte / der zu �betragende Befehl (8Bit)
 */
void LCD_Send(uint8_t value)
{	
    LCD_EN=1;						/*Pos. Flanke am Enable-Pin*/
    LCD_D7 = BIT_TEST(value,7);		/*Oberes Daten-Nibble anlegen*/
    LCD_D6 = BIT_TEST(value,6);
    LCD_D5 = BIT_TEST(value,5);
    LCD_D4 = BIT_TEST(value,4);
    LCD_EN=0;						/*Oberes Nibble senden*/
    #if (_XTAL_FREQ>20000000)
    NOP();							/*1 Zyklus warten-> verhindert zu kurzen Puls bei 32MHz (min. 130ns stable)*/
    #endif
    LCD_EN=1;						/*Pos. Flanke am Enable-Pin*/
    LCD_D7 = BIT_TEST(value,3);		/*Unteres Daten-Nibble anlegen*/
    LCD_D6 = BIT_TEST(value,2);
    LCD_D5 = BIT_TEST(value,1);
    LCD_D4 = BIT_TEST(value,0);
    LCD_EN=0;						/*Unteres Nibble senden*/
    __delay_us(30);					/*Maximaler Zeitbedarf einer �bertragung (blocking)*/
 }

/**
 * @brief Display l�schen.
 * Das gesamte LCD-Daten-RAM wird mit 0x20 (Leerschlag) �berschrieben
 * Da der Vorgang bis zu 2ms dauern kann muss der Programmablauf nach dem Senden des
 * Befehls entsprechend lang blockiert werden (delay).
 */
void LCD_Clear(void)						
{
    LCD_SEND_COMMAND(0b00000001);					/*clear display*/
    DelayMS(2);									/*max. Zeitbedarf des LCD-Clear-Befehls*/
}

/**
 * @brief Rur�cksetzen Anzeigeberich und Cursor.
 * Display-RAM zur�ckschieben (Ausgangsposition). Die Funktion setzt den Anzeigebereich
 * des LCDs und den Cursor zur�ck auf die Anfangsposition (Adresse 0).
 */
void LCD_Home(void)							
 {	
    LCD_SEND_COMMAND(0b00000010);					/*return home*/
    DelayMS(2);
 }

/**
 * @brief Display-RAM im Anzeigebereich nach links schieben.
 * 
 * @param numPos		Anzahl Verschiebungsschritte des Display-RAMs
 */
void LCD_ShiftLeft(uint8_t numPos)			
{
    for(;numPos!=0;numPos--)
        LCD_SEND_COMMAND(0b00011000);				/*display shift left*/
}

/**
 * @brief Display-RAM im Anzeigebereich nach rechts schieben.
 * 
 * @param numPos		Anzahl Verschiebungsschritte des Display-RAMs
 */
void LCD_ShiftRight(uint8_t numPos)			
{
    for(;numPos!=0;numPos--)
        LCD_SEND_COMMAND(0b00011100);				/*display shift right */
}

/**
 * @brief Setzt die Cursor-Position f�r ein 2 Zeilen-Display.
 * 
 * @param posX		X-Koordinate (0-15) resp. Spalte auf die der Cursor gesetzt werden soll
 * @param posY		Y-Koordinate (0-1) resp. Zeile auf die der Cursor gesetzt werden soll
 */
void LCD_GotoXY(uint8_t posX, uint8_t posY)	
{
    uint8_t dispAddress;
    if(0 == posY) dispAddress = 0x00;						/*Basisadresse 1. Zeile*/
    if(1 == posY) dispAddress = 0x40;						/*Basisadresse 2. Zeile*/
    dispAddress += posX;									/*Basisadresse + Spaltenadresse*/
    LCD_SEND_COMMAND((0b10000000 | dispAddress));			/*Cursor-Adresse an LCD senden*/
}

/**
 * @brief Umschreiben der Umlaute f�r japanischen Zeichensatz.
 * Zeichen im erweiterten Zeichensatz (�,�,�) f�r den japanischen Zeichensatz ermitteln. Falls
 * Das �bergebene Zeichen kein Umlaut ist wird das Originalzeichen zur�ck gegeben ansonsten
 * der f�r den dog-m-Zeichensatz korrekte Zeichenwert.
 * 
 * @param ch		Zeichen das gepr�ft und falls Umlaut korrigiert werden soll
 * @return			Originalzeichen oder gem�ss dog-m-Zeichensatz korrigiertes Zeichen
 */
static uint8_t LCD_GetSpecChar(uint8_t ch)
{
    switch (ch)
    {
        case 0xE4: return 0x84;		/*'�'*/
        case 0xC4: return 0x8E;		/*'�'*/
        case 0xF6: return 0x94;		/*'�'*/
        case 0xD6: return 0x99;		/*'�'*/
        case 0xFC: return 0x81;		/*'�'*/
        case 0xDC: return 0x9A;		/*'�'*/
        default:  return ch;
    }
}

/**
 * @brief Ein Zeichen an das LCD senden (inkl. Sonderzeichenbearbeitung).
 * 
 * @param ch		Zeichen das ans LCD gesendet werden soll
 */
void LCD_Putc(uint8_t ch)
{
    switch (ch)
    {
        case '\r':								/*Wenn Zeilenumbruch oder Zeilenvorschub...*/
        case '\n': LCD_GotoXY(0,1); break;		/*-> Cursor auf 1. Zeichen, 2. Zeile*/
        case '\f': LCD_Clear(); break;			/*Wenn form feed -> LCD l�schen*/
        default:
            LCD_SEND_DATA(LCD_GetSpecChar(ch)); 	/*Zeichen an LCD senden*/
    }
}

/**
 * @brief String an LCD ausgeben mit Verwendung der lcd_putc-Funktion.
 * 
 * @param *p_string		Zeiger auf den String mit Null-Terminierung
 */
void LCD_Puts(uint8_t *p_string)
{
    while(*p_string!=0) 								/*Solange Nullterminierung noch nicht erreicht*/
        LCD_Putc(*p_string++);						/*Zeichen an LCD senden und Pointer auf n�chstes Zeichen*/
}

    /**
 * @brief ROM-String an LCD ausgeben mit Verwendung der lcd_putc-Funktion.
 * 
 * @param *p_romString		Zeiger auf den String mit Null-Terminierung
 */
void LCD_PutsRom(const uint8_t *p_romString)
{
    while(*p_romString!=0) 								/*Solange Nullterminierung noch nicht erreicht*/
        LCD_Putc(*p_romString++);						/*Zeichen an LCD senden und Pointer auf n�chstes Zeichen*/
}

/**
 * @brief Cursor oder blinkender Cursor ein-/ausschalten.
 * 
 * @param cursorMode	Der einzustellende Cursor-Modus (CURSOR_ON, CURSOR_OFF, CURSOR_OFF_BLINK oder CURSOR_ON_BLINK)
 */
void LCD_SetCursorMode(uint8_t cursorMode)
{			
    LCD_SEND_COMMAND(0b00001000|cursorMode);		/*Cursor-Wert an LCD senden*/
}

/**
 * @brief Initialisiert das dog-m-Text-LCD.
 * 
 * Initialiert LCD mit 4-Bit-Modus, Cursor increment, Cursor off, Blink off, 
 * Einstellungen des Boost-Converters, ...)
 *
 * Das LCD wird nach dem Initialisierungsvorgang zus�tzlich gel�scht.
 * Diese Funktion sollte in einem Projekt nur einmal ausgef�hrt werden.
 *
 * Die TRIS-Register der LCD-Pins m�ssen diese als Ausgang definieren!
 * @param displayVoltage	Betriebsspannung des Displays (V_3V3, V_5V)
 */
void LCD_Init(displayVoltage_t displayVoltage)
{ 
    DelayMS(40);						/*Betriebsspannung am LCD f�r mind. 40ms vorhanden*/
    LCD_RS=0;							/*LCD interpretiert n�chstes Zeichen als Befehl */
    LCD_RW=0;							/*Auf LCD schreiben...*/
    LCD_SEND_COMMAND(0b00000011);		/*Function Set 8-Bit*/
    DelayMS(2);						/*Pause gem�ss init-Ablauf (siehe Datenblatt)*/
    LCD_SEND_COMMAND(0b00110011);		/*Function Set 8-Bit (2x)*/
    LCD_SEND_COMMAND(0b00100010);		/*Function Set 4-Bit-Modus*/
    LCD_SEND_COMMAND(0b00101001);		/*Function Set*/
    if(V_3V3==displayVoltage)
    {
        LCD_SEND_COMMAND(0b00010100);		/*Internal OSC frequency/BIAS Set(BS:1/5->3.3V)*/
        LCD_SEND_COMMAND(0b01010101);		/*Power/ICON/Contrast control (Booster on->3.3V)*/
        LCD_SEND_COMMAND(0b01101101);		/*Follower control*/
        LCD_SEND_COMMAND(0b01111000);		/*Contrast Set*/
    }
    else if(V_5V==displayVoltage)
    {
        LCD_SEND_COMMAND(0b00011100);		/*Internal OSC frequency/BIAS Set(BS:1/4->5V)*/
        LCD_SEND_COMMAND(0b01010010);		/*Power/ICON/Contrast control (Booster on->3.3V)*/
        LCD_SEND_COMMAND(0b01101001);		/*Follower control*/
        LCD_SEND_COMMAND(0b01110100);		/*Contrast Set*/
    }
    LCD_SEND_COMMAND(0b00101000);		/*Function Set*/
    LCD_SEND_COMMAND(0b00001100);		/*Display ON/OFF control*/
    LCD_SEND_COMMAND(0b00000001);		/*Display l�schen / Cursor home*/
    DelayMS(2);						/*max. Zeitbedarf des LCD-Clear-Befehls*/
    LCD_SEND_COMMAND(0b00000110);		/*Cursor Auto-increment*/
    LCD_SEND_COMMAND(0b00101000);		/*Function Set*/
    LCD_SEND_COMMAND(0b00001100);		/*Display ON/OFF control*/
}