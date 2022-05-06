
//	/******************** Compiler-Includes, Clock Definiton **********************************/
#include <xc.h>

void booster_init(void);
void booster_show(void);
void booster_setRGB(char red, char green, char blue);
void booster_setHSV(char hue_l, char hue_h, char saturation, char volume);
void booster_setLED(char led_number);
void booster_setAll(void);
void booster_setRange(char start, char end);
void booster_rgbOrder(char index_red, char index_green, char index_blue);
