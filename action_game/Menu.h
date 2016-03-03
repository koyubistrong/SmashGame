#ifndef INCLUDE_MENU_H
#define INCLUDE_MENU_H

#include "DxLib.h"
#include "MainSequence.h"
#include "Image.h"
#include "JoyPad.h"
#include "Sound.h"
#include "extern.h"

class MainSequence;

class Menu{
public:
	Menu();
	~Menu();
	void update( MainSequence* mainSequence );
	void drawCenter( int y );
	void drawStringCenter( const char* str, int y, int color );
	void drawGearWheel();
	void drawButtan();
	void drawCredit();
	void setCursol();
	void setCreditPointY();

private:
	int mGearWheelHandle;
	int mCursol;
	int mCount;
	bool mDrawCredit;
	int mCreditPointY;
	int mBackGroundHandle;
	int mMainGameButtanHandle[ 2 ];
	int m1PBattleButtanHandle[ 2 ];
	int mOptionButtanHandle[ 2 ];
	int mCreditButtanHandle[ 2 ];
	int mUnknownButtanHandle[ 2 ];
	JoyPad mJoyPad;

};

#endif