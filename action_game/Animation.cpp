#include "Animation.h"

void Animation::setAnimation( int id, int* infomation ){
	mActionInfo[ id ].page = infomation[ 0 ];
	mActionInfo[ id ].flame = infomation[ 3 ];
	mActionInfo[ id ].correctionX = infomation[ 4 ];
	mActionInfo[ id ].correctionY = infomation[ 5 ];
	mActionInfo[ id ].damage = infomation[ 6 ];
	mActionInfo[ id ].blowX = infomation[ 7 ];
	mActionInfo[ id ].blowY = infomation[ 8 ];
	mActionInfo[ id ].speedX = infomation[ 9 ];
	mActionInfo[ id ].speedY = infomation[ 10 ];
	mActionInfo[ id ].weaponNum = infomation[ 11 ];
}

int Animation::intervalTime( int id, bool mask ){
	if( mActionInfo[ id ].time < mActionInfo[ id ].page * mActionInfo[ id ].flame - 2 ){
		if( mActionInfo[ mAction ].speedY != 0 && mActionInfo[ id ].time == 0 ){
			mSpeedY = mActionInfo[ mAction ].speedY;
		}
		if( mActionInfo[ mAction ].speedX != 0 &&  mActionInfo[ id ].time == 0 ){
			if( mDirectionRight ){
				mSpeedX = mActionInfo[ mAction ].speedX;
			}else{
				mSpeedX = -mActionInfo[ mAction ].speedX;
			}
		}
		mMask = mask;
		return mActionInfo[ id ].time++ / mActionInfo[ id ].flame;
	}else{
		if( mMask ){
			if( mActionInfo[ mAction ].speedY != 0 ){
				mSpeedY = 0;
			}
			if( mActionInfo[ mAction ].speedX != 0 ){
				mSpeedX = 0;
			}
		}
		if( id >= ACT_SIDE_WEAK_ATTACK ){
			for( int i = 0; i < 256; i++ ){
				mHitHantei[ i ] = true;
			}
		}
		mActionInfo[ id ].time = 0;
		mMask = false;
		mPrevSound = false;
		return mActionInfo[ id ].page - 1;
	}
}

bool Animation::getMask(){
	return mMask;
}

void Animation::setTime( int id, int time ){
	mActionInfo[ id ].time = time;
}

int Animation::getTime( int id ){
	if( mActionInfo[ id ].time == 0 ){
		return mActionInfo[ id ].page - 1;
	}
	return mActionInfo[ id ].time / mActionInfo[ id ].flame;
}

void Animation::setAction( Action Act ){
	mAction = Act;
}

int Animation::getAction(){
	return mAction;
}

void Animation::setDirectionRight( bool right ){
	mDirectionRight = right;
}

bool Animation::getDirectionRight(){
	return mDirectionRight;
}

void Animation::setHitHantei( int i, bool hantei ){
	mHitHantei[ i ] = hantei;
}

bool Animation::getHitHantei( int i ){
	return mHitHantei[ i ];
}

int Animation::getCorrectionX(){
	if( mDirectionRight ){
		return mActionInfo[ mAction ].correctionX;
	}else{
		return -mActionInfo[ mAction ].correctionX;
	}
}

int Animation::getCorrectionY(){
	return mActionInfo[ mAction ].correctionY;
}

int Animation::getDamage(){
	return mActionInfo[ mAction ].damage;
}

int Animation::getBlowX(){
	if( mDirectionRight ){
		return mActionInfo[ mAction ].blowX;
	}else{
		return -mActionInfo[ mAction ].blowX;
	}
}

int Animation::getBlowY(){
	return mActionInfo[ mAction ].blowY;
}

void Animation::setDamaged( int damaged ){
	mDamaged = damaged;
}

int Animation::getDamaged(){
	return mDamaged;
}

void Animation::initializeTime(){
	for( int i = 0; i < ACT_MAX_NUM; i++ ){
		mActionInfo[ i ].time = 0;
	}
}

int Animation::getHanteiPointNum(){
	if( !mHanteis[ mAction ]->mSuccess ){
		return 0;
	}
	return mHanteis[ mAction ]->getHanteiPointNum( getTime( mAction ) );
}

void Animation::getHanteiPoint( int address, int* info, int* startX, int* startY, int* endX, int* endY ){
	if( !mHanteis[ mAction ]->mSuccess ){
		*info = *startX = *startY = *endX = *endY = 0;
		return;
	}
	if( mDirectionRight ){
		mHanteis[ mAction ]->getHanteiPoint( getTime( mAction ), address, info, startX, startY, endX, endY );
	}else{
		mHanteis[ mAction ]->getHanteiPointLR( getTime( mAction ), address, info, startX, startY, endX, endY );
	}
}

int Animation::getHantei( int x, int y ){
	if( !mHanteis[ mAction ]->mSuccess ){
		return 0;
	}
	if( mDirectionRight ){
		return mHanteis[ mAction ]->getHantei( getTime( mAction ), x, y );
	}else{
		return mHanteis[ mAction ]->getHanteiLR( getTime( mAction ), x, y );
	}
}

void Animation::getHanteiSize( int* x, int *y ){
	if( !mHanteis[ mAction ]->mSuccess ){
		*x = *y = 0;
		return;
	}
	mHanteis[ mAction ]->getCellSize( x, y );
}

int Animation::getHitSound(){
	return mHitSound[ mAction ];
}