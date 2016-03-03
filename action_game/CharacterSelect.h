#ifndef INCLUDE_CHARACTER_SELECT_H
#define INCLUDE_CHARACTER_SELECT_H

#include <string>
#include "JoyPad.h"
#include "CSV.h"
#include "Sound.h"
#include "extern.h"
#include "SmashSequence.h"
#include "MiniSequence.h"
#include "GameInfomation.h"
#include "DxLib.h"

using namespace std;

class SmashSequence;

class CharacterSelect{
public:
	CharacterSelect( JoyPad* joypad  );
	virtual ~CharacterSelect();
	enum Sizes{
		SIZE_PLAYER_DISPLAY_X = 150,
		SIZE_PLAYER_DISPLAY_Y = 200,
		SIZE_CHARACTER_DISPLAY_X = 65,
		SIZE_CHARACTER_DISPLAY_Y = 65,
	};
	enum Points{
		POINT_PLAYER1_DISPLAY_X = 20,
		POINT_PLAYER1_DISPLAY_Y = 30,
		POINT_PLAYER2_DISPLAY_X = 470,
		POINT_PLAYER2_DISPLAY_Y = POINT_PLAYER1_DISPLAY_Y,
		POINT_PLAYER3_DISPLAY_X = 20,
		POINT_PLAYER3_DISPLAY_Y = 250,
		POINT_PLAYER4_DISPLAY_X = 470,
		POINT_PLAYER4_DISPLAY_Y = 250,
		POINT_CHARACTER_DISPLAY_X = 190,
		POINT_CHARACTER_DISPLAY_Y = 80,
		POINT_TITLE_DISPLAY_Y = 40,
		POINT_OPTION_DISPLAY_Y = 420
	};
	enum CharacterNum{
		NUM_CHARACTER_X = 4,
		NUM_CHARACTER_Y = 5,
	};
	void update( SmashSequence* smashSequence );
	bool checkNext( int num );
	void moveCursol();
	void setComputerFlag();
	void drawString();
	void drawSquares();
	void drawSquares2();
	void drawCursols();
	void drawCharacters();
	void drawNoJoyPad();
	void drawStartAvailable( int i );
	void changeDefficult();
	void setVisibleJoyPad();
	void createMask();
	void deleteMask();

protected:
	int mCursol;
	int mCursolX[ 4 ];
	int mCursolY[ 4 ];
	int mCursolHandle[ 4 ];
	int mCursolComputerHandle[ 4 ];
	bool mCursolVisible[ 4 ];
	bool mOptionSelected;
	int mComputerFlag[ 4 ];
	string* mCharacterName;
	int mCharacterNum;
	bool mCharacterSelect[ 4 ];
	bool mComputerSelect[ 4 ];
	int mComputerDefficult[ 4 ];
	int mCharacterSelectID[ 4 ];
	bool mCharacterOnMouse[ 4 ];
	int* mCharacterSmallHandle;
	int* mCharacterBigHandle;
	int mFlameHandle;
	int mNoJoyPadHandle;
	int mMaskStartAvailableHandle;
	int mTriangleHandle[ 2 ];
	int mFontSize24;
	int mFontSize32;
	int mFontSize64;
	JoyPad* mJoyPad;

};

class MiniSequence;

class CharacterSelectMini : public CharacterSelect{
public:
	CharacterSelectMini( JoyPad* joypad );
	~CharacterSelectMini();
	enum Points2{
		POINT_PLAYER1_DISPLAY_X_2 = 20,
		POINT_PLAYER1_DISPLAY_Y_2 = 480 / 2 - SIZE_PLAYER_DISPLAY_Y / 2,
		POINT_PLAYER2_DISPLAY_X_2 = 470,
		POINT_PLAYER2_DISPLAY_Y_2 = POINT_PLAYER1_DISPLAY_Y_2,
	};
	void update( MiniSequence* miniSequence );
	void drawSquares();
	void drawCharacters();
	void drawNoJoyPad();
	void changeDefficult();
	void setVisibleJoyPad();
};


#endif