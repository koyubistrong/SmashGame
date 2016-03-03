#ifndef INCLUDED_CHARACTER_H
#define INCLUDED_CHARACTER_H

#include <time.h>
#include "DxLib.h"

class Character{
public:
	Character() : mCharacterX( 0 ), mCharacterY( 0 ), mSpeedX( 0 ), mSpeedY( 0 ),mTimeCOM( 0 ), mCharacterHandle( 0 ),
		mCharacterID( OBJ_UNKNOWN ), mJumpNum( 0 ), mBlowRate( 0 ){}
	virtual ~Character(){}
	enum Object{
		OBJ_PLAYER = 0,
		OBJ_COMPUTER = 10,
		OBJ_ENEMY2 = 11,
		OBJ_STAR = 35,
		OBJ_UNKNOWN = 99,
	};
	void setCharacterHandle( int i );
	void setCharacterID( int o );
	int getBlowRate();
	void setBlowRate( int damage );
	void getCharacterPoint( int* x, int* y );
	int outCharacterHandle();
	int outCharacterID();
	int mCharacterX;
	int mCharacterY;
	int mFactCharacterX;
	int mFactCharacterY;
	int mFarstCharacterX;
	int mFarstCharacterY;
	int mSpeedX;
	int mSpeedY;
	int mTimeCOM;
	int mJumpNum;

protected:
	int mBlowRate;
	int mCharacterHandle;
	Object mCharacterID;

};

#endif