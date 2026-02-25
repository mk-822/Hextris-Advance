// ジョイパッド操作 JoyPadCtrl クラス
// キーボード状態だけはプロシージャから取ってくる必要があるので注意。

#include "Joypad.h"
#include "common.h"
#include "DxLib.h"
#include <windows.h>
#include <math.h>

JoyPadCtrl::JoyPadCtrl(){
	IniFileLoad();
	GetJoyStickDeviceState();
	keystate = 0;
	for(int i=0 ; i<MAXPLAYERS ; i++){
		joystate[i].input = 0;
	}
}

INPUT JoyPadCtrl::keystate;	// 静的メンバは使いますよって言ってやらんと使えんよ
INPUT JoyPadCtrl::keystate_old[KEYDELAY];
INPUT JoyPadCtrl::keystate_once;
INPUT JoyPadCtrl::keystate_leg;

// プロシージャから呼び出す静的メソッド。キー状態を入れる。
void JoyPadCtrl::SetKeyState(bool in, int key){
	if(in){
		keystate = keystate|key;
	}else{
		keystate = keystate&~key;
	}
}

// プレイヤーごとの入力状況をげｔするメソッド
INPUT JoyPadCtrl::GetKeyState(int player, int flag){	//0は加工なし,1はﾄ…ﾄﾄﾄ,2は一度だけ,3は押しっぱなしのみ
	INPUT tmp=0;
	for(int i=0 ; i<MAXPLAYERS ; i++){
		if((joystate[i].player == player)&&(joystate[i].enable)){
			switch(flag){
			case 0:
				tmp = tmp|joystate[i].input;
				break;
			case 1:
				tmp = tmp|joystate[i].inputleg;
				break;
			case 2:
				tmp = tmp|joystate[i].inputonce;
				break;
			case 3:
				tmp = tmp|joystate[i].inputold[KEYDELAY-1];
			}
		}
	}
	if(!player){
		switch(flag){
		case 0:
			tmp = tmp|keystate;
			break;
		case 1:
			tmp = tmp|keystate_leg;
			break;
		case 2:
			tmp = tmp|keystate_once;
			break;
		case 3:
			tmp = tmp|keystate_old[KEYDELAY-1];
		}
	}
	return tmp;
}

// 入力状況を更新するメソッド
void JoyPadCtrl::RenewKeyState(){
	char Buf[ 256 ] ;
	GetHitKeyStateAll( Buf ) ;
	if(Buf[KEY_INPUT_UP]){
		keystate|=UP;
	}else{
		keystate&=~UP;
	}
	if(Buf[KEY_INPUT_LEFT]){
		keystate|=LEFT;
	}else{
		keystate&=~LEFT;
	}
	if(Buf[KEY_INPUT_RIGHT]){
		keystate|=RIGHT;
	}else{
		keystate&=~RIGHT;
	}
	if(Buf[KEY_INPUT_DOWN]){
		keystate|=DOWN;
	}else{
		keystate&=~DOWN;
	}
	if(Buf[KEY_INPUT_Z]){
		keystate|=BUTTON[0];
	}else{
		keystate&=~BUTTON[0];
	}
	if(Buf[KEY_INPUT_X]){
		keystate|=BUTTON[1];
	}else{
		keystate&=~BUTTON[1];
	}
	if(Buf[KEY_INPUT_C]){
		keystate|=BUTTON[2];
	}else{
		keystate&=~BUTTON[2];
	}
	if(Buf[KEY_INPUT_V]){
		keystate|=BUTTON[3];
	}else{
		keystate&=~BUTTON[3];
	}
	keystate_once = keystate &~ keystate_old[0];
	keystate_leg = keystate_once;
	if(count % REPEATTIME == 0)
		keystate_leg = keystate_leg | keystate_old[KEYDELAY-1];

	for(int i=0 ; i<MAXPLAYERS ; i++){
		if(joystate[i].enable){
			joystate[i].inputold[0] = joystate[i].input;
			for(int j=KEYDELAY-1 ; 0<j ; j--){
				joystate[i].inputold[j] = joystate[i].inputold[j-1] & joystate[i].input;
			}
			
			JOYINFOEX ji; ZeroMemory( &ji, sizeof( JOYINFOEX ) );
			ji.dwSize = sizeof( JOYINFOEX );
			ji.dwFlags = JOY_RETURNBUTTONS | JOY_RETURNX | JOY_RETURNY;
			if( joyGetPosEx( i, &ji ) != JOYERR_NOERROR ) continue;
			if( GetJoypadInputState(i+1) & PAD_INPUT_UP ) joystate[i].input = joystate[i].input|UP;
			else joystate[i].input = joystate[i].input&~UP;
			if( GetJoypadInputState(i+1) & PAD_INPUT_DOWN ) joystate[i].input = joystate[i].input|DOWN;
			else joystate[i].input = joystate[i].input&~DOWN;
			if( GetJoypadInputState(i+1) & PAD_INPUT_LEFT ) joystate[i].input = joystate[i].input|LEFT;
			else joystate[i].input = joystate[i].input&~LEFT;
			if( GetJoypadInputState(i+1) & PAD_INPUT_RIGHT ) joystate[i].input = joystate[i].input|RIGHT;
			else joystate[i].input = joystate[i].input&~RIGHT;

			for( UINT j = 0; j < MAXBUTTONS; j++ ){
				if( GetJoypadInputState(i+1) >> 4 & (DWORD)pow( 2.0, joystate[i].button[j] )) joystate[i].input = joystate[i].input|BUTTON[j];
				else joystate[i].input = joystate[i].input&~BUTTON[j];
			}

			joystate[i].inputonce = joystate[i].input &~ joystate[i].inputold[0];
			joystate[i].inputleg = joystate[i].inputonce;
			if(count % REPEATTIME == 0)
				joystate[i].inputleg = joystate[i].inputleg | joystate[i].inputold[KEYDELAY-1];
		}
	}

	keystate_old[0] = keystate;
	for(int i=KEYDELAY-1 ; 0<i ; i--){
		keystate_old[i] = keystate_old[i-1] & keystate;
	}
	count++;
}

// -------ここから内部用------

void JoyPadCtrl::IniFileLoad(){
	char str[16];	// 文字列格納用
	for(int i=0; i<MAXPLAYERS; i++){
		if (GetPrivateProfileString(JOYNAME[i],KEYNAME[0],NULL,str,100,JOYINIPATH))		//まずはどのプレイヤーかをGET
			joystate[i].player=atoi(str);	// 取得した文字列を int に変換 (atoi = ASCII to INT)
		for(int j=1; j<MAXBUTTONS+1; j++){
			if (GetPrivateProfileString(JOYNAME[i],KEYNAME[j],NULL,str,100,JOYINIPATH))	//次にボタンのアサインをGET
				joystate[i].button[j-1]=atoi(str);
		}
	}
}

bool GamePadOK( int nGamePadID )
{
	JOYINFO ji;
	if( ( unsigned )nGamePadID >= joyGetNumDevs() ) return FALSE;
	if( joyGetPos( ( unsigned )nGamePadID, &ji ) == JOYERR_NOERROR ) return TRUE;
	return FALSE;
}

void JoyPadCtrl::GetJoyStickDeviceState(){
	for(int i=0; i<MAXPLAYERS; i++){
		joystate[i].enable = GamePadOK(i);
	}
}
