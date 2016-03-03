#include "State.h"

StateMini::StateMini( const char* filename, GameInfomation* gameInfomation ) : State(), mStaticObjectMini( 0 ), mDynamicObjectMini( 0 ){
	mStaticObject = 0;
	mDynamicObject = 0;
	string stageName = "stage/information/";
	CSV* csv_setting = new CSV( "setting/setting.csv" );
	const int* data = csv_setting->getDataInt();
	string stageBGMName = "stage/bgm/";
	string stageDafaultBGMName = stageBGMName + "test.mp3";
	mStageBGMHandle = LoadSoundMem( stageDafaultBGMName.data() );
	stageName += filename;
	stageName += ".smh";
	mSequence = SEQ_START;
	loading();
	mStaticObjectMini = new StaticObjectMini( stageName.data() );
	mDynamicObjectMini = new DynamicObjectMini( stageName.data(), gameInfomation, mStaticObjectMini );
	mNewStageHandle = LoadGraph( "pic/new_stage.png" );
	mBlackHandle = LoadGraph( "pic/black.png" );
	mStartHandle[ 0 ] = LoadGraph( "pic/start_3.png" );
	mStartHandle[ 1 ] = LoadGraph( "pic/start_2.png" );
	mStartHandle[ 2 ] = LoadGraph( "pic/start_1.png" );
	mStartHandle[ 3 ] = LoadGraph( "pic/start_go.png" );
	setMagnification( 1.0 );
	setStageHalfSize();
	mStaticObjectMini->getStageSize( &mSizeX, &mSizeY );
	mCameraX = -8 * 100;
	mCameraY = -8 * 100;
	mStartTime = 0;
	mEndTime = 0;
	mCreateFlag = 0;
	mSmashNumSum = 0;
	mPoseTime = 0;
	mFontSize16Handle = CreateFontToHandle( NULL, 16, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4 );
	mFontSize32Handle = CreateFontToHandle( NULL, 32, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4 );
	//mTimer = data[ 1 ] * 60;
	for( int i = 0; i < 4; i++ ){
		gameInfomation[ i ].gameOverTime = -1;
		gameInfomation[ i ].livesNum = -1;
		gameInfomation[ i ].smashNum = -1;
		gameInfomation[ i ].fallNum = -1;
	}
	mMode = MODE_LIVES;
	delete csv_setting;
}

StateMini::~StateMini(){
	SafeDelete( mStaticObjectMini );
	SafeDelete( mDynamicObjectMini );
	StopSoundMem( mStageBGMHandle );
	DeleteSoundMem( mStageBGMHandle );
}

int StateMini::update( MiniSequence* miniSequence ){
	clsDx();
	switch( mSequence ){
	case SEQ_START:
		start();
		break;
	case SEQ_GAME:
		game();
		break;
	case SEQ_POSE:
		pose();
		break;
	case SEQ_RETURN:
		miniSequence->setSequence( MiniSequence::SEQ_CHARACTER_SELECT );
		break;
	case SEQ_END:
		end();
		break;
	case SEQ_RESULT:
		miniSequence->setSequence( MiniSequence::SEQ_RESULT );
		break;
	}
	return 0;
}

void StateMini::start(){
	int sizeX;
	int sizeY;
	drawStage_Character();
	if( mStartTime < 60 ){
		GetGraphSize( mStartHandle[ 0 ], &sizeX, &sizeY ) ;
		DrawGraph( ( 640 - sizeX ) * 0.5, ( 480 - sizeY ) * 0.5, mStartHandle[ 0 ], TRUE );
	}else if( mStartTime < 120 ){
		GetGraphSize( mStartHandle[ 1 ], &sizeX, &sizeY ) ;
		DrawGraph( ( 640 - sizeX ) * 0.5, ( 480 - sizeY ) * 0.5, mStartHandle[ 1 ], TRUE );
	}else if( mStartTime < 180 ){
		GetGraphSize( mStartHandle[ 2 ], &sizeX, &sizeY ) ;
		DrawGraph( ( 640 - sizeX ) * 0.5, ( 480 - sizeY ) * 0.5, mStartHandle[ 2 ], TRUE );
	}else if( mStartTime < 240 ){
		GetGraphSize( mStartHandle[ 3 ], &sizeX, &sizeY ) ;
		DrawGraph( ( 640 - sizeX ) * 0.5, ( 480 - sizeY ) * 0.5, mStartHandle[ 3 ], TRUE );
		mDynamicObjectMini->update();
		checkGameSet();
		if( !CheckSoundMem( mStageBGMHandle ) ){
			PlaySoundMem( mStageBGMHandle, DX_PLAYTYPE_LOOP, true );
		}
		drawComponents();
	}else if( mStartTime < 300 ){
		mDynamicObjectMini->update();
		checkGameSet();
		drawComponents();
		mSequence = SEQ_GAME;
	}
	mStartTime++;
}

