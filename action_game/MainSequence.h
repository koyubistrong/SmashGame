#ifndef INCLUDE_MAIN_SEQUENCE_H
#define INCLUDE_MAIN_SEQUENCE_H

#include "Title.h"
#include "Menu.h"
#include "MiniSequence.h"
#include "SmashSequence.h"
#include "Sound.h"
#include "extern.h"

class Title;
class Menu;
class SmashSequence;
class MiniSequence;

class MainSequence{
public:
	MainSequence();
	~MainSequence();
	enum Sequence{
		SEQ_TITLE,
		SEQ_MENU,
		SEQ_SMASH,
		SEQ_MINIGAME,
		SEQ_OPTION,

		SEQ_UNKNOWN
	};
	void update();
	void setSequence( Sequence seq );
private:
	Title* mTitle;
	Menu* mMenu;
	MiniSequence* mMiniSequence;
	SmashSequence* mSmashSequence;

};

#endif