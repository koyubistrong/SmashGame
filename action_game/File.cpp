#include "File.h"

File::File( const char* filename ) : mSize( 0 ), mData( 0 ){
	int fileHandle = FileRead_open( filename );
	if ( fileHandle ){
		char* data;
		mSize = FileRead_size( filename );
		data = new char[ mSize ];
		mData = new int[ ( mSize - 4 ) / 4 ];
		for( int i = 0; i < mSize; i++ ){
			data[ i ] = FileRead_getc( fileHandle );
		}
		mSizeX = 0x00FF & static_cast< int >( data[ 0 ] );
		mSizeX |= ( 0x00FF & static_cast< int >( data[ 1 ] ) ) << 8;
		mSizeY = 0x00FF & static_cast< int >( data[ 2 ] );
		mSizeY |= ( 0x00FF & static_cast< int >( data[ 3 ] ) ) << 8;
		for( int i = 4; i < mSize; i++ ){
			if( ( i - 4 ) % 4 == 0 ){
				mData[ ( i - 4 ) / 4 ] = 0;
			}
			int c = 0x00FF & static_cast< int >( data[ i ] );
			mData[ ( i - 4 ) / 4 ] |= c << 8 * ( ( i - 4 ) % 4 );
		}
		mSize = ( mSize - 4 ) / 4;
		delete data;
	}else{
		ClearDrawScreen();
		printfDx( "%sが読み込めませんでした。\n何か、キーを押してください。", filename );
		ScreenFlip();
		WaitKey();
		exit( 1 );
	}
	FileRead_close( fileHandle );
}

File::~File(){
	delete[] mData;
	mData = 0;
}

int File::getSize() const {
	return mSize;
}

int File::getSizeX() const {
	return mSizeX;
}

int File::getSizeY() const {
	return mSizeY;
}

const int* File::getDataInt() const {
	return mData;
}