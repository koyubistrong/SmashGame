#ifndef INCLUDE_SMASH_SEQUENCE_H
#define INCLUDE_SMASH_SEQUENCE_H

#include "CharacterSelect.h"
#include "Setting.h"
#include "StageSelect.h"
#include "State.h"
#include "Result.h"
#include "JoyPad.h"
#include "extern.h"
#include "GameInfomation.h"
#include "MainSequence.h"

class MainSequence;
class CharacterSelect;
class Setting;
class StageSelect;
class State;
class Result;

class SmashSequence{
public:
	SmashSequence();
	~SmashSequence();
	enum Sequence{
		SEQ_CHARACTER_SELECT,
		SEQ_SETTING,
		SEQ_STAGE_SELECT,
		SEQ_GAME,
		SEQ_RESULT,
		SEQ_MENU,

		SEQ_UNKNOWN
	};
	void update( MainSequence* mainSequence );
	void setSequence( Sequence seq );
	void setResult( int* livesNum, int* smashNum, int* fallNum, int* gameOverTime );
	void setCharacterID( int* characterID );
	void setComputerFlag( bool* computerFlag );
	void setComputerDefficult( int* computerDefficult );
	void setStageName( string stageName );

private:
	CharacterSelect* mCharacterSelect;
	Setting* mSetting;
	StageSelect* mStageSelect;
	State* mGame;
	Result* mResult;
	GameInfomation mGameInfomation[ 4 ];
	string mStageName;
	JoyPad* mJoyPad;

};

#endif