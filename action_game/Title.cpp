#include "Title.h"

Title::Title() : mCursol( 0 ){
	mTitleHandle = LoadGraph( "pic/smash.png" );
	magnification = 1.0;
	mCount = 0;
	SetTransColor( 0, 0, 0 );
	/*
	mBackGroundHandle = MakeScreen( 640, 480, false );
	SetDrawScreen( mBackGroundHandle );
	SetDrawBlendMode( DX_BLENDMODE_ADD, 4 );
	for( int i = 0; i < 240; i += 2 ){
		DrawBox( 0, 240 - i, 640, 240 + i, GetColor( 255, 255, 255 ), true );
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	SetDrawScreen( DX_SCREEN_BACK );
	*/
	mBackGroundHandle = LoadGraph( "pic/title_ground_back.png" );
	SetDrawMode( DX_DRAWMODE_NEAREST );
	SetBackgroundColor( 0, 0, 0 );
}

Title::~Title(){
	DeleteGraph( mTitleHandle );
	DeleteGraph( mBackGroundHandle );
}

void Title::update( MainSequence* mainSequence ){
	mCount += 2;
	if( mCount >= 255 ){
		mCount = 0;
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	DrawRotaGraph( 640 / 2, 480 / 2, 1.0, 0, mBackGroundHandle, true, false );
	DrawRotaGraph( 640 / 2, 480 / 2, 1.0, 0, mTitleHandle, true );
	SetFontSize( 32 );
	if( mCount % 255 >= 127 ){
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 - mCount % 128 );
	}else{
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, mCount % 127 + 128 );
	}
	drawStringCenter( "PLAESE START BUTTAN", 380, GetColor( 255, 255, 255 ) );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	SetFontSize( 16 );
	drawStringCenter( "Copyright (C) 2013 マイクロコンピュータ部 All Rights Reserved.", 450, GetColor( 255, 255, 255 ) );
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad.setJoyPad( i ) ){
			continue;
		}
		if( checkMomentKey( KEY_INPUT_RETURN ) || mJoyPad.checkMomentJoyPad( i, PAD_INPUT_10 ) ){
				mainSequence->setSequence( MainSequence::SEQ_MENU );
				break;
		}
	}
	mJoyPad.setFlag();
}

void Title::drawStringCenter( const char* str, int y, int color ){
	int getStrlen = GetDrawStringWidth( str, strlen( str ) ) ;
	DrawFormatString( 640 / 2 - getStrlen / 2, y, color, str );
}
