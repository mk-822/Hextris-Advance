#include "DxGraph.h"
#include "common.h"

#include "bn_blending.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_log.h"

#include "bn_regular_bg_items_bg00.h"
#include "bn_regular_bg_items_bg01.h"
#include "bn_regular_bg_items_bg02.h"
#include "bn_regular_bg_items_bg03.h"
#include "bn_regular_bg_items_bg04.h"
#include "bn_regular_bg_items_bg05.h"
#include "bn_regular_bg_items_bg06.h"
#include "bn_regular_bg_items_bg07.h"
#include "bn_regular_bg_items_bg08.h"
#include "bn_regular_bg_items_bg09.h"
#include "bn_regular_bg_items_bg10.h"
#include "bn_regular_bg_items_bg11.h"
#include "bn_regular_bg_items_blank.h"
#include "bn_regular_bg_items_blocks.h"
#include "bn_regular_bg_items_font.h"
#include "bn_regular_bg_items_font16.h"
#include "bn_regular_bg_items_font_big.h"
#include "bn_regular_bg_items_font_ex.h"
#include "bn_regular_bg_items_font_gray.h"
#include "bn_regular_bg_items_frame.h"
#include "bn_regular_bg_items_logo.h"
#include "bn_regular_bg_items_logobg.h"
#include "bn_regular_bg_items_multi.h"
#include "bn_regular_bg_items_rank.h"
#include "bn_regular_bg_items_scorebg.h"
#include "bn_regular_bg_items_title.h"
#include "bn_regular_bg_items_whitefont.h"
#include "bn_regular_bg_items_words.h"

#include "bn_string_view.h"

namespace
{
    int next_image_handle = 1;

    struct image_entry
    {
        int handle = -1;
        int width = 0;
        int height = 0;
    };

    image_entry loaded_images[IMG_MAX];

    [[nodiscard]] const image_entry* _find_loaded_image(const int handle)
    {
        for(const image_entry& entry : loaded_images)
        {
            if(entry.handle == handle)
            {
                return &entry;
            }
        }

        return nullptr;
    }

