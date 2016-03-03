#include "CharacterSelect.h"

CharacterSelect::CharacterSelect( JoyPad* joypad ){
	CSV* chr = new CSV( "character/characters.csv" );
	mCharacterName = new string[ chr->getSizeY() ];
	for( int i = 0; i < chr->getSizeY(); i++ ){
		mCharacterName[ i ] = chr->getDataString()[ i ];
	}
	mCharacterNum = chr->getSizeY();
	mCharacterSmallHandle = new int[ mCharacterNum ];
	mCharacterBigHandle = new int[ mCharacterNum ];
	mFontSize24 = CreateFontToHandle( NULL, 24, -1, DX_FONTTYPE_ANTIALIASING_4X4 );
	mFontSize32 = CreateFontToHandle( NULL, 32, -1, DX_FONTTYPE_ANTIALIASING_4X4 );
	mFontSize64 = CreateFontToHandle( NULL, 64, -1, DX_FONTTYPE_ANTIALIASING_4X4 );
	for( int i = 0; i < mCharacterNum; i++ ){
		string str( "character/" );
		str += mCharacterName[ i ];
		string bigStr = str + "/pic/big.png";
		string smallStr = str + "/pic/small.png";
		mCharacterBigHandle[ i ] = LoadGraph( bigStr.data() );
		mCharacterSmallHandle[ i ] = LoadGraph( smallStr.data() );
	}
	SetBackgroundColor( 127, 127, 127 );
	setVisibleJoyPad();
	for( int i = 0; i < 4; i++ ){
		string str( "pic/cursol" );
		string str2;
		str += static_cast< char >( 49 + i );
		str2 = str + "_com.png";
		str += ".png";
		mCursolHandle[ i ] = LoadGraph( str.data() );
		mCursolComputerHandle[ i ] = LoadGraph( str2.data() );
		mCursolX[ i ] = ( 190 + 65 * i ) * 100;
		mCursolY[ i ] = ( 480 - 80 ) * 100;
		mCharacterSelect[ i ] = false;
		mCharacterSelectID[ i ] = -1;
		mComputerSelect[ i ] = false;
		mComputerFlag[ i ] = -1;
		mCharacterOnMouse[ i ] = false;
		mComputerDefficult[ i ] = 5;
	}
	mJoyPad = joypad;
	mFlameHandle = LoadGraph( "pic/flame.png" );
	mNoJoyPadHandle = LoadGraph( "pic/no_joypad.png" );
	mMaskStartAvailableHandle = 0;
	mOptionSelected = false;
	SetDrawValidAlphaChannelGraphCreateFlag( TRUE );
	mTriangleHandle[ 0 ] = MakeScreen( 10, 20, true );
	mTriangleHandle[ 1 ] = MakeScreen( 10, 20, true );
	SetDrawScreen( mTriangleHandle[ 0 ] );
	DrawTriangle( 0, 10, 10, 20, 10, 0, GetColor( 255, 255, 255 ), true );
	SetDrawScreen( mTriangleHandle[ 1 ] );
	DrawTriangle( 0, 0, 0, 20, 10, 10, GetColor( 255, 255, 255 ), true );
	SetDrawScreen( DX_SCREEN_BACK );
	SetDrawValidAlphaChannelGraphCreateFlag( FALSE );
	delete chr;
}

CharacterSelect::~CharacterSelect(){
	for( int i = 0; i < 4; i++ ){
		DeleteGraph( mCursolHandle[ i ] );
		DeleteGraph( mCursolComputerHandle[ i ] );
	}
	for( int i = 0; i < mCharacterNum; i++ ){
		DeleteGraph( mCharacterBigHandle[ i ] );
		DeleteGraph( mCharacterSmallHandle[ i ] );
	}
	SafeDeleteArray( mCharacterName );
	SafeDeleteArray( mCharacterSmallHandle );
	SafeDeleteArray( mCharacterBigHandle );
	DeleteGraph( mTriangleHandle[ 0 ] );
	DeleteGraph( mTriangleHandle[ 1 ] );
	DeleteGraph( mFlameHandle );
	DeleteGraph( mNoJoyPadHandle );
	DeleteFontToHandle( mFontSize24 );
	DeleteFontToHandle( mFontSize32 );
	DeleteFontToHandle( mFontSize64 );
	DeleteMask( mMaskStartAvailableHandle );
	DeleteMaskScreen();
}

