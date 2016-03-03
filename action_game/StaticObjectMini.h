#ifndef INCLUDED_STATIC_OBJECT_MINI_H
#define INCLUDED_STATIC_OBJECT_MINI_H

#include <string>
#include <time.h>
#include "DxLib.h"
#include "File.h"
#include "Array2D.h"
#include "Image.h"
#include "StaticObject.h"

using namespace std;

class StaticObjectMini : public StaticObject{
public:
	StaticObjectMini( const char* filename );
	~StaticObjectMini();
	void initiaLizeStage();
	void initiaLizeAppearCharacter();
	void drawStage( int cameraX, int cameraY );
	void crateStage();
	void createFlatStage( int x, int height, int width );
	void createNoStage( int width );
	void createUpStairsStage( int x, int height, int width, int gradeWidth );
	void createDownStairsStage( int x, int height, int width, int gradeWidth );
	void createGround( int x, int height );
	void createGround( int x, int y, int height );
	bool hitStaticObjectCheck( int* point );
	void getStageSize( int *x, int *y );
	void getAppearCharacterPoint( int *x, int *y );
	void setAppearCharacterPoint( int x, int y );
	void setAppearFlag( bool appear );

private:
	struct character{
		int flag;
		int pointX;
		int pointY;
	};
	int mTempX;
	int mTempHeight;
	character mAppearCharacter[ 4 ];
	bool mAppear;

};


#endif