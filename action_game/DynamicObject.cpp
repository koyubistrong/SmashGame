#include "DynamicObject.h"
#define WEAPON_NUM_MAX 5
#define WEAPONS 4

DynamicObject::DynamicObject( const char* filename, GameInfomation* gameInfomation, StaticObject* staticObject ) 
	: mSizeX( 0 ), mSizeY( 0 ), mCharacterNumber( 0 ), mLivesTime( 0 ), mStaticObject( staticObject ), mGameInfomation( gameInfomation ) {
	File* f = new File( filename );
	CSV* csv_setting = new CSV( "setting/setting.csv" );
	const int* data = f->getDataInt();
	const int* csvData = csv_setting->getDataInt();
	mCharacterObject = new Image( "pic/picture.png", 70, 16, 16 );
	mSizeX = f->getSizeX();
	mSizeY = f->getSizeY();
	SRand( ( unsigned int )time( NULL ) );
	mBlowRate = csvData[ 3 ] / 100.0;
	if( csvData[ 0 ] != 0 ){
		mModeLives = true;
	}else{
		mModeLives = false;
	}
	for( int i = 0; i < 256; i++ ){
		mCharacter[ i ] = 0;
	}
	int num = 0;
	for( int y = 0; y < mSizeY; y++ ){
		for( int x = 0; x < mSizeX; x++ ){
			int blockID = data[ y * mSizeX + x ] & 0x0000FFFF;
			int info = ( data[ y * mSizeX + x ] & 0xFFFF0000 ) >> 16;
			/*
			if( info == 0 && mCharacterNumber < 8 ){
				mCharacter[ mCharacterNumber ] = new Computer( 0, mCharacterNumber + 1, mCharacter, staticObject );
				mCharacter[ mCharacterNumber ]->setCharacterID( 10 );
				mCharacter[ mCharacterNumber ]->mCharacterX = 16 * 16;
				mCharacter[ mCharacterNumber ]->mCharacterY = y * 16;
				mCharacter[ mCharacterNumber ]->mFactCharacterX = 16 * 16 * 100;
				mCharacter[ mCharacterNumber ]->mFactCharacterY = y * 16 * 100;
				mCharacter[ mCharacterNumber ]->mFarstCharacterX = 16 * 16;
				mCharacter[ mCharacterNumber ]->mFarstCharacterY = y * 16;
				mCharacter[ mCharacterNumber ]->mTimeCOM = GetRand( 1000 );
				mCharacter[ mCharacterNumber ]->setCharacterHandle( mCharacterObject->getHandle( data[ 0 ] ) );
				mCharacter[ mCharacterNumber ]->mLives = csvData[ 2 ];
				mCharacterNumber++;
			}
			*/
			
			if( info == 99 && num < 4 && mGameInfomation[ num ].characterID != -1 ){
				if( !mGameInfomation[ num ].computerFlag ){
					mCharacter[ mCharacterNumber ] = new Player( mGameInfomation[ num ].characterID, num + 1 );
					mCharacter[ mCharacterNumber ]->setCharacterID( 0 );
				}else{
					mCharacter[ mCharacterNumber ] = new Computer( mGameInfomation[ num ].characterID, num + 1, gameInfomation[ num ].computerDefficult, mCharacter, staticObject );
					mCharacter[ mCharacterNumber ]->setCharacterID( 10 );
				}
				mCharacter[ mCharacterNumber ]->mCharacterX = x * 16;
				mCharacter[ mCharacterNumber ]->mCharacterY = y * 16;
				mCharacter[ mCharacterNumber ]->mFactCharacterX = x * 16 * 100;
				mCharacter[ mCharacterNumber ]->mFactCharacterY = y * 16 * 100;
				mCharacter[ mCharacterNumber ]->mFarstCharacterX = x * 16;
				mCharacter[ mCharacterNumber ]->mFarstCharacterY = y * 16;
				mCharacter[ mCharacterNumber ]->mTimeCOM = GetRand( 1000 );
				mCharacter[ mCharacterNumber ]->setCharacterHandle( mCharacterObject->getHandle( data[ 0 ] ) );
				mCharacter[ mCharacterNumber ]->mTeamID = num;
				mCharacter[ mCharacterNumber ]->mLives = csvData[ 2 ];
				mCharacterNumber++;
			}
			
			if( info == 99 ){
				num++;
			}
		}
	}
	delete f;
	delete csv_setting;
}

DynamicObject::~DynamicObject(){
	for( int i = 0; i < mCharacterNumber; i++ ){
		delete mCharacter[ i ];
		mCharacter[ i ]= 0;
	}
	delete mCharacterObject;
	mCharacterObject = 0;
}

