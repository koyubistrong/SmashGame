#ifndef INCLUDED_FILE_H
#define INCLUDED_FILE_H

#include "DxLib.h"

class File{
public:
	File( const char* filename );
	~File();
	int getSize() const;
	int getSizeX() const;
	int getSizeY() const;
	const int* getDataInt() const;
private:
	int mSize;
	int mSizeX;
	int mSizeY;
	int* mData;
};

#endif