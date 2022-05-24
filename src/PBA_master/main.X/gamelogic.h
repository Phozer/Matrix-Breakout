
//	/******************** Compiler-Includes**********************************/
#include <xc.h>

//***Funktionsprototypen***
//Spielinitialisierung
void initializeGame(void);
//Brettinitialisierung
void initializeBar(void);
//Brickinitialisierung
void initializeBricks(void);
//Bewegen des Bretts
void barMove(char direction);
//Anzeigen des Bretts
void barShow(void);
//Ballinitialisierung
void initializeBall(void);
//Bewegen des Balls
void ballMove(void);
//Gerade Bewegung des Balles
void ballMoveStraight(void);
//Diagonale Bewegung des Balles
void ballMoveDiagonal(void);
//Alte Position löschen des Balles
void ballDeleteOldPos(void);
//Neue Position anzeigen des Balles
void ballShowNewPos(void);
//Detektion, ob der Ball das Brett trifft
void collisionDetectorWithBar(void);
//Detektion, ob der Ball einen oder mehrere Bricks trifft
void collisionDetectorWithBricks(void);
//Brick löschen
void deleteBrick(char arrayIndex);
//Überprüfen, ob das Spiel gewonnen wurde
void checkWinCondition(void);