#ifndef INCLUDE_SETTING_H
#define INCLUDE_SETTING_H

#include "DxLib.h"
#include "extern.h"
#include "CSV.h"
#include "JoyPad.h"
#include "SmashSequence.h"

class SmashSequence;

class Setting{
public:
	Setting( JoyPad* joypad );
	~Setting();
	enum Sizes{
		SIZE_SETTING_DETAIL_X = 400
	};
	enum Points{
		POINT_TITLE_Y = 20,
		POINT_SETTING_DETAIL_Y = 100
	};
	void update( SmashSequence* smashSequence );
	void moveCursol();
	void outputFile();
	void drawString();
	void drawStringCenter( const char* str, int y );
	void drawCursols();
private:
	int mCursol;
	int mCursolX;
	int mCursolY;
	int mCursolHandle;
	int* mSetting;
	bool mCansel;
	bool mSave;
	JoyPad* mJoyPad;
	CSV* mFile;
};

#endif