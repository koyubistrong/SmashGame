#ifndef INCLUDE_COMPUTER_H
#define INCLUDE_COMPUTER_H

#include "Movement.h"
#include "StaticObject.h"
#include "DxLib.h"

class Computer : public Movement{
public:
	Computer( int characterID, int player, int defficult, Movement** movement, StaticObject* staticObject ) : Movement( characterID, player ){
		mMovement = movement;
		mStaticObject = staticObject;
		mDefficult = defficult;
		setDefficult();
		string signName = "pic/computer.png";
		mSignHandle = LoadGraph( signName.data() );
		mTime = GetRand( 99 );
		mNearDistance = 0;
		mVibration = false;
		int sizeX, sizeY;
		mPrecipiceNum = 0;
		staticObject->getStageSize( &sizeX, &sizeY );
		sizeX /= 16;
		sizeY /= 16;
		for( int y = 0; y < sizeY; y++ ){
			for( int x = 0; x < sizeX; x++ ){
				if( staticObject->getBlockInfo( x, y ) == 2 ){
					mPrecipiceNum++;
				}
			}
		}
		if( mPrecipiceNum == 0 ){
			mPrecipice = 0;
		}else{
			mPrecipice = new point*[ mPrecipiceNum ];
		}
		int num = 0;
		for( int y = 0; y < sizeY; y++ ){
			for( int x = 0; x < sizeX; x++ ){
				if( staticObject->getBlockInfo( x, y ) == 2 ){
					mPrecipice[ num ] = new point();
					mPrecipice[ num ]->pointX = x;
					mPrecipice[ num ]->pointY = y;
					num++;
				}
			}
		}
		for(int i=0;i<mPrecipiceNum;i++){
			for(int j=0;j<(mPrecipiceNum-i-1);j++){
				point* temp;
				if(mPrecipice[j]->pointX>mPrecipice[j+1]->pointX){
					temp=mPrecipice[j];
					mPrecipice[j]=mPrecipice[j+1];
					mPrecipice[j+1]=temp;
				}
			}
		}
	}
	virtual ~Computer(){
		for( int i = 0; i < mPrecipiceNum; i++ ){
			delete mPrecipice[ i ];
			mPrecipice[ i ] = 0;
		}
		delete[] mPrecipice;
		mPrecipice = 0;
	}
	void setNextAction();
	void setDefficult();
	void brain();
	void caluculateDistance();
	void changeDefficult( int defficult );
	bool isPlayer();
private:
	struct point{
		int pointY;
		int pointX;
	};
	int mPrecipiceNum;
	point** mPrecipice;
	Movement** mMovement;
	int mNearEnemy;
	StaticObject* mStaticObject;
	int mNearDistance;
	int mTime;
	int mDefficult;
	int mMoveInterval;
	int mAttackInterval;
	int mMoveSpeed;

};

#endif