
//	/******************** Compiler-Includes, Clock Definiton **********************************/
#include <xc.h>

//Funktionsprototypen
void initializeGame(void);
void initializeBar(void);
void initializeBricks(void);
void barMove(char direction);
void barShow(void);
void initializeBall(void);
void ballMove(void);;
void ballMoveStraight(void);
void ballMoveDiagonal(void);
void ballDeleteOldPos(void);
void ballShowNewPos(void);
void collisionDetectorWithBar(void);
void collisionDetectorWithBricks(void);
//void deleteBrick(void);
void deleteBrick(char arrayIndex);
void checkWinCondition(void);