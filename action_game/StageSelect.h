#ifndef INCLUDE_STAGE_SELECT_H
#define INCLUDE_STAGE_SELECT_H

#include <string>
#include "JoyPad.h"
#include "CSV.h"
#include "extern.h"
#include "SmashSequence.h"
#include "DxLib.h"

using namespace std;

class SmashSequence;

class StageSelect{
public:
	StageSelect( JoyPad* joypad );
	~StageSelect();
	enum Sizes{
		SIZE_STAGE_SELECT_DISPLAY_X = 380,
		SIZE_STAGE_SELECT_DISPLAY_Y = 180,
		SIZE_STAGE_DISPLAY_X = 125,
		SIZE_STAGE_DISPLAY_Y = 80,
		SIZE_STAGE_SPACE_X = 20,
		SIZE_STAGE_SPACE_Y = 20
	};
	enum Points{
		POINT_STAGE_SELECT_DISPLAY_X = 130,
		POINT_STAGE_SELECT_DISPLAY_Y = 60,
		POINT_STAGE_DISPLAY_X = 40,
		POINT_STAGE_DISPLAY_Y = 260
	};
	enum CharacterNum{
		NUM_STAGE_X = 4,
		NUM_STAGE_Y = 2,
	};
	void update( SmashSequence* smashSequence );
	void moveCursol();
	void drawSquares();
	void drawCursols();
	void drawStages();
	void drawString();
	void setVisibleJoyPad();

private:
	int mCursol;
	int mCursolX;
	int mCursolY;
	int mCursolHandle;
	bool mCursolVisible;
	string* mStageName;
	bool mStageSelected;
	int mStageSelectedID;
	int mStageNum;
	int* mStageSmallHandle;
	int* mStageBigHandle;
	int mTempShowStage;
	JoyPad* mJoyPad;

};

#endif