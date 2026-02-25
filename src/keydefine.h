#ifndef KEYDEFINEHEADER
#define KEYDEFINEHEADER

static const char* JOYINIPATH = "./setting.ini";	// 設定読み込み ini パス
static const int MAXPLAYERS = 4;					// 最高プレイヤー数
static const int MAXBUTTONS = 4;					// 最大ボタン数
static const int KEYDELAY = 15;						// リピートまでの時間
static const int REPEATTIME = 2;					// リピート間隔（n フレームに 1 回）
static const char* JOYNAME[MAXPLAYERS] = {"JoyPad1","JoyPad2","JoyPad3","JoyPad4"};
													// ini から読み込むための項目名
static const char* KEYNAME[MAXBUTTONS+1] = {"Player","Button1","Button2","Button3","Button4"};
													// 項目名そのに

static const int UP = 1;							// うえ
static const int DOWN = 2;							// した
static const int LEFT = 4;							// ひだり
static const int RIGHT = 8;							// みぎ
static const int BUTTON[] = {16,32,64,128};			// ぼたん

#endif