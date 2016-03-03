#include "Result.h"

Result::Result( GameInfomation* gameInfomation ){
	CSV* csv_setting = new CSV( "setting/setting.csv" );
	const int* data = csv_setting->getDataInt();
	mPlayerNum = 0;
	SetBackgroundColor( 255, 255, 255 );
	for( int i = 0; i < 4; i++ ){
		if( gameInfomation[ i ].characterID != -1 ){
			mPlayer[ mPlayerNum ] = i + 1;
			mComputerFlag[ mPlayerNum ] = gameInfomation[ i ].computerFlag;
			mCharacterID[ mPlayerNum ] = gameInfomation[ i ].characterID;
			mGameOverTime[ mPlayerNum ] = gameInfomation[ i ].gameOverTime;
			mLivesNum[ mPlayerNum ] = gameInfomation[ i ].livesNum;
			mSmashNum[ mPlayerNum ] = gameInfomation[ i ].smashNum;
			mFallNum[ mPlayerNum ] = gameInfomation[ i ].fallNum;
			mScore[ mPlayerNum ] = gameInfomation[ i ].smashNum - gameInfomation[ i ].fallNum;
			mPlayerNum++;
		}
	}
	if( data[ 0 ] == MODE_TIME ){
		for( int i = 0; i < mPlayerNum; i++ ){
			for( int j = 0; j < mPlayerNum - i - 1; j++ ){
				if( mScore[ j ] < mScore[ j + 1 ] ){
					swap( &mPlayer[ j ], &mPlayer[ j + 1 ] );
					swap( &mComputerFlag[ j ], &mComputerFlag[ j + 1 ] );
					swap( &mCharacterID[ j ], &mCharacterID[ j + 1 ] );
					swap( &mGameOverTime[ j ], &mGameOverTime[ j + 1 ] );
					swap( &mLivesNum[ j ], &mLivesNum[ j + 1 ] );
					swap( &mSmashNum[ j ], &mSmashNum[ j + 1 ] );
					swap( &mFallNum[ j ], &mFallNum[ j + 1 ] );
					swap( &mScore[ j ], &mScore[ j + 1 ] );
				}
			}
		}
		mMode = MODE_TIME;
	}else if( data[ 0 ] == MODE_LIVES ){
		for( int i = 0; i < mPlayerNum; i++ ){
			for( int j = 0; j < mPlayerNum - i - 1; j++ ){
				if( ( mGameOverTime[ j ] != -1 && mGameOverTime[ j + 1 ] == -1 ) ||
					( mGameOverTime[ j ] != -1 && mGameOverTime[ j + 1 ] != -1 && mGameOverTime[ j ] < mGameOverTime[ j + 1 ] ) ){
					swap( &mPlayer[ j ], &mPlayer[ j + 1 ] );
					swap( &mComputerFlag[ j ], &mComputerFlag[ j + 1 ] );
					swap( &mCharacterID[ j ], &mCharacterID[ j + 1 ] );
					swap( &mGameOverTime[ j ], &mGameOverTime[ j + 1 ] );
					swap( &mLivesNum[ j ], &mLivesNum[ j + 1 ] );
					swap( &mSmashNum[ j ], &mSmashNum[ j + 1 ] );
					swap( &mFallNum[ j ], &mFallNum[ j + 1 ] );
					swap( &mScore[ j ], &mScore[ j + 1 ] );
				}
			}
		}
		mMode = MODE_LIVES;
	}else if( data[ 0 ] == MODE_TIME_LIVES ){
		for( int i = 0; i < mPlayerNum; i++ ){
			for( int j = 0; j < mPlayerNum - i - 1; j++ ){
				if( ( mGameOverTime[ j ] == -1 && mGameOverTime[ j + 1 ] == -1 && mLivesNum[ j ] < mLivesNum[ j + 1 ] ) ||
					( mGameOverTime[ j ] != -1 && mGameOverTime[ j + 1 ] == -1 ) ||
					( mGameOverTime[ j ] != -1 && mGameOverTime[ j + 1 ] != -1 && mGameOverTime[ j ] < mGameOverTime[ j + 1 ] ) ){
					swap( &mPlayer[ j ], &mPlayer[ j + 1 ] );
					swap( &mComputerFlag[ j ], &mComputerFlag[ j + 1 ] );
					swap( &mCharacterID[ j ], &mCharacterID[ j + 1 ] );
					swap( &mGameOverTime[ j ], &mGameOverTime[ j + 1 ] );
					swap( &mLivesNum[ j ], &mLivesNum[ j + 1 ] );
					swap( &mSmashNum[ j ], &mSmashNum[ j + 1 ] );
					swap( &mFallNum[ j ], &mFallNum[ j + 1 ] );
					swap( &mScore[ j ], &mScore[ j + 1 ] );
				}
			}
		}
		mMode = MODE_TIME_LIVES;
	}
	delete csv_setting;
}

