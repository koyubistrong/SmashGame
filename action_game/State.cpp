#include "State.h"
int gSizeX = 200;
int gSizeY = 60;

State::State( const char* filename, GameInfomation* gameInfomation ) : FPS(50), mStaticObject( 0 ), mDynamicObject( 0 ), mSequence( SEQ_START ){
	int minX;
	int minY;
	int maxX;
	int maxY;
	string stageName = "stage/information/";
	string stageBGMName = "stage/bgm/";
	string stageDafaultBGMName = stageBGMName + "test.mp3";
	CSV* csv_setting = new CSV( "setting/setting.csv" );
	const int* data = csv_setting->getDataInt();
	stageName += filename;
	stageName += ".smh";
	stageBGMName += filename;
	stageBGMName += ".mp3";
	loading();
	setMagnification( 1.0 );
	mStaticObject = new StaticObject( filename );
	mDynamicObject = new DynamicObject( stageName.data(), gameInfomation, mStaticObject );
	mStageBGMHandle = LoadSoundMem( stageBGMName.data() );
	if( mStageBGMHandle == -1 ){ mStageBGMHandle = LoadSoundMem( stageDafaultBGMName.data() ); }
	mStaticObject->getStageSize( &mSizeX, &mSizeY );
	mNewStageHandle = LoadGraph( "pic/new_stage.png" );
	mBlackHandle = LoadGraph( "pic/black.png" );
	mStartHandle[ 0 ] = LoadGraph( "pic/start_3.png" );
	mStartHandle[ 1 ] = LoadGraph( "pic/start_2.png" );
	mStartHandle[ 2 ] = LoadGraph( "pic/start_1.png" );
	mStartHandle[ 3 ] = LoadGraph( "pic/start_go.png" );
	setStageHalfSize();
	mDynamicObject->getPlayerPoint( &minX, &minY, &maxX, &maxY );
	{
		double degreeMinX = 640.0 / ( double )mSizeX * 1.1;
		double degreeMinY = 480.0 / ( double )mSizeY * 1.1;
		mMinDegree = ( degreeMinX > degreeMinY ) ? degreeMinX : degreeMinY;
		double degreeX = 640.0 * 100.0 / ( double )( maxX - minX );
		double degreeY = 480.0 * 100.0 / ( double )( maxY - minY );
		magnification = ( degreeX < degreeY ) ? degreeX : degreeY;
		magnification *= 0.8;
		if( magnification <= mMinDegree ){
			magnification = mMinDegree;
		}else if( magnification > 1.5 ){
			magnification = 1.5;
		}
		mCameraX = ( minX + maxX ) / 2 * magnification - ( 640 * 0.5 * 100 );
		mCameraY = ( minY + maxY ) / 2 * magnification - ( ( 480 + 80 ) * 0.5 * 100 );
		if( mCameraX < 0 ){
			mCameraX = 0;
		}else if( mCameraX + 640 * 100 >= mSizeX * 100 * magnification - 800 * magnification ){
			mCameraX = mSizeX * 100 * magnification - 640 * 100 - 800 * magnification;
		}
		if( mCameraY < 0 ){
			mCameraY = 0;
		}else if( mCameraY + 480 * 100 >= mSizeY * 100 * magnification - 800 * magnification ){
			mCameraY = mSizeY * 100 * magnification - 480 * 100 - 800 * magnification;
		}
	}
	mState = true;
	mStartTime = 0;
	mEndTime = 0;
	mPoseTime = 0;
	mTimer = data[ 1 ] * FPS;
	mFontSize16Handle = CreateFontToHandle( NULL, 16, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4 );
	mFontSize32Handle = CreateFontToHandle( NULL, 32, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4 );
	SetDrawMode( DX_DRAWMODE_BILINEAR );
	for( int i = 0; i < 4; i++ ){
		gameInfomation[ i ].gameOverTime = -1;
		gameInfomation[ i ].livesNum = -1;
		gameInfomation[ i ].smashNum = -1;
		gameInfomation[ i ].fallNum = -1;
		mComputerFlag[ i ] = gameInfomation[ i ].computerFlag;
	}
	if( data[ 0 ] == MODE_TIME ){
		mMode = MODE_TIME;
	}else if( data[ 0 ] == MODE_LIVES ){
		mMode = MODE_LIVES;
	}else if( data[ 0 ] == MODE_TIME_LIVES ){
		mMode = MODE_TIME_LIVES;
	}
	SetBackgroundColor( 0, 0, 0 );
	delete csv_setting;
	//loading();
}

