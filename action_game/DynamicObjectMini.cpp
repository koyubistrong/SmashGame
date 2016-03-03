#include "DynamicObjectMini.h"
#define WEAPON_NUM_MAX 5
#define WEAPONS 4

DynamicObjectMini::DynamicObjectMini( const char* filename, GameInfomation* gameInfomation, StaticObjectMini* staticObject ) 
	: DynamicObject(), mStaticObjectMini( staticObject ) {
	mLivesTime = 0;
	mCharacterNumber = 0;
	mCharacterObject = new Image( "pic/picture.png", 70, 16, 16 );
	staticObject->getStageSize( &mSizeX, &mSizeY );
	SRand( ( unsigned int )time( NULL ) );
	mBlowRate = 100.0 / 100.0;
	mModeLives = true;
	mGameInfomation = gameInfomation;
	mDefficult = 1;
	mCameraX = 0;
	int appearCharacterY = 0;
	int appearCharacterX = 5;
	while( staticObject->getBlockInfo( appearCharacterX, appearCharacterY + 1 ) != 1 ){
		appearCharacterY++;
	}
	for( int i = 0; i < 256; i++ ){
		mCharacter[ i ] = 0;
	}
	int num = 0;
	for( int i = 0; i < 4; i++ ){
		if( mGameInfomation[ i ].characterID != -1 ){
			mCharacter[ mCharacterNumber ] = new Player( mGameInfomation[ i ].characterID, i + 1 );
			mCharacter[ mCharacterNumber ]->mCharacterX = appearCharacterX * 16;
			mCharacter[ mCharacterNumber ]->mCharacterY = appearCharacterY * 16;
			mCharacter[ mCharacterNumber ]->mFactCharacterX = appearCharacterX * 16 * 100;
			mCharacter[ mCharacterNumber ]->mFactCharacterY = appearCharacterY * 16 * 100;
			mCharacter[ mCharacterNumber ]->mFarstCharacterX = appearCharacterX * 16;
			mCharacter[ mCharacterNumber ]->mFarstCharacterY = appearCharacterY * 16;
			mCharacter[ mCharacterNumber ]->mTimeCOM = GetRand( 1000 );
			mCharacter[ mCharacterNumber ]->setCharacterHandle( mCharacterObject->getHandle( 0 ) );
			mCharacter[ mCharacterNumber ]->mTeamID = 0;
			mCharacter[ mCharacterNumber ]->setCharacterID( 0 );
			mCharacter[ mCharacterNumber ]->mVisible = true;
			mCharacter[ mCharacterNumber ]->mLives = 3;
			mCharacterNumber++;
		}
	}
	if( mCharacterNumber == 1 ){
		mPlayerMode = MODE_PLAYER1;
	}else if( mCharacterNumber == 2 ){
		mPlayerMode = MODE_PLAYER2;
	}
	for( int i = 0; i < 2; i++ ){
			mCharacter[ mCharacterNumber ] = new Computer( i, i + 5, mDefficult, mCharacter, staticObject );
			mCharacter[ mCharacterNumber ]->mCharacterX = 0;
			mCharacter[ mCharacterNumber ]->mCharacterY = 0;
			mCharacter[ mCharacterNumber ]->mFactCharacterX = 0;
			mCharacter[ mCharacterNumber ]->mFactCharacterY = 0;
			mCharacter[ mCharacterNumber ]->mFarstCharacterX = 0;
			mCharacter[ mCharacterNumber ]->mFarstCharacterY = 0;
			mCharacter[ mCharacterNumber ]->mTimeCOM = GetRand( 1000 );
			mCharacter[ mCharacterNumber ]->setCharacterHandle( mCharacterObject->getHandle( 0 ) );
			mCharacter[ mCharacterNumber ]->mTeamID = 1;
			mCharacter[ mCharacterNumber ]->setCharacterID( 10 );
			mCharacter[ mCharacterNumber ]->mVisible = false;
			mCharacter[ mCharacterNumber ]->mLives = 1;
			mCharacterNumber++;
	}
}

