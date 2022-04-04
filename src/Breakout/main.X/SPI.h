
//	/******************** Compiler-Includes, Clock Definiton **********************************/
#include <xc.h>
#define _XTAL_FREQ 8000000

//Initaliserung SPI
void SPI_init();
//SPI senden
void SPI_write(char data);
//SPI senden eines Arrays
void SPI_write_array(int array[], int arrayindex);




	
