#ifndef INCLUDED_ANIMATION_H
#define INCLUDED_ANIMATION_H

#include "DxLib.h"
#include "Character.h"
#include "HanteiImage.h"

class Animation : public Character{
public:
	Animation( int player ) : mAction( ACT_STAND ), mMask( false ), 
		mDirectionRight( true ),mPlayer( player ), mDamaged( 0 ), mAppearingTime( 0 ){
		mActionInfo = new ActionInfo[ ACT_MAX_NUM ];
		mPrevSound = false;
		for( int i = 0; i < 256; i++ ){
			mHitHantei[ i ] = true;
		}
		initializeTime();
	};
public:
	Animation() : mAction( ACT_STAND ), mMask( false ), 
		mDirectionRight( true ),mPlayer( 0 ), mDamaged( 0 ){
		mActionInfo = new ActionInfo[ 1 ];
		mPrevSound = false;
		for( int i = 0; i < 256; i++ ){
			mHitHantei[ i ] = true;
		}
		mActionInfo[ 0 ].time = 0;
	};
	virtual ~Animation(){
		delete[] mActionInfo;
		mActionInfo = 0;
	};
	enum Action{
		ACT_STAND,
		ACT_WALK,
		ACT_RUN,//
		ACT_JUMP,
		ACT_FALL,
		ACT_DAMAGED_LEVEL1,
		ACT_DAMAGED_LEVEL2,
		ACT_SQUAT_1,
		ACT_SQUAT_2,
		ACT_SQUAT_3,
		ACT_LIE,//
		ACT_GETUP,//
		ACT_SIDE_WEAK_ATTACK,
		ACT_SIDE_WEAK_ATTACK_2,
		ACT_SIDE_WEAK_ATTACK_3,
		ACT_SIDE_HEAVY_ATTACK,
		ACT_SIDE_SMASH_ATTACK,
		ACT_NORMAL_SP_ATTACK,

		ACT_MAX_NUM
	};
	enum Weapons{
		WEA_WEAPON1,
		WEA_WEAPON2,
		WEA_WEAPON3,
		WEA_WEAPON4,

		WEA_MAX_NUM
	};
	void setAnimation( int id, int* actInfo  );
	int intervalTime( int id, bool mask );
	bool getMask();
	void setTime( int id, int time );
	int getTime( int id );
	void setAction( Action Act );
	int getAction();
	void setDirectionRight( bool right );
	bool getDirectionRight();
	void setHitHantei( int i, bool hantei );
	bool getHitHantei( int i );
	int getCorrectionX();
	int getCorrectionY();
	int getDamage();
	int getBlowX();
	int getBlowY();
	void setDamaged( int damaged );
	int getDamaged();
	void initializeTime();
	int getHanteiPointNum();
	void getHanteiPoint( int address, int* info, int* startX, int* startY, int* endX, int* endY );
	int getHantei( int x, int y );
	void getHanteiSize( int* x, int *y );
	int getHitSound();
	bool mVisible;
	int mAppearingTime;
	int mPlayer;

protected:
	struct ActionInfo{
		int time;
		int page;
		int flame;
		int correctionX;
		int correctionY;
		int damage;
		int blowX;
		int blowY;
		int speedX;
		int speedY;
		int weaponNum;
	};
	bool mMask;
	bool mDirectionRight;
	bool mHitHantei[ 256 ];
	bool mPrevSound;
	int mDamaged;
	Action mAction;
	ActionInfo* mActionInfo;
	int* mHitSound;
	int* mSound;
	HanteiImage** mHanteis;
};

#endif