DynamicObjectMini::~DynamicObjectMini(){}

void DynamicObjectMini::update(){
	int x, y;
	for( int i = 0; i < mCharacterNumber; i++ ){
		Movement* character = mCharacter[ i ];
		if( !character->mVisible ){
			if( mModeLives && character->mLives <= 0 || character->outCharacterID() == Character::OBJ_COMPUTER ){
				continue;
			}
			if( character->mAppearingTime >= 180 ){
				appearCharacter( character, 5, 10 );
			}
			character->mAppearingTime++;
			continue;
		}
		checkFallCharacter( character );
		checkMoveCharacter( character, &x, &y );
		character->update();
		moveCharacter( character, checkMoveCharacter( character, x ,y ) );
		checkHitAttack1( i );
		for( int n = 0; n < WEAPONS; n++ ){
			for( int m = 0; m < WEAPON_NUM_MAX; m++ ){
				Weapon* weapon = character->getWeapon( n, m );
				if( !weapon ){ continue; }
				if( weapon->mStart ){
					weapon->update();
					moveWeapon( character, weapon );
					checkHitWeapon1( i, weapon );
				}
			}
		}
		checkStageRangeCharacter( i, character );
		//printfDx( "%d : ダメージ %4d 撃墜 %2d 落下 %2d \n", i, character->getBlowRate(), character->mSmashNum, character->mFallNum );
	}
	checkComputerNum();
	mLivesTime++;

}

void DynamicObjectMini::drawCharacter( int cameraX, int cameraY ){
	for( int i = 0; i < mCharacterNumber; i++ ){
		Movement* character = mCharacter[ i ];
		if( !character->mVisible ){
			continue;
		}
		int x = character->mCharacterX, y = character->mCharacterY;
		if( ( cameraX - 32 ) / magnification <= x && ( cameraX + 640 + 32 ) / magnification >= x && 
			( cameraY - 32 ) / magnification <= y && ( cameraY + 480 + 32 ) / magnification >= y ){
			int tx = x * magnification - cameraX;
			int ty = y * magnification - cameraY;
			character->drawCharacter( tx, ty );
			for( int n = 0; n < WEAPONS; n++ ){
				for( int m = 0; m < WEAPON_NUM_MAX; m++ ){
					Weapon* weapon = character->getWeapon( n, m );
					if( !weapon ){ continue; }
					if( weapon->mStart ){
						tx = weapon->mCharacterX * magnification - cameraX;
						ty = weapon->mCharacterY * magnification - cameraY;
						weapon->drawWeapon( tx, ty );
					}
				}
			}
		}
	}
}

