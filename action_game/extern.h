#ifndef INCLUDED_EXTERN_H
#define INCLUDED_EXTERN_H

#include "DxLib.h"

extern double magnification;
extern int stagehalfsizeX;
extern int stagehalfsizeY;
bool checkMomentKey( int key );
bool checkRange( int mx, int my, int x1, int y1, int x2, int y2 );

template <typename T>
inline void SafeDelete( T*& p ){
	if( p != 0 ) {
		delete ( p );
		( p ) = 0;
	}
}

template <typename T>
inline void SafeDeleteArray( T*& p ){
	if( p != 0 ) {
		delete[] ( p );
		( p ) = 0;
	}
}

#endif