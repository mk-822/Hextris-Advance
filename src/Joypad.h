#ifndef JOYPADHEADER
#define JOYPADHEADER

#include "keydefine.h"

typedef unsigned int INPUT;

class JoyPadCtrl{
private:	
	struct GamePadState
	{
		INPUT input;		// どのボタンが押されているか(keydefine.h に一覧があります)
		INPUT inputleg;		// ト・・・・トトトトトトトトトトト
		INPUT inputonce;	// ト。
		INPUT inputold[KEYDELAY];	// 入力状態のバックアップ

		int player;					// どのプレイヤーにアサインするか(ini から読み込み)
		int button[MAXBUTTONS];				// どのボタンにアサインするか(ini から読み込み)
		bool enable;					// 使用可能かどうか
	}joystate[MAXPLAYERS];
	static INPUT keystate;
	static INPUT keystate_leg;
	static INPUT keystate_once;
	static INPUT keystate_old[KEYDELAY];

	int count;							// カウンター（キーリピートするためだけに）
	void IniFileLoad();					// ini ファイルをロードするための内部メソッド
	void GetJoyStickDeviceState();
public:
	JoyPadCtrl();
	static void SetKeyState(bool in, int key);	// WM_KEYDOWN WM_KEYUP の時にキーボード情報を更新する用
	void RenewKeyState();				// 入力状況を更新
	INPUT GetKeyState(int player, int flag=0);		// プレイヤーごとの入力状態をげと
};

#endif