#include "Movement.h"

void Movement::update(){
	bool mask = mMask;
	setNextAction();
	mRota = false;
	if( mBlowPlayer >= 0 && mJumpNum == 0 && mDamaged == 0 ){
		mBlowPlayer = -1;
	}
	if( mDamaged == 0 && mPrevDamaged ){
		mDegree = 0;
		for( int i = 0; i < 256; i++ ){
			mHitHantei[ i ] = true;
		}
		mDirectionRight = !mDirectionRight;
		mPrevDamaged = false;
	}
	if( mDamaged >= 2 ){
		setTime( mAction, 0 );
		setAction( ACT_DAMAGED_LEVEL2 );
		mMask = true;
		mask = true;
		mPrevSound = false;
		mPrevDamaged = true;
		mContinueAction = 0;
		if( mDamaged == 3 ){
			mRota = true;
			mDegree += 20.0;
		}
	}else if( mDamaged == 1 ){
		setDamaged( 0 );
		setTime( mAction, 0 );
		setAction( ACT_DAMAGED_LEVEL1 );
		mMask = true;
		mask = true;
		mPrevSound = false;
		mPrevDamaged = true;
		mContinueAction = 0;
	}else if( mAction == ACT_SIDE_WEAK_ATTACK && mNextAction == ACT_SIDE_WEAK_ATTACK ){
		mContinueAction = 1;
	}else if( mAction == ACT_SIDE_WEAK_ATTACK_2 && mNextAction == ACT_SIDE_WEAK_ATTACK ){
		mContinueAction = 2;
	}
	//printfDx( "%4d %4d %d %d\n", mJoyPadX, mJoyPadY, mMoveX, mMoveY );
	if( !mMask ){
		if( mMoveY == -2 && ( mAction == ACT_SQUAT_1 || mAction == ACT_SQUAT_2 ) ){
			setAction( ACT_SQUAT_2 );
			mask = true;
		}else if( mMoveY != -2 && ( mAction == ACT_SQUAT_1 || mAction == ACT_SQUAT_2 ) ){
			setAction( ACT_SQUAT_3 );
			mask = true;
		}else if( mMoveX == 2 || mMoveX == 1 ){
			mDirectionRight = true;
			setAction( ACT_WALK );
		}else if( mMoveX == -2 || mMoveX == -1 ){
			mDirectionRight = false;
			setAction( ACT_WALK );
		}
		if( !mask ){
			if( mMoveY == -2 && mJumpNum == 0 && mAction != ACT_SQUAT_2 ){
				//setAction( ACT_SQUAT_1 );
			}else if( mNextAction == ACT_SIDE_SMASH_ATTACK ){
				setAction( ACT_SIDE_SMASH_ATTACK );
				mask = true;
			}else if( mNextAction == ACT_SIDE_HEAVY_ATTACK ){
				setAction( ACT_SIDE_HEAVY_ATTACK );
				mask = true;
			}else if( mContinueAction == 1 ){
				setAction( ACT_SIDE_WEAK_ATTACK_2 );
				mContinueAction = 0;
				mask = true;
			}else if( mContinueAction == 2 ){
				setAction( ACT_SIDE_WEAK_ATTACK_3 );
				mContinueAction = 0;
				mask = true;
			}else if( mNextAction == ACT_SIDE_WEAK_ATTACK ){
				setAction( ACT_SIDE_WEAK_ATTACK );
				mask = true;
			}else if( mNextAction == ACT_NORMAL_SP_ATTACK ){
				setAction( ACT_NORMAL_SP_ATTACK );
				mask = true;
			}else if( mSpeedY < 0 && mJumpNum > 0 ){
				setAction( ACT_JUMP );
			}else if( mSpeedY >= 0 && mJumpNum > 0 ){
				setAction( ACT_FALL );
			}else if( mMoveX == 0 ){
				setAction( ACT_STAND );
			}
		}
		if( mActionInfo[ mAction ].weaponNum > 0 ){
			int i = 0;
			while( i < 5 && mWeapons[ mActionInfo[ mAction ].weaponNum - 1 ][ i ]->mStart ){ i++; }
			if( i < 5 ){
				mWeapons[ mActionInfo[ mAction ].weaponNum - 1 ][ i ]->setWeapon( mDirectionRight );
			}
		}
	}
	soundCharacter();
	intervalTime( mAction, mask );
}

void Movement::drawCharacter( int x, int y ){
	int flame = getTime( mAction );
	int ty = y + mActionInfo[ mAction ].correctionY * magnification;
	int sizeX, sizeY;
	int tySign = y + mActionInfo[ ACT_STAND ].correctionY * magnification;
	int txSign = x + mActionInfo[ ACT_STAND ].correctionX * magnification;
	mHanteis[ ACT_STAND ]->getCellSize( &sizeX, &sizeY );
	DrawRotaGraph( txSign, tySign - ( sizeY + 25 ) * 0.5 * magnification, magnification + 0.0538, 0, mSignHandle, true, false );
	if( !mImages[ mAction ]->getSuccess() ){
		return;
	}
	if( mDirectionRight ){
		int tx = x + mActionInfo[ mAction ].correctionX * magnification;
		if( !mRota ){
			mImages[ mAction ]->drawImage2( flame, tx, ty );
		}else{
			mImages[ mAction ]->drawImageRota( flame, tx, ty, mDegree );
		}
	}else{
		int tx = x + -mActionInfo[ mAction ].correctionX * magnification;
		if( !mRota ){
			mImages[ mAction ]->drawImageLR( flame, tx, ty );
		}else{
			mImages[ mAction ]->drawImageRotaLR( flame, tx, ty, mDegree );
		}
	}
}

void Movement::soundCharacter(){
	if( mHanteis[ mAction ]->getHanteiInfomation( mActionInfo[ mAction ].time / mActionInfo[ mAction ].flame, 0 ) == 2 && !mPrevSound ){
		PlaySoundMem( mSound[ mAction ], DX_PLAYTYPE_BACK, true );
		mPrevSound = true;
	}
}

void Movement::setSpeeds( int maxMoveSpeed, int firstJumpSpeed, int maxFallSpeed, int moveSpeed, int fallSpeed ){
	mMaxMoveSpeed = maxMoveSpeed;
	mFirstJumpSpeed = firstJumpSpeed;
	mMaxFallSpeed = maxFallSpeed;
	mMoveSpeed = moveSpeed;
	mFallSpeed = fallSpeed;
}

Weapon* Movement::getWeapon( int weapons, int weaponNum ){
	if( mWeapons[ weapons ][ weaponNum ]->mStart ){
		return mWeapons[ weapons ][ weaponNum ];
	}
	return 0;
}

void Movement::resetCharacter(){
	mAppearingTime = 0;
	mBlowPlayer = 0;
	mCharacterX = mFarstCharacterX;
	mCharacterY = mFarstCharacterY;
	mFactCharacterX = mFarstCharacterX * 100;
	mFactCharacterY = mFarstCharacterY * 100;
	mSpeedX = 0;
	mSpeedY = 0;
	mDamaged = 0;
	mBlowRate = 0;
	mJumpNum = 0;
	mMask = false;
	mVisible = false;
	mDirectionRight = true;
	mPrevJump = false;
	mLanding = true;
	mAction = ACT_STAND;
	for( int i = 0; i < 256; i++ ){
		mHitHantei[ i ] = true;
	}
}