#include "extern.h"

bool checkMomentKey( int key ){
	static bool prevKey[ 256 ] = { false };
	char Buf[ 256 ];
	GetHitKeyStateAll( Buf );
	if( Buf[ key ] == 1 && !prevKey[ key ] ){
		prevKey[ key ] = true;
		return true;
	}else if( Buf[ key ] == 1 ){
		prevKey[ key ] = true;
		return false;
	}
	prevKey[ key ] = false;
	return false;
}

bool checkRange( int mx, int my, int x1, int y1, int x2, int y2 ){
	if( mx >= x1 && mx <= x2 && my >= y1 && my <= y2 ){
		return true;
	}else{
		return false;
	}
}