void CharacterSelect::update( SmashSequence* smashSequence ){
	setVisibleJoyPad();
	moveCursol();
	setComputerFlag();
	drawSquares();
	drawCharacters();
	drawSquares2();
	drawNoJoyPad();
	drawString();
	changeDefficult();
	drawCursols();
	drawStartAvailable( 2 );
	bool nextStart = checkNext( 2 );
	for( int i = 0; i < 4; i++ ){
		if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_9 ) ){
			smashSequence->setSequence( SmashSequence::SEQ_MENU );
			return;
		}
		//printfDx( "%d %d\n", mCharacterSelectID[ i ], mComputerSelect[ i ] );
		if( nextStart ){
			if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_10 ) ){
				smashSequence->setCharacterID( mCharacterSelectID );
				smashSequence->setComputerFlag( mComputerSelect );
				smashSequence->setComputerDefficult( mComputerDefficult );
				smashSequence->setSequence( SmashSequence::SEQ_STAGE_SELECT );
				deleteMask();
				return;
			}
		}
	}
	if( mOptionSelected ){
		smashSequence->setSequence( SmashSequence::SEQ_SETTING );
		mOptionSelected = false;
		deleteMask();
		return;
	}
}

bool CharacterSelect::checkNext( int num ){
	int selectNum = 0;
	for( int s = 0; s < 4; s++ ){
		if( mCharacterSelect[ s ] ){
			selectNum++;
		}
	}
	return ( selectNum >= num ) ? true : false;
}

void CharacterSelect::drawString(){
	const char* str = "OPTION";
	const char* str2 = "CHARACTER SELECT";
	int getStrlen = GetDrawFormatStringWidthToHandle( mFontSize24, str, strlen( str ) );
	int getStrlen2 = GetDrawFormatStringWidthToHandle( mFontSize32, str2, strlen( str2 ) );
	DrawFormatStringToHandle( 640 / 2 - getStrlen / 2, POINT_OPTION_DISPLAY_Y, GetColor( 0, 0, 0 ), mFontSize24, str );
	DrawFormatStringToHandle( 640 / 2 - getStrlen2 / 2, POINT_TITLE_DISPLAY_Y, GetColor( 0, 0, 0 ), mFontSize32, str2 );
	for( int i = 0; i < 4; i++ ){
		int tx1 = 640 / 2 - getStrlen / 2;
		int ty1 = POINT_OPTION_DISPLAY_Y;
		int tx2 = 640 / 2 + getStrlen / 2;
		int ty2 = POINT_OPTION_DISPLAY_Y + 16 - 1;
		if( checkRange( mCursolX[ i ] / 100, mCursolY[ i ] / 100, tx1, ty1, tx2, ty2 ) ){
			if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_2 ) ){
				mOptionSelected = true;
				break;
			}
		}
	}
}

void CharacterSelect::moveCursol(){
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad->setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad->checkInputJoyPad( i, PAD_INPUT_UP ) ){
			mCursolY[ i ] -= 450;
		}
		if( mJoyPad->checkInputJoyPad( i, PAD_INPUT_DOWN ) ){
			mCursolY[ i ] += 450;
		}
		if( mJoyPad->checkInputJoyPad( i, PAD_INPUT_RIGHT ) ){
			mCursolX[ i ] += 450;
		}
		if( mJoyPad->checkInputJoyPad( i, PAD_INPUT_LEFT ) ){
			mCursolX[ i ] -= 450;
		}
		if( mCursolX[ i ] / 100 < -8 ){
			mCursolX[ i ] = -8 * 100;
		}
		if( mCursolY[ i ] / 100 < -8 ){
			mCursolY[ i ] = -8 * 100;
		}
		if( mCursolX[ i ] / 100 >= 640 - 16 ){
			mCursolX[ i ] = ( 640 - 16 ) * 100;
		}
		if( mCursolY[ i ] / 100 >= 480 - 16 ){
			mCursolY[ i ] = ( 480 - 16 ) * 100;
		}
	}
	mJoyPad->setFlag();
}

