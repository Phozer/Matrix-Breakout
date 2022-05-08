/*
 * File:   DIGI_DOT_BOOSTER.c
 * Author: sven_stocker
 *
 * Created on 4. April 2022, 10:13
 */



#include "SPI.h"
#include "DIGI_DOT_BOOSTER.h"


#define BOOSTER_INIT        0xB1
#define BOOSTER_SHOW        0xB2
#define BOOSTER_SETRGB      0xA1
#define BOOSTER_SETHSV      0xA3
#define BOOSTER_SETLED      0xA4
#define BOOSTER_SETALL      0xA5
#define BOOSTER_SETRANGE    0xA6
#define BOOSTER_RGBORDER    0xC1
#define BOOSTER_SHIFTUP     0xB3
#define BOOSTER_SHIFTDOWN   0xB4

#define bitsPerLED   24             //24 bit fuer WS2812(RGB)
#define ledCount     255            //Anzahl LED
           

void booster_init(void){    
    int buffer[3];
    buffer[0] = BOOSTER_INIT;
    buffer[1] = ledCount;           
    buffer[2] = bitsPerLED;
    SPI_write_array_DDB(buffer, 2);

    //RGB-Order von GRB auf RGB setzen
//    booster_rgbOrder(1, 2, 3);    
}

void booster_show(void){
    SPI_write_DDB(BOOSTER_SHOW);
    __delay_ms(8);                  //pro LED 30us, d.h. 256*30us = 7.68 ms
}

void booster_setRGB(char red, char green, char blue){
    int buffer[4];
    buffer[0] = BOOSTER_SETRGB;
    buffer[1] = red;
    buffer[2] = green;
    buffer[3] = blue;
    SPI_write_array_DDB(buffer, 3);
}

void booster_setHSV(char hue_l, char hue_h, char saturation, char volume){
    int buffer[5];
    buffer[0] = BOOSTER_SETHSV;
    buffer[1] = hue_l;
    buffer[2] = hue_h;
    buffer[3] = saturation;
    buffer[4] = volume;
    SPI_write_array_DDB(buffer, 4);
}

void booster_setLED(char led_number){       //led_number von 0 bis 255
    int buffer[2];
    buffer[0] = BOOSTER_SETLED;
    buffer[1] = led_number;
    SPI_write_array_DDB(buffer, 1);
}

void booster_setAll(void){
    SPI_write_DDB(BOOSTER_SETALL);
}

void booster_setRange(char start, char end){
    int buffer[3];
    buffer[0] = BOOSTER_SETRANGE;
    buffer[1] = start;
    buffer[2] = end;
    SPI_write_array_DDB(buffer, 2);
}

void booster_rgbOrder(char index_red, char index_green, char index_blue){
    int buffer[4];
    buffer[0] = BOOSTER_RGBORDER;
    buffer[1] = index_red;
    buffer[2] = index_green;
    buffer[3] = index_blue;
    SPI_write_array_DDB(buffer, 3);
}

void booster_shiftup(char start, char end, char count){
  int buffer[4];
  buffer[0] = BOOSTER_SHIFTUP;
  buffer[1] = start;
  buffer[2] = end;
  buffer[3] = count;
  SPI_write_array_DDB(buffer, 3);
}

void booster_shiftdown(char start, char end, char count){
  int buffer[4];
  buffer[0] = BOOSTER_SHIFTDOWN;
  buffer[1] = start;
  buffer[2] = end;
  buffer[3] = count;
  SPI_write_array_DDB(buffer, 3);
}








