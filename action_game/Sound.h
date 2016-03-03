#ifndef INCLUDE_SOUND_H
#define INCLUDE_SOUND_H

#include "DxLib.h"
#include <vector>
#include <iostream>

using namespace std;

class Sound{
public:
	enum SoundsName{
		SOUND_MENU,
		SOUND_SELET,
		SOUND_EFFECT_DECIDE_1,
		SOUND_EFFECT_DECIDE_2,
		SOUND_EFFECT_DECIDE_3,
		SOUND_EFFECT_MOVE,
	};
	static Sound* Instance(){
		static Sound* sound = new Sound();
		return sound;
	}
	void setSound( const char* fileName ){
		mSoundHandle.push_back( LoadSoundMem( fileName ) );
	}
	void playBackSound( SoundsName soundsName ){
		PlaySoundMem( mSoundHandle[ soundsName ], DX_PLAYTYPE_LOOP );
		mTempSound = soundsName;
	}
	void playSoundEffect( SoundsName soundsName ){
		PlaySoundMem( mSoundHandle[ soundsName ], DX_PLAYTYPE_BACK );
	}
	void stopSound(){
		StopSoundMem( mSoundHandle[ mTempSound ] );
	}
	bool checkSound( SoundsName soundsName ){
		return ( CheckSoundMem( mSoundHandle[ soundsName ] ) == 1 ) ? true : false;
	}

private:
	Sound(){}
    Sound( const Sound& sound );
    Sound& operator=( const Sound& sound );
	~Sound(){
		for( vector<int>::iterator it = mSoundHandle.begin(); it != mSoundHandle.end(); it++ ){
			StopSoundMem( *it );
			DeleteSoundMem( *it );
		}
	}
	SoundsName mTempSound;
	vector< int > mSoundHandle;

};

#endif