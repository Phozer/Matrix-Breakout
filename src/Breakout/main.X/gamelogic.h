
//	/******************** Compiler-Includes, Clock Definiton **********************************/
#include <xc.h>

char posX1Bar, posX2Bar, posBall;
void initalizeBar(char posX1Bar, char posX2Bar);
void barMove(char posX1Bar, char posX2Bar);
void barChangePosition(char direction);
void initalizeBall(char posBall);
void ballMoveStraight(void);