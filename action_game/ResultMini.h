#ifndef INCLUDE_RESULT_MINI_H
#define INCLUDE_RESULT_MINI_H

#include "MiniSequence.h"
#include "GameInfomation.h"

class MiniSequence;

class ResultMini{
public:
	ResultMini( GameInfomation* gameInfomation );
	~ResultMini();
	void update( MiniSequence* miniSequence );
};

#endif