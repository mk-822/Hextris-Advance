#ifndef COMMONHEADER
#define COMMONHEADER

#define for if(0);else for	//for の謎スコープを無理矢理fix
#pragma warning ( disable : 4996 )
#include "bn_timer.h"
#include "bn_timers.h"
#include "Sound.h"

inline int compat_strlen(const char* text){
	if(! text){
		return 0;
	}

	int len = 0;
	while(text[len] != '\0'){
		len++;
	}

	return len;
}

inline unsigned long GetTickCount(){
	static bn::timer timer;
	const int elapsed_ticks = timer.elapsed_ticks();
	const int ticks_per_second = bn::timers::ticks_per_second();

	return static_cast<unsigned long>(
		(static_cast<long long>(elapsed_ticks) * 1000) / ticks_per_second
	);
}

//画面モード定義
static const char* WINDOW_INIPATH = "./setting.ini";
static const int WINDOW_WIDE = 320;
static const int WINDOW_HEIGHT = 240;
extern bool WINDOW_FULL;
extern int WINDOW_ZOOM;

//画像ファイル定義
static const int RANK_IMG = 0;
static const int BLANK_IMG = 1;
static const int FONT_IMG = 2;
static const int FONTEX_IMG = 3;
static const int BLOCK_IMG = 4;
static const int LOGO_IMG = 5;
static const int TITLE_IMG = 6;
static const int WORDS_IMG = 7;
static const int FRAME_IMG = 8;
static const int BG_IMG[] = {9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
static const int SCOREBG_IMG = 26;
static const int BIGINT_IMG = 27;
static const int BIGFONT_IMG = 28;
static const int WHITEFONT_IMG = 29;
static const int OP_BG_IMG = 30;
static const int GRAYFONT_IMG = 31;
static const int MULTI_IMG = 32;

//シーン管理定義
static const int LOGO_SCENE = 0;			// ロゴ画面
static const int TITLE_SCENE = 1;			// タイトル画面
//static const int OPTION_SCENE = 2;			//オプション
static const int SCORE_SCENE = 3;			// スコアランキング
static const int SINGLE_SCENE = 4;		// シングルプレイ
//static const int MULTI_SCENE = 5;		// マルチプレイ

//ブレンド方法定義
static const int NORMAL_BLEND = 0;	// 通常ブレンド
static const int TRANS_BLEND = 1;	// 透過付きブレンド
static const int ADD_BLEND = 2;		// 加算合成

#endif
