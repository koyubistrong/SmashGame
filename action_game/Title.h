#ifndef INCLUDE_TITLE_H
#define INCLUDE_TITLE_H

#include "DxLib.h"
#include "MainSequence.h"
#include "Image.h"
#include "JoyPad.h"
#include "extern.h"

class MainSequence;

class Title{
public:
	Title();
	~Title();
	void update( MainSequence* mainSequence );
	void drawCenter( int y );
	void drawStringCenter( const char* str, int y, int color );

private:
	int mTitleHandle;
	int mCursol;
	int mCount;
	int mBackGroundHandle;
	JoyPad mJoyPad;

};

#endif