void StateMini::game(){
	moveCamera();
	checkCreateFlag();
	mDynamicObjectMini->update();
	checkGameSet();
	drawStage_Character();
	drawScore();
	drawComponents();
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad.setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad.checkMomentJoyPad( i, PAD_INPUT_10 ) ){
			mSequence = SEQ_POSE;
			break;
		}
	}
	mJoyPad.setFlag();
}

void StateMini::pose(){
	drawStage_Character();
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 127 );
	DrawBox( 0, 0, 640, 480, GetColor( 0, 0, 0 ), true );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	if( ( ( mPoseTime % 120 ) / 60 ) == 0 ){
		SetFontSize( 64 );
		int getStrlen = GetDrawFormatStringWidth( "POSE" );
		int tx = 640 / 2 - getStrlen / 2;
		int ty = 480 / 2 - 64 / 2;
		DrawFormatString( tx, ty, GetColor( 0, 0, 0 ), "POSE" );
		SetFontSize( 16 );
	}
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad.setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad.checkMomentJoyPad( i, PAD_INPUT_10 ) ){
			mSequence = SEQ_GAME;
			break;
		}
		if( mJoyPad.checkMomentJoyPad( i, PAD_INPUT_9 ) ){
			mSequence = SEQ_RETURN;
			break;
		}
	}
	mJoyPad.setFlag();
	mPoseTime++;
}

void StateMini::end(){
	drawStage_Character();
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 127 );
	DrawBox( 0, 0, 640, 480, GetColor( 0, 0, 0 ), true );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	SetFontSize( 64 );
	int smashNum[ 4 ] = { -1, -1, -1, -1 };
	mDynamicObjectMini->getCharacterSmashNum( smashNum );
	int smashNumSum = 0;
	for( int i = 0; i < 4; i++ ){
		if( smashNum[ i ] != -1 ){
			smashNumSum += smashNum[ i ];
		}
	}
	mSmashNumSum = smashNumSum;
	int getStrlen = GetDrawFormatStringWidth( "スコア" );
	int getStrlen2 = GetDrawFormatStringWidth( "%d", mSmashNumSum );
	int tx = 640 / 2 - getStrlen / 2;
	int ty = 480 / 2 - 128;
	int tx2 = 640 / 2 - getStrlen2 / 2;
	int ty2 = 480 / 2;
	DrawFormatString( tx, ty, GetColor( 255, 255, 255 ), "スコア" );
	DrawFormatString( tx2, ty2, GetColor( 255, 255, 255 ), "%d", mSmashNumSum );
	SetFontSize( 16 );
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad.setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad.checkMomentJoyPad( i, PAD_INPUT_10 ) ){
			mSequence = SEQ_RETURN;
			break;
		}
	}
	mJoyPad.setFlag();
	mEndTime++;
}

void StateMini::drawStage_Character(){
	mStaticObjectMini->drawStage( mCameraX * 0.01, mCameraY * 0.01 );
	mDynamicObjectMini->drawCharacter( mCameraX * 0.01, mCameraY * 0.01 );
}