void CharacterSelect::setComputerFlag(){
	for( int i = 0; i < 4; i++ ){
		for( int j = 0; j < 4; j++ ){
			mComputerFlag[ i ] = -1;
			if( mJoyPad->checkInputJoyPad( i, ( PAD_INPUT_5 << j ) ) ){
				mComputerFlag[ i ] = j;
				break;
			}
		}
	}
}

void CharacterSelect::drawSquares(){
	int pointX[] = { POINT_PLAYER1_DISPLAY_X, POINT_PLAYER2_DISPLAY_X, POINT_PLAYER3_DISPLAY_X, POINT_PLAYER4_DISPLAY_X };
	int pointY[] = { POINT_PLAYER1_DISPLAY_Y, POINT_PLAYER2_DISPLAY_Y, POINT_PLAYER3_DISPLAY_Y, POINT_PLAYER4_DISPLAY_Y };
	int color[] = { GetColor( 255, 50, 50 ), GetColor( 50, 50, 255 ), GetColor( 255, 212, 50 ), GetColor( 50, 255, 50 ), GetColor( 50, 50, 50 ) };
	int sizeX = SIZE_PLAYER_DISPLAY_X;
	int sizeY = SIZE_PLAYER_DISPLAY_Y;
	int stringSize = 64;
	int flame = 4;
	static int count = 0;
	bool computerSelect[] = { false, false, false, false };
	for( int i = 0; i < 4; i++ ){
		if( mComputerFlag[ i ] != -1 ){
			computerSelect[ mComputerFlag[ i ] ] = true;
		}
	}
	DrawBox( 0, 0, 640, 20, GetColor( 0, 0, 0 ), true );
	DrawBox( 0, 480 - 20, 640, 480, GetColor( 0, 0, 0 ), true );
	for( int i = 0; i < 4; i++ ){
		//DrawGraph( pointX[ i ], pointY[ i ], mFlameHandle, true );
		if( ( !computerSelect[ i ] && !mComputerSelect[ i ] ) || ( mCharacterSelect[ i ] && !mComputerSelect[ i ] ) ){
			int getStrlen = GetDrawFormatStringWidthToHandle( mFontSize64, "P%d", i + 1 );
			DrawBox( pointX[ i ], pointY[ i ], pointX[ i ] + sizeX, pointY[ i ] + sizeY, color[ i ], true );
			if( i + 1 <= GetJoypadNum() || i == 0 )
				DrawFormatStringToHandle( pointX[ i ] + sizeX / 2 - getStrlen / 2, pointY[ i ] + sizeY / 2 - stringSize / 2, GetColor( 255, 255, 255 ), mFontSize64, "P%d", i + 1 );
		}else{
			int getStrlen = GetDrawFormatStringWidthToHandle( mFontSize64, "COM%d", i + 1 );
			DrawBox( pointX[ i ], pointY[ i ], pointX[ i ] + sizeX, pointY[ i ] + sizeY, color[ 4 ], true );
			DrawFormatStringToHandle( pointX[ i ] + sizeX / 2 - getStrlen / 2, pointY[ i ] + sizeY / 2 - stringSize / 2, GetColor( 255, 255, 255 ), mFontSize64, "COM%d", i + 1 );
		}
		if( mCharacterSelect[ i ] || mComputerSelect[ i ] ){
			if( count % 127 >= 64 ){
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, 127 - count % 64 );
			}else{
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, 64 + count % 64 );
			}
			DrawBox( pointX[ i ], pointY[ i ], pointX[ i ] + sizeX, pointY[ i ] + sizeY, GetColor( 255, 255, 255 ), true );
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
		DrawGraph( pointX[ i ], pointY[ i ], mFlameHandle, true );
		//DrawBox( pointX[ i ] + flame, pointY[ i ] + flame, pointX[ i ] + sizeX - flame, pointY[ i ] + sizeY - flame, GetColor( 255, 255, 255 ), true );
	}
	count += 2;
	if( count >= 127 ){
		count = 0;
	}
}