int DynamicObjectMini::checkMoveCharacter( Movement* character, int x, int y ){
	int tx, ty;
	getCharacterPoint( character, &tx, &ty );
	int tLeftUpper[ 2 ] = { tx >> 4, ty >> 4 };
	int tRightUpper[ 2 ] = { ( tx + 15 ) >> 4, ty >> 4 };
	int tLeftLower[ 2 ] = { tx >> 4, ( ty + 15 ) >> 4 };
	int tRightLower[ 2 ] = { ( tx + 15 ) >> 4, ( ty + 15 ) >> 4 };
	int numX = abs( ( ( x - tx ) >> 4 ) ) + 1, numY = abs( ( ( y - ty ) >> 4 ) ) + 1;
	int num = ( numX > numY ) ? numX : numY;
	float increaseX = ( float )( x - tx ) / num, increaseY = ( float )( y - ty ) / num;
	for( int n = 0; n < num; n++ ){
		int cx = tx + increaseX * ( n + 1 ), cy = ty + increaseY * ( n + 1 );
		if( n + 1 >= num ){
			cx = x, cy = y;
		}
		int leftUpper[ 2 ] = { cx >> 4, cy >> 4 };
		int rightUpper[ 2 ] = { ( cx + 15 ) >> 4, cy >> 4 };
		int leftLower[ 2 ] = { cx  >> 4, ( cy + 15 ) >> 4 };
		int rightLower[ 2 ] = { ( cx + 15 ) >> 4, ( cy + 15 ) >> 4 };
		//左上角衝突
		if( mStaticObjectMini->hitStaticObjectCheck( leftUpper ) &&
			mStaticObjectMini->hitStaticObjectCheck( rightUpper ) &&
			mStaticObjectMini->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, ( leftUpper[ 0 ] + 1 ) * 16, ( leftUpper[ 1 ] + 1 ) * 16 );
				return 3;
		}
		//左下角衝突
		if( mStaticObjectMini->hitStaticObjectCheck( leftUpper ) &&
			mStaticObjectMini->hitStaticObjectCheck( rightLower ) &&
			mStaticObjectMini->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, ( leftLower[ 0 ] + 1 ) * 16, ( leftLower[ 1 ] - 1 ) * 16 );
				return 3;
		}
		//右上角衝突
		if( mStaticObjectMini->hitStaticObjectCheck( leftUpper ) &&
			mStaticObjectMini->hitStaticObjectCheck( rightUpper ) &&
			mStaticObjectMini->hitStaticObjectCheck( rightLower ) ){
				setCharacterPoint( character, ( rightUpper[ 0 ] - 1 ) * 16, ( rightUpper[ 1 ] + 1 ) * 16 );
				return 3;
		}
		//右下角衝突
		if( mStaticObjectMini->hitStaticObjectCheck( rightUpper ) &&
			mStaticObjectMini->hitStaticObjectCheck( rightLower ) &&
			mStaticObjectMini->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, ( rightLower[ 0 ] - 1 ) * 16, ( rightLower[ 1 ] - 1 ) * 16 );
				return 3;
		}
		//上衝突（２ブロック）
		if( mStaticObjectMini->hitStaticObjectCheck( rightUpper ) &&
			mStaticObjectMini->hitStaticObjectCheck( leftUpper ) ){
				setCharacterPoint( character, -1, ( rightUpper[ 1 ] + 1 ) * 16 );
				return 2;
		}
		//下衝突（２ブロック）
		if( mStaticObjectMini->hitStaticObjectCheck( rightLower ) &&
			mStaticObjectMini->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, -1, ( rightLower[ 1 ] - 1 ) * 16 );
				return 2;
		}
		//右衝突（２ブロック）
		if( mStaticObjectMini->hitStaticObjectCheck( rightLower ) &&
			mStaticObjectMini->hitStaticObjectCheck( rightUpper ) ){
				setCharacterPoint( character, ( rightLower[ 0 ] - 1 ) * 16, -1 );
				return 1;
		}
		//左衝突（２ブロック）
		if( mStaticObjectMini->hitStaticObjectCheck( leftUpper ) &&
			mStaticObjectMini->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, ( leftLower[ 0 ] + 1 ) * 16, -1 );
				return 1;
		}
		if( mStaticObjectMini->hitStaticObjectCheck( rightUpper ) && rightUpper[ 1 ] < tRightUpper[ 1 ] ){
			//上衝突（１ブロック）
				setCharacterPoint( character, -1, ( rightUpper[ 1 ] + 1 ) * 16 );
				return 2;
		}else if( mStaticObjectMini->hitStaticObjectCheck( rightUpper ) && rightUpper[ 1 ] == tRightUpper[ 1 ] ){
			//右衝突（１ブロック）
				setCharacterPoint( character, ( rightUpper[ 0 ] - 1 ) * 16, -1 );
				return 1;
		}
		if( mStaticObjectMini->hitStaticObjectCheck( rightLower ) && rightLower[ 1 ] > tRightLower[ 1 ] ){
			//下衝突（１ブロック）
				setCharacterPoint( character, -1, ( rightLower[ 1 ] - 1 ) * 16 );
				return 2;
		}else if( mStaticObjectMini->hitStaticObjectCheck( rightLower ) && rightLower[ 1 ] == tRightLower[ 1 ] ){
			//右衝突（１ブロック）
				setCharacterPoint( character, ( rightLower[ 0 ] - 1 ) * 16, -1 );
				return 1;
		}
		if( mStaticObjectMini->hitStaticObjectCheck( leftUpper ) && leftUpper[ 1 ] < tLeftUpper[ 1 ] ){
			//上衝突（１ブロック）
				setCharacterPoint( character, -1, ( leftUpper[ 1 ] + 1 ) * 16 );
				return 2;
		}else if( mStaticObjectMini->hitStaticObjectCheck( leftUpper ) && leftUpper[ 1 ] == tLeftUpper[ 1 ] ){
			//左衝突（１ブロック）
				setCharacterPoint( character, ( leftUpper[ 0 ] + 1 ) * 16, -1 );
				return 1;
		}
		if( mStaticObjectMini->hitStaticObjectCheck( leftLower ) && leftLower[ 1 ] > tLeftLower[ 1 ] ){
			//下衝突（１ブロック）
				setCharacterPoint( character, -1, ( leftLower[ 1 ] - 1 ) * 16 );
				return 2;
		}else if( mStaticObjectMini->hitStaticObjectCheck( leftLower ) && leftLower[ 1 ] == tLeftLower[ 1 ] ){
			//左衝突（１ブロック）
				setCharacterPoint( character, ( leftLower[ 0 ] + 1 ) * 16, -1 );
				return 1;
		}
	}
	return 0;
}