Result::~Result(){

}

void Result::update( SmashSequence* smashSequence ){
	drawString();
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad.setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad.checkMomentJoyPad( i, PAD_INPUT_10 ) ){
			smashSequence->setSequence( SmashSequence::SEQ_CHARACTER_SELECT );
			break;
		}
	}
	mJoyPad.setFlag();
}

void Result::drawStringCenter( const char* str, int y ){
	int getStrlen = GetDrawStringWidth( str, strlen( str ) ) ;
	DrawFormatString( 640 * 0.5 - getStrlen * 0.5, y, GetColor( 0, 0, 0 ), str );
}

void Result::drawString(){
	char* strLives[] = { "順位", "プレイヤー", "タイム", "残機" };
	char* strFormatLives[] = { "%d位", "%s%d", "%d:%02d", "%d" };
	char* strTime[] = { "順位", "プレイヤー", "撃破", "落下", "スコア" };
	char* strFormatTime[] = { "%d位", "%s%d", "%d", "%d", "%d" };
	int address = 0;
	int spaceLen = GetDrawStringWidth( "  ", strlen( "  " ) );
	int sizeTitle = 32;
	int sizeOther = 24;
	int space = 10;
	int tieNum = 0;
	string connectStr;
	SetFontSize( sizeTitle );
	drawStringCenter( "結果", POINT_TITLE_Y );
	SetFontSize( sizeOther );
	switch( mMode ){
	case MODE_TIME:
		for( int i = 0; i < 5; i++ ){
			connectStr += strTime[ i ];
			if( i != 4 ){ connectStr += "  "; }
		}
		address = GetDrawStringWidth( connectStr.data(), strlen( connectStr.data() ) );
		DrawFormatString( 640 * 0.5 - address * 0.5, POINT_TITLE_Y + sizeTitle + space, GetColor( 0, 0, 0 ), "%s", connectStr.data() );
		for( int i = 0; i < mPlayerNum; i++ ){
			if( i != 0 && mScore[ i ] == mScore[ i - 1 ] ){
				tieNum++;
			}else{
				tieNum = 0;
			}
			char* strPlayerCom;
			address = 640 * 0.5 - GetDrawStringWidth( connectStr.data(), strlen( connectStr.data() ) ) * 0.5;
			for( int j = 0; j < 5; j++ ){
				int getStrlen = GetDrawStringWidth( strTime[ j ], strlen( strTime[ j ] ) );
				int center = 0;
				int ty = POINT_TITLE_Y + sizeTitle + ( sizeOther + space ) * ( i + 1 ) + space;
				switch( j ){
				case 0:
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatTime[ j ], i + 1 - tieNum ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatTime[ j ], i + 1 - tieNum );
					break;
				case 1:
					strPlayerCom = ( mComputerFlag[ i ] ) ? "COM" : "P";
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatTime[ j ], strPlayerCom, mPlayer[ i ] ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatTime[ j ], strPlayerCom, mPlayer[ i ] );
					break;
				case 2:
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatTime[ j ], mSmashNum[ i ] ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatTime[ j ], mSmashNum[ i ] );
					break;
				case 3:
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatTime[ j ], mFallNum[ i ] ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatTime[ j ], mFallNum[ i ] );
					break;
				case 4:
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatTime[ j ], mScore[ i ] ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatTime[ j ], mScore[ i ] );
					break;
				}
				address += getStrlen + spaceLen;
			}
		}
		break;
	case MODE_LIVES:
		for( int i = 0; i < 3; i++ ){
			connectStr += strLives[ i ];
			if( i != 2 ){ connectStr += "  "; }
		}
		address = GetDrawStringWidth( connectStr.data(), strlen( connectStr.data() ) );
		DrawFormatString( 640 * 0.5 - address * 0.5, POINT_TITLE_Y + sizeTitle + space, GetColor( 0, 0, 0 ), "%s", connectStr.data() );
		for( int i = 0; i < mPlayerNum; i++ ){
			if( i != 0 && mGameOverTime[ i ] == mGameOverTime[ i - 1 ] ){
				tieNum++;
			}else{
				tieNum = 0;
			}
			char* strPlayerCom;
			address = 640 * 0.5 - GetDrawStringWidth( connectStr.data(), strlen( connectStr.data() ) ) * 0.5;
			for( int j = 0; j < 3; j++ ){
				int getStrlen = GetDrawStringWidth( strLives[ j ], strlen( strLives[ j ] ) );
				int center = 0;
				int ty = POINT_TITLE_Y + sizeTitle + ( sizeOther + space ) * ( i + 1 ) + space;
				switch( j ){
				case 0:
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatLives[ j ], i + 1 - tieNum ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatLives[ j ], i + 1 - tieNum );
					break;
				case 1:
					strPlayerCom = ( mComputerFlag[ i ] ) ? "COM" : "P";
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatLives[ j ], strPlayerCom, mPlayer[ i ] ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatLives[ j ], strPlayerCom, mPlayer[ i ] );
					break;
				case 2:
					if( mGameOverTime[ i ] != -1 ){
						center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatLives[ j ], mGameOverTime[ i ] / 60 / 60, ( mGameOverTime[ i ] / 60 ) % 60 ) * 0.5;
						DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatLives[ j ], mGameOverTime[ i ] / 60 / 60, ( mGameOverTime[ i ] / 60 ) % 60 );
					}else{
						center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( "━" ) * 0.5;
						DrawFormatString( center, ty, GetColor( 0, 0, 0 ), "━" );
					}
					break;
				}
				address += getStrlen + spaceLen;
			}
		}
		break;
	case MODE_TIME_LIVES:
		for( int i = 0; i < 4; i++ ){
			connectStr += strLives[ i ];
			if( i != 3 ){ connectStr += "  "; }
		}
		address = GetDrawStringWidth( connectStr.data(), strlen( connectStr.data() ) );
		DrawFormatString( 640 * 0.5 - address * 0.5, POINT_TITLE_Y + sizeTitle + space, GetColor( 0, 0, 0 ), "%s", connectStr.data() );
		for( int i = 0; i < mPlayerNum; i++ ){
			if( i != 0 && mGameOverTime[ i ] == mGameOverTime[ i - 1 ] && mLivesNum[ i ] == mLivesNum[ i - 1 ] ){
				tieNum++;
			}else{
				tieNum = 0;
			}
			char* strPlayerCom;
			address = 640 * 0.5 - GetDrawStringWidth( connectStr.data(), strlen( connectStr.data() ) ) * 0.5;
			for( int j = 0; j < 4; j++ ){
				int getStrlen = GetDrawStringWidth( strLives[ j ], strlen( strLives[ j ] ) );
				int center = 0;
				int ty = POINT_TITLE_Y + sizeTitle + ( sizeOther + space ) * ( i + 1 ) + space;
				switch( j ){
				case 0:
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatLives[ j ], i + 1 - tieNum ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatLives[ j ], i + 1 - tieNum );
					break;
				case 1:
					strPlayerCom = ( mComputerFlag[ i ] ) ? "COM" : "P";
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatLives[ j ], strPlayerCom, mPlayer[ i ] ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatLives[ j ], strPlayerCom, mPlayer[ i ] );
					break;
				case 2:
					if( mGameOverTime[ i ] != -1 ){
						center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatLives[ j ], mGameOverTime[ i ] / 60 / 60, ( mGameOverTime[ i ] / 60 ) % 60 ) * 0.5;
						DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatLives[ j ], mGameOverTime[ i ] / 60 / 60, ( mGameOverTime[ i ] / 60 ) % 60 );
					}else{
						center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( "━" ) * 0.5;
						DrawFormatString( center, ty, GetColor( 0, 0, 0 ), "━" );
					}
					break;
				case 3:
					center = address + getStrlen * 0.5 - GetDrawFormatStringWidth( strFormatLives[ j ], mLivesNum[ i ] ) * 0.5;
					DrawFormatString( center, ty, GetColor( 0, 0, 0 ), strFormatLives[ j ], mLivesNum[ i ] );
					break;
				}
				address += getStrlen + spaceLen;
			}
		}
		break;
	}
}
void Result::swap( int* t1, int* t2 ){
	int temp;
	temp = *t1;
	*t1 = *t2;
	*t2 = temp;
}