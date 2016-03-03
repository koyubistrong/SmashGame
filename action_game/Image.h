#ifndef INCLUDED_IMAGE_H
#define INCLUDED_IMAGE_H

#include "DxLib.h"
#include "extern.h"
#define PI 3.14159265359

class Image{
public:
	Image( const char* FileName, int AllNum, int cellX, int cellY )
		: mCellX( cellX ), mCellY( cellY ), mNum( AllNum ), mFileName( FileName ){
		int sizeX, sizeY, grHandle;
		grHandle = LoadGraph( mFileName );
		GetGraphSize( grHandle , &sizeX , &sizeY ) ;
		if( cellX == 0 )cellX = 1;
		if( cellY == 0 )cellY = 1;
		if( AllNum == 0 ) AllNum = 1;
		mNumX = sizeX / cellX;
		mNumY = sizeY / cellY;
		mHandle = new int[ AllNum ];
		if( LoadDivGraph( FileName, AllNum, mNumX, mNumY, mCellX, mCellY, mHandle ) != 0 ){
			AllNum = 1;
			mNumX = 1;
			mNumY = 1;
			mCellX = 16;
			mCellY = 16;
			mNum = 1;
			delete mHandle;
			mHandle = new int[ 1 ];
			LoadDivGraph( "pic/space.png", AllNum, mNumX, mNumY, mCellX, mCellY, mHandle );
			ClearDrawScreen();
			mSuccess = false;
			//printfDx( "%sÇ™ì«Ç›çûÇﬂÇ‹ÇπÇÒÇ≈ÇµÇΩÅB\n", FileName );
			//ScreenFlip();
			//WaitKey();
			//exit( 1 );
		}else{
			mSuccess = true;
		}
		DeleteGraph( grHandle );
	}
	~Image(){
		for( int i = 0; i < mNum; i++ ){
			DeleteGraph( mHandle[ i ] );
		}
		delete[] mHandle;
		mHandle = 0;
	}
	void drawImage( int o, int x, int y ){
		//DrawGraph( x, y, o, true );
		DrawRotaGraph( x, y, magnification + 0.0538, 0, o, true, false );
		//DrawRotaGraph( x, y, magnification, 0, o, true, false );
	}
	void drawImage2( int o, int x, int y ){
		//DrawGraph( x, y, mHandle[ o ], true );
		DrawRotaGraph( x, y, magnification + 0.0538, 0, mHandle[ o ], true, false );
		//DrawRotaGraph( x, y, magnification, 0, mHandle[ o ], true, false );
	}
	void drawImageLR( int o, int x, int y ){
		//DrawTurnGraph( x, y, mHandle[ o ], true );
		DrawRotaGraph( x, y, magnification, 0, mHandle[ o ], true, true );
	}
	void drawImageRota( int o, int x, int y, double degree ){
		//DrawGraph( x, y, mHandle[ o ], true );
		DrawRotaGraph( x, y, magnification, PI * degree / 180.0, mHandle[ o ], true, false );
	}
	void drawImageRotaLR( int o, int x, int y, double degree ){
		//DrawGraph( x, y, mHandle[ o ], true );
		DrawRotaGraph( x, y, magnification, PI * degree / 180.0, mHandle[ o ], true, true );
	}
	void drawImageCell( int o, int x, int y, int pointX, int pointY  ){
		DrawGraph( x * mCellX, y * mCellY, mHandle[ o ], true );
	}
	int getHandle( int o ){
		return mHandle[ o ];
	}
	int getCellX(){
		return mNumX;
	}
	int getCellY(){
		return mNumY;
	}
	bool getSuccess(){
		return mSuccess;
	}

private:
	int* mHandle;
	int mNumX;
	int mNumY;
	int mCellX;
	int mCellY;
	int mNum;
	bool mSuccess;
	const char* mFileName;

};

#endif