bool DynamicObjectMini::checkMoveWeapon( Weapon* weapon, int startAttackerX, int startAttackerY, int endAttackerX, int endAttackerY ){
	int leftUpper[ 2 ] = { startAttackerX / 16, startAttackerY / 16 };
	int rightUpper[ 2 ] = { endAttackerX / 16, startAttackerY / 16 };
	int leftLower[ 2 ] = { startAttackerX / 16, endAttackerY / 16 };
	int rightLower[ 2 ] = { endAttackerX / 16, endAttackerY / 16 };
	if( mStaticObjectMini->hitStaticObjectCheck( leftUpper ) ||
		mStaticObjectMini->hitStaticObjectCheck( rightUpper ) ||
		mStaticObjectMini->hitStaticObjectCheck( leftLower ) ||
		mStaticObjectMini->hitStaticObjectCheck( rightLower ) ){
			weapon->resetWeapon();
			return true;
	}
	return false;
}

void DynamicObjectMini::checkMoveCharacter( Movement* character, int* x, int* y ){
	switch( character->outCharacterID() ){
	case Character::OBJ_PLAYER:
		movePlayer( character );
		break;
	case Character::OBJ_COMPUTER:
		moveComputer( character );
		break;
	case Character::OBJ_ENEMY2:
		moveEnemy2( character );
		break;
	}
	*x = ( character->mFactCharacterX + character->mSpeedX ) * 0.01;
	*y = ( character->mFactCharacterY + character->mSpeedY ) * 0.01;
}

void DynamicObjectMini::checkFallCharacter( Movement* character ){
	int tx, ty;
	getCharacterPoint( character, &tx, &ty );
	int leftLower[ 2 ] = { tx / 16, ( ty + 16 ) / 16 };
	int rightLower[ 2 ] = { ( tx + 15 ) / 16, ( ty + 16 ) / 16 };
	if( !( mStaticObjectMini->hitStaticObjectCheck( rightLower ) ||
		mStaticObjectMini->hitStaticObjectCheck( leftLower ) ) ){
			setJumpNum( character, true );
	}else{
			setJumpNum( character, false );
			if( character->getDamaged() == 0 && character->mSpeedY > 0 ){
				setSpeedY( character, 0 );
			}
	}
}

void DynamicObjectMini::checkStageRangeCharacter( int i, Movement* character ){
	if( character->mCharacterX - mCameraX < 0 || character->mCharacterX - mCameraX > 640
		|| character->mCharacterY > mSizeY || character->mCharacterY < -32 ){
			if( character->mBlowPlayer >= 0 ){
				mCharacter[ character->mBlowPlayer ]->mSmashNum++;
			}
			character->mFallNum++;
			if( mModeLives ){
				character->mLives--;
				if( character->mLives <= 0 ){
					if( i < 4 ){
						mGameInfomation[ i ].gameOverTime = mLivesTime;
					}
				}
			}
			PlaySoundMem( character->mSoundCrushedHandle, DX_PLAYTYPE_BACK, true );
			character->resetCharacter();
	}
}

