#ifndef INCLUDED_HANTEI_IMAGE_H
#define INCLUDED_HANTEI_IMAGE_H

#include "DxLib.h"
#include "Array2D.h"

class HanteiImage{
public:
	HanteiImage( const char* FileName, int AllNum, int XSize, int YSize )
		: mCellX( XSize ), mCellY( YSize ), mNum( AllNum ){
		mHandle = LoadSoftImage( FileName );
		if( mHandle == -1 ){
			ClearDrawScreen();
			//printfDx( "%sÇ™ì«Ç›çûÇﬂÇ‹ÇπÇÒÇ≈ÇµÇΩÅB\n", FileName );
			//ScreenFlip();
			//WaitKey();
			//exit( 1 );
		}
		for( int i = 0; i < 20; i++ ){
			for( int n = 0; n < 10; n++ ){
				for( int m = 0; m < 2; m++ ){
					mHanteiPointNum[ i ] = 0;
					mHanteiInfo[ n ][ i ] = 0;
					mHanteiStartPoint[ n ][ m ][ i ] = 0;
					mHanteiEndPoint[ n ][ m ][ i ] = 0;
					mHanteiStartPointLR[ n ][ m ][ i ] = 0;
					mHanteiEndPointLR[ n ][ m ][ i ] = 0;
				}
			}
		}
		mHantei.setSize( mCellX * mNum, mCellY );
		mHanteiLR.setSize( mCellX * mNum, mCellY );
		if( mHandle != -1 ){
			readHantei();
			readHanteiPoint();
			setHanteiLR();
			DeleteSoftImage( mHandle );
			mSuccess = true;
		}else{
			mSuccess = false;
		}
	}

	~HanteiImage(){

	}

	void readHanteiPoint(){
		int r, g, b, a;
		for( int i = 0; i < mNum; i++ ){
			int num = 0;
			for( int y = 0; y < mCellY; y++ ){
				for( int x = 0; x < mCellX; x++ ){
					bool getPoint = false;
					GetPixelSoftImage( mHandle, i * mCellX + x, y, &r, &g, &b, &a );
					if( r == 0 && g == 0 && b == 0 ){
						//mHanteiInfo[ num / 2 ][ i ] = 1;
						//getPoint = true;
					}else if( r == 127 && g == 127 && b == 127 ){
						mHanteiInfo[ num / 2 ][ i ] = 2;
						getPoint = true;
					}
					if( getPoint ){
						if( num % 2 == 0 ){
							mHanteiStartPoint[ num / 2 ][ 0 ][ i ] = x;
							mHanteiStartPoint[ num / 2 ][ 1 ][ i ] = y;
							mHanteiStartPointLR[ num / 2 ][ 0 ][ i ] = mCellX - x - 1;
							mHanteiStartPointLR[ num / 2 ][ 1 ][ i ] = y;
						}else{
							mHanteiEndPoint[ num / 2 ][ 0 ][ i ] = x;
							mHanteiEndPoint[ num / 2 ][ 1 ][ i ] = y;
							mHanteiEndPointLR[ num / 2 ][ 0 ][ i ] = mHanteiStartPointLR[ num / 2 ][ 0 ][ i ];
							mHanteiStartPointLR[ num / 2 ][ 0 ][ i ] = mCellX - x - 1;
							mHanteiEndPointLR[ num / 2 ][ 1 ][ i ] = y;
						}
						num++;
					}
					if( num >= 20 ){
						break;
					}
				}
			}
			mHanteiPointNum[ i ] = num / 2;
		}
	}

	void readHanteiPointLR(){
		int r, g, b, a;
		for( int x = 0; x < mCellX * mNum; x++ ){
			for( int y = 0; y < mCellY; y++ ){
				GetPixelSoftImage( mHandle, x, y, &r, &g, &b, &a );
				if( r == 0 && g == 0 && b == 255 ){
					mHantei( x, y ) = 1;
				}else if( r == 255 && g == 0 && b == 0 ){
					mHantei( x, y ) = 2;
				}else{
					mHantei( x, y ) = 0;
				}
			}
		}
	}

	void readHantei(){
		int r, g, b, a;
		for( int x = 0; x < mCellX * mNum; x++ ){
			for( int y = 0; y < mCellY; y++ ){
				GetPixelSoftImage( mHandle, x, y, &r, &g, &b, &a );
				if( r == 0 && g == 0 && b == 255 ){
					mHantei( x, y ) = 1;
				}else if( r == 255 && g == 0 && b == 0 ){
					mHantei( x, y ) = 2;
				}else{
					mHantei( x, y ) = 0;
				}
			}
		}
	}

	void setHanteiLR(){
		for( int i = 0; i < mNum; i++ ){
			for( int x = 0; x < mCellX; x++ ){
				for( int y = 0; y < mCellY; y++ ){
					mHanteiLR( i * mCellX + x, y ) = mHantei( ( i + 1 ) * mCellX - x - 1, y );
				}
			}
		}
	}

	int getHantei( int flame, int x, int y ){
		return mHantei( flame * mCellX + x, y );
	}

	int getHanteiLR( int flame, int x, int y ){
		return mHanteiLR( flame * mCellX + x, y );
	}

	int getHanteiPointNum( int flame ){
		return mHanteiPointNum[ flame ];
	}

	int getHanteiInfomation( int flame, int address ){
		return mHanteiInfo[ address ][ flame ];
	}

	void getHanteiPoint( int flame, int address, int* info,int* startX, int* startY, int* endX, int* endY ){
		*info = mHanteiInfo[ address ][ flame ];
		*startX = mHanteiStartPoint[ address ][ 0 ][ flame ];
		*startY = mHanteiStartPoint[ address ][ 1 ][ flame ];
		*endX = mHanteiEndPoint[ address ][ 0 ][ flame ];
		*endY = mHanteiEndPoint[ address ][ 1 ][ flame ];
	}

	void getHanteiPointLR( int flame, int address, int* info,int* startX, int* startY, int* endX, int* endY ){
		*info = mHanteiInfo[ address ][ flame ];
		*startX = mHanteiStartPointLR[ address ][ 0 ][ flame ];
		*startY = mHanteiStartPointLR[ address ][ 1 ][ flame ];
		*endX = mHanteiEndPointLR[ address ][ 0 ][ flame ];
		*endY = mHanteiEndPointLR[ address ][ 1 ][ flame ];
	}

	void getCellSize( int* x, int* y ){
		*x = mCellX;
		*y = mCellY;
	}
	bool mSuccess;

private:
	int mHandle;
	int mCellX;
	int mCellY;
	int mNum;
	Array2D< int > mHantei;
	Array2D< int > mHanteiLR;
	int mHanteiPointNum[ 20 ];
	int mHanteiInfo[ 10 ][ 20 ];
	int mHanteiStartPoint[ 10 ][ 2 ][ 20 ];
	int mHanteiEndPoint[ 10 ][ 2 ][ 20 ];
	int mHanteiStartPointLR[ 10 ][ 2 ][ 20 ];
	int mHanteiEndPointLR[ 10 ][ 2 ][ 20 ];

};

#endif