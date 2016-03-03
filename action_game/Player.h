#ifndef INCLUDE_PLAYER_H
#define INCLUDE_PLAYER_H

#include "Movement.h"

class Player : public Movement{
public:
	Player( int characterID, int player ) : Movement( characterID, player ), mInputTime( 0 )
	, mJoyPad( 0 ), mJoyPadX( 0 ), mJoyPadY( 0 ), mPrevJoyPad( 0 ), mSetFlag( 0 ), mT( 0 ){
		CSV* csv_joy = new CSV( "joypad/joypad.csv" );
		mJoyPadSetting = new int*[ csv_joy->getSizeY() ];
		const int* data = csv_joy->getDataInt();
		for( int y = 0; y < csv_joy->getSizeY(); y++ ){
			mJoyPadSetting[ y ] = new int[ csv_joy->getSizeX() ];
			for( int x = 0; x < csv_joy->getSizeX(); x++ ){
				mJoyPadSetting[ y ][ x ] = 1 << ( data[ x + y * csv_joy->getSizeX() ] + 3 );
			}
			if( y == player - 1 ) mVibration = ( mJoyPadSetting[ y ][ csv_joy->getSizeX() - 1 ] >> 4 ) && 1;
		}
		string signName = "pic/player";
		signName += static_cast< char >( player + 48 );
		signName += ".png";
		mSignHandle = LoadGraph( signName.data() );
		setKey();
		delete csv_joy;
	}
	virtual ~Player(){
		for( int y = 0; y < 4; y++ ){
			SafeDeleteArray( mJoyPadSetting[ y ] );
		}
		SafeDeleteArray( mJoyPadSetting );
	}
	void setNextAction();
	void setKey();
	void setJoyPad();
	bool checkInputJoyPad( int input );
	bool checkMomentJoyPad( int input );
	void setInputTime();
	void changeDefficult( int defficult );
	bool isPlayer();

private:
	int mInputTime;
	int mJoyPad;
	int mJoyPadX;
	int mJoyPadY;
	int mT;
	int mPrevJoyPadX;
	int mPrevJoyPadY;
	int mPrevJoyPad;
	int mSetFlag;
	int** mJoyPadSetting;

};

#endif