bool DynamicObjectMini::checkLives(){
	if( mPlayerMode == MODE_PLAYER1 ){
		Movement* player1 = mCharacter[ 0 ];
		if( player1->mLives <= 0 ){
			return true;
		}else{
			return false;
		}
	}else if( mPlayerMode == MODE_PLAYER2 ){
		Movement* player1 = mCharacter[ 0 ];
		Movement* player2 = mCharacter[ 1 ];
		if( player1->mLives <= 0 && player2->mLives <= 0 ){
			return true;
		}else{
			return false;
		}
	}
	return false;
}

void DynamicObjectMini::resetCharacterPoint(){
	for( int i = 0; i < mCharacterNumber; i++ ){
		Movement* character = mCharacter[ i ];
		if( character->mVisible ){
			character->mFactCharacterX -= mSizeX * 100;
			character->mCharacterX -= mSizeX;
			for( int n = 0; n < WEAPONS; n++ ){
				for( int m = 0; m < WEAPON_NUM_MAX; m++ ){
					Weapon* weapon = character->getWeapon( n, m );
					if( !weapon ){ continue; }
					if( weapon->mStart ){
						weapon->mFactCharacterX -= mSizeX * 100;
						weapon->mCharacterX -= mSizeX;
					}
				}
			}
		}
	}
}

void DynamicObjectMini::appearCharacter( Movement* character, int x, int y ){
		character->mCharacterX = mCameraX + x * 16;
		character->mCharacterY = y * 16;
		character->mFactCharacterX = mCameraX * 100 + x * 16 * 100;
		character->mFactCharacterY = y * 16 * 100;
		character->mFarstCharacterX = mCameraX + x * 16;
		character->mFarstCharacterY = y * 16;
		character->mVisible = true;
}

void DynamicObjectMini::checkComputerNum(){
	int num = 0;
	for( int i = 0; i < mCharacterNumber; i++ ){
		if( mCharacter[ i ]->mVisible && mCharacter[ i ]->outCharacterID() == Character::OBJ_COMPUTER ){
			num++;
		}
	}
	if( num <= 0 ){
		int rand = GetRand( 1 ) + 1;
		while( true ){
			if( !mCharacter[ rand ]->mVisible && mCharacter[ rand ]->outCharacterID() == Character::OBJ_COMPUTER ){
				appearCharacter( mCharacter[ rand ], 10 + GetRand( 20 ), 10 );
				mCharacter[ rand ]->setBlowRate( 500 );
				int SmashNum = 0;
				if( mPlayerMode == MODE_PLAYER1 ){
					SmashNum = mCharacter[ 0 ]->mSmashNum;
				}else if( mPlayerMode == MODE_PLAYER2 ){
					SmashNum = mCharacter[ 0 ]->mSmashNum + mCharacter[ 1 ]->mSmashNum;
				}
				if( mDefficult < 9 && ( SmashNum % 10 ) == 0 ){
					mDefficult++;
				}
				mCharacter[ rand ]->changeDefficult( mDefficult );
				break;
			}
			rand++;
			if( rand > mCharacterNumber - 1 ){
				rand = 1;
			}
		}
	}
}

void DynamicObjectMini::setCameraX( int cameraX ){
	mCameraX = cameraX;
}

void DynamicObjectMini::getCharacterSmashNum( int* smashNum ){
	for( int i = 0; i < 4; i++ ){
		if( mCharacter[ i ] != 0 && mCharacter[ i ]->isPlayer() && mCharacter[ i ]->mPlayer - 1 < 4 ){
			smashNum[ mCharacter[ i ]->mPlayer - 1 ] = mCharacter[ i ]->mSmashNum;
		}
	}
}

DynamicObjectMini::PlayerMode DynamicObjectMini::getPlayerMode(){
	return mPlayerMode;
}