void CharacterSelect::drawSquares2(){
	int tx = POINT_CHARACTER_DISPLAY_X;
	int ty = POINT_CHARACTER_DISPLAY_Y;
	int sx = SIZE_CHARACTER_DISPLAY_X;
	int sy = SIZE_CHARACTER_DISPLAY_Y;
	DrawBox( tx - 3, ty - 3, tx + sx * NUM_CHARACTER_X + 3, ty + sy * NUM_CHARACTER_Y + 3, GetColor( 0, 0, 0 ), false );
	for( int y = 0; y < NUM_CHARACTER_Y; y++ ){
		for( int x = 0; x < NUM_CHARACTER_X; x++ ){
			int tx1 = tx + sx * x;
			int ty1 = ty + sy * y;
			int tx2 = tx + sx * ( x + 1 );
			int ty2 = ty + sy * ( y + 1 );
			for( int i = 0; i < 3; i++ ){
				DrawBox( tx1 - i, ty1 - i, tx2 + i, ty2 + i, GetColor( 0, 0, 0 ), false );
			}
		}
	}
}

void CharacterSelect::drawCursols(){
	for( int i = 0; i < 4; i++ ){
		if( mCursolVisible[ i ] ){
			if( mComputerFlag[ i ] == -1 ){ 
				DrawGraph( mCursolX[ i ] / 100, mCursolY[ i ] / 100, mCursolHandle[ i ], true );
			}else{
				DrawGraph( mCursolX[ i ] / 100, mCursolY[ i ] / 100, mCursolComputerHandle[ mComputerFlag[ i ]  ], true );
			}
		}
	}
}

void CharacterSelect::drawCharacters(){
	int pointX[] = { POINT_PLAYER1_DISPLAY_X, POINT_PLAYER2_DISPLAY_X, POINT_PLAYER3_DISPLAY_X, POINT_PLAYER4_DISPLAY_X };
	int pointY[] = { POINT_PLAYER1_DISPLAY_Y, POINT_PLAYER2_DISPLAY_Y, POINT_PLAYER3_DISPLAY_Y, POINT_PLAYER4_DISPLAY_Y };
	bool throughFlag[] = { false, false, false, false };
	for( int i = 0; i < 4; i++ ){
		mCharacterOnMouse[ i ] = false;
		if( mComputerFlag[ i ] != -1 ){
			throughFlag[ mComputerFlag[ i ] ] = true;
		}
	}
	for( int y = 0; y < NUM_CHARACTER_Y; y++ ){
		for( int x = 0; x < NUM_CHARACTER_X; x++ ){
			int num = x + y * NUM_CHARACTER_X;
			if( num < mCharacterNum ){
				int tx1 = POINT_CHARACTER_DISPLAY_X + SIZE_CHARACTER_DISPLAY_X * x;
				int ty1 = POINT_CHARACTER_DISPLAY_Y + SIZE_CHARACTER_DISPLAY_Y * y;
				int tx2 = POINT_CHARACTER_DISPLAY_X + SIZE_CHARACTER_DISPLAY_X * ( x + 1 ) - 1;
				int ty2 = POINT_CHARACTER_DISPLAY_Y + SIZE_CHARACTER_DISPLAY_Y * ( y + 1 ) - 1;
				DrawGraph( tx1, ty1, mCharacterSmallHandle[ num ], false );
				for( int i = 0; i < 4; i++ ){
					if( mCharacterSelect[ i ] ){
						if( mComputerFlag[ i ] == -1 || mComputerFlag[ i ] == i ) continue;
					}
					if( checkRange( mCursolX[ i ] / 100, mCursolY[ i ] / 100, tx1, ty1, tx2, ty2 ) ){
						int c = i;
						bool flag = false;
						if( mComputerFlag[ i ] != -1 ){ 
							c = mComputerFlag[ i ];
							if( mCharacterSelect[ c ] && mComputerFlag[ c ] ) continue;
							flag = true;
						}
						mCharacterOnMouse[ c ] = true;
						if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_2 ) ){
							mCharacterSelect[ c ] = true;
							mCharacterSelectID[ c ] = num;
							mComputerSelect[ c ] = flag;
							Sound::Instance()->playSoundEffect( Sound::SOUND_EFFECT_DECIDE_2 );
							continue;
						}
						DrawGraph( pointX[ c ], pointY[ c ], mCharacterBigHandle[ num ], true );
					}
				}
			}
		}
	}
	for( int i = 0; i < 4; i++ ){
		if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_3 ) ){
			int c = i;
			if( mComputerFlag[ i ] != -1 ){ 
				c = mComputerFlag[ i ];
			}
			mCharacterSelect[ c ] = false;
			mCharacterSelectID[ c ] = -1;
			mComputerSelect[ c ] = false;
		}
		if( !mCharacterSelect[ i ] ){
			continue;
		}
		DrawGraph( pointX[ i ], pointY[ i ], mCharacterBigHandle[ mCharacterSelectID[ i ] ], true );
	}
}

