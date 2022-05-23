
//	/******************** Compiler-Includes, Clock Definiton **********************************/
#include <xc.h>
#define _XTAL_FREQ 8000000

//***Funktionsprototypen***
//Initaliserung SPI
void SPI_init();
//SPI Digi Dot Booster senden eines einzelnen Befehls
void SPI_write_DDB(char data);
//SPI Digi Dot Booster senden eines Arrays
void SPI_write_array_DDB(int array[], int arrayindex);





	