State::~State(){
	SafeDelete( mStaticObject );
	SafeDelete( mDynamicObject );
	for( int i = 0; i < 4; i++ ){
		DeleteGraph( mStartHandle[ i ] );
	}
	DeleteGraph( mNewStageHandle );
	DeleteGraph( mBlackHandle );
	DeleteFontToHandle( mFontSize32Handle );
	DeleteFontToHandle( mFontSize16Handle );
	StopSoundMem( mStageBGMHandle );
	DeleteSoundMem( mStageBGMHandle );
	DeleteFontToHandle( mFontSize32Handle );
	DeleteFontToHandle( mFontSize16Handle );
}

int State::update( SmashSequence* smashSequence ){
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
		smashSequence->setSequence( SmashSequence::SEQ_CHARACTER_SELECT );
		break;
	case SEQ_END:
		end();
		break;
	case SEQ_RESULT:
		smashSequence->setSequence( SmashSequence::SEQ_RESULT );
		break;
	}
	return 0;
}

void State::start(){
	int sizeX;
	int sizeY;
	drawStage_Character();
	if( mStartTime < FPS ){
		GetGraphSize( mStartHandle[ 0 ], &sizeX, &sizeY ) ;
		DrawGraph( ( 640 - sizeX ) * 0.5, ( 480 - sizeY ) * 0.5, mStartHandle[ 0 ], TRUE );
	}else if( mStartTime < FPS * 2 ){
		GetGraphSize( mStartHandle[ 1 ], &sizeX, &sizeY ) ;
		DrawGraph( ( 640 - sizeX ) * 0.5, ( 480 - sizeY ) * 0.5, mStartHandle[ 1 ], TRUE );
	}else if( mStartTime < FPS * 3 ){
		GetGraphSize( mStartHandle[ 2 ], &sizeX, &sizeY ) ;
		DrawGraph( ( 640 - sizeX ) * 0.5, ( 480 - sizeY ) * 0.5, mStartHandle[ 2 ], TRUE );
	}else if( mStartTime < FPS * 4 ){
		GetGraphSize( mStartHandle[ 3 ], &sizeX, &sizeY ) ;
		DrawGraph( ( 640 - sizeX ) * 0.5, ( 480 - sizeY ) * 0.5, mStartHandle[ 3 ], TRUE );
		moveCamera();
		mDynamicObject->update();
		if( !CheckSoundMem( mStageBGMHandle ) ){
			PlaySoundMem( mStageBGMHandle, DX_PLAYTYPE_LOOP, true );
		}
		checkGameSet();
	}else if( mStartTime >= FPS * 4 ){
		moveCamera();
		mDynamicObject->update();
		checkGameSet();
		drawComponents();
		mSequence = SEQ_GAME;
	}
	mStartTime++;
}

void State::game(){
	moveCamera();
	mDynamicObject->update();
	checkGameSet();
	drawStage_Character();
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

void State::pose(){
	drawStage_Character();
	drawComponents();
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 127 );
	DrawBox( 0, 0, 640, 480, GetColor( 0, 0, 0 ), true );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	if( ( ( mPoseTime % (FPS * 2) ) / FPS ) == 0 ){
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
			mPoseTime = 0;
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

void State::end(){
	drawStage_Character();
	drawComponents();
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 127 );
	DrawBox( 0, 0, 640, 480, GetColor( 0, 0, 0 ), true );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	SetFontSize( 64 );
	int getStrlen = GetDrawFormatStringWidth( "GAME SET" );
	int tx = 640 / 2 - getStrlen / 2;
	int ty = 480 / 2 - 64 / 2;
	DrawFormatString( tx, ty, GetColor( 0, 0, 0 ), "GAME SET" );
	SetFontSize( 16 );
	if( mEndTime > 120 ){
		mDynamicObject->getResult();
		mSequence = SEQ_RESULT;
	}
	mEndTime++;
}

void State::loading(){
	int count = 2;
	SetFontSize( 24 );
	int getStrlen = GetDrawFormatStringWidth( "Loading..." );
	char* tenten[] = { ".", "..", "..." };
	int tx = 640 / 2 - getStrlen / 2;
	int ty = 480 / 2 - 24 / 2;
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 127 );
	DrawBox( 0, 0, 640, 480, GetColor( 0, 0, 0 ), true );
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );
	DrawFormatString( tx, ty, GetColor( 255, 255, 255 ), "Loading%s", tenten[ count ] );
	SetFontSize( 16 );
	ScreenFlip();
	/*
	int count = 0;
	int getStrlen = GetDrawFormatStringWidth( "Loading..." );
	char* tenten[] = { ".", "..", "..." };
	int tx = 640 / 2 - getStrlen / 2;
	int ty = 480 / 2 - 64 / 2;
	SetFontSize( 16 );
	while( GetASyncLoadNum() >= 1 ){
        ProcessMessage();
        ClearDrawScreen();
		DrawBox( 0, 0, 640, 480, GetColor( 0, 0, 0 ), true );
		DrawFormatString( tx, ty, GetColor( 255, 255, 255 ), "Loading%s", tenten[ ( count / 60 ) % 3 ] );
		printfDx( "%d", GetASyncLoadNum() );
		ScreenFlip();
		WaitKey();
		Sleep( 1000 );
		count++;
	}
	*/
}

