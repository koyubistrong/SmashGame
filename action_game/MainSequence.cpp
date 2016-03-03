#include "MainSequence.h"

MainSequence::MainSequence() : mSmashSequence( 0 ), mMiniSequence( 0 ), mMenu( 0 ){
	ChangeFontType( DX_FONTTYPE_ANTIALIASING_4X4 );
	Sound* sound = Sound::Instance();
	char* soundFileName[] = { "loop_47.wav", "loop_47.wav", "decide1.mp3", "decide2.mp3", "hito_ge_ki_kan.mp3", "move.mp3" };
	for( int i = 0; i < 6; i++ ){
		string strSound = "sound/";
		strSound += soundFileName[ i ];
		sound->setSound( strSound.data() );
	}
	/*
	sound->playSoundEffect( Sound::SOUND_EFFECT_DECIDE_1 );
	Sleep( 1000 );
	sound->playSoundEffect( Sound::SOUND_EFFECT_DECIDE_2 );
	Sleep( 1000 );
	sound->playSoundEffect( Sound::SOUND_EFFECT_MOVE );
	Sleep( 1000 );
	*/
	mTitle = new Title();
}

MainSequence::~MainSequence(){
	SafeDelete( mTitle );
	SafeDelete( mMenu );
	SafeDelete( mMiniSequence );
	SafeDelete( mSmashSequence );
}

void MainSequence::update(){
	if( mTitle ){
		mTitle->update( this );
	}else if( mMenu ){
		mMenu->update( this );
	}else if( mSmashSequence ){
		mSmashSequence->update( this );
	}else if( mMiniSequence ){
		mMiniSequence->update( this );
	}
}

void MainSequence::setSequence( Sequence seq ){
	switch( seq ){
	case SEQ_TITLE:
		mTitle = new Title();
		SafeDelete( mSmashSequence );
		SafeDelete( mMiniSequence );
		SafeDelete( mMenu );
		Sound::Instance()->stopSound();
		break;
	case SEQ_MENU:
		mMenu = new Menu();
		SafeDelete( mSmashSequence );
		SafeDelete( mMiniSequence );
		SafeDelete( mTitle );
		if( !Sound::Instance()->checkSound( Sound::SOUND_MENU ) ) Sound::Instance()->playBackSound( Sound::SOUND_MENU );
		break;
	case SEQ_SMASH:
		mSmashSequence = new SmashSequence();
		SafeDelete( mMiniSequence );
		SafeDelete( mTitle );
		SafeDelete( mMenu );
		break;
	case SEQ_MINIGAME:
		mMiniSequence = new MiniSequence();
		SafeDelete( mSmashSequence );
		SafeDelete( mTitle );
		SafeDelete( mMenu );
		break;
	case SEQ_OPTION:
		break;
	}
}