// Option.h: Option クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTION_H__16F7011C_D5AF_4EE4_A878_75073E9E66F3__INCLUDED_)
#define AFX_OPTION_H__16F7011C_D5AF_4EE4_A878_75073E9E66F3__INCLUDED_


#include "common.h"
#include "GameCtrl.h"
#include "HextrisCtrl.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef unsigned int UINT;
static const int Option_Menu = 3;		//オプションで選択できる総数
static const int ChangeWindowSize = 0;	//ウィンドウのサイズを変えれるモード
static const int Appry = 1;				//Appry
static const int Cancel = 2;				//Exit
static const int Size320x240 = 0;		//ウィンドウの大きさが320*240
static const int Size640x480 = 1;		//ウィンドウの大きさが640*480
static const int SizeFull = 2;			//ウィンドウの大きさがフルスクリーン
//static const int InterlinearSpace = 30;
class Option : public Mode  
{
private :
	int mode[Option_Menu];//おのおのにあった行のモードをを保持
	int Height;//行
	int SRAM[Option_Menu];//一時的に、initデータを保持しておく
	int WindowSize;//窓な大きさ
	void WriteInitFile();//朝起きて～♪書き込むiniファイル..........（Appryのとき発動
	void OptionEnd(bool flag = true);//オプションを終了したら呼び出される(AppryかCancel)
public:
	Option();//コンストラクタ。ここでファイルの読み込みをなさいますわ！！奥さん
	void Main();//肉体(精神はmodeにある)
};

#endif // !defined(AFX_OPTION_H__16F7011C_D5AF_4EE4_A878_75073E9E66F3__INCLUDED_)
