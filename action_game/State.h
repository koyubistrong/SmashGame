#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "DxLib.h"
#include "CSV.h"
#include "Array2D.h"
#include "DynamicObject.h"
#include "StaticObject.h"
#include "DynamicObjectMini.h"
#include "StaticObjectMini.h"
#include "MiniSequence.h"
#include "SmashSequence.h"
#include "JoyPad.h"
#include "GameInfomation.h"
#include "extern.h"

class DynamicObject;
class StaticObject;
class SmashSequence;

class State{
	const int FPS;
public:
	State( const char* filename, GameInfomation* gameInfomation );
	State() : FPS(50){}
	virtual ~State();
	enum Points{
		POINT_TIME_X = 640,
		POINT_TIME_Y = 0,
		POINT_BLOWRATE_X = 40,
		POINT_BLOWRATE_Y = 440
	};
	enum Size{
		SIZE_BLOWRATE_X = 140
	};
	enum Mode{
		MODE_TIME,
		MODE_LIVES,
		MODE_TIME_LIVES
	};
	enum Sequence{
		SEQ_START,
		SEQ_GAME,
		SEQ_POSE,
		SEQ_RETURN,
		SEQ_END,
		SEQ_RESULT
	};
	int update( SmashSequence* smashSequence );
	void start();
	void game();
	void pose();
	void end();
	void result();
	void loading();
	void drawStage_Character();
	void drawComponents();
	virtual void drawBlowRate();
	virtual void drawLives();
	void drawTimer();
	void moveCamera();
	void setMagnification( double mag );
	void setStageHalfSize();
	bool checkTimer();
	void checkGameSet();
	bool checkMouse( int x1, int x2, int y1, int y2 );
	int mTimer;

protected:
	StaticObject* mStaticObject;
	DynamicObject* mDynamicObject;
	Mode mMode;
	Sequence mSequence;
	bool mComputerFlag[ 4 ];
	int mNewStageHandle;
	int mBlackHandle;
	int mStartHandle[ 4 ];
	int mStageBGMHandle;
	int mCameraX;
	int mCameraY;
	bool mState;
	int mNumber;
	int mStartTime;
	int mEndTime;
	int mPoseTime;
	int mFontSize32Handle;
	int mFontSize16Handle;
	int mSizeX;
	int mSizeY;
	double mMinDegree;
	JoyPad mJoyPad;

};

class StaticObjectMini;
class DynamicObjectMini;
class MiniSequence;

class StateMini : public State{
public:
	StateMini( const char* filename, GameInfomation* gameInfomation );
	virtual ~StateMini();
	int update( MiniSequence* miniSequence );
	void start();
	void game();
	void pose();
	void end();
	void result();
	void drawStage_Character();
	void drawBlowRate();
	void drawLives();
	void drawScore();
	void moveCamera();
	void setMagnification( double mag );
	void setStageHalfSize();
	bool checkTimer();
	void checkGameSet();
	void checkCreateFlag();

private:
	StaticObjectMini* mStaticObjectMini;
	DynamicObjectMini* mDynamicObjectMini;
	int mCameraSpeedX;
	int mCreateFlag;
	int mSmashNumSum;

};

#endif