void StateMini::drawBlowRate(){
	int blowRate[ 4 ] = { -1, -1, -1, -1 };
	int color[] = { GetColor( 255, 0, 0 ), GetColor( 0, 0, 255 ), GetColor( 255, 255, 0 ), GetColor( 0, 255, 0 ), GetColor( 127, 127, 127 ) };
	mDynamicObjectMini->getCharacterBlowRate( blowRate );
	SetFontSize( 32 );
	for( int i = 0; i < 4; i++ ){
		if( blowRate[ i ] != -1 ){
			int tx = POINT_BLOWRATE_X + SIZE_BLOWRATE_X * 0.5 - GetDrawFormatStringWidthToHandle( mFontSize32Handle, "%d%%", blowRate[ i ] ) * 0.5 + SIZE_BLOWRATE_X * i;
			DrawFormatStringToHandle( tx, POINT_BLOWRATE_Y, color[ i ], mFontSize32Handle, "%d%%", blowRate[ i ] );
		}
	}
	SetFontSize( 16 );
}

void StateMini::drawLives(){
	int livesNum[ 4 ] = { -1, -1, -1, -1 };
	int color[] = { GetColor( 255, 0, 0 ), GetColor( 0, 0, 255 ), GetColor( 255, 255, 0 ), GetColor( 0, 255, 0 ), GetColor( 127, 127, 127 ) };
	mDynamicObjectMini->getCharacterLivesNum( livesNum );
	SetFontSize( 16 );
	for( int i = 0; i < 4; i++ ){
		if( livesNum[ i ] != -1 ){
			int tx = POINT_BLOWRATE_X + SIZE_BLOWRATE_X * 0.5 - GetDrawFormatStringWidthToHandle( mFontSize16Handle, "●×%d", livesNum[ i ] ) * 0.5 + SIZE_BLOWRATE_X * i;
			DrawFormatStringToHandle( tx, POINT_BLOWRATE_Y - 16, color[ i ], mFontSize16Handle, "●×%d", livesNum[ i ] );
		}
	}
}

void StateMini::drawScore(){
	int smashNum[ 4 ] = { -1, -1, -1, -1 };
	mDynamicObjectMini->getCharacterSmashNum( smashNum );
	int smashNumSum = 0;
	for( int i = 0; i < 4; i++ ){
		if( smashNum[ i ] != -1 ){
			smashNumSum += smashNum[ i ];
		}
	}
	mSmashNumSum = smashNumSum;
	int strLen = GetDrawFormatStringWidthToHandle( mFontSize32Handle, "%5d", mSmashNumSum );
	DrawFormatStringToHandle( POINT_TIME_X - strLen - 10, POINT_TIME_Y, GetColor( 240, 240, 240 ), mFontSize32Handle, "%5d", mSmashNumSum );
}

void StateMini::moveCamera(){
	mCameraSpeedX = 50;
	mCameraX += mCameraSpeedX;
	if( mCameraX >= mSizeX * 100 ){
		mCameraX -= mSizeX * 100;
		mDynamicObjectMini->resetCharacterPoint();
	}
	mDynamicObjectMini->setCameraX( mCameraX * 0.01 );
	//printfDx( "cameraX : %d\n", mCameraX );
	//mCameraY += -( mCameraY - tempCameraY ) / 8;
}

void StateMini::checkCreateFlag(){
	mCreateFlag += mCameraSpeedX;
	//printfDx( "%d\n", mCreateFlag );
	if( mCreateFlag >= 640 * 100 ){
		mStaticObjectMini->crateStage();
		mCreateFlag = mCreateFlag % 640;
	}
}

void StateMini::setMagnification( double mag ){
	magnification = mag;
}

bool StateMini::checkTimer(){
	mTimer--;
	if( mTimer <= 0 ){
		return true;
	}
	return false;
}

void StateMini::checkGameSet(){
	switch( mMode ){
	case MODE_TIME:
		if( checkTimer() ){
			mSequence = SEQ_END;
		}
		break;
	case MODE_LIVES:
		if( mDynamicObjectMini->checkLives() ){
			mDynamicObjectMini->getResult();
			mSequence = SEQ_END;
		}
		break;
	case MODE_TIME_LIVES:
		if( checkTimer() || mDynamicObjectMini->checkLives() ){
			mSequence = SEQ_END;
		}
		break;
	}
}

void StateMini::setStageHalfSize(){
	mStaticObjectMini->getStageSize( &stagehalfsizeX, &stagehalfsizeX );
	stagehalfsizeX *= 0.5;
	stagehalfsizeY *= 0.5;
}
