#include "StageSelect.h"

StageSelect::StageSelect( JoyPad* joypad ) : mStageSelected( false ){
	CSV* stage_csv = new CSV( "stage/information/stage.csv" );
	mStageName = new string[ stage_csv->getSizeY() ];
	for( int i = 0; i < stage_csv->getSizeY(); i++ ){
		mStageName[ i ] = stage_csv->getDataString()[ i ];
	}
	mStageNum = stage_csv->getSizeY();
	mTempShowStage = 0;
	mStageSmallHandle = new int[ mStageNum ];
	mStageBigHandle = new int[ mStageNum ];
	for( int i = 0; i < mStageNum; i++ ){
		string str( "stage/" );
		string bigStr = str + "pic/" + mStageName[ i ] + "_big.png";
		string smallStr = str + "pic/" + mStageName[ i ] + "_small.png";
		mStageBigHandle[ i ] = LoadGraph( bigStr.data() );
		mStageSmallHandle[ i ] = LoadGraph( smallStr.data() );
	}
	SetBackgroundColor( 127, 127, 127 );
	setVisibleJoyPad();
	string str( "pic/cursol1.png" );
	mCursolHandle = LoadGraph( str.data() );
	mCursolX = 640 / 2 * 100;
	mCursolY = 480 / 2 * 100;
	mJoyPad = joypad;
	SafeDelete( stage_csv );
}

StageSelect::~StageSelect(){
	for( int i = 0; i < mStageNum; i++ ){
		DeleteGraph( mStageSmallHandle[ i ] );
		DeleteGraph( mStageBigHandle[ i ] );
	}
	DeleteGraph( mCursolHandle );
	SafeDeleteArray( mStageName );
	SafeDeleteArray( mStageSmallHandle );
	SafeDeleteArray( mStageBigHandle );
}

void StageSelect::update( SmashSequence* smashSequence ){
	setVisibleJoyPad();
	moveCursol();
	drawString();
	drawStages();
	drawSquares();
	drawCursols();
	for( int i = 0; i < 4; i++ ){
		if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_9 ) ){
			smashSequence->setSequence( SmashSequence::SEQ_CHARACTER_SELECT );
			break;
		}
		if( mStageSelected ){
			Sound::Instance()->stopSound();
			Sound::Instance()->playSoundEffect( Sound::SOUND_EFFECT_DECIDE_3 );
			smashSequence->setStageName( mStageName[ mStageSelectedID ] );
			smashSequence->setSequence( SmashSequence::SEQ_GAME );
			break;
		}
	}
}

void StageSelect::moveCursol(){
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad->setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad->checkInputJoyPad( i, PAD_INPUT_UP ) ){
			mCursolY -= 450;
		}
		if( mJoyPad->checkInputJoyPad( i, PAD_INPUT_DOWN ) ){
			mCursolY += 450;
		}
		if( mJoyPad->checkInputJoyPad( i, PAD_INPUT_RIGHT ) ){
			mCursolX += 450;
		}
		if( mJoyPad->checkInputJoyPad( i, PAD_INPUT_LEFT ) ){
			mCursolX -= 450;
		}
		if( mCursolX / 100 < -8 ){
			mCursolX = -8 * 100;
		}
		if( mCursolY / 100 < -8 ){
			mCursolY = -8 * 100;
		}
		if( mCursolX / 100 >= 640 - 16 ){
			mCursolX = ( 640 - 16 ) * 100;
		}
		if( mCursolY / 100 >= 480 - 16 ){
			mCursolY = ( 480 - 16 ) * 100;
		}
	}
	mJoyPad->setFlag();
}

void StageSelect::drawSquares(){
	for( int i = 0; i < 3; i++ ){
	DrawBox( POINT_STAGE_SELECT_DISPLAY_X - i, POINT_STAGE_SELECT_DISPLAY_Y - i,
		POINT_STAGE_SELECT_DISPLAY_X + SIZE_STAGE_SELECT_DISPLAY_X + i, 
		POINT_STAGE_SELECT_DISPLAY_Y + SIZE_STAGE_SELECT_DISPLAY_Y + i, GetColor( 0, 0, 0 ), false );
	}
	for( int y = 0; y < NUM_STAGE_Y; y++ ){
		for( int x = 0; x < NUM_STAGE_X; x++ ){
			int tx1 = POINT_STAGE_DISPLAY_X + ( SIZE_STAGE_DISPLAY_X + SIZE_STAGE_SPACE_X ) * x;
			int ty1 = POINT_STAGE_DISPLAY_Y + ( SIZE_STAGE_DISPLAY_Y + SIZE_STAGE_SPACE_Y ) * y;
			int tx2 = tx1 + SIZE_STAGE_DISPLAY_X;
			int ty2 = ty1 + SIZE_STAGE_DISPLAY_Y;
			for( int i = 0; i < 3; i++ ){
				DrawBox( tx1 - i, ty1 - i, tx2 + i, ty2 + i, GetColor( 0, 0, 0 ), false );
			}
		}
	}
	DrawBox( 0, 0, 640, 20, GetColor( 0, 0, 0 ), true );
	DrawBox( 0, 480 - 20, 640, 480, GetColor( 0, 0, 0 ), true );
}

void StageSelect::drawCursols(){
	DrawGraph( mCursolX / 100, mCursolY / 100, mCursolHandle, true );
}

void StageSelect::drawStages(){
	for( int y = 0; y < NUM_STAGE_Y; y++ ){
		for( int x = 0; x < NUM_STAGE_X; x++ ){
			int num = x + y * NUM_STAGE_X;
			if( num < mStageNum ){
				int tx1 = POINT_STAGE_DISPLAY_X + ( SIZE_STAGE_DISPLAY_X + SIZE_STAGE_SPACE_X ) * x;
				int ty1 = POINT_STAGE_DISPLAY_Y + ( SIZE_STAGE_DISPLAY_Y + SIZE_STAGE_SPACE_Y ) * y;
				int tx2 = POINT_STAGE_DISPLAY_X + ( SIZE_STAGE_DISPLAY_X ) * ( x + 1 ) - 1;
				int ty2 = POINT_STAGE_DISPLAY_Y + ( SIZE_STAGE_DISPLAY_Y ) * ( y + 1 ) - 1;
				DrawGraph( tx1, ty1, mStageSmallHandle[ num ], false );
				if( checkRange( mCursolX / 100, mCursolY / 100, tx1, ty1, tx2, ty2 ) ){
					mTempShowStage = num;
					for( int i = 0; i < 4; i++ ){
						if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_2 ) ){
							mStageSelectedID = num;
							mStageSelected = true;
							continue;
						}
					}
				}
			}
		}
	}
	DrawGraph( POINT_STAGE_SELECT_DISPLAY_X, POINT_STAGE_SELECT_DISPLAY_Y, mStageBigHandle[ mTempShowStage ], false );
}

void StageSelect::drawString(){
	SetFontSize( 32 );
	char* str = "STAGE SELECT";
	int getStrlen = GetDrawStringWidth( str, strlen( str ) ) ;
	DrawFormatString( 640 / 2 - getStrlen / 2, 25, GetColor( 0, 0, 0 ), str );
	SetFontSize( 16 );
}

void StageSelect::setVisibleJoyPad(){
	mCursolVisible = true;
}