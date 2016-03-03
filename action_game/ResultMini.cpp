#include "Result.h"

ResultMini::ResultMini( GameInfomation* gameInfomation ) : Result( gameInfomation ){}

ResultMini::~ResultMini(){}

void ResultMini::update( MiniSequence* miniSequence ){
	drawString();
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad.setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad.checkMomentJoyPad( i, PAD_INPUT_10 ) ){
			miniSequence->setSequence( MiniSequence::SEQ_CHARACTER_SELECT );
			break;
		}
	}
	mJoyPad.setFlag();
}