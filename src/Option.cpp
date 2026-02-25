// Option.cpp: Option クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Option.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
//	GetPrivateProfileString(JOYNAME[i],KEYNAME[0],NULL,str,100,JOYINIPATH)
Option::Option(){
	char str[10];
	GetPrivateProfileString("Window","FullScreen",NULL,str,2,"./setting.ini");
	WindowSize = atoi(str);
	Height = 0;
	for(int i = 0 ; i < Option_Menu ; i++)
		mode[i] = 0;
	if(WindowSize == 1){
		mode[ChangeWindowSize] = SizeFull;
	}else if(WindowSize == 0){
		GetPrivateProfileString("Window","Zoom",NULL,str,2,"./setting.ini");
		WindowSize = atoi(str);
		if(WindowSize == 1)
			mode[ChangeWindowSize] = Size320x240;
		if(WindowSize == 2)
			mode[ChangeWindowSize] = Size640x480;
		for(int i = 0 ; i < Option_Menu ; i++)
			SRAM[i] = mode[i];
	}
}
//コンストラクタでinitからの読み込み






void Option :: Main(){
	Sound::ChangeBgm(-1);
	dxg->TexturePos();
	dxg->ChangeEFXState();
	dxg->Draw(image->i[OP_BG_IMG],0,0);
	DrawImageFont(20,50,dxg,image->i[GRAYFONT_IMG],"WindowSize");
	DrawImageFont(75,70,dxg,image->i[GRAYFONT_IMG] ,"320*240");
	DrawImageFont(75*2,70,dxg,image->i[GRAYFONT_IMG],"640*480");
	DrawImageFont(75*3,70,dxg,image->i[GRAYFONT_IMG],"FullScrean");
	DrawImageFont(150,195,dxg,image->i[GRAYFONT_IMG],"  OK  ");
	DrawImageFont(150,210,dxg,image->i[GRAYFONT_IMG],"Cancel");
	if(input->GetKeyState(0,1) & UP)
		Height--;
	if(input->GetKeyState(0,1) & DOWN)
		Height++;
	//////////Heightの制御/////////
	if(Height >= Option_Menu)
		Height = 0;
	if(Height < 0)
		Height = Option_Menu - 1;
	////////////////////////////////
	if(input->GetKeyState(0,1) & LEFT)
		mode[Height]--;
	if(input->GetKeyState(0,1) & RIGHT)
		mode[Height]++;
	///////modeの制御//////////
	switch(Height){
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		if(mode[Height] > 2)
			mode[Height] = 0;
		if(mode[Height] < 0)
			mode[Height] = 2;
		break;
	}
	//これで仮想上のカーソルの位置は決定//////////
	/////////////////////イベント//////////////////////
	if(Height == Appry && input->GetKeyState(0,1) & BUTTON[0])
		OptionEnd();
	if(Height == Cancel && input->GetKeyState(0,1) & BUTTON[0])
		OptionEnd(false);
	//////////////////////////////////////////////////////
	//////////カーソル描画////////////////////////////////////
	if(Height == ChangeWindowSize){
		DrawImageFont(20,50,dxg,image->i[FONT_IMG],"WindowSize");
		switch(mode[Height]){
		case 0:
			DrawImageFont(75,70,dxg,image->i[FONT_IMG] ,"320*240");
			break;
		case 1:
			DrawImageFont(75*2,70,dxg,image->i[FONT_IMG],"640*480");
			break;
		case 2:
			DrawImageFont(75*3,70,dxg,image->i[FONT_IMG],"FullScrean");
			break;
		}
	}
	
	
	if(Height == Appry)
		DrawImageFont(150,195,dxg,image->i[FONT_IMG],"  OK  ");
	if(Height == Cancel)
		DrawImageFont(150,210,dxg,image->i[FONT_IMG],"Cancel");
}



/////////iniに書き込みー/////////////////
void Option::WriteInitFile()
{
	switch(mode[ChangeWindowSize]){
	case Size320x240:
		WritePrivateProfileString("Window","FullScreen","0","./setting.ini");
		WritePrivateProfileString("Window","Zoom","1","./setting.ini");
		break;
	case Size640x480:
		WritePrivateProfileString("Window","FullScreen","0","./setting.ini");
		WritePrivateProfileString("Window","Zoom","2","./setting.ini");
		break;
	case SizeFull:
		WritePrivateProfileString("Window","FullScreen","1","./setting.ini");
		WritePrivateProfileString("Window","Zoom","0","./setting.ini");
		break;
	}
}
//////////////////////////////////////////////////

//////オプションを終わらせていただきます///////////
void Option ::OptionEnd(bool flag)
{
	if(flag){
		WriteInitFile();
		*scene = TITLE_SCENE;
	}else{
		for(int i = 0 ; i < Option_Menu ; i++)
			mode[i] = SRAM[i];
		*scene = TITLE_SCENE;
	}

}
/////////////////////////////////////////////


/*

新しいアプリケーションを開く方法(プライマリースレッドも作成できるAPIではこれを使おう！！)
BOOL CreateProcess(
  LPCTSTR lpApplicationName,                 // 実行可能モジュールの名前
  LPTSTR lpCommandLine,                      // コマンドラインの文字列
  LPSECURITY_ATTRIBUTES lpProcessAttributes, // セキュリティ記述子
  LPSECURITY_ATTRIBUTES lpThreadAttributes,  // セキュリティ記述子
  BOOL bInheritHandles,                      // ハンドルの継承オプション
  DWORD dwCreationFlags,                     // 作成のフラグ
  LPVOID lpEnvironment,                      // 新しい環境ブロック
  LPCTSTR lpCurrentDirectory,                // カレントディレクトリの名前
  LPSTARTUPINFO lpStartupInfo,               // スタートアップ情報
  LPPROCESS_INFORMATION lpProcessInformation // プロセス情報
);
*/




/*
static const int RANK_IMG = 0;
static const int BLANK_IMG = 1;
static const int FONT_IMG = 2;
static const int FONTEX_IMG = 3;
static const int GRAYFONT_IMG = 18;
*/