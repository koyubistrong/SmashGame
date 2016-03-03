#include "Character.h"

void Character::setBlowRate( int damage ){
	mBlowRate = damage;
}

int Character::getBlowRate(){
	return mBlowRate;
}

void Character::setCharacterHandle( int i ){
	mCharacterHandle = i;
}

void Character::getCharacterPoint( int* x, int* y ){
	*x = mCharacterX;
	*y = mCharacterY;
}

void Character::setCharacterID( int o ){
	switch( o ){
	case Object::OBJ_PLAYER:
		mCharacterID = Object::OBJ_PLAYER;
		break;
	case Object::OBJ_COMPUTER:
		mCharacterID = Object::OBJ_COMPUTER;
		break;
	case Object::OBJ_ENEMY2:
		mCharacterID = Object::OBJ_ENEMY2;
		break;
	case Object::OBJ_STAR:
		mCharacterID = Object::OBJ_STAR;
		break;
	}
}

int Character::outCharacterHandle(){
	return mCharacterHandle;
}

int Character::outCharacterID(){
	return mCharacterID;
}
