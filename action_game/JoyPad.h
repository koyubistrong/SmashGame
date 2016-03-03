#ifndef INCLUDE_JOYPAD_H
#define INCLUDE_JOYPAD_H

#include "DxLib.h"
#include "CSV.h"
#include "extern.h"

class JoyPad{
public:
	JoyPad(){
		CSV* csv_joy = new CSV( "joypad/joypad.csv" );
		mJoyPadSetting = new int*[ csv_joy->getSizeY() ];
		const int* data = csv_joy->getDataInt();
		for( int y = 0; y < csv_joy->getSizeY(); y++ ){
			mJoyPadSetting[ y ] = new int[ csv_joy->getSizeX() ];
			for( int x = 0; x < csv_joy->getSizeX(); x++ ){
				mJoyPadSetting[ y ][ x ] = 1 << ( data[ x + y * csv_joy->getSizeX() ] + 3 );
			}
		}
		for( int i = 0; i < 4; i++ ){
			mMoveX[ i ] = 0;
			mMoveY[ i ] = 0;
			mJoyPad[ i ] = 0;
			mJoyPadX[ i ] = 0;
			mJoyPadY[ i ] = 0;
			mPrevJoyPadX[ i ] = 0;
			mPrevJoyPadY[ i ] = 0;
			mPrevJoyPad[ i ] = 0;
			mSetFlag[ i ] = 0;
		}
		delete csv_joy;
	}
	~JoyPad(){
		for( int y = 0; y < 4; y++ ){
			SafeDeleteArray( mJoyPadSetting[ y ] );
		}
		SafeDeleteArray( mJoyPadSetting );
	}
	bool setJoyPad( int padNum ){
	if( padNum + 1 > GetJoypadNum() ){
		if( padNum != 0 ){
			return false;
		}
	}
	mJoyPad[ padNum ] = 0;
	switch( padNum ){
	case 0:
		mJoyPad[ padNum ] = GetJoypadInputState( DX_INPUT_KEY_PAD1 );
		GetJoypadAnalogInput( &mJoyPadX[ padNum ], &mJoyPadY[ padNum ], DX_INPUT_KEY_PAD1 );
		break;
	case 1:
		mJoyPad[ padNum ] = GetJoypadInputState( DX_INPUT_PAD2 );
		GetJoypadAnalogInput( &mJoyPadX[ padNum ], &mJoyPadY[ padNum ], DX_INPUT_PAD2 );
		break;
	case 2:
		mJoyPad[ padNum ] = GetJoypadInputState( DX_INPUT_PAD3 );
		GetJoypadAnalogInput( &mJoyPadX[ padNum ], &mJoyPadY[ padNum ], DX_INPUT_PAD3 );
		break;
	case 3:
		mJoyPad[ padNum ] = GetJoypadInputState( DX_INPUT_PAD4 );
		GetJoypadAnalogInput( &mJoyPadX[ padNum ], &mJoyPadY[ padNum ], DX_INPUT_PAD4 );
		break;
	}
	if( mJoyPadX[ padNum ] > -100 && mJoyPadX[ padNum ] < 100 ){
		mMoveX[ padNum ] = 0;
	}else if( mJoyPadX[ padNum ] >= 100 && mJoyPadX[ padNum ] < 1000 ){
		mMoveX[ padNum ] = 1;
	}else if( mJoyPadX[ padNum ] <= -100 && mJoyPadX[ padNum ] > -1000 ){
		mMoveX[ padNum ] = -1;
	}else if( mJoyPadX[ padNum ] == 1000 ){
		mMoveX[ padNum ] = 2;
	}else if( mJoyPadX[ padNum ] == -1000 ){
		mMoveX[ padNum ] = -2;
	}
	if( mJoyPadY[ padNum ] > -100 && mJoyPadY[ padNum ] < 100 ){
		mMoveY[ padNum ] = 0;
	}else if( mJoyPadY[ padNum ] >= 100 && mJoyPadY[ padNum ] < 1000 ){
		mMoveY[ padNum ] = -1;
	}else if( mJoyPadY[ padNum ] <= -100 && mJoyPadY[ padNum ] > -1000 ){
		mMoveY[ padNum ] = 1;
	}else if( mJoyPadY[ padNum ] == 1000 ){
		mMoveY[ padNum ] = -2;
	}else if( mJoyPadX[ padNum ] == -1000 ){
		mMoveY[ padNum ] = 2;
	}
	return true;
}

	bool checkInputJoyPad( int padNum, int input ){
	int searchInput = -1;
	for( int i = 4; i < 14; i++ ){
		if( input & ( 1 << i ) ){
			searchInput = ( i - 4 >= 0 ) ? i - 4 : 13 ;
			break;
		}
	}
	if( searchInput != -1 ){
		input = mJoyPadSetting[ padNum ][ searchInput ];
	}
	if( ( mJoyPad[ padNum ] & input ) != 0 ){
		return true;
	}
	return false;
}

bool checkMomentJoyPad( int padNum, int input ){
	int searchInput = -1;
	int saveInput = input;
	for( int i = 4; i < 14; i++ ){
		if( input & ( 1 << i ) ){
			searchInput = ( i - 4 >= 0 ) ? i - 4 : 13 ;
			break;
		}
	}
	if( searchInput != -1 ){
		input = mJoyPadSetting[ padNum ][ searchInput ];
	}
	if( ( mPrevJoyPad[ padNum ] & input ) == 0 && checkInputJoyPad( padNum, saveInput ) ){
		mSetFlag[ padNum ] |= input;
		return true;
	}else if( checkInputJoyPad( padNum, saveInput ) ){
		mSetFlag[ padNum ] |= input;
		return false;
	}
	mSetFlag[ padNum ] &= ~input;
	return false;
}

void setFlag(){
	for( int i = 0; i < 4; i++ ){
		mPrevJoyPad[ i ] = mSetFlag[ i ];
	}
}

void maskFlag(){
	for( int i = 0; i < 4; i++ ){
		mPrevJoyPad[ i ] = 0xFFFFFFFF;
		mSetFlag[ i ] = 0xFFFFFFFF;
	}
}

	int mMoveX[ 4 ];
	int mMoveY[ 4 ];
private:
	int mJoyPad[ 4 ];
	int mJoyPadX[ 4 ];
	int mJoyPadY[ 4 ];
	int mPrevJoyPadX[ 4 ];
	int mPrevJoyPadY[ 4 ];
	int mPrevJoyPad[ 4 ];
	int mSetFlag[ 4 ];
	int** mJoyPadSetting;
};

#endif