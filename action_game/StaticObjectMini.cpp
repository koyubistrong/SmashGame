#include "StaticObjectMini.h"
#define PIC_SIZE 1026

StaticObjectMini::StaticObjectMini( const char* filename ) : StaticObject(){
	mCellPicX = 16;
	mCellPicY = 16;
	mSizeX = 0;
	mSizeY = 0;
	for( int i = 0; i < 9; i++ ){
		string str = "pic/picture";
		str += static_cast< char >( i + 49 );
		str += ".png";
		mStageObject[ i ] = new Image( str.data(), PIC_SIZE, mCellPicX, mCellPicY );
	}
	mSizeX = 40 * 3;
	mSizeY = 30;
	mBlockID.setSize( mSizeX, mSizeY );
	mBlockInfo.setSize( mSizeX, mSizeY );
	mTempX = 0;
	mTempHeight = 0;
	mAppear = true;
	initiaLizeAppearCharacter();
	initiaLizeStage();
	createFlatStage( 0, 5, mSizeX );
	mSizePicX = mStageObject[ 0 ]->getCellX();
	mSizePicY = mStageObject[ 0 ]->getCellY();
	SRand( ( unsigned )time( NULL ) );
}

StaticObjectMini::~StaticObjectMini(){}

void StaticObjectMini::initiaLizeStage(){
	for( int y = 0; y < mSizeY; y++ ){
		for( int x = 0; x < mSizeX; x++ ){
			mBlockID( x, y ) = 0;
			mBlockInfo( x, y ) = 0;
		}
	}
}

void StaticObjectMini::initiaLizeAppearCharacter(){
	for( int i = 0; i < 4; i++ ){
		mAppearCharacter[ i ].flag = 0;
		mAppearCharacter[ i ].pointX = -1;
		mAppearCharacter[ i ].pointY = -1;
	}
}

void StaticObjectMini::drawStage( int cameraX, int cameraY ){
	const double displaySizeX = ( 640 + 32 ) / 16 / magnification;
	const double displaySizeY = ( 480 + 32 ) / 16 / magnification;
	double cx = cameraX / 16 / magnification;
	double cy = cameraY / 16 / magnification;
	for( int y = cy; y <= cy + displaySizeY; y++ ){
		if( 0 > y || mSizeY <= y ){
			continue;
		}
		for( int x = cx; x <= cx + displaySizeX; x++ ){
			int tx = x * 16 * magnification - cameraX;
			int ty = y * 16 * magnification - cameraY;
			if( 0 > x ){
				continue;
			}else if( mSizeX <= x ){
				mStageObject[ mBlockID( x - mSizeX, y ) / PIC_SIZE ]->drawImage2( mBlockID( x - mSizeX, y ) % PIC_SIZE, tx, ty );
				continue;
			}
			mStageObject[ mBlockID( x, y ) / PIC_SIZE ]->drawImage2( mBlockID( x, y ) % PIC_SIZE, tx, ty );
		}
	}
	//printfDx( "tempX : %d\n", mTempX );
}

void StaticObjectMini::crateStage(){
	for( int x = 0; x < 40; x++ ){
		for( int y = 0; y < 30; y++ ){
			mBlockID( x + mTempX, y ) = 0;
			mBlockInfo( x + mTempX, y ) = 0;
		}
	}
	int x = 0;
	int prevRand = 0;
	while( x < 40 ){
		int width = GetRand( 3 ) + 4;
		int rand = GetRand( 100 );
		if( x + width >= 40 ){
			width = 40 - x;
		}
		if( rand < 40 ){
			createFlatStage( mTempX, mTempHeight, width );
		}else if( rand < 50 && !( prevRand >= 40 && prevRand < 50 ) ){
			createNoStage( width );
		}else if( rand < 75 ){
			createUpStairsStage( mTempX, mTempHeight, width, 3 );
		}else if( rand < 100 ){
			createDownStairsStage( mTempX, mTempHeight, width, 3 );
		}
		if( prevRand >= 40 && prevRand < 50 ){
			createFlatStage( mTempX, mTempHeight, width );
		}
		mTempX += width;
		mTempX = mTempX % mSizeX;
		x += width;
		prevRand = rand;
	}
}

void StaticObjectMini::createFlatStage( int x, int height, int width  ){
	for( int tx = x; tx < width + x; tx++ ){
		createGround( tx, height );
	}
}

void StaticObjectMini::createNoStage( int width ){
	for( int i = 0; i < width; i++ ){
		//mTempX++;
		//mTempX = mTempX % mSizeX;
	}
}

void StaticObjectMini::createUpStairsStage( int x, int height, int width, int gradeWidth ){
	int addY = 0;
	for( int tx = x; tx < width + x; tx++ ){
		createGround( tx, height + addY / gradeWidth );
		addY++;
	}
}

void StaticObjectMini::createDownStairsStage( int x, int height, int width, int gradeWidth ){
	int addY = 0;
	for( int tx = x; tx < width + x; tx++ ){
		if( height - addY / gradeWidth <= 0 ){ 
			createGround( tx, 1 );
			addY++;
			continue;
		}
		createGround( tx, height - addY / gradeWidth );
		addY++;
	}
}

void StaticObjectMini::createGround( int x, int height ){
	if( height > 10 ){
		height = 10;
	}
	for( int ty = mSizeY - 1; ty >= mSizeY - height; ty-- ){
		if( ty <= 2 ){
			break;
		}
		mBlockID( x, ty ) = 1;
		mBlockInfo( x, ty ) = 1;
	}
	mTempHeight = height;
}

void StaticObjectMini::createGround( int x, int y, int height ){
	for( int ty = y; ty <= y + height; ty++ ){
		if( ty <= 2 || ty >= mSizeY ){
			break;
		}
		mBlockID( x, ty ) = 1;
		if( x == 0 ){
			mBlockID( x, ty ) = 2;
		}
		mBlockInfo( x, ty ) = 1;
	}
	mTempHeight = y;
}

bool StaticObjectMini::hitStaticObjectCheck( int* point ){
	if( 0 > point[ 1 ] || mSizeY <= point[ 1 ] ){
		return false;
	}
	if( 0 > point[ 0 ] ){
		if( mBlockInfo( point[ 0 ] + mSizeX, point[ 1 ] ) == 1 ){
			return true;
		}
	}
	if( mBlockInfo( point[ 0 ] % mSizeX, point[ 1 ] ) == 1 ){
		return true;
	}
	return false;
}

void StaticObjectMini::getStageSize( int *x, int *y ){
	*x = mSizeX * 16;
	*y = mSizeY * 16;
}

void StaticObjectMini::getAppearCharacterPoint( int *x, int *y ){
	for( int i = 0; i < 4; i++ ){
		if( mAppearCharacter[ i ].flag == 0 ){
			mAppearCharacter[ i ].flag = 1;
			x[ i ] = mAppearCharacter[ i ].pointX;
			y[ i ] = mAppearCharacter[ i ].pointY;
		}
	}
}

void StaticObjectMini::setAppearCharacterPoint( int x, int y ){
	mAppearCharacter[ 0 ].flag = 0;
	mAppearCharacter[ 0 ].pointX = x * 16;
	mAppearCharacter[ 0 ].pointY = y * 16;
}

void StaticObjectMini::setAppearFlag( bool appear ){
	mAppear = appear;
}