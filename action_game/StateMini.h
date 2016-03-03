#ifndef INCLUDE_STATE_MINI_H
#define INCLUDE_STATE_MINI_H

#include "DxLib.h"
#include "CSV.h"
#include "Array2D.h"
#include "DynamicObjectMini.h"
#include "StaticObjectMini.h"
#include "MiniSequence.h"
#include "State.h"
#include "JoyPad.h"
#include "extern.h"

class StaticObjectMini;
class DynamicObjectMini;
class MiniSequence;

class StateMini{
public:
	StateMini( const char* filename, int* characterID );
	~StateMini();
	int update( MiniSequence* miniSequence );
	void start();
	void game();
	void pose();
	void end();
	void result();
	void drawStage_Character();
	void moveCamera();
	void setMagnification( double mag );
	void setStageHalfSize();
	bool checkTimer();
	void checkGameSet();

private:
	StaticObjectMini* mStaticObjectMini;
	DynamicObjectMini* mDynamicObjectMini;
	int mSizeX;
	int mSizeY;
	int mCameraSpeedX;
	int mCharacterIndex;

};

#endif