#include "CharacterSelect.h"

CharacterSelectMini::CharacterSelectMini( JoyPad* joypad ) : CharacterSelect( joypad ){}

CharacterSelectMini::~CharacterSelectMini(){}

void CharacterSelectMini::update( MiniSequence* miniSequence ){
	setVisibleJoyPad();
	moveCursol();
	SetFontSize( 32 );
	const char* str = "CHARACTER SELECT";
	int getStrlen = GetDrawStringWidth( str, strlen( str ) );
	DrawFormatString( 640 / 2 - getStrlen / 2, POINT_TITLE_DISPLAY_Y, GetColor( 0, 0, 0 ), str );
	SetFontSize( 16 );
	drawSquares();
	drawCharacters();
	drawSquares2();
	drawNoJoyPad();
	drawCursols();
	drawStartAvailable( 1 );
	bool nextStart = checkNext( 1 );
	for( int i = 0; i < 4; i++ ){
		if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_9 ) ){
			miniSequence->setSequence( MiniSequence::SEQ_MENU );
			return;
		}
		//printfDx( "%d\n", mCharacterSelectID[ i ] );
		if( nextStart ){
			if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_10 ) ){
				deleteMask();
				Sound::Instance()->stopSound();
				Sound::Instance()->playSoundEffect( Sound::SOUND_EFFECT_DECIDE_3 );
				miniSequence->setCharacterID( mCharacterSelectID );
				miniSequence->setSequence( MiniSequence::SEQ_GAME );
				return;
			}
		}
	}
}
void CharacterSelectMini::drawSquares(){
	int pointX[] = { POINT_PLAYER1_DISPLAY_X_2, POINT_PLAYER2_DISPLAY_X_2 };
	int pointY[] = { POINT_PLAYER1_DISPLAY_Y_2, POINT_PLAYER2_DISPLAY_Y_2 };
	int color[] = { GetColor( 255, 50, 50 ), GetColor( 50, 50, 255 ), GetColor( 255, 212, 50 ), GetColor( 50, 255, 50 ), GetColor( 50, 50, 50 ) };
	int sizeX = SIZE_PLAYER_DISPLAY_X;
	int sizeY = SIZE_PLAYER_DISPLAY_Y;
	int stringSize = 64;
	int flame = 4;
	static int count = 0;
	bool computerSelect[] = { false, false, false, false };
	DrawBox( 0, 0, 640, 20, GetColor( 0, 0, 0 ), true );
	DrawBox( 0, 480 - 20, 640, 480, GetColor( 0, 0, 0 ), true );
	for( int i = 0; i < 2; i++ ){
		//DrawGraph( pointX[ i ], pointY[ i ], mFlameHandle, true );
		if( !( computerSelect[ i ] || mComputerSelect[ i ] ) ){
			int getStrlen = GetDrawFormatStringWidthToHandle( mFontSize64, "P%d", i + 1 );
			DrawBox( pointX[ i ], pointY[ i ], pointX[ i ] + sizeX, pointY[ i ] + sizeY, color[ i ], true );
			if( i + 1 <= GetJoypadNum() || i == 0 )
				DrawFormatStringToHandle( pointX[ i ] + sizeX / 2 - getStrlen / 2, pointY[ i ] + sizeY / 2 - stringSize / 2, GetColor( 255, 255, 255 ), mFontSize64, "P%d", i + 1 );
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

void CharacterSelectMini::drawCharacters(){
	int pointX[] = { POINT_PLAYER1_DISPLAY_X_2, POINT_PLAYER2_DISPLAY_X_2 };
	int pointY[] = { POINT_PLAYER1_DISPLAY_Y_2, POINT_PLAYER2_DISPLAY_Y_2 };
	for( int i = 0; i < 2; i++ ){
		mCharacterOnMouse[ i ] = false;
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
				for( int i = 0; i < 2; i++ ){
					if( mCharacterSelect[ i ] && mComputerFlag[ i ] == -1 ){
						continue;
					}
					if( checkRange( mCursolX[ i ] / 100, mCursolY[ i ] / 100, tx1, ty1, tx2, ty2 ) ){
						mCharacterOnMouse[ i ] = true;
						if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_2 ) ){
							mCharacterSelect[ i ] = true;
							mCharacterSelectID[ i ] = num;
							Sound::Instance()->playSoundEffect( Sound::SOUND_EFFECT_DECIDE_2 );
							continue;
						}
						DrawGraph( pointX[ i ], pointY[ i ], mCharacterBigHandle[ num ], true );
					}
				}
			}
		}
	}
	for( int i = 0; i < 2; i++ ){
		if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_3 ) ){
			mCharacterSelect[ i ] = false;
			mCharacterSelectID[ i ] = -1;
		}
		if( !mCharacterSelect[ i ] ){
			continue;
		}
		DrawGraph( pointX[ i ], pointY[ i ], mCharacterBigHandle[ mCharacterSelectID[ i ] ], true );
	}
}

void CharacterSelectMini::drawNoJoyPad(){
	int pointX[] = { POINT_PLAYER1_DISPLAY_X_2, POINT_PLAYER2_DISPLAY_X_2 };
	int pointY[] = { POINT_PLAYER1_DISPLAY_Y_2, POINT_PLAYER2_DISPLAY_Y_2 };
	for( int i = 0; i < 2; i++ ){
		if( mCursolVisible[ i ] || mCharacterOnMouse[ i ] ||( !mCursolVisible[ i ] && ( mComputerSelect[ i ] || mComputerFlag[ i ] != -1 ) ) ){
			continue;
		}
		DrawGraph( pointX[ i ], pointY[ i ], mNoJoyPadHandle, true );
	}
}

void CharacterSelectMini::setVisibleJoyPad(){
	mCursolVisible[ 0 ] = true;
	for( int i = 1; i < 2; i++ ){
		if( i < GetJoypadNum() ){
			mCursolVisible[ i ] = true;
		}else{
			mCursolVisible[ i ] = false;
		}
	}
}