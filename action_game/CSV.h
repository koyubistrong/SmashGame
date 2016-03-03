#ifndef INCLUDED_CSV_H
#define INCLUDED_CSV_H

#include <fstream>
#include "DxLib.h"
#include "extern.h"

using namespace std;

class CSV{
public:
	CSV( const char* fileName );
	~CSV();
	void setData( int param, int x, int y );
	int getSize() const;
	int getSizeX() const;
	int getSizeY() const;
	const int* getDataInt() const;
	const string* getDataString() const;
	void outputFile();
private:
	int mSize;
	int mSizeX;
	int mSizeY;
	int* mData;
	string* mString;
	string mDummy;
	const char* mFileName;
};

#endif