void CharacterSelect::drawNoJoyPad(){
	int pointX[] = { POINT_PLAYER1_DISPLAY_X, POINT_PLAYER2_DISPLAY_X, POINT_PLAYER3_DISPLAY_X, POINT_PLAYER4_DISPLAY_X };
	int pointY[] = { POINT_PLAYER1_DISPLAY_Y, POINT_PLAYER2_DISPLAY_Y, POINT_PLAYER3_DISPLAY_Y, POINT_PLAYER4_DISPLAY_Y };
	bool computerSelect[] = { false, false, false, false };
	for( int i = 0; i < 4; i++ ){
		if( mComputerFlag[ i ] != -1 ){
			computerSelect[ mComputerFlag[ i ] ] = true;
		}
	}
	for( int i = 0; i < 4; i++ ){
		if( computerSelect[ i ] || mCursolVisible[ i ] || mCharacterOnMouse[ i ] ||( !mCursolVisible[ i ] && ( mComputerSelect[ i ] || mComputerFlag[ i ] != -1 ) ) ){
			continue;
		}
		DrawGraph( pointX[ i ], pointY[ i ], mNoJoyPadHandle, true );
	}
}

void CharacterSelect::drawStartAvailable( int i ){
	static int count = 0;
	if( !checkNext( i ) ){
		count = 0;
		if( mMaskStartAvailableHandle != 0 ){
			deleteMask();
		}
		return;
	}
	if( mMaskStartAvailableHandle == 0 ){
		createMask();
	}
	DrawMask( count % ( 640 + 320 ) - 320, 480 / 2 - 26 / 2, mMaskStartAvailableHandle, DX_MASKTRANS_NONE );
	//DrawMask( 640 / 2 - 317 / 2, 480 / 2 - 26 / 2, mMaskStartAvailableHandle, DX_MASKTRANS_NONE );
	//DrawBox( 0, 0, 640, 480, GetColor( 0, 0, 0 ), true );
	if( count % 127 >= 64 ){
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 127 - ( count % 127 ) % 64 );
	}else{
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 64 + ( count % 127 ) % 64 );
	}
	DrawBox( 0, 480 / 2 - 20, 640, 480 / 2 + 20, GetColor( 255, 255, 255 ), true );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	count++;
}

