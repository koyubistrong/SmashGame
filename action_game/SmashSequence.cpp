#include "SmashSequence.h"

SmashSequence::SmashSequence() : mStageSelect( 0 ), mSetting( 0 ), mCharacterSelect( 0 ), mGame( 0 ), mResult( 0 ){
	mJoyPad = new JoyPad();
	mCharacterSelect = new CharacterSelect( mJoyPad );
	for( int i = 0; i < 4; i++ ){
		mGameInfomation[ i ].characterID = -1;
	}
	mStageName = "";
}

SmashSequence::~SmashSequence(){
	SafeDelete( mJoyPad );
}

void SmashSequence::update( MainSequence* mainSequence ){
	if( mCharacterSelect && !mSetting && !mStageSelect && !mGame && !mResult ){
		mCharacterSelect->update( this );
	}else if( mSetting ){
		mSetting->update( this );
	}else if( mStageSelect ){
		mStageSelect->update( this );
	}else if( mGame ){
		mGame->update( this );
	}else if( mResult ){
		mResult->update( this );
	}else{
		mainSequence->setSequence( MainSequence::SEQ_MENU );
	}
}

void SmashSequence::setSequence( Sequence seq ){
	switch( seq ){
	case SEQ_CHARACTER_SELECT:
		if( !mCharacterSelect ) mCharacterSelect = new CharacterSelect( mJoyPad );
		SafeDelete( mSetting );
		SafeDelete( mStageSelect );
		SafeDelete( mGame );
		SafeDelete( mResult );
		if( !Sound::Instance()->checkSound( Sound::SOUND_MENU ) ) Sound::Instance()->playBackSound( Sound::SOUND_MENU );
		mJoyPad->maskFlag();
		SetBackgroundColor( 127, 127, 127 );
		break;
	case SEQ_SETTING:
		mSetting = new Setting( mJoyPad );
		SafeDelete( mStageSelect );
		SafeDelete( mGame );
		SafeDelete( mResult );
		break;
	case SEQ_STAGE_SELECT:
		mStageSelect = new StageSelect( mJoyPad );
		SafeDelete( mSetting );
		SafeDelete( mGame );
		SafeDelete( mResult );
		break;
	case SEQ_GAME:
		mGame = new State( mStageName.data(), mGameInfomation );
		SafeDelete( mSetting );
		SafeDelete( mStageSelect );
		SafeDelete( mResult );
		break;
	case SEQ_RESULT:
		mResult = new Result( mGameInfomation );
		SafeDelete( mSetting );
		SafeDelete( mStageSelect );
		SafeDelete( mGame );
		break;
	case SEQ_MENU:
		SafeDelete( mCharacterSelect );
		SafeDelete( mSetting );
		SafeDelete( mStageSelect );
		SafeDelete( mGame );
		SafeDelete( mResult );
		break;
	}
}

void SmashSequence::setCharacterID( int* characterID ){
	for( int i = 0; i < 4; i++ ){
		mGameInfomation[ i ].characterID = characterID[ i ];
	}
}

void SmashSequence::setComputerFlag( bool* computerFlag ){
	for( int i = 0; i < 4; i++ ){
		mGameInfomation[ i ].computerFlag = computerFlag[ i ];
	}
}

void SmashSequence::setComputerDefficult( int* computerDefficult ){
	for( int i = 0; i < 4; i++ ){
		mGameInfomation[ i ].computerDefficult = computerDefficult[ i ];
	}
}

void SmashSequence::setResult( int* livesNum, int* smashNum, int* fallNum, int* gameOverTime ){
	for( int i = 0; i < 4; i++ ){
		mGameInfomation[ i ].gameOverTime = gameOverTime[ i ];
		mGameInfomation[ i ].livesNum = livesNum[ i ];
		mGameInfomation[ i ].smashNum = smashNum[ i ];
		mGameInfomation[ i ].fallNum = fallNum[ i ];
	}
}

void SmashSequence::setStageName( string stageName ){
	mStageName = stageName;
}