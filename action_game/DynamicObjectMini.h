#ifndef INCLUDED_DYNAMIC_OBJECT_MINI_H
#define INCLUDED_DYNAMIC_OBJECT_MINI_H

#include "DynamicObject.h"
#include "StaticObjectMini.h"
#include "Movement.h"

class DynamicObjectMini : public DynamicObject{
public:
	DynamicObjectMini( const char* fileName, GameInfomation* gameInfomation, StaticObjectMini* staticObjectMini );
	~DynamicObjectMini();
	enum PlayerMode{
		MODE_PLAYER1,
		MODE_PLAYER2
	};
	void update();
	void drawCharacter( int cameraX, int cameraY );
	int checkMoveCharacter( Movement* character, int x, int y );
	void checkMoveCharacter( Movement* character, int* x, int* y );
	bool checkMoveWeapon( Weapon* weapon, int startAttackerX, int startAttackerY, int endAttackerX, int endAttackerY );
	void checkFallCharacter( Movement* character );
	void checkStageRangeCharacter( int i, Movement* character );
	bool checkLives();
	void resetCharacterPoint();
	void appearCharacter( Movement* character, int x, int y );
	void checkComputerNum();
	void setCameraX( int cameraX );
	void getCharacterSmashNum( int* smashNum );
	void getDefficult();
	PlayerMode getPlayerMode();
private:
	StaticObjectMini* mStaticObjectMini;
	int mCameraX;
	int mDefficult;
	PlayerMode mPlayerMode;

};

#endif