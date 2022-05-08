
//	/******************** Compiler-Includes, Clock Definiton **********************************/
#include <xc.h>
#define _XTAL_FREQ 8000000

//Initaliserung SPI
void SPI_init();

//SPI Digi Dot Booster senden
void SPI_write_DDB(char data);
//SPI Digi Dot Booster senden eines Arrays
void SPI_write_array_DDB(int array[], int arrayindex);

//SPI Gyroscope
char SPI_read_GS(char instruction);

void SPI_start(char cs);

void SPI_end(char cs);




	
