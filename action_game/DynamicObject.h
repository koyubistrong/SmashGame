#ifndef INCLUDED_DYNAMIC_OBJECT_H
#define INCLUDED_DYNAMIC_OBJECT_H

#include "DxLib.h"
#include "File.h"
#include "Array2D.h"
#include "Image.h"
#include "StaticObject.h"
#include "Movement.h"
#include "Player.h"
#include "Computer.h"
#include "Weapon.h"
#include "GameInfomation.h"
#include "extern.h"

class DynamicObject{
public:
	DynamicObject( const char* fileName, GameInfomation* gameInfomation, StaticObject* staticObject );
	DynamicObject(){}
	virtual ~DynamicObject();
	void update();
	void drawCharacter( int cameraX, int cameraY );
	void checkMoveCharacter( Movement* character, int* x, int* y );
	void moveCharacter( Movement* character, int check );
	void setCharacterPoint( Movement* character, int x, int y );
	void setSpeedY( Movement* character, int speedY );
	void setJumpNum( Movement* character, bool jump );
	void movePlayer( Movement* character );
	void moveWeapon( Movement* character, Weapon* weapon );
	void moveComputer( Movement* character );
	void moveEnemy2( Movement* character );
	void checkHitEnemy();
	bool checkHitStar();
	bool checkHitAttack1( int i );
	bool checkHitAttack2( int attacker, int startAttackerX, int startAttackerY, int endAttackerX, int endAttackerY );
	bool checkHitWeapon1( int i, Weapon* weapon  );
	bool checkHitWeapon2( int attacker, Weapon* weapon,int startAttackerX, int startAttackerY, int endAttackerX, int endAttackerY );
	int checkMoveCharacter( Movement* character, int x, int y );
	virtual bool checkMoveWeapon( Weapon* weapon, int startAttackerX, int startAttackerY, int endAttackerX, int endAttackerY );
	void checkFallCharacter( Movement* character, int* x, int* y );
	void checkStageRangeCharacter( int i, Movement* character );
	void getCharacterPoint( Movement* character, int* x, int* y );
	void getPlayerPoint( int* minX, int* minY, int* maxX, int* maxY );
	void getCharacterBlowRate( int* blowRate );
	void getCharacterLivesNum( int* livesNum );
	void getCharacterSmashNum( int* smashNum );
	int getLivesTime();
	int getCharacterNumber();
	void getResult();
	bool checkLives();
	bool checkHit( int i1, int i2 );

protected:
	Image* mCharacterObject;
	StaticObject* mStaticObject;
	Array2D< int > mCharacterHandle;
	int mCharacterNumber;
	Movement* mCharacter[ 256 ];
	int mPlayerID;
	int mStarID;
	int mSizeX;
	int mSizeY;
	int size;
	float mBlowRate;
	int mLivesTime;
	bool mModeLives;
	GameInfomation* mGameInfomation;

};

#endif