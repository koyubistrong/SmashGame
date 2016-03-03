#include "StaticObject.h"
#define PIC_SIZE 1026

StaticObject::StaticObject( const char* filename ) : mSizeX( 0 ), mSizeY( 0 ){
	mCellPicX = 16;
	mCellPicY = 16;
	for( int i = 0; i < 9; i++ ){
		string stagePic = "stage/pic2/";
		stagePic += filename;
		stagePic += "/picture";
		stagePic += static_cast< char >( i + 49 );
		stagePic += ".png";
		mStageObject[ i ] = new Image( stagePic.data(), PIC_SIZE, mCellPicX, mCellPicY );
	}
	string stageName = "stage/information/";
	stageName += filename;
	stageName += ".smh";
	File* f = new File( stageName.data() );
	const int* data = f->getDataInt();
	mSizeX = f->getSizeX();
	mSizeY = f->getSizeY();
	mBlockID.setSize( mSizeX, mSizeY );
	mBlockInfo.setSize( mSizeX, mSizeY );
	mStageHandle = MakeScreen( mSizeX * mCellPicX, mSizeY * mCellPicY, false );
	SetDrawScreen( mStageHandle );
	for( int y = 0; y < mSizeY; y++ ){
		for( int x = 0; x < mSizeX; x++ ){
			int blockID = data[ y * mSizeX + x ] & 0x0000FFFF;
			int info = ( data[ y * mSizeX + x ] & 0xFFFF0000 ) >> 16;
			mBlockID( x, y ) = blockID;
			mBlockInfo( x, y ) = info;
			mStageObject[ mBlockID( x, y ) / PIC_SIZE ]->drawImage2( mBlockID( x, y ) % PIC_SIZE, x * 16, y * 16 );
		}
	}
	SetDrawScreen( DX_SCREEN_BACK );
	mSizePicX = mStageObject[ 0 ]->getCellX();
	mSizePicY = mStageObject[ 0 ]->getCellY();
	delete f;
	for( int i = 0; i < 9; i++ ){
		delete mStageObject[ i ];
		mStageObject[ i ] = 0;
	}
}

StaticObject::~StaticObject(){
	DeleteGraph( mStageHandle );
}

void StaticObject::initiaLizeStage(){
	for( int y = 0; y < mSizeY; y++ ){
		for( int x = 0; x < mSizeX; x++ ){
			mBlockID( x, y ) = 0;
			mBlockInfo( x, y ) = 0;
		}
	}
}

void StaticObject::drawStage( int cameraX, int cameraY ){
	/*
	const double displaySizeX = ( 640 + 32 ) / 16 / magnification;
	const double displaySizeY = ( 480 + 32 ) / 16 / magnification;
	double cx = cameraX / 16 / magnification;
	double cy = cameraY / 16 / magnification;
	for( int y = cy; y <= cy + displaySizeY; y++ ){
			if( 0 > y || mSizeY <= y ){
				continue;
			}
		for( int x = cx;x <= cx + displaySizeX; x++ ){
			if( 0 > x || mSizeX <= x ){
				continue;
			}
			int tx = x * 16 * magnification - cameraX;
			int ty = y * 16 * magnification - cameraY;
			//mStageObject[ mBlockID( x, y ) / PIC_SIZE ]->drawImage2( mBlockID( x, y ) % PIC_SIZE, tx, ty );
		}
	}
	*/
	DrawRotaGraph( -cameraX + mSizeX * 16 * 0.5 * magnification, -cameraY + mSizeY * 16 * 0.5 * magnification, magnification, 0, mStageHandle, true, false );
}

bool StaticObject::hitStaticObjectCheck( int* point ){
	if( 0 > point[ 0 ] || mSizeX <= point[ 0 ] || 0 > point[ 1 ] || mSizeY <= point[ 1 ] ){
		return false;
	}
	if( mBlockInfo( point[ 0 ], point[ 1 ] ) == 1 || mBlockInfo( point[ 0 ], point[ 1 ] ) == 2 ){
		return true;
	}
	return false;
}

bool StaticObject::hitCharacterFallStaticObjectCheck( int* point ){
	if( 0 > point[ 0 ] || mSizeX <= point[ 0 ] || 0 > point[ 1 ] || mSizeY <= point[ 1 ] ){
		return false;
	}
	if( mBlockInfo( point[ 0 ], point[ 1 ] ) == 3 ){
		return true;
	}
	return false;
}

void StaticObject::getStageSize( int *x, int *y ){
	*x = mSizeX * 16;
	*y = mSizeY * 16;
}

int StaticObject::getBlockInfo( int x, int y ){
	return mBlockInfo( x, y );
}