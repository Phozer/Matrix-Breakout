
//	/******************** Compiler-Includes **********************************/
#include <xc.h>

//Boosterinitialisierung
void booster_init(void);
//Anzeige aktualisieren
void booster_show(void);
//RGB-Werte einstellen
void booster_setRGB(char red, char green, char blue);
//Helligkeit einstellen
void booster_setHSV(char hue_l, char hue_h, char saturation, char volume);
//LED-Position setzen
void booster_setLED(char led_number);
//Ganze LED-Matrix setzen
void booster_setAll(void);
//Eine bestimmte Range setzen
void booster_setRange(char start, char end);
//RGB-Reihenfolge einstellen
void booster_rgbOrder(char index_red, char index_green, char index_blue);