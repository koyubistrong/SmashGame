#include "Weapon.h"

void Weapon::update(){
	//mSpeedX = mSpeedX;
	if( mWepTime == mWait ){
		mCharacterX = *mUsedCharacterX;
		mCharacterY = *mUsedCharacterY;
		mFarstCharacterX = *mUsedCharacterX;
		mFarstCharacterY = *mUsedCharacterY;
		mFactCharacterX = *mUsedCharacterX * 100;
		mFactCharacterY = *mUsedCharacterY * 100;
	}
	if( mWepTime >= mWait && mWepTime < mEnd ){
		intervalTime( mAction, false );
		mVisible = true;
	}else if( mWepTime >= mEnd ){
		resetWeapon();
	}
	soundWeapon();
	mWepTime++;
}

void Weapon::drawWeapon( int x, int y ){
	if( mWepTime >= mWait && mWepTime < mEnd ){
		int flame = getTime( mAction );
		int ty = y + mActionInfo[ mAction ].correctionY * magnification;
		if( mDirectionRight ){
			int tx = x + mActionInfo[ mAction ].correctionX * magnification;
			mWeaponImages->drawImageRota( flame, tx, ty, mDegree );
		}else{
			int tx = x + -mActionInfo[ mAction ].correctionX * magnification;
			mWeaponImages->drawImageRotaLR( flame, tx, ty, mDegree );
		}
	}
}

void Weapon::soundWeapon(){
	if( mWepTime == mWait ){
		PlaySoundMem( mSound[ mAction ], DX_PLAYTYPE_BACK, true );
	}
}

void Weapon::setStart(){
	mStart = true;
}

void Weapon::setWeapon( bool directionRight ){
	mStart = true;
	mWepTime = 0;
	mSpeedX = mActionInfo[ 0 ].speedX;
	mSpeedY = mActionInfo[ 0 ].speedY;
	mDirectionRight = directionRight;
	if( !mDirectionRight ){
		mSpeedX *= -1;
	}
	for( int i = 0; i < 256; i++ ){
		mHitHantei[ i ] = true;
	}
}

void Weapon::setUsedCharacter( int* usedCharacterX, int* usedCharacterY ){
	mUsedCharacterX = usedCharacterX;
	mUsedCharacterY = usedCharacterY;
}

void Weapon::resetWeapon(){
	mWepTime = 0;
	mStart = false;
	mVisible = false;
	mCharacterX = 0;
	mCharacterY = 0;
	mFarstCharacterX = 0;
	mFarstCharacterY = 0;
	mFactCharacterX = 0;
	mFactCharacterY = 0;
}