#ifndef INCLUDE_CHARACTER_SELECT_MINI_H
#define INCLUDE_CHARACTER_SELECT_MINI_H

#include <string>
#include "JoyPad.h"
#include "MiniSequence.h"
#include "DxLib.h"

using namespace std;

class MiniSequence;

class CharacterSelectMini{
public:
	CharacterSelectMini( JoyPad* joypad );
	~CharacterSelectMini();
	enum Points2{
		POINT_PLAYER1_DISPLAY_X_2 = 20,
		POINT_PLAYER1_DISPLAY_Y_2 = 30,
		POINT_PLAYER2_DISPLAY_X_2 = 470,
		POINT_PLAYER2_DISPLAY_Y_2 = POINT_PLAYER1_DISPLAY_Y_2,
	};
	void update( MiniSequence* miniSequence );
	void drawSquares();
	void drawCharacters();
	void drawNoJoyPad();

};

#endif