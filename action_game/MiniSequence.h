#ifndef INCLUDE_MINI_SEQUENCE
#define INCLUDE_MINI_SEQUENCE

#include "CharacterSelect.h"
#include "State.h"
#include "Result.h"
#include "MainSequence.h"
#include "JoyPad.h"
#include "GameInfomation.h"
#include "extern.h"

class MainSequence;
class CharacterSelectMini;
class StateMini;
class ResultMini;

class MiniSequence{
public:
	MiniSequence();
	~MiniSequence();
	enum Sequence{
		SEQ_CHARACTER_SELECT,
		SEQ_SETTING,
		SEQ_GAME,
		SEQ_RESULT,
		SEQ_MENU,

		SEQ_UNKNOWN
	};
	void update( MainSequence* mainSequence );
	void setSequence( Sequence seq );
	void setResult( int* livesNum, int* smashNum, int* fallNum, int* gameOverTime );
	void setCharacterID( int* characterID );
	void setStageName( string stageName );

protected:
	CharacterSelectMini* mCharacterSelectMini;
	StateMini* mGame;
	ResultMini* mResultMini;
	GameInfomation mGameInfomation[ 4 ];
	string mStageName;
	JoyPad* mJoyPad;
};

#endif
