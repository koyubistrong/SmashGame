#include "CSV.h"

CSV::CSV( const char* fileName ) : mSize( 0 ), mData( 0 ), mSizeX( 0 ), mSizeY( 0 ), mFileName( fileName ){
	int fileHandle = FileRead_open( mFileName );
	if ( fileHandle ){
		mSize = FileRead_size( mFileName );
		char* data = new char[ mSize + 1 ];
		for( int i = 0; i < mSize; i++ ){
			data[ i ] = FileRead_getc( fileHandle );
		}
		data[ mSize ] = '\0';
		int fileIt = 0;
		int dummySize = 0;
		int savePoint = 0;
		for( int i = 0; data[ i ] != '\n'; i++ ){
			if( data[ i ] == ',' ){
				mSizeX++;
			}
			dummySize++;
			fileIt++;
		};
		for( int i = 0; data[ i ] != '\n'; i++ ){
			mDummy += data[ i ];
		}
		mDummy += data[ fileIt ];
		savePoint = fileIt + 1;
		for( int i = 0; data[ i ] != '\0'; i++ ){
			if( data[ i ] == '\n' ){
				mSizeY++;
			}
		}
		mSizeY--;
		mData = new int[ mSizeX * mSizeY ];
		mString = new string[ mSizeY ];
		fileIt = savePoint;
		for( int y = 0; y < mSizeY; y++ ){
			char str[ 256 ];
			int n = 0;
			while( data[ fileIt ] != ',' && n < 255 ){ str[ n ] = data[ fileIt ]; fileIt++; n++; }
			str[ n ] = '\0';
			for( int s = 0; str[ s ] != '\0'; s++ ){ mString[ y ] += str[ s ]; }
			fileIt++;
			for( int x = 0; x < mSizeX; x++ ){
				int sign = 1;
				mData[ y * mSizeX + x ] = 0;
				while( data[ fileIt ] != ',' && data[ fileIt ] != '\n' ){
					int c = 0x00FF & static_cast< int >( data[ fileIt ] );
					if( c == 45 ){
						sign = -1;
					}
					if( c >= 48 && c <= 57 ){
						mData[ y * mSizeX + x ] *= 10;
						mData[ y * mSizeX + x ] += c - 48;
					}
					fileIt++;
				}
				mData[ y * mSizeX + x ] *= sign;
				fileIt++;
			}
		}
		delete[] data;
	}else{
		ClearDrawScreen();
		printfDx( "%sが読み込めませんでした。\n何か、キーを押してください。", fileName );
		ScreenFlip();
		WaitKey();
		exit( 1 );
	}
	FileRead_close( fileHandle );
}

CSV::~CSV(){
	SafeDeleteArray( mString );
	SafeDeleteArray( mData );
}

void CSV::setData( int param, int x, int y ){
	mData[ y * mSizeX + x ] = param;
}

int CSV::getSize() const {
	return mSize;
}

int CSV::getSizeX() const {
	return mSizeX;
}

int CSV::getSizeY() const {
	return mSizeY;
}

const int* CSV::getDataInt() const {
	return mData;
}

const string* CSV::getDataString() const {
	return mString;
}

void CSV::outputFile(){
	ofstream out( mFileName, ofstream::binary );
	for( int i = 0; i < mDummy.length(); i++ ){
		out << mDummy[ i ];
	}
	for( int y = 0; y < mSizeY; y++ ){
		for( int i = 0; i < mString[ y ].length(); i++ ){
			out << mString[ y ][ i ];
		}
		out << ',';
		for( int x = 0; x < mSizeX; x++ ){
			out << mData[ y * mSizeX + x ];
			if( x != mSizeX - 1 ){
				out << ',';
			}
		}
		out << '\n';
	}
	out.close();
}