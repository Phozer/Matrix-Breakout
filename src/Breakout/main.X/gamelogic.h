
//	/******************** Compiler-Includes, Clock Definiton **********************************/
#include <xc.h>

//Funktionsprototypen
void initalizeGame(void);
void initalizeBar(void);
void initializeBricks(void);
void barMove(char direction);
void barShow(void);
void initalizeBall(void);
void ballMove(void);;
void ballMoveStraight(void);
void ballMoveDiagonal(void);
void ballDeleteOldPos(void);
void ballShowNewPos(void);
void collisionDetectorWithBar(void);
void collisionDetectorWithBricks(void);