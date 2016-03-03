#ifndef INCLUDED_STICK_FIGURE_WHITE_H
#define INCLUDED_STICK_FIGURE_WHITE_H

#include <string>
#include "DxLib.h"
#include "Weapon.h"
#include "Image.h"
#include "Animation.h"
#include "GameInfomation.h"
#include "HanteiImage.h"
#include "CSV.h"
#include "extern.h"

class Movement : public Animation{
public:
	Movement( int characterID, int player ) : Animation( player ), mDegree( 0.0 ), mNextAction( ACT_STAND ), mContinueAction( 0 ),
	 mBlowPlayer( 0 ), mSmashNum( 0 ), mFallNum( 0 ), mPrevJump( false ), mPrevDamaged( false ), mLanding( true ), mMoveX( 0 ), mMoveY( 0 ){
		string folder1( "character/" );
		string chr_csv = folder1 + "characters.csv";
		CSV* chr = new CSV( chr_csv.data() );
		const string* chrString = chr->getDataString();
		const int* chrData = chr->getDataInt();
		int chrSizeX = chr->getSizeX();
		int chrSizeY = chr->getSizeY();
		int* chrInfo = new int[ chrSizeX ];

		string character( chrString[ characterID ] );
		string act_csv = folder1 + character + "/information/action.csv";
		string picFolder = folder1 + character + "/pic/";
		string seFolder = folder1 + character + "/se/";

		CSV* action = new CSV( act_csv.data() );
		const string* actString = action->getDataString();
		const int* actData = action->getDataInt();
		int actSizeX = action->getSizeX();
		int actSizeY = action->getSizeY();
		mImages = new Image*[ actSizeY ];
		mHanteis = new HanteiImage*[ actSizeY ];
		mSound = new int[ actSizeY ];
		mHitSound = new int[ actSizeY ];
		int* actInfo = new int[ actSizeX ];
		string seDefaultName = seFolder + "attack_default_normal.mp3";
		string seHitDefaultName = seFolder + "attack_default_hit.mp3";
		for( int y = 0; y < actSizeY; y++ ){
			string imgName = picFolder + actString[ y ] + ".png";
			string hanteiName = picFolder + actString[ y ] + "_hantei.png";
			string seNormalName = seFolder + actString[ y ] + "_normal.mp3";
			string seHitName = seFolder + actString[ y ] + "_hit.mp3";
			for( int x = 0; x < actSizeX; x++ ){
				actInfo[ x ] = actData[ y * actSizeX + x ];
			}
			mImages[ y ] = new Image( imgName.data(), actInfo[ 0 ], actInfo[ 1 ], actInfo[ 2 ] );
			mHanteis[ y ] = new HanteiImage( hanteiName.data(), actInfo[ 0 ], actInfo[ 1 ], actInfo[ 2 ] );
			mSound[ y ] = LoadSoundMem( seNormalName.data() );
			if( mSound[ y ] == -1 && y >= ACT_SIDE_WEAK_ATTACK ){ mSound[ y ] = LoadSoundMem( seDefaultName.data() ); }
			mHitSound[ y ] = LoadSoundMem( seHitName.data() );
			if( mHitSound[ y ] == -1 && y >= ACT_SIDE_WEAK_ATTACK  ){ mHitSound[ y ] = LoadSoundMem( seHitDefaultName.data() ); }
			setAnimation( y, actInfo );
		}

		for( int x = 0; x < chrSizeX; x++ ){
			chrInfo[ x ] = chrData[ characterID * chrSizeX + x ];
		}
		setSpeeds( chrInfo[ 0 ], chrInfo[ 1 ], chrInfo[ 2 ], chrInfo[ 3 ], chrInfo[ 4 ] );

		mWeapons = new Weapon**[ 4 ];

		for( int i = 0; i < 4; i++ ){
			mWeapons[ i ] = new Weapon*[ 5 ];
			for( int j = 0; j < 5; j++ ){
				mWeapons[ i ][ j ] = new Weapon( chrString[ characterID ].data(), i );
				mWeapons[ i ][ j ]->setUsedCharacter( &mCharacterX, &mCharacterY );
			}
		}
		string seJumpSound = seFolder + "jump.mp3";
		string seCollisionSound = seFolder + "collision.mp3";
		string seCrushedSound = seFolder + "crushed.mp3";
		mSoundJumpHandle = LoadSoundMem( seJumpSound.data() );
		mSoundCollisionHandle = LoadSoundMem( seCollisionSound.data() );
		mSoundCrushedHandle = LoadSoundMem( seCrushedSound.data() );
		mVisible = true;
		delete chr;
		delete action;
		delete chrInfo;
	}
	virtual ~Movement(){
		for( int i = 0; i < ACT_MAX_NUM; i++ ){
			delete mImages[ i ];
			delete mHanteis[ i ];
			mImages[ i ] = 0;
			mHanteis[ i ] = 0;
			StopSoundMem( mSound[ i ] );
			StopSoundMem( mHitSound[ i ] );
			DeleteSoundMem( mSound[ i ] );
			DeleteSoundMem( mHitSound[ i ] );
		}
		for( int i = 0; i < 4; i++ ){
			for( int j = 0; j < 5; j++ ){
				delete mWeapons[ i ][ j ];
				mWeapons[ i ][ j ] = 0;
			}
			delete[] mWeapons[ i ];
			mWeapons[ i ] = 0;
		}
		delete[] mImages;
		delete[] mHanteis;
		delete[] mWeapons;
		delete[] mHitSound;
		delete[] mSound;
		mImages = 0;
		mHanteis = 0;
		mWeapons = 0;
		mHitSound = 0;
		mSound = 0;
		StopSoundMem( mSoundJumpHandle );
		StopSoundMem( mSoundCollisionHandle );
		StopSoundMem( mSoundCrushedHandle );
		DeleteSoundMem( mSoundJumpHandle );
		DeleteSoundMem( mSoundCollisionHandle );
		DeleteSoundMem( mSoundCrushedHandle );
		DeleteGraph( mSignHandle );
	}
	struct Key{
		int KEY_LEFT;
		int KEY_RIGHT;
		int KEY_UP;
		int KEY_DOWN;
		int KEY_JUMP;
		int KEY_NORMAL_ATTACK;
		int KEY_SPECIAL_ATTACK;
	};
	void update();
	virtual void setNextAction() = 0;
	virtual bool isPlayer() = 0;
	virtual void changeDefficult( int defficult ) = 0;
	void drawCharacter( int x, int y );
	void soundCharacter();
	void setSpeeds( int maxMoveSpeed, int firstJumpSpeed, int maxFallSpeed, int moveSeed, int fallSpeed );
	void resetCharacter();
	Weapon* getWeapon( int weapons, int weaponNum );
	Weapon*** mWeapons;
	int mMaxMoveSpeed;
	int mFirstJumpSpeed;
	int mMaxFallSpeed;
	int mMoveSpeed;
	int mFallSpeed;
	int mBlowPlayer;
	int mSmashNum;
	int mFallNum;
	int mPrevJump;
	bool mPrevDamaged;
	bool mLanding;
	int mTeamID;
	int mLives;
	Key mKey;
	int mMoveX;
	int mMoveY;
	int mSoundJumpHandle;
	int mSoundCollisionHandle;
	int mSoundCrushedHandle;
	int mVibration;

protected:
	double mDegree;
	bool mRota;
	Action mNextAction;
	int mContinueAction;
	Image** mImages;
	int mSignHandle;
	
};

#endif
