#include "Player.h"

void Player::setNextAction(){
	setJoyPad();
	bool padInfo[] = { false, checkMomentJoyPad( PAD_INPUT_1 ), checkMomentJoyPad( PAD_INPUT_2 ), 
		checkMomentJoyPad( PAD_INPUT_3 ), checkMomentJoyPad( PAD_INPUT_4 ) };
	mNextAction = ACT_STAND;
	if( padInfo[ 2 ] && abs( mMoveX ) == 2 && mInputTime <= 10 ){
		mNextAction = ACT_SIDE_SMASH_ATTACK;
	}else if( padInfo[ 2 ] && abs( mMoveX ) == 2 ){
		mNextAction = ACT_SIDE_HEAVY_ATTACK;
	}else if( padInfo[ 2 ] ){
		mNextAction = ACT_SIDE_WEAK_ATTACK;
	}else if( padInfo[ 3 ] ){
		mNextAction = ACT_NORMAL_SP_ATTACK;
	}
	mPrevJoyPad = mSetFlag;
	setInputTime();
}

void Player::setKey(){
	if( mPlayer == 1 ){
		mKey.KEY_LEFT = KEY_INPUT_LEFT;
		mKey.KEY_RIGHT = KEY_INPUT_RIGHT;
		mKey.KEY_UP = KEY_INPUT_UP;
		mKey.KEY_DOWN = KEY_INPUT_DOWN;
		mKey.KEY_JUMP = KEY_INPUT_SPACE;
		mKey.KEY_NORMAL_ATTACK = KEY_INPUT_Z;
		mKey.KEY_SPECIAL_ATTACK = KEY_INPUT_X;
	}else if( mPlayer == 2 ){
		mKey.KEY_LEFT = KEY_INPUT_A;
		mKey.KEY_RIGHT = KEY_INPUT_D;
		mKey.KEY_UP = KEY_INPUT_W;
		mKey.KEY_DOWN = KEY_INPUT_S;
		mKey.KEY_JUMP = KEY_INPUT_E;
		mKey.KEY_NORMAL_ATTACK = KEY_INPUT_R;
		mKey.KEY_SPECIAL_ATTACK = KEY_INPUT_Q;
	}else{
		mKey.KEY_LEFT = KEY_INPUT_A;
		mKey.KEY_RIGHT = KEY_INPUT_D;
		mKey.KEY_UP = KEY_INPUT_W;
		mKey.KEY_DOWN = KEY_INPUT_S;
		mKey.KEY_JUMP = KEY_INPUT_E;
		mKey.KEY_NORMAL_ATTACK = KEY_INPUT_R;
		mKey.KEY_SPECIAL_ATTACK = KEY_INPUT_Q;
	}
}

void Player::setJoyPad(){
	if( mPlayer > GetJoypadNum() ){
		if( mPlayer != 1 ){
			return;
		}
	}
	int checkJoyPad[ 4 ] = { DX_INPUT_KEY_PAD1, DX_INPUT_PAD2, DX_INPUT_PAD3, DX_INPUT_PAD4 };
	mJoyPad = 0;
	mJoyPad = GetJoypadInputState( checkJoyPad[ mPlayer - 1 ] );
	GetJoypadAnalogInput( &mJoyPadX, &mJoyPadY, checkJoyPad[ mPlayer - 1 ] );
	if( mJoyPadX > -100 && mJoyPadX < 100 ){
		mMoveX = 0;
	}else if( mJoyPadX >= 100 && mJoyPadX < 1000 ){
		mMoveX = 1;
	}else if( mJoyPadX <= -100 && mJoyPadX > -1000 ){
		mMoveX = -1;
	}else if( mJoyPadX == 1000 ){
		mMoveX = 2;
	}else if( mJoyPadX == -1000 ){
		mMoveX = -2;
	}
	int distanceY = mJoyPadY - mPrevJoyPadY;
	if( mJoyPadY > -100 && mJoyPadY < 100 ){
		mMoveY = 0;
	}else if( mJoyPadY >= 100 && mJoyPadY < 1000 ){
		mMoveY = -1;
	}
	//}else if( mJoyPadY <= -100 && mJoyPadY >= -1000 && distanceY > -500 ){
	else if( mJoyPadY == -1000 && checkInputJoyPad( PAD_INPUT_7 ) ){
		mMoveY = 1;
	}else if( mJoyPadY == 1000 ){
		mMoveY = -2;
	}else if( mJoyPadY < 0 && distanceY <= -500 && !checkInputJoyPad( PAD_INPUT_7 ) ){
		mMoveY = 2;
	}
	if( mT == 0 ){
		mPrevJoyPadX = mJoyPadX;
		mPrevJoyPadY = mJoyPadY;
		mT = 1;
	}else{
		mT = 0;
	}
}

bool Player::checkInputJoyPad( int input ){
	int searchInput = -1;
	for( int i = 4; i < 14; i++ ){
		if( input & ( 1 << i ) ){
			searchInput = ( i - 4 >= 0 ) ? i - 4 : 13 ;
			break;
		}
	}
	if( searchInput != -1 ){
		input = mJoyPadSetting[ mPlayer - 1 ][ searchInput ];
	}
	if( ( mJoyPad & input ) != 0 ){
		return true;
	}
	return false;
}

bool Player::checkMomentJoyPad( int input ){
	int searchInput = -1;
	int saveInput = input;
	for( int i = 4; i < 14; i++ ){
		if( input & ( 1 << i ) ){
			searchInput = ( i - 4 >= 0 ) ? i - 4 : 13 ;
			break;
		}
	}
	if( searchInput != -1 ){
		input = mJoyPadSetting[ mPlayer - 1 ][ searchInput ];
	}
	if( ( mPrevJoyPad & input ) == 0 && checkInputJoyPad( saveInput ) ){
		mSetFlag |= input;
		return true;
	}else if( checkInputJoyPad( saveInput ) ){
		mSetFlag |= input;
		return false;
	}
	mSetFlag &= ~input;
	return false;
}

void Player::setInputTime(){
	if( CheckHitKey( mKey.KEY_LEFT ) || CheckHitKey( mKey.KEY_RIGHT ) || abs( mMoveX ) == 2 ){
		mInputTime++;
	}else{
		mInputTime = 0;
	}
}

void Player::changeDefficult( int defficult ){
	return;
}

bool Player::isPlayer(){
	return true;
}