void State::drawStage_Character(){
	mStaticObject->drawStage( mCameraX * 0.01, mCameraY * 0.01 );
	mDynamicObject->drawCharacter( mCameraX * 0.01, mCameraY * 0.01 );
}

void State::drawComponents(){
	drawBlowRate();
	if( mMode != MODE_LIVES ){
		drawTimer();
	}
	if( mMode != MODE_TIME ){
		drawLives();
	}
}

void State::drawBlowRate(){
	int blowRate[ 4 ] = { -1, -1, -1, -1 };
	int color[] = { GetColor( 255, 0, 0 ), GetColor( 0, 0, 255 ), GetColor( 255, 255, 0 ), GetColor( 0, 255, 0 ), GetColor( 127, 127, 127 ) };
	mDynamicObject->getCharacterBlowRate( blowRate );
	for( int i = 0; i < 4; i++ ){
		if( blowRate[ i ] != -1 ){
			int tx = POINT_BLOWRATE_X + SIZE_BLOWRATE_X * 0.5 - GetDrawFormatStringWidthToHandle( mFontSize32Handle, "%d%%", blowRate[ i ] ) * 0.5 + SIZE_BLOWRATE_X * i;
			if( !mComputerFlag[ i ] ){
				DrawFormatStringToHandle( tx, POINT_BLOWRATE_Y, color[ i ], mFontSize32Handle, "%d%%", blowRate[ i ] );
			}else{
				DrawFormatStringToHandle( tx, POINT_BLOWRATE_Y, color[ 4 ], mFontSize32Handle, "%d%%", blowRate[ i ] );
			}
		}
	}
}

void State::drawLives(){
	int livesNum[ 4 ] = { -1, -1, -1, -1 };
	int color[] = { GetColor( 255, 0, 0 ), GetColor( 0, 0, 255 ), GetColor( 255, 255, 0 ), GetColor( 0, 255, 0 ), GetColor( 127, 127, 127 ) };
	mDynamicObject->getCharacterLivesNum( livesNum );
	for( int i = 0; i < 4; i++ ){
		if( livesNum[ i ] != -1 ){
			int tx = POINT_BLOWRATE_X + SIZE_BLOWRATE_X * 0.5 - GetDrawFormatStringWidthToHandle( mFontSize16Handle, "œ~%d", livesNum[ i ] ) * 0.5 + SIZE_BLOWRATE_X * i;
			if( !mComputerFlag[ i ] ){
				DrawFormatStringToHandle( tx, POINT_BLOWRATE_Y - 16, color[ i ], mFontSize16Handle, "œ~%d", livesNum[ i ] );
			}else{
				DrawFormatStringToHandle( tx, POINT_BLOWRATE_Y - 16, color[ 4 ], mFontSize16Handle, "œ~%d", livesNum[ i ] );
			}
		}
	}
}

void State::drawTimer(){
	int strLen = GetDrawFormatStringWidthToHandle( mFontSize32Handle, "%d:%2d", mTimer / (FPS * 60), ( mTimer / FPS ) % 60 );
	DrawFormatStringToHandle( POINT_TIME_X - strLen - 10, POINT_TIME_Y, GetColor( 240, 240, 240 ), mFontSize32Handle, "%d:%02d", mTimer / (FPS * 60), ( mTimer / FPS ) % 60 );
}

