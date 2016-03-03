#ifndef INCLUDED_WEAPON_H
#define INCLUDED_WEAPON_H

#include <string>
#include "DxLib.h"
#include "CSV.h"
#include "Image.h"
#include "HanteiImage.h"
#include "Animation.h"
#include "extern.h"

class Weapon : public Animation{
public:
	Weapon( const char* folderName, int weaponNum ) : Animation(), mWepTime( 0 ), mStart( false ), mWeaponNum( weaponNum ){
		string folder1( "character/" );
		string character( folderName );
		string seFolder = folder1 + character + "/se/";
		string wea_csv = folder1 + character + "/information/weapon.csv";
		CSV* weapon = new CSV( wea_csv.data() );
		const int* weaData = weapon->getDataInt();
		int weaSizeX = weapon->getSizeX();
		int weaSizeY = weapon->getSizeY();
		const string* weaString = weapon->getDataString();
		string pic_png = folder1 + character + "/pic/";
		int* weaInfo = new int[ weaSizeX ];
		mSound = new int[ 1 ];
		mHitSound = new int[ 1 ];
		for( int y = 0; y < weaSizeY; y++ ){
			string imgName = pic_png + weaString[ y ] + ".png";
			string hanteiName = pic_png + weaString[ y ] + "_hantei.png";
			string seNormalName = seFolder + weaString[ y ] + "_normal.mp3";
			string seHitName = seFolder + weaString[ y ] + "_hit.mp3";
			for( int x = 0; x < weaSizeX; x++ ){
				weaInfo[ x ] = weaData[ y * weaSizeX + x ];
			}
			if( weaponNum == y ){
				mWeaponImages = new Image( imgName.data(), weaInfo[ 0 ], weaInfo[ 1 ], weaInfo[ 2 ] ); 
				mWeaponHanteis = new HanteiImage( hanteiName.data(), weaInfo[ 0 ], weaInfo[ 1 ], weaInfo[ 2 ] );
				setAnimation( 0, weaInfo );
				mWait = weaInfo[ 12 ];
				mEnd = weaInfo[ 13 ];
				mDegree = weaInfo[ 14 ];
				mSound[ 0 ] = LoadSoundMem( seNormalName.data() );
				mHitSound[ 0 ] = LoadSoundMem( seHitName.data() );
				break;
			}
		}

		mHanteis = new HanteiImage*[ 1 ];
		mHanteis[ 0 ] = mWeaponHanteis;
		mVisible = false;
		delete weapon;
		delete[] weaInfo;
		//setAnimation( id, page, flame, correctionX, correctionY, damage, blowX, blowY, speedX, speedY, weapon->Num );

	}

	virtual ~Weapon(){
		StopSoundMem( mSound[ 0 ] );
		StopSoundMem( mHitSound[ 0 ] );
		DeleteSoundMem( mSound[ 0 ] );
		DeleteSoundMem( mHitSound[ 0 ] );
		delete mWeaponImages;
		delete mWeaponHanteis;
		delete[] mHanteis;
		delete[] mSound;
		delete[] mHitSound;
		mWeaponImages = 0;
		mWeaponHanteis = 0;
		mHanteis = 0;
		mUsedCharacterX = 0;
		mUsedCharacterY = 0;
		mSound = 0;
		mHitSound = 0;
	}
	void update();
	void drawWeapon( int x, int y );
	void soundWeapon();
	void setStart();
	void setWeapon( bool directionRight );
	void setUsedCharacter( int* x, int* y );
	void resetWeapon();
	int mWepTime;
	int mWait;
	int mEnd;
	int mWeaponNum;
	double mDegree;
	bool mStart;

private:
	Image* mWeaponImages;
	HanteiImage* mWeaponHanteis;
	int* mUsedCharacterX;
	int* mUsedCharacterY;
};

#endif