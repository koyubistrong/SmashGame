#include "Computer.h"

void Computer::setNextAction(){
	//nearCharacter->mCharacterX, nearCharacter->mCharacterY �͋߂��ɂ���G�̍��W�@������16��1�u���b�N��
	//this->mCharacterX, this->mCharacterY �͎����̍��W�@������16��1�u���b�N��
	//mPrecipice[ n ].pointX, mPrecipice[ n ].pointY  ��n�Ԗڂ̊R������ʒu�@������1��1�u���b�N��
	//mStaticObject->hitStaticObjectCheck( x, y )�@��(x,y)�ɓ����蔻�肪���邩�ǂ����̃`�F�b�N�@������1��1�u���b�N��
	//mPrecipiceNum�͊R�̐�
	//mNearDistance�͋߂��̓G�Ƃ̋���
	//mNextAction�͍U���̎w��
	//mMoveX = 2�ŉE�ړ��@mMoveX = -2�ō��ړ�
	//mMoveY = 2�ŃW�����v�@mMoveY = -2�ł��Ⴊ��
	//mDirectionRight��true�ŉE�����@false�ō�����
	//mTime�͎���  mTime��60�ň�b

	//������
	mMoveY = 0;
	mNextAction = ACT_STAND;
	int mCharacterpoint[ 2 ];

	//�߂��ɂ���G��������
	caluculateDistance();

	//���Ȃ��Ȃ�Ȃɂ����Ȃ�
	if( mNearEnemy == -1 ){
		mMoveX = 0;
		return;
	}

	Movement* nearCharacter = mMovement[ mNearEnemy ];

	mCharacterpoint[0]=this->mCharacterX / 16;mCharacterpoint[1]=this->mCharacterY / 16;

	if( abs( mTime ) % ( ( mMoveInterval + 1 ) * 2 ) <= mMoveInterval ){
		if( this->mCharacterX + 16 * 2 < nearCharacter->mCharacterX ){
			mMoveX = mMoveSpeed;
		}else if( this->mCharacterX - 16 * 1 > nearCharacter->mCharacterX ){
			mMoveX = -mMoveSpeed;
		}
		if( !mMask && this->mCharacterX < nearCharacter->mCharacterX ){
			mDirectionRight = true;
		}else if( !mMask && this->mCharacterX >= nearCharacter->mCharacterX ){
			mDirectionRight = false;
		}
		if( this->mCharacterY > nearCharacter->mCharacterY + 16 * 2 ){
			mMoveY = 2;
		}
	}

	if( this->mCharacterX < nearCharacter->mCharacterX + 16 * 2 && this->mCharacterX > nearCharacter->mCharacterX - 16 * 1 ){
		mMoveX = 0;
	}

	if( mPrecipice ){
		if( !( nearCharacter->mCharacterX < mPrecipice[mPrecipiceNum-1]->pointX*16 && nearCharacter->mCharacterX > mPrecipice[0]->pointX*16 ) ){
			mMoveX = 0;
		}
		if(!( this->mCharacterX < mPrecipice[ mPrecipiceNum-1 ]->pointX*16)){
			mMoveX = -2;
		}else if(!( this->mCharacterX > mPrecipice[ 0 ]->pointX*16)){
			mMoveX = 2;
		}
	}

	if( mDirectionRight ){
		mCharacterpoint[ 0 ] += 1;
	}else{
		mCharacterpoint[ 0 ] += -1;
	}

	if(mStaticObject->hitStaticObjectCheck(mCharacterpoint)){
		mMoveY = 2;
	}

	if( abs( mTime ) % ( mAttackInterval + 1 ) >= mAttackInterval ){
		int rand;
		rand = GetRand( 99 );
		if( rand < 10 ){
			mMoveY = 2;
		}else if( rand < 15 ){
			mNextAction = ACT_SIDE_WEAK_ATTACK;
		}else if( rand < 20 ){
			mNextAction = ACT_SIDE_HEAVY_ATTACK;
		}else if( rand < 25 ){
			mNextAction = ACT_SIDE_SMASH_ATTACK;
		}
		if( mNearDistance < 48 ){
			rand = GetRand( 99 );
			if( rand < 50 ){
				mNextAction = ACT_SIDE_WEAK_ATTACK; //�ʏ��U��
			}else if( rand < 80 ){
				mNextAction = ACT_SIDE_HEAVY_ATTACK; //�����U��
			}else if( rand < 100 ){
				mNextAction = ACT_SIDE_SMASH_ATTACK; //���X�}�b�V��
			}
		}else if( mNearDistance >= 48 && mNearDistance < 16 * 38 ){
			rand = GetRand( 99 );
			if( rand < 25 ){
				mNextAction = ACT_NORMAL_SP_ATTACK;  //�ʏ����U��
			}
		}
	}
	mTime++;
}

void Computer::setDefficult(){
	int moveInterval[] = { 30, 20, 10, 5, 5, 5, 5, 0, 0 };
	int moveSpeed[] = { 1, 1, 1, 1, 2, 2, 2, 2, 2 };
	int attackInterval[] ={ 360, 270, 180, 90, 60, 50, 40, 30, 10 };
	mMoveInterval = moveInterval[ mDefficult - 1 ];
	mMoveSpeed = moveSpeed[ mDefficult - 1 ];
	mAttackInterval = attackInterval[ mDefficult - 1 ];
}

void Computer::changeDefficult( int defficult ){
	int moveInterval[] = { 30, 20, 10, 5, 5, 5, 5, 0, 0 };
	int moveSpeed[] = { 1, 1, 1, 1, 2, 2, 2, 2, 2 };
	int attackInterval[] ={ 360, 270, 180, 90, 60, 50, 40, 30, 10 };
	mMoveInterval = moveInterval[ defficult - 1 ];
	mMoveSpeed = moveSpeed[ defficult - 1 ];
	mAttackInterval = attackInterval[ defficult - 1 ];
}

void Computer::caluculateDistance(){
	int min = 0x7FFFFFFF;
	int minPlayer = -1;
	for( int i = 0; i < 4; i++ ){
		if( mMovement[ i ] != 0 && mMovement[ i ] != this && mMovement[ i ]->mVisible && this->mTeamID != mMovement[ i ]->mTeamID ){
			double dX = mMovement[ i ]->mCharacterX - this->mCharacterX;
			double dY = mMovement[ i ]->mCharacterY - this->mCharacterY;
			int distance = sqrt( dX * dX + dY * dY );
			if( min > distance ){
				min = distance;
				minPlayer = i;
			}
			//printfDx( "%dDistance : %d\n", i, distance );
		}
	}
	mNearDistance = min;
	mNearEnemy = minPlayer;
}

bool Computer::isPlayer(){
	return false;
}