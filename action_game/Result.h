#ifndef INCLUDE_RESULT_H
#define INCLUDE_RESULT_H

#include "DxLib.h"
#include "JoyPad.h"
#include "MiniSequence.h"
#include "SmashSequence.h"
#include "GameInfomation.h"

class SmashSequence;

class Result{
public:
	Result( GameInfomation* gameInfomation );
	virtual ~Result();
	enum Mode{
		MODE_TIME,
		MODE_LIVES,
		MODE_TIME_LIVES
	};
	enum Points{
		POINT_TITLE_Y = 20
	};
	void update( SmashSequence* smashSequence );
	void drawStringCenter( const char* str, int y );
	void drawString();
	void drawModeTime();
	void drawModeLives();
	void swap( int* t1, int* t2 );
protected:
	Mode mMode;
	JoyPad mJoyPad;
	int mPlayerNum;
	int mPlayer[ 4 ];
	int mCharacterID[ 4 ];
	int mGameOverTime[ 4 ];
	int mLivesNum[ 4 ];
	int mSmashNum[ 4 ];
	int mFallNum[ 4 ];
	int mScore[ 4 ];
	int mComputerFlag[ 4 ];
};

class MiniSequence;

class ResultMini : public Result{
public:
	ResultMini( GameInfomation* gameInfomation );
	~ResultMini();
	void update( MiniSequence* miniSequence );
};

#endif