void DynamicObject::update(){
	int x, y;
	for( int i = 0; i < mCharacterNumber; i++ ){
		Movement* character = mCharacter[ i ];
		if( !character->mVisible ){
			if( mModeLives && character->mLives <= 0 ){
				continue;
			}
			if( character->mAppearingTime >= 180 ){
				character->mVisible = true;
			}
			character->mAppearingTime++;
			continue;
		}
		checkFallCharacter( character, &x, &y );
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
	mLivesTime++;
	//checkHitEnemy();
}

void DynamicObject::drawCharacter( int cameraX, int cameraY ){
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

void DynamicObject::checkMoveCharacter( Movement* character, int* x, int* y  ){
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

void DynamicObject::moveCharacter( Movement* character, int check ){
	if( check == 0 ){
		//( character->mSpeedX < 100 && character->mSpeedX > -100 ) ? 0 : 
		character->mFactCharacterX += character->mSpeedX;
		character->mFactCharacterY += character->mSpeedY;
	}else if( check == 1 ){
		if( character->getDamaged() >= 2 ){
			character->mSpeedX = -character->mSpeedX * 0.5;
			PlaySoundMem( character->mSoundCollisionHandle, DX_PLAYTYPE_BACK, true );
		}
		character->mFactCharacterY += character->mSpeedY;
	}else if( check == 2 ){
		if( character->getDamaged() >= 2 ){
			if( character->mSpeedY < 0 ){
				character->mSpeedY = -character->mSpeedY;
			}else{
				character->mSpeedY = -character->mSpeedY * 0.5;
			}
			if( abs( character->mSpeedY ) < 200 ){
				character->mSpeedY = 0;
			}
			PlaySoundMem( character->mSoundCollisionHandle, DX_PLAYTYPE_BACK, true );
		}
		if( character->mSpeedY > 0 ){
			character->mLanding = true;
		}
		character->mFactCharacterX += character->mSpeedX;
	}else if( check == 3 ){
		//動かさない
		if( character->getDamaged() >= 2 ){
			character->mSpeedX = -character->mSpeedX * 0.5;
			character->mSpeedY = -character->mSpeedY * 0.5;
			PlaySoundMem( character->mSoundCollisionHandle, DX_PLAYTYPE_BACK, true );
		}
	}
	character->mCharacterX = character->mFactCharacterX * 0.01;
	character->mCharacterY = character->mFactCharacterY * 0.01;
}

void DynamicObject::setCharacterPoint( Movement* character, int x, int y ){
	if( x == -1 ){
		character->mFactCharacterY = y * 100;
		character->mCharacterY = y;
	}else if( y == -1 ){
		character->mFactCharacterX = x * 100;
		character->mCharacterX = x;
	}else{
		character->mFactCharacterX = x * 100;
		character->mFactCharacterY = y * 100;
		character->mCharacterX = x;
		character->mCharacterY = y;
	}
}

void DynamicObject::setSpeedY( Movement* character, int speedY ){
	character->mSpeedY = speedY;
}

void DynamicObject::setJumpNum( Movement* character, bool jump ){
	if( jump ){
		character->mLanding = false;
		if( character->mJumpNum == 0 ){
			character->mJumpNum = 1;
		}
	}else{
		character->mJumpNum = 0;
	}
}

void DynamicObject::movePlayer( Movement* character ){
	if( ( CheckHitKey( character->mKey.KEY_RIGHT ) || character->mMoveX >= 1 ) 
		&& !character->getMask() ){
		if( character->mSpeedX <= character->mMaxMoveSpeed / 2 && character->mMoveX == 1 ){
			character->mSpeedX += character->mMoveSpeed;
		}else if( character->mSpeedX <= character->mMaxMoveSpeed && character->mMoveX == 2 ){
			character->mSpeedX += character->mMoveSpeed;
		}
	}
	if( ( CheckHitKey( character->mKey.KEY_LEFT ) || character->mMoveX <= -1 ) && !character->getMask() ){
		if( character->mSpeedX >= -character->mMaxMoveSpeed / 2 && character->mMoveX == -1 ){
			character->mSpeedX += -character->mMoveSpeed;
		}else if( character->mSpeedX >= -character->mMaxMoveSpeed && character->mMoveX == -2 ){
			character->mSpeedX += -character->mMoveSpeed;
		}
	}
	if( ( CheckHitKey( character->mKey.KEY_DOWN ) || character->mMoveY == -2 ) && !character->getMask() ){
		if( character->mJumpNum >= 1 ){
			character->mSpeedY = character->mMaxFallSpeed + 200;
		}
	}
	if( character->getMask() || !CheckHitKey( character->mKey.KEY_LEFT ) && 
		!CheckHitKey( character->mKey.KEY_RIGHT ) && abs( character->mMoveX ) == 0 ){
		if( character->mSpeedX <= 40 && character->mSpeedX >= -40 ){
			character->mSpeedX = 0;
		}else if( character->mSpeedX > 40 && character->mJumpNum >= 1 && character->getDamaged() == 0 ){
			character->mSpeedX += -1;
		}else if( character->mSpeedX < -40 && character->mJumpNum >= 1 && character->getDamaged() == 0 ){
			character->mSpeedX += 1;
		}else if( character->mSpeedX > 40 ){
			character->mSpeedX += -10;
		}else if( character->mSpeedX < -40 ){
			character->mSpeedX += 10;
		}
	}
	if( ( CheckHitKey( character->mKey.KEY_JUMP ) || character->mMoveY >= 1 )
		&& !character->mPrevJump && !character->getMask() && character->mJumpNum <= 1 ){
		if( character->mMoveY == 1 ){
			character->mSpeedY = -character->mFirstJumpSpeed / 2;
		}else if( character->mMoveY == 2 ){
			character->mSpeedY = -character->mFirstJumpSpeed;
		}
		PlaySoundMem( character->mSoundJumpHandle, DX_PLAYTYPE_BACK, true );
		character->mJumpNum++;
		character->mPrevJump = true;
	}else if( character->mJumpNum >= 1 ){
		if( character->mSpeedY <= character->mMaxFallSpeed ){
			character->mSpeedY += character->mFallSpeed;
		}
	}
	if( character->getDamaged() >= 2 ){
		if( sqrt( ( double )character->mSpeedX * character->mSpeedX + ( double )character->mSpeedY * character->mSpeedY ) <= 300
			|| character->mSpeedY <= character->mMaxFallSpeed && character->mSpeedY >= 100 ){
			character->setDamaged( 0 );
		}
	}
	if( !CheckHitKey( character->mKey.KEY_JUMP ) && character->mMoveY == 0 ){
		character->mPrevJump = false;
	}
}

void DynamicObject::moveWeapon( Movement* character, Weapon* weapon ){
	if( weapon->mVisible ){
		weapon->mFactCharacterX += weapon->mSpeedX;
		weapon->mFactCharacterY += weapon->mSpeedY;
		weapon->mCharacterX = weapon->mFactCharacterX / 100;
		weapon->mCharacterY = weapon->mFactCharacterY / 100;
	}
}

bool DynamicObject::checkMoveWeapon( Weapon* weapon, int startAttackerX, int startAttackerY, int endAttackerX, int endAttackerY ){
	int leftUpper[ 2 ] = { startAttackerX / 16, startAttackerY / 16 };
	int rightUpper[ 2 ] = { endAttackerX / 16, startAttackerY / 16 };
	int leftLower[ 2 ] = { startAttackerX / 16, endAttackerY / 16 };
	int rightLower[ 2 ] = { endAttackerX / 16, endAttackerY / 16 };
	if( mStaticObject->hitStaticObjectCheck( leftUpper ) ||
		mStaticObject->hitStaticObjectCheck( rightUpper ) ||
		mStaticObject->hitStaticObjectCheck( leftLower ) ||
		mStaticObject->hitStaticObjectCheck( rightLower ) ){
			weapon->resetWeapon();
			return true;
	}
	return false;
}

void DynamicObject::moveComputer( Movement* character ){
	if( character->mMoveX >= 1 && !character->getMask() ){
		if( character->mSpeedX <= character->mMaxMoveSpeed / 2 && character->mMoveX == 1 ){
			character->mSpeedX += character->mMoveSpeed;
		}else if( character->mSpeedX <= character->mMaxMoveSpeed && character->mMoveX == 2 ){
			character->mSpeedX += character->mMoveSpeed;
		}
	}
	if( character->mMoveX <= -1 && !character->getMask() ){
		if( character->mSpeedX >= -character->mMaxMoveSpeed / 2 && character->mMoveX == -1 ){
			character->mSpeedX += -character->mMoveSpeed;
		}else if( character->mSpeedX >= -character->mMaxMoveSpeed && character->mMoveX == -2 ){
			character->mSpeedX += -character->mMoveSpeed;
		}
	}
	if( character->mMoveY == -2 && !character->getMask() ){
		if( character->mJumpNum >= 1 ){
			character->mSpeedY = character->mMaxFallSpeed + 200;
		}
	}
	if( character->getMask() || character->mMoveX == 0 ){
		if( character->mSpeedX <= 40 && character->mSpeedX >= -40 ){
			character->mSpeedX = 0;
		}else if( character->mSpeedX > 40 && character->mJumpNum >= 1 && character->getDamaged() == 0 ){
			character->mSpeedX += -1;
		}else if( character->mSpeedX < -40 && character->mJumpNum >= 1 && character->getDamaged() == 0 ){
			character->mSpeedX += 1;
		}else if( character->mSpeedX > 40 ){
			character->mSpeedX += -10;
		}else if( character->mSpeedX < -40 ){
			character->mSpeedX += 10;
		}
	}
	if( character->mMoveY >= 1 && !character->mPrevJump && !character->getMask() && character->mJumpNum <= 1 ){
		if( character->mMoveY == 1 ){
			character->mSpeedY = -character->mFirstJumpSpeed / 2;
		}else if( character->mMoveY == 2 ){
			character->mSpeedY = -character->mFirstJumpSpeed;
		}
		PlaySoundMem( character->mSoundJumpHandle, DX_PLAYTYPE_BACK, true );
		character->mJumpNum++;
		character->mPrevJump = true;
	}else if( character->mJumpNum >= 1 ){
		if( character->mSpeedY <= character->mMaxFallSpeed ){
			character->mSpeedY += character->mFallSpeed;
		}
	}
	if( character->getDamaged() >= 2 ){
		if( sqrt( ( double )character->mSpeedX * character->mSpeedX + ( double )character->mSpeedY * character->mSpeedY ) <= 300
			|| character->mSpeedY <= character->mMaxFallSpeed && character->mSpeedY >= 100 ){
			character->setDamaged( 0 );
		}
	}
	//printfDx( "%lf\n", sqrt( ( double )character->mSpeedX * character->mSpeedX + ( double )character->mSpeedY * character->mSpeedY ) );
	if( character->mMoveY == 0 ){
		character->mPrevJump = false;
	}
}

void DynamicObject::moveEnemy2( Movement* character ){
	int distanceX = mCharacter[ mPlayerID ]->mCharacterX - character->mCharacterX;
	if( distanceX < 0 ){
		distanceX *= -1;
	}
	if( distanceX <= 16 * 480 / 16 / 2 ){
		if( character->mCharacterX >= mCharacter[ mPlayerID ]->mCharacterX ){
			if( character->mSpeedX >= -100 ){
				character->mSpeedX -= 50;
			}
		}else if( character->mCharacterX <= mCharacter[ mPlayerID ]->mCharacterX ){
			if( character->mSpeedX <= 100 ){
				character->mSpeedX += 100;
			}
		}
	}
	if( ( character->mTimeCOM % 10 ) == 0 && character->mJumpNum == 0 ){
		character->mSpeedY = -450;
		character->mJumpNum++;
	}else if( character->mJumpNum >= 1 ){
		if( character->mSpeedY <= 300 ){
			character->mSpeedY += 15;
		}
	}
	if( character->mTimeCOM > 1000 ){
		character->mTimeCOM = 0;
	}
	character->mTimeCOM++;
}

void DynamicObject::checkHitEnemy(){
	for( int i = 0; i < mCharacterNumber; i++ ){
		if( !( i == mPlayerID ) ){
			if( checkHit( mPlayerID, i ) ){
				//mCharacter[ mPlayerID ]->mCharacterX = mCharacter[ mPlayerID ]->mFarstCharacterX;
				//mCharacter[ mPlayerID ]->mCharacterY = mCharacter[ mPlayerID ]->mFarstCharacterY;
			}
		}
	}
}

bool DynamicObject::checkHitStar(){
	return false;
}

bool DynamicObject::checkHitAttack1( int i ){
	Movement* character = mCharacter[ i ];
	int me_sizeX;
	int me_sizeY;
	int pointNum = character->getHanteiPointNum();
	int info;
	int startX;
	int startY;
	int endX;
	int endY;
	character->getHanteiSize( &me_sizeX, &me_sizeY );
	for( int n = 0; n < pointNum; n++ ){
		character->getHanteiPoint( n, &info, &startX, &startY, &endX, &endY );
		if( info == 2 ){
			startX += character->mCharacterX + character->getCorrectionX() - me_sizeX / 2;
			startY += character->mCharacterY + character->getCorrectionY() - me_sizeY / 2;
			endX += character->mCharacterX + character->getCorrectionX() - me_sizeX / 2;
			endY += character->mCharacterY + character->getCorrectionY() - me_sizeY / 2;
			if( checkHitAttack2( i, startX, startY, endX, endY ) ){
				return true;
			}
		}
	}
	return false;
}

bool DynamicObject::checkHitAttack2( int attacker, int startAttackerX, int startAttackerY, int endAttackerX, int endAttackerY ){
	int me_sizeX;
	int me_sizeY;
	for( int i = 0; i < mCharacterNumber; i++ ){
		Movement* character = mCharacter[ i ];
		if( attacker != i && character->mTeamID != mCharacter[ attacker ]->mTeamID && mCharacter[ attacker ]->getHitHantei( i ) && character->mVisible ){
			character->getHanteiSize( &me_sizeX, &me_sizeY );
			for( int y = 0; y < me_sizeY; y++ ){
				for( int x = 0; x < me_sizeX; x++ ){
					if( character->getHantei( x, y ) == 1 ){
						int ux = x + character->mCharacterX + character->getCorrectionX() - me_sizeX / 2;
						int uy = y + character->mCharacterY + character->getCorrectionY() - me_sizeY / 2;
						if( startAttackerX <= ux && endAttackerX >= ux && startAttackerY <= uy && endAttackerY >= uy ){
							character->setDirectionRight( mCharacter[ attacker ]->getDirectionRight() );
							character->mSpeedX = mCharacter[ attacker ]->getBlowX() * character->getBlowRate() / 100 * mBlowRate;
							character->mSpeedY = mCharacter[ attacker ]->getBlowY() * character->getBlowRate() / 100 * mBlowRate;
							character->mBlowPlayer = attacker;
							character->setBlowRate( character->getBlowRate() + mCharacter[ attacker ]->getDamage() );
							mCharacter[ attacker ]->setHitHantei( i, false );
							PlaySoundMem( mCharacter[ attacker ]->getHitSound(), DX_PLAYTYPE_BACK, true );
							if( character->mVibration ){
								int vibrationTime = sqrt( ( double )character->mSpeedX * character->mSpeedX + ( double )character->mSpeedY * character->mSpeedY ) / 5;
								StartJoypadVibration( DX_INPUT_PAD1 << ( character->mPlayer - 1 ), 500, vibrationTime );
							}
							if( ( abs( character->mSpeedX ) >= 450 || abs( character->mSpeedY ) >= 450 ) ){
								character->setDamaged( 3 );
							}else if( abs( character->mSpeedX ) >= 250 || abs( character->mSpeedY ) >= 250 ){
								character->setDamaged( 2 );
							}else{
								character->setDamaged( 1 );
							}
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool DynamicObject::checkHitWeapon1( int i, Weapon* weapon ){
	int me_sizeX;
	int me_sizeY;
	int info;
	int startX;
	int startY;
	int endX;
	int endY;
	if( !weapon->mVisible ){ return false; }
	weapon->getHanteiSize( &me_sizeX, &me_sizeY );
	int pointNum = weapon->getHanteiPointNum();
	for( int l = 0; l < pointNum; l++ ){
		weapon->getHanteiPoint( l, &info, &startX, &startY, &endX, &endY );
		if( info == 2 ){
			startX += weapon->mCharacterX + weapon->getCorrectionX() - me_sizeX / 2;
			startY += weapon->mCharacterY + weapon->getCorrectionY() - me_sizeY / 2;
			endX += weapon->mCharacterX + weapon->getCorrectionX() - me_sizeX / 2;
			endY += weapon->mCharacterY + weapon->getCorrectionY() - me_sizeY / 2;
			if( checkHitWeapon2( i, weapon, startX, startY, endX, endY ) ){
				return true;
			}
			if( checkMoveWeapon( weapon, startX, startY, endX, endY ) ){
				return false;
			}
		}
	}
	return false;
}

bool DynamicObject::checkHitWeapon2( int attacker, Weapon* weapon, int startAttackerX, int startAttackerY, int endAttackerX, int endAttackerY ){
	int me_sizeX;
	int me_sizeY;
	for( int i = 0; i < mCharacterNumber; i++ ){
		Movement* character = mCharacter[ i ];
		if( attacker != i && character->mTeamID != mCharacter[ attacker ]->mTeamID && weapon->getHitHantei( i ) && character->mVisible ){
			character->getHanteiSize( &me_sizeX, &me_sizeY );
			for( int y = 0; y < me_sizeY; y++ ){
				for( int x = 0; x < me_sizeX; x++ ){
					if( character->getHantei( x, y ) == 1 ){
						int ux = x + character->mCharacterX + character->getCorrectionX() - me_sizeX / 2;
						int uy = y + character->mCharacterY + character->getCorrectionY() - me_sizeY / 2;
						if( startAttackerX <= ux && endAttackerX >= ux && startAttackerY <= uy && endAttackerY >= uy ){
							character->setDirectionRight( weapon->getDirectionRight() );
							character->mSpeedX = weapon->getBlowX() * character->getBlowRate() / 100 * mBlowRate;
							character->mSpeedY = weapon->getBlowY() * character->getBlowRate() / 100 * mBlowRate;
							character->mBlowPlayer = attacker;
							character->setBlowRate( character->getBlowRate() + weapon->getDamage() );
							weapon->setHitHantei( i, false );
							weapon->resetWeapon();
							PlaySoundMem( weapon->getHitSound(), DX_PLAYTYPE_BACK, true );
							if( character->mVibration ){
								int vibrationTime = sqrt( ( double )character->mSpeedX * character->mSpeedX + ( double )character->mSpeedY * character->mSpeedY ) / 5;
								StartJoypadVibration( DX_INPUT_PAD1 << ( character->mPlayer - 1 ), 500, vibrationTime );
							}
							if( ( abs( character->mSpeedX ) >= 650 || abs( character->mSpeedY ) >= 650 ) ){
								character->setDamaged( 3 );
							}else if( abs( character->mSpeedX ) >= 400 || abs( character->mSpeedY ) >= 400 ){
								character->setDamaged( 2 );
							}else{
								character->setDamaged( 1 );
							}
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

int DynamicObject::checkMoveCharacter( Movement* character, int x, int y ){
	int tx, ty;
	getCharacterPoint( character, &tx, &ty );
	int tLeftUpper[ 2 ] = { tx >> 4, ty >> 4 };
	int tRightUpper[ 2 ] = { ( tx + 15 ) >> 4, ty >> 4 };
	int tLeftLower[ 2 ] = { tx >> 4, ( ty + 15 ) >> 4 };
	int tRightLower[ 2 ] = { ( tx + 15 ) >> 4, ( ty + 15 ) >> 4 };
	bool characterFall = character->mSpeedY > 0; 
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
		if( characterFall ){
			if( rightLower[ 1 ] > tRightLower[ 1 ] && character->mMoveY >= 0 ){
				if( !mStaticObject->hitCharacterFallStaticObjectCheck( tRightLower ) &&
					!mStaticObject->hitCharacterFallStaticObjectCheck( tLeftLower ) &&
					( mStaticObject->hitCharacterFallStaticObjectCheck( rightLower ) ||
					mStaticObject->hitCharacterFallStaticObjectCheck( leftLower ) ) ){
						setCharacterPoint( character, -1, ( rightLower[ 1 ] - 1 ) * 16 );
						return 2;
				}
			}
		}
		//左上角衝突
		if( mStaticObject->hitStaticObjectCheck( leftUpper ) &&
			mStaticObject->hitStaticObjectCheck( rightUpper ) &&
			mStaticObject->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, ( leftUpper[ 0 ] + 1 ) * 16, ( leftUpper[ 1 ] + 1 ) * 16 );
				return 3;
		}
		//左下角衝突
		if( mStaticObject->hitStaticObjectCheck( leftUpper ) &&
			mStaticObject->hitStaticObjectCheck( rightLower ) &&
			mStaticObject->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, ( leftLower[ 0 ] + 1 ) * 16, ( leftLower[ 1 ] - 1 ) * 16 );
				return 3;
		}
		//右上角衝突
		if( mStaticObject->hitStaticObjectCheck( leftUpper ) &&
			mStaticObject->hitStaticObjectCheck( rightUpper ) &&
			mStaticObject->hitStaticObjectCheck( rightLower ) ){
				setCharacterPoint( character, ( rightUpper[ 0 ] - 1 ) * 16, ( rightUpper[ 1 ] + 1 ) * 16 );
				return 3;
		}
		//右下角衝突
		if( mStaticObject->hitStaticObjectCheck( rightUpper ) &&
			mStaticObject->hitStaticObjectCheck( rightLower ) &&
			mStaticObject->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, ( rightLower[ 0 ] - 1 ) * 16, ( rightLower[ 1 ] - 1 ) * 16 );
				return 3;
		}
		//上衝突（２ブロック）
		if( mStaticObject->hitStaticObjectCheck( rightUpper ) &&
			mStaticObject->hitStaticObjectCheck( leftUpper ) ){
				setCharacterPoint( character, -1, ( rightUpper[ 1 ] + 1 ) * 16 );
				return 2;
		}
		//下衝突（２ブロック）
		if( mStaticObject->hitStaticObjectCheck( rightLower ) &&
			mStaticObject->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, -1, ( rightLower[ 1 ] - 1 ) * 16 );
				return 2;
		}
		//右衝突（２ブロック）
		if( mStaticObject->hitStaticObjectCheck( rightLower ) &&
			mStaticObject->hitStaticObjectCheck( rightUpper ) ){
				setCharacterPoint( character, ( rightLower[ 0 ] - 1 ) * 16, -1 );
				return 1;
		}
		//左衝突（２ブロック）
		if( mStaticObject->hitStaticObjectCheck( leftUpper ) &&
			mStaticObject->hitStaticObjectCheck( leftLower ) ){
				setCharacterPoint( character, ( leftLower[ 0 ] + 1 ) * 16, -1 );
				return 1;
		}
		if( mStaticObject->hitStaticObjectCheck( rightUpper ) && rightUpper[ 1 ] < tRightUpper[ 1 ] ){
			//上衝突（１ブロック）
				setCharacterPoint( character, -1, ( rightUpper[ 1 ] + 1 ) * 16 );
				return 2;
		}else if( mStaticObject->hitStaticObjectCheck( rightUpper ) && rightUpper[ 1 ] == tRightUpper[ 1 ] ){
			//右衝突（１ブロック）
				setCharacterPoint( character, ( rightUpper[ 0 ] - 1 ) * 16, -1 );
				return 1;
		}
		if( mStaticObject->hitStaticObjectCheck( rightLower ) && rightLower[ 1 ] > tRightLower[ 1 ] ){
			//下衝突（１ブロック）
				setCharacterPoint( character, -1, ( rightLower[ 1 ] - 1 ) * 16 );
				return 2;
		}else if( mStaticObject->hitStaticObjectCheck( rightLower ) && rightLower[ 1 ] == tRightLower[ 1 ] ){
			//右衝突（１ブロック）
				setCharacterPoint( character, ( rightLower[ 0 ] - 1 ) * 16, -1 );
				return 1;
		}
		if( mStaticObject->hitStaticObjectCheck( leftUpper ) && leftUpper[ 1 ] < tLeftUpper[ 1 ] ){
			//上衝突（１ブロック）
				setCharacterPoint( character, -1, ( leftUpper[ 1 ] + 1 ) * 16 );
				return 2;
		}else if( mStaticObject->hitStaticObjectCheck( leftUpper ) && leftUpper[ 1 ] == tLeftUpper[ 1 ] ){
			//左衝突（１ブロック）
				setCharacterPoint( character, ( leftUpper[ 0 ] + 1 ) * 16, -1 );
				return 1;
		}
		if( mStaticObject->hitStaticObjectCheck( leftLower ) && leftLower[ 1 ] > tLeftLower[ 1 ] ){
			//下衝突（１ブロック）
				setCharacterPoint( character, -1, ( leftLower[ 1 ] - 1 ) * 16 );
				return 2;
		}else if( mStaticObject->hitStaticObjectCheck( leftLower ) && leftLower[ 1 ] == tLeftLower[ 1 ] ){
			//左衝突（１ブロック）
				setCharacterPoint( character, ( leftLower[ 0 ] + 1 ) * 16, -1 );
				return 1;
		}
	}
	return 0;
}

void DynamicObject::checkFallCharacter( Movement* character, int* x, int* y ){
	int tx, ty;
	getCharacterPoint( character, &tx, &ty );
	int tLeftLower[ 2 ] = { tx >> 4, ( ty + 16 ) >> 4 };
	int tRightLower[ 2 ] = { ( tx + 15 ) >> 4, ( ty + 16 ) >> 4 };
	int leftLower[ 2 ] = { *x >> 4, ( *y + 16 ) >> 4 };
	int rightLower[ 2 ] = { ( *x + 15 ) >> 4, ( *y + 16 ) >> 4 };
	if( mStaticObject->hitStaticObjectCheck( tRightLower ) ||
		mStaticObject->hitStaticObjectCheck( tLeftLower ) ){
			if( character->getDamaged() == 0 && character->mSpeedY > 0 ){
				setJumpNum( character, false );
				setSpeedY( character, 0 );
			}
	}else if( mStaticObject->hitCharacterFallStaticObjectCheck( tRightLower ) ||
			  mStaticObject->hitCharacterFallStaticObjectCheck( tLeftLower ) ){
			if( character->mLanding && character->getDamaged() == 0 && character->mSpeedY > 0 ){
				setJumpNum( character, false );
				setSpeedY( character, 0 );
			}
			if( character->mMoveY == -2 ){
				setJumpNum( character, true );
			}
	}else{
			setJumpNum( character, true );
	}
}

void DynamicObject::checkStageRangeCharacter( int i, Movement* character ){
	if( character->mCharacterX < -16 * 10 || character->mCharacterX > ( mSizeX + 10 ) * 16
		|| character->mCharacterY < -16 * 10 || character->mCharacterY > ( mSizeY + 10 ) * 16 ){
			if( character->mBlowPlayer >= 0 ){
				mCharacter[ character->mBlowPlayer ]->mSmashNum++;
			}
			character->mFallNum++;
			if( mModeLives ){
				character->mLives--;
				if( character->mLives <= 0 ){
					if( character->mPlayer - 1 < 4 ){
						mGameInfomation[ character->mPlayer - 1 ].gameOverTime = mLivesTime;
					}
				}
			}
			PlaySoundMem( character->mSoundCrushedHandle, DX_PLAYTYPE_BACK, true );
			if( character->mVibration ){
				StartJoypadVibration( DX_INPUT_PAD1 << ( character->mPlayer - 1 ), 500, 1000 );
			}
			character->resetCharacter();
	}
}

void DynamicObject::getCharacterPoint( Movement* character, int* x, int* y ){
	*x = character->mCharacterX;
	*y = character->mCharacterY;
}

void DynamicObject::getPlayerPoint( int* minX, int* minY, int* maxX, int* maxY ){
	int noVisibleNum = 0;
	*minX = ( mSizeX + 1 ) * 16 * 100;
	*minY = ( mSizeY + 1 ) * 16 * 100;
	*maxX = -16 * 100;
	*maxY = -16 * 100;
	for( int i = 0; i < mCharacterNumber; i++ ){
		Movement* character = mCharacter[ i ];
		if( character->mLives == 0 ){
			noVisibleNum++;
			continue;
		}
		/*
		if( !character->mVisible ){
			noVisibleNum++;
		}
		*/
		if( *minX > character->mFactCharacterX ){
			*minX = character->mFactCharacterX;
		}
		if( *minY > character->mFactCharacterY ){
			*minY = character->mFactCharacterY;
		}
		if( *maxX < character->mFactCharacterX ){
			*maxX = character->mFactCharacterX;
		}
		if( *maxY < character->mFactCharacterY ){
			*maxY = character->mFactCharacterY;
		}
	}
	if( noVisibleNum == mCharacterNumber ){
		*minX = 0;
		*minY = 0;
		*maxX = mSizeX * 16 * 100;
		*maxY = mSizeY * 16 * 100;
	}
	mPlayerID = 0;
}

void DynamicObject::getCharacterBlowRate( int* blowRate ){
	for( int i = 0; i < 4; i++ ){
		if( mCharacter[ i ] != 0 && mCharacter[ i ]->mPlayer - 1 < 4 ){
			blowRate[ mCharacter[ i ]->mPlayer - 1 ] = mCharacter[ i ]->getBlowRate();
		}
	}
}

void DynamicObject::getCharacterLivesNum( int* livesNum ){
	for( int i = 0; i < 4; i++ ){
		if( mCharacter[ i ] != 0 && mCharacter[ i ]->mPlayer - 1 < 4 ){
			livesNum[ mCharacter[ i ]->mPlayer - 1 ] = mCharacter[ i ]->mLives;
		}
	}
}

void DynamicObject::getCharacterSmashNum( int* smashNum ){
	for( int i = 0; i < 4; i++ ){
		if( mCharacter[ i ] != 0 && mCharacter[ i ]->mPlayer - 1 < 4 ){
			smashNum[ mCharacter[ i ]->mPlayer - 1 ] = mCharacter[ i ]->mSmashNum;
		}
	}
}

int DynamicObject::getLivesTime(){
	return mLivesTime;
}

int DynamicObject::getCharacterNumber(){
	return mCharacterNumber;
}

void DynamicObject::getResult(){
	for( int i = 0; i < 4; i++ ){
		Movement* character = mCharacter[ i ];
		if( character != 0 && character->mPlayer - 1 < 4 ){
			mGameInfomation[ character->mPlayer - 1 ].smashNum = character->mSmashNum;
			mGameInfomation[ character->mPlayer - 1 ].fallNum = character->mFallNum;
			mGameInfomation[ character->mPlayer - 1 ].livesNum = character->mLives;
		}
	}
}

bool DynamicObject::checkLives(){
	int lives = 0;
	for( int i = 0; i < mCharacterNumber; i++ ){
		Movement* character = mCharacter[ i ];
		if( character->mLives > 0 ){
			lives++;
		}
	}
	if( lives <= 1 ){
		return true;
	}else{
		return false;
	}
}

bool DynamicObject::checkHit( int i1, int i2 ){
	int characterX1[ 2 ] = { mCharacter[ i1 ]->mCharacterX, mCharacter[ i1 ]->mCharacterX + 15 };
	int characterY1[ 2 ] = { mCharacter[ i1 ]->mCharacterY, mCharacter[ i1 ]->mCharacterY + 15 };
	int characterX2[ 2 ] = { mCharacter[ i2 ]->mCharacterX, mCharacter[ i2 ]->mCharacterX + 15 };
	int characterY2[ 2 ] = { mCharacter[ i2 ]->mCharacterY, mCharacter[ i2 ]->mCharacterY + 15 };
	if( characterX1[ 1 ] >= characterX2[ 0 ] && characterX1[ 0 ] <= characterX2[ 1 ] ){
		if( characterY1[ 1 ] >= characterY2[ 0 ] && characterY1[ 0 ] <= characterY2[ 1 ] ){
			return true;
		}
	}
	return false;
}

