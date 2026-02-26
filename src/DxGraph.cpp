#include "DxGraph.h"
#include "common.h"

#include "bn_blending.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_log.h"

namespace
{
    int next_image_handle = 1;

    [[nodiscard]] bn::fixed _to_alpha(const int tr_all)
    {
        if(tr_all <= 0)
        {
            return 0;
        }

        if(tr_all >= 255)
        {
            return 1;
        }

        return bn::fixed(tr_all) / 255;
    }
}
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
	BN_LOG("draw::Initialize full:", full, " zoom:", zoom, " size:", X, "x", Y);
	bn::core::init();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////読み込み//////////////////////////////////////////////////////////////////////////
int draw :: LoadImage(char* FileName,int mode , int r , int g , int b)
{
	BN_LOG("draw::LoadImage path:", FileName, " mode:", mode, " trans:", r, ",", g, ",", b);
	return next_image_handle++;
}

//////////////////////////////背景と前景の合成////////////////////////////////////////
void draw::ColorChange(int num,bool flag)
{
	switch(num){
	case 0:
		mode = 0;
		break;
	case 1:
		mode = 1;
		break;
	case 2:
		mode = 2;
		break;
	case 3:
		mode = 3;
		break;
	default:
		mode = 4;
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
	bn::core::update();
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
	BN_LOG("draw::Draw image:", num, " src:", X, ",", Y, " size:", W, "x", H, " color:", r, ",", g, ",", b);

	if(flag){
		bn::blending::set_transparency_alpha(_to_alpha(tr_all));
	}else{
		switch(mode)
		{
		case NORMAL_BLEND:
			bn::blending::set_transparency_alpha(1);
			break;
		case TRANS_BLEND:
			bn::blending::set_transparency_alpha(_to_alpha(tr_all));
			break;
		case ADD_BLEND:
			bn::blending::set_intensity_alpha(_to_alpha(tr_all));
			break;
		default:
			bn::blending::set_transparency_alpha(_to_alpha(tr_all));
			break;
		}
	}

	if(match_vpos){
		transX = transX * scale_x;
		transY = transY * scale_y;
	}
	int pos_x = (int)transX + shift_x;
	int pos_y = (int)transY + shift_y;

	// NOTE:
	// Butano doesn't support loading BMP files or cropping/scaling them dynamically at runtime
	// in the same way as DxLib's DerivationGraph + DrawGraph APIs.
	// The game needs to be migrated to bn::sprite_item / bn::regular_bg_item generated assets,
	// and then this method can instantiate and update sprite/bg pointers from those items.
	//BN_LOG("draw::Draw dst:", pos_x, ",", pos_y, " scale:", scale_x, ",", scale_y);
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
