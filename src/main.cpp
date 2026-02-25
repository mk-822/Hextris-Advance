#include <windows.h>
#include "DxGraph.h"
#include "GameCtrl.h"
#include "StdAfx.h"
#include "main.h"
#include "keydefine.h"
#include "debug.h"
#include "common.h"
#include "Joypad.h"
#include "DataFileLoader.h"
#include "sound.h"

bool WINDOW_FULL;
int WINDOW_ZOOM;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow ){
	char str[10];		// ini からの Window 設定読み込み
	GetPrivateProfileString("Window","FullScreen",NULL,str,100,WINDOW_INIPATH);
	if(atoi(str))
		WINDOW_FULL = true;
	else
		WINDOW_FULL = false;
	GetPrivateProfileString("Window","Zoom",NULL,str,100,WINDOW_INIPATH);
		WINDOW_ZOOM = atoi(str);

	WaitTimer(250);
	GameCtrl gctrl;

	Sound::Initialize();

	for(;;){
		if( ProcessMessage() == -1 )
			break;
		gctrl.Main();
	}

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0;
}
