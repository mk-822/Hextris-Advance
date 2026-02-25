#include "StdAfx.h"
#include "dmusic.h"

// DirectMusicの初期化
bool Dmusic::init(HWND hWnd)
{
	g_lpPerformance	= NULL;		// パフォーマンス
	g_lpLoader		= NULL;		// ローダー
	
	HRESULT hr;

	// パフォーマンスの作成
	hr = CoCreateInstance( CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC,
						   IID_IDirectMusicPerformance8, (LPVOID*)&g_lpPerformance );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "DirectMusicPerformance8オブジェクトの作成に失敗", hr );
		return FALSE;
	}

	// パフォーマンスの初期化
	hr = g_lpPerformance->InitAudio( 
		NULL,									// IDirectMusicインターフェイスは不要
		NULL,									// IDirectSoundインターフェイスは不要
		hWnd,									// ウィンドウハンドル
		DMUS_APATH_SHARED_STEREOPLUSREVERB,		// デフォルトのオーディオパスタイプ
		64,										// パフォーマンスチャンネルの数
		DMUS_AUDIOF_ALL,						// シンセサイザの機能
		NULL );									// オーディオパラメータにはデフォルトを使用
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "DirectMusicPerformance8の初期化に失敗", hr );
		return FALSE;
	}

	// ローダーの作成
	hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC,
						   IID_IDirectMusicLoader8, (LPVOID*)&g_lpLoader );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "DirectMusicLoader8オブジェクトの作成に失敗", hr );
		return FALSE;
	}

	// ローダーの初期化（検索パスをカレントディレクトリに設定）
	CHAR strPath[ MAX_PATH ];
	DWORD i = GetCurrentDirectory( MAX_PATH, strPath );
	if( i==0 || MAX_PATH<i )
	{
		DXTRACE_MSG( "カレントディレクトリの取得に失敗" );
		return FALSE;
	}

	// マルチバイト文字をUNICODEに変換
	WCHAR wstrSearchPath[ MAX_PATH ];
	MultiByteToWideChar( CP_ACP, 0, strPath, -1, wstrSearchPath, MAX_PATH );

	// ローダーに検索パスを設定
	hr = g_lpLoader->SetSearchDirectory( GUID_DirectMusicAllTypes, wstrSearchPath, FALSE );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "検索パスの設定に失敗", hr );
		return FALSE;
	}

	return TRUE;
}

bool Dmusic::play(char* filename,int loop)
{
	g_lpSegment		= NULL;		// セグメント
	HRESULT hr;

	stop();

	// セグメント作成
	WCHAR wstrFileName[MAX_PATH];
	MultiByteToWideChar( CP_ACP, 0, filename, -1, wstrFileName, MAX_PATH );
	hr = g_lpLoader->LoadObjectFromFile( CLSID_DirectMusicSegment, IID_IDirectMusicSegment8,
										 wstrFileName, (LPVOID*)&g_lpSegment );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "セグメントのロードに失敗", hr );
		return FALSE;
	}

	// MIDIファイルの場合は、パラメータを設定する
	hr = g_lpSegment->SetParam( GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL );
	if( FAILED(hr) )
	{
		DXTRACE_ERR( "セグメントのパラメータ設定に失敗", hr );
		return FALSE;
	}

	// バンドのダウンロード
	hr = g_lpSegment->Download( g_lpPerformance );
	if( FAILED(hr) )
	{
		DXTRACE_ERR("セグメント１のバンドのダウンロードに失敗", hr);
		return FALSE;
	}

	if(loop<0){
		g_lpSegment->SetRepeats( DMUS_SEG_REPEAT_INFINITE );
	}else{
		g_lpSegment->SetRepeats( loop );
	}

	// セグメントの再生
	g_lpPerformance->PlaySegmentEx(
		g_lpSegment,	// 演奏するセグメント
		NULL,			// 使用しない
		NULL,			// トランザクション用
		0,				// フラグ
		0,				// 開始タイム、0 は即時
		NULL,			// セグメント状態を受け取るポインタ
		NULL,			// 停止するオブジェクト
		NULL			// オーディオパス、デフォルトでない場合
	);

	return TRUE;
}

bool Dmusic::stop(){
	HRESULT hr;

	// 演奏停止
	if( g_lpPerformance )
	{
		hr = g_lpPerformance->Stop( NULL, NULL, 0, 0 );
		if( FAILED(hr) )
			DXTRACE_ERR( "全セグメントの演奏停止に失敗", hr );
	}

	return TRUE;
}

bool Dmusic::volume(long vol){
	g_lpPerformance->SetGlobalParam( GUID_PerfMasterVolume, &vol, sizeof(long) );
	return TRUE;
}

long Dmusic::volume(){
	long lVolume;
	g_lpPerformance->GetGlobalParam( GUID_PerfMasterVolume, &lVolume, sizeof(long) );
	return lVolume;
}


// DirectMusicの終了処理
bool Dmusic::release()
{
	HRESULT hr;

	// 演奏停止
	stop();

	// セグメントのアンロード
	if( g_lpSegment )
	{
		hr = g_lpSegment->Unload( g_lpPerformance );
		if( FAILED(hr) )
			DXTRACE_ERR( "セグメントのアンロードに失敗", hr );
	}

	// セグメントを開放
	RELEASE( g_lpSegment );

	// ローダを開放
	RELEASE( g_lpLoader );

	// パフォーマンスを解放
	if( g_lpPerformance )
	{
		hr = g_lpPerformance->CloseDown();
		if( FAILED(hr) )
			DXTRACE_ERR( "IDirectMusicPerformance8::CloseDownに失敗", hr );
	}
	RELEASE( g_lpPerformance );

	return TRUE;
}
