#ifndef INCLUDE_OPTION_H
#define INCLUDE_OPTION_H

#include "DxLib.h"
#include "MainSequence.h"
#include "Image.h"
#include "JoyPad.h"
#include "extern.h"

class Option{
public:
	Option();
	~Option();
	void update( MainSequence* mainSequence );

private:
	int mCursol;
};

#endif