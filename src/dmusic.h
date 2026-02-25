#ifndef __DMUSIC_H__
#define __DMUSIC_H__

#include <dmusici.h>
#include <dxerr9.h>
#define RELEASE(p)	{ if(p){(p)->Release();p=NULL;} }

class Dmusic{
public:
	bool init(HWND hWnd);
	//初期化します。p1 にはウィンドウハンドル

	bool play(char* filename,int loop);
	//読み込み＆再生します。p1 にはファイル名(相対パス可)、p2 にはループ回数(-1で無限ループ)

	bool stop();
	//再生を停止します。

	bool volume(long vol);
	//ボリュームを設定します。

	long volume();
	//ボリュームを取得します。

	bool release();
	//解放します。

private:
	// DirectMusicの変数
	IDirectMusicPerformance8* g_lpPerformance;		// パフォーマンス
	IDirectMusicLoader8* g_lpLoader;		// ローダー
	IDirectMusicSegment8* g_lpSegment;		// セグメント
};

#endif