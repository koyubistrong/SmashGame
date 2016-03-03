#ifndef INCLUDED_STATIC_OBJECT_H
#define INCLUDED_STATIC_OBJECT_H

#include <string>
#include "DxLib.h"
#include "File.h"
#include "Array2D.h"
#include "Image.h"

using namespace std;

class StaticObject{
public:
	StaticObject( const char* filename );
	StaticObject(){}
	virtual ~StaticObject();
	void initiaLizeStage();
	void drawStage( int cameraX, int cameraY );
	virtual bool hitStaticObjectCheck( int* point );
	virtual bool hitCharacterFallStaticObjectCheck( int* point );
	void getStageSize( int *x, int *y );
	int getBlockInfo( int x, int y );

protected:
	Image* mStageObject[ 100 ];
	Array2D< int > mBlockID;
	Array2D< int > mBlockInfo;
	int mStageHandle;
	int mSizeX;
	int mSizeY;
	int mSizePicX;
	int mSizePicY;
	int mCellPicX;
	int mCellPicY;
	int size;

};


#endif