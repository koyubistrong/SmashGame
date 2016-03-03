#include "Menu.h"

Menu::Menu() : mCursol( 0 ), mDrawCredit( false ){
	magnification = 1.0;
	mCount = 0;
	mCreditPointY = 0;
	SetTransColor( 0, 0, 0 );

	mMainGameButtanHandle[ 0 ] = LoadGraph( "pic/maingame_no_select.png" );
	mMainGameButtanHandle[ 1 ] = LoadGraph( "pic/maingame_select.png" );
	m1PBattleButtanHandle[ 0 ] = LoadGraph( "pic/1pbattle_no_select.png" );
	m1PBattleButtanHandle[ 1 ] = LoadGraph( "pic/1pbattle_select.png" );
	mOptionButtanHandle[ 0 ] = LoadGraph( "pic/option_no_select.png" );
	mOptionButtanHandle[ 1 ] = LoadGraph( "pic/option_select.png" );
	mCreditButtanHandle[ 0 ] = LoadGraph( "pic/credit_no_select.png" );
	mCreditButtanHandle[ 1 ] = LoadGraph( "pic/credit_select.png" );
	mUnknownButtanHandle[ 0 ] = LoadGraph( "pic/unknown_no_select.png" );
	mUnknownButtanHandle[ 1 ] = LoadGraph( "pic/unknown_select.png" );

	int box = MakeScreen( 20, 20, false );
	SetDrawScreen( box );
	DrawBox( 0, 0, 20, 20, GetColor( 255, 255, 255 ), true );

	mBackGroundHandle = MakeScreen( 640, 480, true );
	SetDrawScreen( mBackGroundHandle );
	DrawBox( 0, 0, 640, 480, GetColor( 255, 255, 255 ), true );
	/*
	for( int y = 0; y < 480 / 32; y++ ){
		for( int x = 0; x < 640 / 32; x += 2 ){
			int tx = ( x + y % 1 ) * 32, ty = y * 32;
			DrawBox( tx, ty, tx + 32, ty + 32, GetColor( 127, 127, 127 ), true );
		}
	}
	*/
	DrawTriangle( 0, 0, 0, 480, 300, 0, GetColor( 0, 0, 0 ), true );
	DrawTriangle( 640, 480, 640, 480 - 480, 640 - 300, 480, GetColor( 0, 0, 0 ), true );

	SetDrawValidAlphaChannelGraphCreateFlag( TRUE );
	mGearWheelHandle = MakeScreen( 320, 320, true );
	SetDrawValidAlphaChannelGraphCreateFlag( FALSE );
	SetDrawScreen( mGearWheelHandle );
	for( int i = 0; i < 360; i += 1 ){
		DrawRotaGraph2( 320 / 2, 320 / 2, 10, 90, 1.0, (double)i * PI / 180.0, box, true );
	}
	for( int i = 0; i < 360; i += 1 ){
		DrawRotaGraph2( 320 / 2, 320 / 2, 10, 50, 1.0, (double)i * PI / 180.0, box, true );
	}
	for( int i = 0; i < 360; i += 24 ){
		DrawRotaGraph2( 320 / 2, 320 / 2, 10, 110, 1.0, (double)i * PI / 180.0, box, true );
	}
	DeleteGraph( box );
	SetDrawScreen( DX_SCREEN_BACK );
	SetDrawMode( DX_DRAWMODE_NEAREST );
	SetBackgroundColor( 0, 0, 0 );
	mJoyPad.maskFlag();
}

Menu::~Menu(){
	DeleteGraph( mGearWheelHandle );
	DeleteGraph( mBackGroundHandle );
	for( int i = 0; i < 2; i++ ){
		DeleteGraph( mMainGameButtanHandle[ i ] );
		DeleteGraph( m1PBattleButtanHandle[ i ] );
		DeleteGraph( mOptionButtanHandle[ i ] );
		DeleteGraph( mCreditButtanHandle[ i ] );
		DeleteGraph( mUnknownButtanHandle[ i ] );
	}
}

void Menu::update( MainSequence* mainSequence ){
	drawGearWheel();
	drawButtan();
	if( !mDrawCredit ){
		setCursol();
	}else{
		drawCredit();
	}
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad.setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad.checkMomentJoyPad( i, PAD_INPUT_9 ) ){
			mainSequence->setSequence( MainSequence::SEQ_TITLE );
			return;
		}
		if( checkMomentKey( KEY_INPUT_ESCAPE ) || mJoyPad.checkMomentJoyPad( i, PAD_INPUT_3 ) ){
			mDrawCredit = false;
			break;
		}
		if( checkMomentKey( KEY_INPUT_RETURN ) || mJoyPad.checkMomentJoyPad( i, PAD_INPUT_2 ) ){
			Sound::Instance()->playSoundEffect( Sound::SOUND_EFFECT_DECIDE_1 );
			switch( mCursol ){
			case 0:
				mainSequence->setSequence( MainSequence::SEQ_SMASH );
				return;
			case 1:
				mainSequence->setSequence( MainSequence::SEQ_MINIGAME );
				return;
			case 2:

				break;
			case 3:

				break;

			case 4:
				mDrawCredit = true;
				mCreditPointY = 0;
				break;
			}
			break;
		}
	}
	mJoyPad.setFlag();
}

void Menu::drawStringCenter( const char* str, int y, int color ){
	int getStrlen = GetDrawStringWidth( str, strlen( str ) ) ;
	DrawFormatString( 640 / 2 - getStrlen / 2, y, color, str );
}