    bool _register_image(const int handle, const int width, const int height)
    {
        for(image_entry& entry : loaded_images)
        {
            if(entry.handle < 0)
            {
                entry.handle = handle;
                entry.width = width;
                entry.height = height;
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] bool _match_name(const char* file_name, const bn::string_view expected_name)
    {
        if(! file_name)
        {
            return false;
        }

        const bn::string_view path(file_name);
        const int slash_index = path.rfind('/');
        const int name_index = slash_index >= 0 ? slash_index + 1 : 0;
        const bn::string_view name_with_extension = path.substr(name_index);
        const int extension_index = name_with_extension.rfind('.');
        const bn::string_view name = extension_index > 0 ?
                name_with_extension.substr(0, extension_index) : name_with_extension;
        return name == expected_name;
    }

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

	const int handle = next_image_handle++;

	if(_match_name(FileName, "rank")) _register_image(handle, 512, 32);
	else if(_match_name(FileName, "blank")) _register_image(handle, 512, 256);
	else if(_match_name(FileName, "font")) _register_image(handle, 1024, 8);
	else if(_match_name(FileName, "font_ex")) _register_image(handle, 1024, 8);
	else if(_match_name(FileName, "blocks")) _register_image(handle, 512, 256);
	else if(_match_name(FileName, "logo")) _register_image(handle, 64, 32);
	else if(_match_name(FileName, "title")) _register_image(handle, 256, 256);
	else if(_match_name(FileName, "words")) _register_image(handle, 256, 256);
	else if(_match_name(FileName, "frame")) _register_image(handle, 256, 256);
	else if(_match_name(FileName, "font_big")) _register_image(handle, 512, 16);
	else if(_match_name(FileName, "whitefont")) _register_image(handle, 1024, 8);
	else if(_match_name(FileName, "font_gray")) _register_image(handle, 1024, 8);
	else if(_match_name(FileName, "logobg")) _register_image(handle, 320, 240);
	else if(_match_name(FileName, "scorebg")) _register_image(handle, 320, 240);
	else if(_match_name(FileName, "font16")) _register_image(handle, 2048, 16);
	else if(_match_name(FileName, "multi")) _register_image(handle, 128, 64);
	else if(_match_name(FileName, "bg00") || _match_name(FileName, "bg01") || _match_name(FileName, "bg02") ||
			_match_name(FileName, "bg03") || _match_name(FileName, "bg04") || _match_name(FileName, "bg05") ||
			_match_name(FileName, "bg06") || _match_name(FileName, "bg07") || _match_name(FileName, "bg08") ||
			_match_name(FileName, "bg09") || _match_name(FileName, "bg10") || _match_name(FileName, "bg11")){
		_register_image(handle, 256, 256);
	}else{
		BN_LOG("draw::LoadImage unknown asset:", FileName);
	}

	return handle;
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
	const image_entry* image = _find_loaded_image(num);

	if(! image)
	{
		BN_LOG("draw::Draw unknown handle:", num);
		return;
	}

	if(X != 0 || Y != 0 || W != image->width || H != image->height || scale_x != 1 || scale_y != 1)
	{
		BN_LOG("draw::Draw partial/scale draw not supported yet. handle:", num,
				" src:", X, ",", Y, " size:", W, "x", H, " image:", image->width, "x", image->height,
				" scale:", scale_x, ",", scale_y);
		return;
	}

	if(num == RANK_IMG) bn::regular_bg_items::rank.create_bg(pos_x, pos_y);
	else if(num == BLANK_IMG) bn::regular_bg_items::blank.create_bg(pos_x, pos_y);
	else if(num == FONT_IMG) bn::regular_bg_items::font.create_bg(pos_x, pos_y);
	else if(num == FONTEX_IMG) bn::regular_bg_items::font_ex.create_bg(pos_x, pos_y);
	else if(num == BLOCK_IMG) bn::regular_bg_items::blocks.create_bg(pos_x, pos_y);
	else if(num == LOGO_IMG) bn::regular_bg_items::logo.create_bg(pos_x, pos_y);
	else if(num == TITLE_IMG) bn::regular_bg_items::title.create_bg(pos_x, pos_y);
	else if(num == WORDS_IMG) bn::regular_bg_items::words.create_bg(pos_x, pos_y);
	else if(num == FRAME_IMG) bn::regular_bg_items::frame.create_bg(pos_x, pos_y);
	else if(num == BIGINT_IMG) bn::regular_bg_items::font_big.create_bg(pos_x, pos_y);
	else if(num == WHITEFONT_IMG) bn::regular_bg_items::whitefont.create_bg(pos_x, pos_y);
	else if(num == GRAYFONT_IMG) bn::regular_bg_items::font_gray.create_bg(pos_x, pos_y);
	else if(num == OP_BG_IMG) bn::regular_bg_items::logobg.create_bg(pos_x, pos_y);
	else if(num == SCOREBG_IMG) bn::regular_bg_items::scorebg.create_bg(pos_x, pos_y);
	else if(num == BIGFONT_IMG) bn::regular_bg_items::font16.create_bg(pos_x, pos_y);
	else if(num == MULTI_IMG) bn::regular_bg_items::multi.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[0]) bn::regular_bg_items::bg00.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[1]) bn::regular_bg_items::bg01.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[2]) bn::regular_bg_items::bg02.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[3]) bn::regular_bg_items::bg03.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[4]) bn::regular_bg_items::bg04.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[5]) bn::regular_bg_items::bg05.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[6]) bn::regular_bg_items::bg06.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[7]) bn::regular_bg_items::bg07.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[8]) bn::regular_bg_items::bg08.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[9]) bn::regular_bg_items::bg09.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[10]) bn::regular_bg_items::bg10.create_bg(pos_x, pos_y);
	else if(num == BG_IMG[11]) bn::regular_bg_items::bg11.create_bg(pos_x, pos_y);
	else BN_LOG("draw::Draw no asset mapping for handle:", num);
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
