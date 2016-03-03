#include "Setting.h"

Setting::Setting( JoyPad* joypad ) : mCansel( false ), mSave( false ){
	mFile = new CSV( "setting/setting.csv" );
	const int* setting = mFile->getDataInt();
	mSetting = new int[ mFile->getSizeX() ];
	for( int i = 0; i < mFile->getSizeX(); i++ ){
		mSetting[ i ] = setting[ i ];
	}
	SetBackgroundColor( 255, 255, 255 );
	string str( "pic/cursol1.png" );
	mCursolHandle = LoadGraph( str.data() );
	mCursolX = 640 / 2 * 100;
	mCursolY = 480 / 2 * 100 + 480 / 4 * 100;
	mJoyPad = joypad;
}

Setting::~Setting(){
	SafeDelete( mFile );
	SafeDeleteArray( mSetting );
}

void Setting::update( SmashSequence* smashSequence ){
	moveCursol();
	drawString();
	drawCursols();
	for( int i = 0; i < 4; i++ ){
		if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_9 ) ){
			smashSequence->setSequence( SmashSequence::SEQ_CHARACTER_SELECT );
			return;
		}
		if( mJoyPad->checkMomentJoyPad( i, PAD_INPUT_10 ) ){
			outputFile();
			smashSequence->setSequence( SmashSequence::SEQ_CHARACTER_SELECT );
			return;
		}
	}
	if( mSave ){
		outputFile();
		smashSequence->setSequence( SmashSequence::SEQ_CHARACTER_SELECT );
		return;
	}else if( mCansel ){
		smashSequence->setSequence( SmashSequence::SEQ_CHARACTER_SELECT );
	}
}

void Setting::outputFile(){
	for( int x = 0; x < mFile->getSizeX(); x++ ){
		mFile->setData( mSetting[ x ], x, 0 );
	}
	mFile->outputFile();
}

void Setting::moveCursol(){
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
}

void Setting::drawCursols(){
	DrawGraph( mCursolX / 100, mCursolY / 100, mCursolHandle, true );
}

void Setting::drawStringCenter( const char* str, int y ){
	int getStrlen = GetDrawStringWidth( str, strlen( str ) ) ;
	DrawFormatString( 640 / 2 - getStrlen / 2, y, GetColor( 0, 0, 0 ), str );
}

void Setting::drawString(){
	char* str[] = { "モード", "時間", "残機", "吹っ飛び率" };
	char* mode[] = { "時間制", "残機制", "時間･残機制" };
	char* format[] = { "%s", "%d", "%d", "%d%%" };
	int rise[] = { 1, 30, 1, 25 };
	int min[] = { 0, 60, 1, 50 };
	int max[] = { 2, 60 * 99, 99, 900 };
	bool noEffect[][ 4 ] = { { 0, 0, 1, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 } };
	SetFontSize( 64 );
	drawStringCenter( "SETTING", POINT_TITLE_Y );
	SetFontSize( 32 );
	for( int i = 0; i < 5; i++ ){
		int getStrlen;
		int tx1 = 640 / 2 - SIZE_SETTING_DETAIL_X / 2;
		int ty1 = POINT_SETTING_DETAIL_Y + i * 48;
		int tx2 = tx1 + SIZE_SETTING_DETAIL_X / 2;
		int ty2 = ty1;
		int tx3 = tx1 + SIZE_SETTING_DETAIL_X - 5;
		int ty3 = ty1;
		if( i == 4 ){
			int getCanselStrlen = GetDrawFormatStringWidth( "キャンセル" );
			int getSpaceStrlen = GetDrawFormatStringWidth( "　" );
			int getSaveStrlen = GetDrawFormatStringWidth( "セーブ" );
			getStrlen = GetDrawFormatStringWidth( "キャンセル　セーブ" );
			tx1 = 640 / 2 - getStrlen / 2;
			DrawFormatString( tx1, ty1, GetColor( 0, 0, 0 ), "キャンセル　セーブ" );
			if( checkRange( mCursolX / 100, mCursolY / 100, tx1, ty1, tx1 + getCanselStrlen, ty1 + 32 ) ){
				for( int j = 0; j < 4; j++ ){
					if( mJoyPad->checkMomentJoyPad( j, PAD_INPUT_2 ) ){
						mCansel = true;
					}
				}
			}
			tx1 += getCanselStrlen + getSpaceStrlen;
			if( checkRange( mCursolX / 100, mCursolY / 100, tx1, ty1, tx1 + getSaveStrlen, ty1 + 32 ) ){
				for( int j = 0; j < 4; j++ ){
					if( mJoyPad->checkMomentJoyPad( j, PAD_INPUT_2 ) ){
						mSave = true;
					}
				}
			}
			continue;
		}
		int color = GetColor( 0, 0, 0 );
		if( noEffect[ mSetting[ 0 ] ][ i ] ){
			color = GetColor( 127, 127, 127 );
		}
		if( i == 0 ){
			getStrlen = GetDrawFormatStringWidth( format[ i ], mode[ mSetting[ i ] ] );
			DrawFormatString( tx2 + SIZE_SETTING_DETAIL_X / 4 - getStrlen / 2, ty2 - 5, color,  format[ i ], mode[ mSetting[ i ] ] );
		}else{
			getStrlen = GetDrawFormatStringWidth( format[ i ], mSetting[ i ] );
			DrawFormatString( tx2 + SIZE_SETTING_DETAIL_X / 4 - getStrlen / 2, ty2 - 5, color,  format[ i ], mSetting[ i ] );
		}
		DrawFormatString( tx1, ty1, color, str[ i ] );
		DrawTriangle( tx2, ty2, tx2, ty2 + 20, tx2 - 10, ty2 + 10, color, true );
		DrawTriangle( tx3, ty3, tx3, ty3 + 20, tx3 + 10, ty3 + 10, color, true );
		if( checkRange( mCursolX / 100, mCursolY / 100, tx2 - 10 - 10, ty2 - 10, tx2, ty2 + 20 + 10 ) ){
			DrawTriangle( tx2, ty2, tx2, ty2 + 20, tx2 - 10, ty2 + 10, GetColor( 255, 255, 0 ), true );
			for( int j = 0; j < 4; j++ ){
				if( mJoyPad->checkMomentJoyPad( j, PAD_INPUT_2 ) ){
					mSetting[ i ] -= rise[ i ];
				}
				if( mSetting[ i ] < min[ i ] ){
					mSetting[ i ] = max[ i ];
				}
			}
		}
		if( checkRange( mCursolX / 100, mCursolY / 100, tx3, ty3 - 10, tx3 + 10 + 10, ty3 + 20 + 10  ) ){
			DrawTriangle( tx3, ty3, tx3, ty3 + 20, tx3 + 10, ty3 + 10, GetColor( 255, 255, 0 ), true );
			for( int j = 0; j < 4; j++ ){
				if( mJoyPad->checkMomentJoyPad( j, PAD_INPUT_2 ) ){
					mSetting[ i ] += rise[ i ];
				}
				if( mSetting[ i ] > max[ i ] ){
					mSetting[ i ] = min[ i ];
				}
			}
		}
	}
	mJoyPad->setFlag();
}