void State::moveCamera(){
	int minX;
	int minY;
	int maxX;
	int maxY;
	int tempCameraX;
	int tempCameraY;
	double degreeX;
	double degreeY;
	double tempDegree;
	static double prevMagnification = magnification;
	mDynamicObject->getPlayerPoint( &minX, &minY, &maxX, &maxY );
	degreeX = 640.0 * 100.0 / ( double )( maxX - minX );
	degreeY = 480.0 * 100.0 / ( double )( maxY - minY );
	if( maxX == minX ){
		degreeX = 99999.0;
	}
	if( maxY == minY ){
		degreeY = 99999.0;
	}
	tempDegree = ( degreeX < degreeY ) ? degreeX : degreeY;
	tempDegree *= 0.8;
	if( tempDegree < mMinDegree ){
		tempDegree = mMinDegree;
	}else if( tempDegree > 1.5 ){
		tempDegree = 1.5;
	}
	if( tempDegree + 0.01 <= magnification ){
		setMagnification( magnification - 0.01 );
	}else if( tempDegree - 0.01 >= magnification ){
		setMagnification( magnification + 0.01 );
	}

	tempCameraX = ( minX + maxX ) * 0.5 * magnification - ( 640 * 100 * 0.5 );
	tempCameraY = ( minY + maxY ) * 0.5 * magnification - ( 480 * 100 * 0.5 );

	//mCameraX = tempCameraX;
	//mCameraY = tempCameraY;

	//printfDx( "tempCameraX %5d  cameraX %5d  %d\n", tempCameraX, mCameraX, ( tempCameraX - mCameraX ) / 8 );
	//printfDx( "tempCameraY %5d  cameraY %5d  %d\n", tempCameraY, mCameraY, ( tempCameraY - mCameraY ) / 8 );
	//printfDx( "magnification %lf\n", magnification );
	/*
	double tx = tempCameraX - mCameraX;
	double ty = tempCameraY - mCameraY;
	double angle = atan2( ty, tx );
	double speedX = cos( angle ) * 4;
	double speedY = sin( angle ) * 4;
	*/
	if( abs( ( tempCameraX - mCameraX ) ) > 200 ){
		mCameraX += ( tempCameraX - mCameraX ) / 8;
		if( ( tempCameraX - mCameraX ) / 8 == 0 ){
			if( tempCameraX - mCameraX > 0 ){
				mCameraX++;
			}else{
				mCameraX--;
			}
		}
		//mCameraX += speedX;
	}
	if( abs( ( tempCameraY - mCameraY ) ) > 200 ){
		mCameraY += ( tempCameraY - mCameraY ) / 8;
		if( ( tempCameraY - mCameraY ) / 8 == 0 ){
			if( tempCameraY - mCameraY > 0 ){
				mCameraY++;
			}else{
				mCameraY--;
			}
		}
		//mCameraY += speedY;
	}
	if( mCameraX < 0 ){
		mCameraX = 0;
	}else if( mCameraX + 640 * 100 >= mSizeX * 100 * magnification - 800 * magnification ){
		mCameraX = mSizeX * 100 * magnification - 640 * 100 - 800 * magnification;
	}
	if( mCameraY < 0 ){
		mCameraY = 0;
	}else if( mCameraY + 480 * 100 >= mSizeY * 100 * magnification - 800 * magnification ){
		mCameraY = mSizeY * 100 * magnification - 480 * 100 - 800 * magnification;
	}
	/*
	if( tempCameraX + 4 < mCameraX ){
		mCameraX += -( mCameraX - tempCameraX ) / 8;
		//mCameraX += speedX;
	}else if( tempCameraX - 4 > mCameraX ){
		mCameraX += ( tempCameraX - mCameraX ) / 8;
		//mCameraX += speedX;
	}
	if( tempCameraY + 4 < mCameraY ){
		mCameraY += -( mCameraY - tempCameraY ) / 8;
		//mCameraY += speedY;
	}else if( tempCameraY - 4 > mCameraY ){
		mCameraY += ( tempCameraY - mCameraY ) / 8;
		//mCameraY += speedY;
	}
	*/
	prevMagnification = magnification;
}

void State::setMagnification( double mag ){
	magnification = mag;
}

bool State::checkTimer(){
	mTimer--;
	if( mTimer <= 0 ){
		return true;
	}
	return false;
}

void State::checkGameSet(){
	switch( mMode ){
	case MODE_TIME:
		if( checkTimer() ){
			mSequence = SEQ_END;
		}
		break;
	case MODE_LIVES:
		if( mDynamicObject->checkLives() ){
			mSequence = SEQ_END;
		}
		break;
	case MODE_TIME_LIVES:
		if( checkTimer() || mDynamicObject->checkLives() ){
			mSequence = SEQ_END;
		}
		break;
	}
}

void State::setStageHalfSize(){
	mStaticObject->getStageSize( &stagehalfsizeX, &stagehalfsizeX );
	stagehalfsizeX *= 0.5;
	stagehalfsizeY *= 0.5;
}

bool State::checkMouse( int x1, int x2, int y1, int y2 ){
	int mouseX, mouseY;
	static bool prevMouse = false;
	GetMousePoint( &mouseX, &mouseY );
	if( !prevMouse && ( GetMouseInput() & MOUSE_INPUT_LEFT ) != 0 && mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2 ){
		prevMouse = true;
		return true;
	}else{
		prevMouse = false;
	}
	return false;
}
