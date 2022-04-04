/*
 * File:   DIGI_DOT_BOOSTER.c
 * Author: sven_stocker
 *
 * Created on 4. April 2022, 10:13
 */


#include <xc.h>
#include "SPI.h"


#define BOOSTER_INIT 0xB1


void booster_init(int ledCount){
    if(ledCount > 256){
        ledCount = 255;
    }
}