void Menu::drawGearWheel(){
	mCount += 1;
	if( mCount >= 360 * 8 ){
		mCount = 0;
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	DrawRotaGraph( 640 / 2, 480 / 2, 1.0, 0, mBackGroundHandle, true, false );
	DrawRotaGraph( 80, 80, 1.0, ( double )mCount / 8.0 * PI / 180.0 + ( double )60 * PI / 180.0, mGearWheelHandle, true );
	DrawRotaGraph( 160, 320, 1.0, ( double )mCount / 8.0 * PI / 180.0 + ( double )60 * PI / 180.0, mGearWheelHandle, true );
	DrawRotaGraph( 640 - 80, 480 - 80, 1.0, ( double )mCount / 8.0 * PI / 180.0, mGearWheelHandle, true );
	DrawRotaGraph( 640 - 120, 480 - 320, 1.0, ( double )mCount / 8.0 * PI / 180.0, mGearWheelHandle, true );
}

void Menu::drawButtan(){
	int pointX = 400;
	int pointY = 110;
	int space = 80;
	int noSelect[] = { mMainGameButtanHandle[ 0 ], m1PBattleButtanHandle[ 0 ], mOptionButtanHandle[ 0 ], mUnknownButtanHandle[ 0 ], mCreditButtanHandle[ 0 ] };
	int select[] = { mMainGameButtanHandle[ 1 ], m1PBattleButtanHandle[ 1 ], mOptionButtanHandle[ 1 ], mUnknownButtanHandle[ 1 ], mCreditButtanHandle[ 1 ] };
	char* str = "MENU";
	SetFontSize( 42 );
	int getStrlen = GetDrawStringWidth( str, strlen( str ) ) ;
	DrawFormatString( 300 - getStrlen / 2 + 340 / 2 - 25, 20, GetColor( 0, 0, 0 ), str );
	SetFontSize( 16 );
	for( int i = 0; i < 5; i++ ){
		if( mCursol == i ){
			DrawRotaGraph( pointX - 0.625 * space * i, pointY + space * i, 1.0, 0, select[ i ], true );
		}else{
			DrawRotaGraph( pointX - 0.625 * space * i, pointY + space * i, 1.0, 0, noSelect[ i ], true );
		}
	}
}

void Menu::drawCredit(){
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 127 );
	DrawBox( 0, 0, 640, 480, GetColor( 0, 0, 0 ), true );
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );
	char* str[] = { "CREDIT",
					"BGM･SE",
					"「On-Jin ～音人～」　様",
					"http://www.yen-soft.com/ssse/",
					"「魔王魂」　様",
					"http://maoudamashii.jokersounds.com/",
					"以上のところからお借りさせていただきました。",
					"全ての著作物において",
					"二次配布することを禁じます。",
					"libpng　Copyright (c) 1998-2011 Glenn Randers-Pehrson.",
					"zlib　Copyright (C) 1995-2010 Jean-loup Gailly and Mark Adler."};
	char* strCopyrightGetRand[] = { "Mersenne Twister",
									"Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,",
									"All rights reserved.",	
									" ",
									"Redistribution and use in source and binary forms, with or without",
									"modification, are permitted provided that the following conditions",
									"are met:",
									" ",
									"1. Redistributions of source code must retain the above copyright",
									"notice, this list of conditions and the following disclaimer.",
									" ",
									"2. Redistributions in binary form must reproduce the above copyright",
									"notice, this list of conditions and the following disclaimer in the",
									"documentation and/or other materials provided with the distribution.",
									" ",
									"3. The name of the author may not be used to endorse or promote products",
									"derived from this software without specific prior written permission.",
									" ",
									"THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR",
									"IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES",
									"OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.",
									"IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,",
									"INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT",
									"NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,",
									"DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY",
									"THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT",
									"(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF",
									"THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
	};
	int strY[] = { 20, 60, 100, 120, 160, 180, 220, 260, 280, 320, 340 };
	for( int i = 0; i < 11; i++ ){
		if( strY[ i ] - mCreditPointY > 660 ) break;
		if( strY[ i ] - mCreditPointY < -20 ) continue;
		drawStringCenter( str[ i ], strY[ i ] - mCreditPointY, GetColor( 255, 255, 255 ) );
	}
	for( int i = 0; i < 28; i++ ){
		if( strY[ 10 ] + ( i + 2 ) * 20 - mCreditPointY > 660 ) break;
		if( strY[ 10 ] + ( i + 2 ) * 20 - mCreditPointY < -20 ) continue;
		drawStringCenter( strCopyrightGetRand[ i ], strY[ 10 ] + ( i + 2 ) * 20 - mCreditPointY, GetColor( 255, 255, 255 ) );
	}
	setCreditPointY();
}

void Menu::setCursol(){
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad.setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad.checkMomentJoyPad( i, PAD_INPUT_UP ) ){
			mCursol--;
			Sound::Instance()->playSoundEffect( Sound::SOUND_EFFECT_MOVE );
		}
		if( mJoyPad.checkMomentJoyPad( i, PAD_INPUT_DOWN ) ){
			mCursol++;
			Sound::Instance()->playSoundEffect( Sound::SOUND_EFFECT_MOVE );
		}
		if( mCursol >= 5 ){
			mCursol = 0;
		}else if( mCursol < 0 ){
			mCursol = 4;
		}
	}
}

void Menu::setCreditPointY(){
	for( int i = 0; i < 4; i++ ){
		if( !mJoyPad.setJoyPad( i ) ){
			continue;
		}
		if( mJoyPad.checkInputJoyPad( i, PAD_INPUT_UP ) ){
			mCreditPointY -= 8;
		}
		if( mJoyPad.checkInputJoyPad( i, PAD_INPUT_DOWN ) ){
			mCreditPointY += 8;
		}
		if( mCreditPointY < -150 ){
			mCreditPointY = -150;
		}else if( mCreditPointY >= 600 ){
			mCreditPointY = 600;
		}
	}
}