void CharacterSelect::changeDefficult(){
	int pointX[] = { POINT_PLAYER1_DISPLAY_X, POINT_PLAYER2_DISPLAY_X, POINT_PLAYER3_DISPLAY_X, POINT_PLAYER4_DISPLAY_X };
	int pointY[] = { POINT_PLAYER1_DISPLAY_Y, POINT_PLAYER2_DISPLAY_Y, POINT_PLAYER3_DISPLAY_Y, POINT_PLAYER4_DISPLAY_Y };
	bool cursolRangeFlagBox[] = { false, false, false, false };
	int sizeX = SIZE_PLAYER_DISPLAY_X;
	int sizeY = SIZE_PLAYER_DISPLAY_Y;
	int stringSize = 64;
	int flame = 4;
	for( int i = 0; i < 4; i++ ){
		for( int j = 0; j < 4; j++ ){
			int tx1 = pointX[ j ];
			int ty1 = pointY[ j ];
			int tx2 = tx1 + sizeX;
			int ty2 = ty1 + sizeY;
			if( checkRange( mCursolX[ i ] / 100, mCursolY[ i ] / 100, tx1, ty1, tx2, ty2 ) ){
				cursolRangeFlagBox[ j ] = true;
			}
			int trgX1 = pointX[ j ] + sizeX / 8;
			int trgY1 = pointY[ j ] + sizeY / 2 - 12 + 30;
			int trgX2 = pointX[ j ] + sizeX * 7 / 8 - 10;
			int trgY2 = pointY[ j ] + sizeY / 2 - 12 + 30;
			if( checkRange( mCursolX[ i ] / 100, mCursolY[ i ] / 100, trgX1 - 20, trgY1 - 20, trgX1 + 20, trgY1 + 20 ) ){
				if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_2 ) ){
					mComputerDefficult[ j ]--;
				}
			}
			if( checkRange( mCursolX[ i ] / 100, mCursolY[ i ] / 100, trgX2 - 20, trgY2 - 20, trgX2 + 20, trgY2 + 20 ) ){
				if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_2 ) ){
					mComputerDefficult[ j ]++;
				}
			}
			if( mComputerDefficult[ j ] < 1 ){
				mComputerDefficult[ j ] = 9;
			}else if( mComputerDefficult[ j ] > 9 ){
				mComputerDefficult[ j ] = 1;
			}
		}
	}
	DrawBox( 0, 0, 640, 20, GetColor( 0, 0, 0 ), true );
	DrawBox( 0, 480 - 20, 640, 480, GetColor( 0, 0, 0 ), true );
	for( int i = 0; i < 4; i++ ){
		int tx1 = pointX[ i ];
		int ty1 = pointY[ i ];
		int tx2 = tx1 + sizeX;
		int ty2 = ty1 + sizeY;
		if( cursolRangeFlagBox[ i ] && mCharacterSelect[ i ] && mComputerSelect[ i ] ){
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, 200 );
			DrawBox( tx1, ty1, tx2, ty2, GetColor( 0, 0, 0 ), true );
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
			int trgX1 = pointX[ i ] + sizeX / 8;
			int trgY1 = pointY[ i ] + sizeY / 2 - 12 + 30;
			int trgX2 = pointX[ i ] + sizeX * 7 / 8 - 10;
			int trgY2 = pointY[ i ] + sizeY / 2 - 12 + 30;
			DrawGraph( trgX1, trgY1, mTriangleHandle[ 0 ], true );
			DrawGraph( trgX2, trgY2, mTriangleHandle[ 1 ], true );
			int getStrlen1 = GetDrawFormatStringWidthToHandle( mFontSize24, "Defficult" );
			int getStrlen2 = GetDrawFormatStringWidthToHandle( mFontSize24, "%d", mComputerDefficult[ i ] );
			DrawFormatStringToHandle( pointX[ i ] + sizeX / 2 - getStrlen1 / 2, pointY[ i ] + sizeY / 2 - 12 - 30, GetColor( 255, 255, 255 ), mFontSize24, "Defficult" );
			DrawFormatStringToHandle( pointX[ i ] + sizeX / 2 - getStrlen2 / 2, pointY[ i ] + sizeY / 2 - 16 + 30, GetColor( 255, 255, 255 ), mFontSize32, "%d", mComputerDefficult[ i ] );
			//printfDx( "CursolX %3d CursolY %3d\n", mCursolX[ i ] / 100, mCursolY[ i ] / 100 );
			//printfDx( "tx1 %3d ty1 %3d tx2 %3d ty2 %3d", tx1, ty1, tx2, ty2 );
		}
	}
}

void CharacterSelect::setVisibleJoyPad(){
	mCursolVisible[ 0 ] = true;
	for( int i = 1; i < 4; i++ ){
		if( i < GetJoypadNum() ){
			mCursolVisible[ i ] = true;
		}else{
			mCursolVisible[ i ] = false;
		}
	}
}

void CharacterSelect::createMask(){
	CreateMaskScreen();
	DeleteMask( mMaskStartAvailableHandle );
	mMaskStartAvailableHandle = LoadMask( "pic/start_available.png" );
}

void CharacterSelect::deleteMask(){
	CreateMaskScreen();
	DeleteMask( mMaskStartAvailableHandle );
	mMaskStartAvailableHandle = 0;
	DeleteMaskScreen();
}