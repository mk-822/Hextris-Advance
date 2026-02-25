#include "DxGraph.h"
#include "resource.h"
#include "common.h"
////////////////////////////コンストラクタ//////////////////////////////////
draw :: draw()//初期化が必要な変数
{
	shift_x = 0;
	shift_y = 0;
	scale_x = 1;
	scale_y = 1;
}

draw::~draw()//デストラクタで開放
{
}
////////////////////////////////////////////////////////////////////////////


////////解像度指定//////////////////////////////////////////////////////////
bool draw :: Initialize(bool full , int zoom , int X , int Y)
{
	// 起動方法の指定
	if(GetPrivateProfileInt("Window","Vram",1,WINDOW_INIPATH)){
	}else{
		SetUse3DFlag(FALSE);
		SetUseVramFlag(FALSE);
		SetScreenMemToVramFlag(FALSE);
	}

	// 画面モードの変更
	SetGraphMode( X , Y , GetPrivateProfileInt("Window","ColorBit",32,WINDOW_INIPATH));
	SetWindowSizeExtendRate( (double)zoom );
	SetOutApplicationLogValidFlag(GetPrivateProfileInt("Main","Log",0,WINDOW_INIPATH));
	SetMainWindowText("HEXARIS 2007 SUMMER EDITION");

	ChangeWindowMode( !full );
	if( DxLib_Init() == -1 ){
		return false;
	}

	// 描画先画面を裏にする
	SetDrawScreen( DX_SCREEN_BACK ) ;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////読み込み//////////////////////////////////////////////////////////////////////////
int draw :: LoadImage(char* FileName,int mode , int r , int g , int b)
{
	return LoadGraph(FileName);
}

//////////////////////////////背景と前景の合成////////////////////////////////////////
void draw::ColorChange(int num,bool flag)
{
	switch(num){
	case 0:
		mode = DX_BLENDMODE_NOBLEND;
		break;
	case 1:
		mode = DX_BLENDMODE_ALPHA;
		break;
	case 2:
		mode = DX_BLENDMODE_ADD;
		break;
	case 3:
		mode = DX_BLENDMODE_SUB;
		break;
	default:
		mode = DX_BLENDMODE_MUL;
		break;
	}
}
//ブレンドの種類	INVSRCALPHA INVDESTALPHA ONE ZERO SRCCOLOR DESTCOLOR INVSRCCOLOR INVDESTCOLOR SRCALPHA DESTALPHA
//DESTは背景、SRCは描画する画像

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////描画の初期化////////////////////////////////////////////////////
void draw::DrawBegin(bool Clear)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////描画を終了させる//////////////////////////////////////////////////////////////
void draw :: DrawEnd()
{
	// 裏画面の内容を表画面に反映します
	ScreenFlip() ;
	// 画面を初期化
	ClearDrawScreen() ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////描画範囲の指定///////////////////////////////////////////////////////////////////
void draw :: TexturePos(int x, int y, int w, int h)
{
	X = x;
	Y = y;
	W = w;
	H = h;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////描画//////////////////////////////////////////////////////////////////




void draw :: Draw(int num , float transX , float transY , bool flag, int tr_all, int r , int g , int b)
{
	SetTransColor(r, g, b);
	if(flag){
		if(tr_all>=255){
			tr_all = 255;
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, tr_all);
		}else{
			if(tr_all<0)
				tr_all = 0;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, tr_all);
		}
	}else{
		SetDrawBlendMode(mode, tr_all);
	}
	int tmp = DerivationGraph( X, Y, W, H, num);
	if(match_vpos){
		transX = transX * scale_x;
		transY = transY * scale_y;
	}
	int pos_x = (int)transX + shift_x;
	int pos_y = (int)transY + shift_y;
	if((scale_x==1)&&(scale_y==1)){
		DrawGraph( pos_x, pos_y, tmp, TRUE);
	}else{
		DrawExtendGraph( pos_x, pos_y, pos_x+(int)(W*scale_x), pos_y+(int)(H*scale_y), tmp, TRUE);
	}
	DeleteGraph(tmp);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void draw::ChangeEFXState(float scaleX, float scaleY, SPIN Spin, float SPCX, float SPCY)
{
	scale_x = scaleX;
	scale_y = scaleY;
}

void draw::ShiftDrawPosition(int x, int y)
{
	shift_x = x;
	shift_y = y;
}

void draw::MatchVirtualPosition(bool flag)
{
	match_vpos = flag;
}
