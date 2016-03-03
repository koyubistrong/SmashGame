#include "DxLib.h"
//#include "State.h"
#include "MainSequence.h"
#include <windows.h>

double magnification;
int stagehalfsizeX;
int stagehalfsizeY;
bool checkMomentKey( int key );
void wait_fanc();

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd){
	SetMainWindowText( "SMASH" );
	SetOutApplicationLogValidFlag( FALSE );
	SetDxLibEndPostQuitMessageFlag( FALSE );
	ChangeWindowMode( TRUE ); // ウィンドウモードに設定
	SetWaitVSyncFlag(FALSE);
	DxLib_Init(); // DXライブラリ初期化処理
	SetDrawScreen( DX_SCREEN_BACK ); //描画先を裏画面に設定
	SRand( ( unsigned )time( NULL ) );
	//State* gState = 0;
	MainSequence* gMainSequence = 0;

	while( !ProcessMessage() && !CheckHitKey( KEY_INPUT_DELETE ) ){

		ClearDrawScreen(); // 画面を消す
		//clsDx();

		if( !gMainSequence ){
			gMainSequence = new MainSequence();
		}

		gMainSequence->update();
				
		wait_fanc();

		ScreenFlip(); //裏画面を表画面に反映

	}
	
	DxLib_End(); // DXライブラリ終了処理
	SafeDelete( gMainSequence );

	return 0;
}

void wait_fanc(){
    int term1;
	int term2;
    static int t=0;
	static int i=0;
	static int num = 0;
	static int save = 0;
	static int font = CreateFontToHandle( "ＭＳ　ゴシック", 16, 4, DX_FONTTYPE_ANTIALIASING_EDGE_4X4 );
    term1 = GetNowCount() - t;
	term2 = GetNowCount() - i;
	num++;
    if( 20 - term1 > 0 ){
		Sleep( 20 - term1 );
	}
    if( 1000 - term2 < 0){
		save = num;
		num = 0;
		i = GetNowCount();
	}
	//DrawFormatStringToHandle( 0, 0, GetColor(255, 255, 255), font, "%d fps", save );
	//printfDx( "%d fps", save );
	t = GetNowCount();

    return;
}