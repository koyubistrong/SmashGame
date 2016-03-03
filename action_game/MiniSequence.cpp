#include "MiniSequence.h"

MiniSequence::MiniSequence() : mCharacterSelectMini( 0 ), mGame( 0 ), mResultMini( 0 ){
	mJoyPad = new JoyPad();
	mCharacterSelectMini = new CharacterSelectMini( mJoyPad );
	for( int i = 0; i < 4; i++ ){
		mGameInfomation[ i ].characterID = -1;
	}
	mStageName = "";
}

MiniSequence::~MiniSequence(){
	SafeDelete( mJoyPad );
}

void MiniSequence::update( MainSequence* mainSequence ){
	if( mCharacterSelectMini && !mGame && !mResultMini ){
		mCharacterSelectMini->update( this );
	}else if( mGame ){
		mGame->update( this );
	}else if( mResultMini ){
		mResultMini->update( this );
	}else{
		mainSequence->setSequence( MainSequence::SEQ_MENU );
	}
}

void MiniSequence::setSequence( Sequence seq ){
	switch( seq ){
	case SEQ_CHARACTER_SELECT:
		if( !mCharacterSelectMini ) mCharacterSelectMini = new CharacterSelectMini( mJoyPad );
		SafeDelete( mGame );
		SafeDelete( mResultMini );
		mJoyPad->maskFlag();
		if( !Sound::Instance()->checkSound( Sound::SOUND_MENU ) ) Sound::Instance()->playBackSound( Sound::SOUND_MENU );
		break;
	case SEQ_GAME:
		mGame = new StateMini( mStageName.data(), mGameInfomation );
		SafeDelete( mResultMini );
		break;
	case SEQ_RESULT:
		mResultMini = new ResultMini( mGameInfomation );
		SafeDelete( mGame );
		break;
	case SEQ_MENU:
		SafeDelete( mCharacterSelectMini );
		SafeDelete( mGame );
		SafeDelete( mResultMini );
		break;
	}
}

void MiniSequence::setCharacterID( int* characterID ){
	for( int i = 0; i < 4; i++ ){
		mGameInfomation[ i ].characterID = characterID[ i ];
	}
}

void MiniSequence::setResult( int* livesNum, int* smashNum, int* fallNum, int* gameOverTime ){
	for( int i = 0; i < 4; i++ ){
		mGameInfomation[ i ].gameOverTime = gameOverTime[ i ];
		mGameInfomation[ i ].livesNum = livesNum[ i ];
		mGameInfomation[ i ].smashNum = smashNum[ i ];
		mGameInfomation[ i ].fallNum = fallNum[ i ];
	}
}

void MiniSequence::setStageName( string stageName ){
	mStageName = stageName;
}