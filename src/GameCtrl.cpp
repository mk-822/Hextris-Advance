#include "GameCtrl.h"
#include "logo.h"
#include "soloplay.h"
#include "title.h"
#include "ScoreDisplay.h"
#include <stdarg.h>
#include "bn_array.h"
#include "bn_optional.h"
#include "bn_sprite_items_font.h"
#include "bn_sprite_items_font16.h"
#include "bn_sprite_items_font16_2.h"
#include "bn_sprite_items_font_gray.h"
#include "bn_sprite_items_whitefont.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_items_font_bg.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_tiles_ptr.h"
#include "bn_bg_palette_ptr.h"
#include "bn_blending.h"

namespace
{
	struct text_bg_layer
	{
		bn::optional<bn::regular_bg_ptr> bg;
		bn::optional<bn::regular_bg_map_ptr> map;
		bn::array<int, 128> glyph_tiles = {};
		bn::array<int, 128> white_glyph_tiles = {};
		int space_tile = 0;
	};

	text_bg_layer _font_bg_layer;

	enum class font_kind
	{
		normal_8,
		white_8,
		gray_8,
		normal_16,
		int_16
	};

	struct font_sprite_slot
	{
		bn::optional<bn::sprite_ptr> sprite;
		font_kind kind = font_kind::normal_8;
	};

	constexpr int FONT_SPRITE_MAX = 112;
	bn::array<font_sprite_slot, FONT_SPRITE_MAX> _font_sprite_slots;
	int _font_sprite_used = 0;

	int _virtual_to_gba(int value)
	{
		return value - 40;
	}

	int _tile_div8(int value)
	{
		if(value >= 0)
		{
			return value / 8;
		}

		return -(((-value) + 7) / 8);
	}

	void _init_text_bg_layer(
			text_bg_layer& layer,
			const bn::regular_bg_item& item,
			const bn::regular_bg_map_cell* glyph_map_cells)
	{
		if(layer.bg)
		{
			return;
		}

		layer.bg = item.create_bg(0, 0);
		layer.bg->set_priority(0);
		layer.bg->set_z_order(0);
		layer.bg->set_visible(false);

		layer.map = bn::regular_bg_map_ptr::allocate(bn::size(32, 32), layer.bg->tiles(), layer.bg->palette());
		layer.bg->set_map(*layer.map);

		for(int i = 0; i < 128; ++i)
		{
			const int cell_x = i % 16;
			const int cell_y = i / 16;
			const int map_index = cell_y * 32 + cell_x;
			bn::regular_bg_map_cell_info info(glyph_map_cells[map_index]);
			layer.glyph_tiles[i] = info.tile_index();

			const int white_map_index = (cell_y + 8) * 32 + cell_x;
			bn::regular_bg_map_cell_info white_info(glyph_map_cells[white_map_index]);
			layer.white_glyph_tiles[i] = white_info.tile_index();
		}

		// Use tile 0 as clear cell to avoid depending on font glyph layout.
		layer.space_tile = 0;
	}

	void _clear_text_bg_layer(text_bg_layer& layer)
	{
		if(! layer.map)
		{
			return;
		}

		bn::optional<bn::span<bn::regular_bg_map_cell>> vram = layer.map->vram();
		if(! vram)
		{
			return;
		}

		bn::regular_bg_map_cell_info info;
		info.set_tile_index(layer.space_tile);
		const bn::regular_bg_map_cell space_cell = info.cell();

		for(bn::regular_bg_map_cell& cell : *vram)
		{
			cell = space_cell;
		}
	}

	void _release_text_bg_layer(text_bg_layer& layer)
	{
		if(layer.bg)
		{
			layer.bg.reset();
		}

		if(layer.map)
		{
			layer.map.reset();
		}
	}

	void release_image_font_resources()
	{
		_release_text_bg_layer(_font_bg_layer);
	}

	void release_font_sprite_resources()
	{
		_font_sprite_used = 0;

		for(font_sprite_slot& slot : _font_sprite_slots)
		{
			if(slot.sprite)
			{
				slot.sprite.reset();
			}
		}
	}

	void _draw_string_bg_impl(text_bg_layer& layer, int x, int y, bool white, const char* buffer)
	{
		if(! layer.map)
		{
			return;
		}

		if(layer.bg)
		{
			layer.bg->set_visible(true);
		}

		const int base_x = _virtual_to_gba(x);
		const int base_y = _virtual_to_gba(y);
		const int row = _tile_div8(base_y);
		if(row < 0 || row >= 32)
		{
			return;
		}

		bn::optional<bn::span<bn::regular_bg_map_cell>> vram = layer.map->vram();
		if(! vram)
		{
			return;
		}

		const int len = compat_strlen(buffer);
		for(int i = 0; i < len; ++i)
		{
			const int draw_x = base_x + i * 8;
			const int col = _tile_div8(draw_x);
			if(col < 0 || col >= 32)
			{
				continue;
			}

			int glyph = static_cast<unsigned char>(buffer[i]) - 32;
			if(glyph < 0 || glyph >= 128)
			{
				glyph = 0;
			}

			// Leave background transparent for spaces.
			if(glyph == 0)
			{
				continue;
			}

			bn::regular_bg_map_cell_info info((*vram)[row * 32 + col]);
			info.set_tile_index(white ? layer.white_glyph_tiles[glyph] : layer.glyph_tiles[glyph]);
			(*vram)[row * 32 + col] = info.cell();
		}
	}

	void begin_image_font_frame()
	{
		_font_sprite_used = 0;
		_clear_text_bg_layer(_font_bg_layer);
		if(_font_bg_layer.bg)
		{
			_font_bg_layer.bg->set_visible(false);
		}

		for(font_sprite_slot& slot : _font_sprite_slots)
		{
			if(slot.sprite)
			{
				slot.sprite->set_visible(false);
			}
		}
	}

	int _bn_x_from_virtual(int x, int sizex)
	{
		return x + sizex / 2 - WINDOW_WIDE / 2;
	}

	int _bn_y_from_virtual(int y, int sizey)
	{
		return y + sizey / 2 - WINDOW_HEIGHT / 2;
	}

	int _glyph_index(char chr)
	{
		int index = static_cast<unsigned char>(chr) - 32;
		if(index < 0 || index >= 128)
		{
			index = 0;
		}
		return index;
	}

	font_kind _font_kind(int font, int sizex, int sizey)
	{
		if(sizex == 16 && sizey == 16)
		{
			if(font == BIGINT_IMG)
			{
				return font_kind::int_16;
			}
			return font_kind::normal_16;
		}

		if(font == WHITEFONT_IMG)
		{
			return font_kind::white_8;
		}

		if(font == GRAYFONT_IMG)
		{
			return font_kind::gray_8;
		}

		return font_kind::normal_8;
	}

	bn::sprite_ptr _create_font_sprite(font_kind kind, int x, int y, int glyph)
	{
		switch(kind)
		{
		case font_kind::white_8:
			return bn::sprite_items::whitefont.create_sprite(x, y, glyph);
		case font_kind::gray_8:
			return bn::sprite_items::font_gray.create_sprite(x, y, glyph);
		case font_kind::normal_16:
			return bn::sprite_items::font16.create_sprite(x, y, glyph);
		case font_kind::int_16:
			return bn::sprite_items::font16_2.create_sprite(x, y, glyph);
		case font_kind::normal_8:
		default:
			return bn::sprite_items::font.create_sprite(x, y, glyph);
		}
	}

	void _set_font_tiles(bn::sprite_ptr& sprite, font_kind kind, int glyph)
	{
		switch(kind)
		{
		case font_kind::white_8:
			sprite.set_tiles(bn::sprite_items::whitefont.tiles_item(), glyph);
			break;
		case font_kind::gray_8:
			sprite.set_tiles(bn::sprite_items::font_gray.tiles_item(), glyph);
			break;
		case font_kind::normal_16:
			sprite.set_tiles(bn::sprite_items::font16.tiles_item(), glyph);
			break;
		case font_kind::int_16:
			sprite.set_tiles(bn::sprite_items::font16_2.tiles_item(), glyph);
			break;
		case font_kind::normal_8:
		default:
			sprite.set_tiles(bn::sprite_items::font.tiles_item(), glyph);
			break;
		}
	}

	void _draw_sprite_font_impl(int x, int y, int font, int sizex, int sizey, int offsetx, const char* buffer)
	{
		const int len = compat_strlen(buffer);
		for(int i = 0; i < len && _font_sprite_used < FONT_SPRITE_MAX; ++i)
		{
			const int glyph = _glyph_index(buffer[i]);
			const int draw_x = x + i * (sizex + offsetx);
			const int sprite_x = _bn_x_from_virtual(draw_x, sizex);
			const int sprite_y = _bn_y_from_virtual(y, sizey);
			const font_kind kind = _font_kind(font, sizex, sizey);

			font_sprite_slot& slot = _font_sprite_slots[_font_sprite_used++];
			if(! slot.sprite || slot.kind != kind)
			{
				slot.sprite.reset();
				slot.sprite = _create_font_sprite(kind, sprite_x, sprite_y, glyph);
				slot.kind = kind;
			}
			else
			{
				slot.sprite->set_position(sprite_x, sprite_y);
				_set_font_tiles(*slot.sprite, kind, glyph);
			}

			slot.sprite->set_visible(true);
		}
	}
}

GameCtrl::GameCtrl(){
	// Number of frames since game start
	count = 0;
	scene = LOGO_SCENE;
	mode = new Mode();
	mode->Initialize(&scene,&dxg,&image,&input);

	// Initialize clock library & load images
	dxg.Initialize(WINDOW_FULL,WINDOW_ZOOM,WINDOW_WIDE,WINDOW_HEIGHT);
	image.i[RANK_IMG] = dxg.LoadImage("./image/rank.bmp",1,0,0,0);//drawing load(file pathchar* , drawing mode , transparent colorR , transparent colorG , transparent colorB)
	image.i[BLANK_IMG] = BLANK_IMG;
	//image.i[BLANK_IMG] = dxg.LoadImage("./image/blank.bmp",0,0,0,0);
	image.i[FONT_IMG] = dxg.LoadImage("./image/font.bmp",1,0,0,0);
	image.i[FONTEX_IMG] = dxg.LoadImage("./image/font_ex.bmp",1,0,0,0);
	image.i[BLOCK_IMG] = dxg.LoadImage("./image/blocks.bmp",1,0,0,0);
	image.i[LOGO_IMG] = dxg.LoadImage("./image/logo.bmp",1,0,0,0);
	image.i[TITLE_IMG] = dxg.LoadImage("./image/title.bmp",1,0,0,0);
	image.i[WORDS_IMG] = dxg.LoadImage("./image/words.bmp",1,0,0,0);
	image.i[FRAME_IMG] = dxg.LoadImage("./image/frame.bmp",1,0,0,0);
	image.i[BIGINT_IMG] = dxg.LoadImage("./image/font_big.bmp",1,0,0,0);
	image.i[WHITEFONT_IMG] = dxg.LoadImage("./image/whitefont.bmp",1,0,0,0);
	image.i[GRAYFONT_IMG] = dxg.LoadImage("./image/font_gray.bmp",1,0,0,0);
	image.i[OP_BG_IMG] = dxg.LoadImage("./image/logobg.bmp",1,0,0,0);
	image.i[SCOREBG_IMG] = dxg.LoadImage("./image/scorebg.bmp",1,0,0,0);
	image.i[BIGFONT_IMG] = dxg.LoadImage("./image/font16.bmp",1,0,0,0);
	image.i[MULTI_IMG] = dxg.LoadImage("./image/multi.bmp",1,0,0,0);

	// image.i[BG_IMG[0]] = dxg.LoadImage("./image/bg00.bmp",1,0,0,0);
	// image.i[BG_IMG[1]] = dxg.LoadImage("./image/bg01.bmp",1,0,0,0);
	// image.i[BG_IMG[2]] = dxg.LoadImage("./image/bg02.bmp",1,0,0,0);
	// image.i[BG_IMG[3]] = dxg.LoadImage("./image/bg03.bmp",1,0,0,0);
	// image.i[BG_IMG[4]] = dxg.LoadImage("./image/bg04.bmp",1,0,0,0);
	// image.i[BG_IMG[5]] = dxg.LoadImage("./image/bg05.bmp",1,0,0,0);
	// image.i[BG_IMG[6]] = dxg.LoadImage("./image/bg06.bmp",1,0,0,0);
	// image.i[BG_IMG[7]] = dxg.LoadImage("./image/bg07.bmp",1,0,0,0);
	// image.i[BG_IMG[8]] = dxg.LoadImage("./image/bg08.bmp",1,0,0,0);
	// image.i[BG_IMG[9]] = dxg.LoadImage("./image/bg09.bmp",1,0,0,0);
	// image.i[BG_IMG[10]] = dxg.LoadImage("./image/bg10.bmp",1,0,0,0);
	// image.i[BG_IMG[11]] = dxg.LoadImage("./image/bg11.bmp",1,0,0,0);
	image.i[BG_IMG[0]] = BG_IMG[0];
	image.i[BG_IMG[1]] = BG_IMG[1];
	image.i[BG_IMG[2]] = BG_IMG[2];
	image.i[BG_IMG[3]] = BG_IMG[3];
	image.i[BG_IMG[4]] = BG_IMG[4];
	image.i[BG_IMG[5]] = BG_IMG[5];
	image.i[BG_IMG[6]] = BG_IMG[6];
	image.i[BG_IMG[7]] = BG_IMG[7];
	image.i[BG_IMG[8]] = BG_IMG[8];
	image.i[BG_IMG[9]] = BG_IMG[9];
	image.i[BG_IMG[10]] = BG_IMG[10];
	image.i[BG_IMG[11]] = BG_IMG[11];

	// Butano Since the image handle is not used on the side, the font type is fixed.IDhold in
	image.i[FONT_IMG] = FONT_IMG;
	image.i[BIGINT_IMG] = BIGINT_IMG;
	image.i[BIGFONT_IMG] = BIGFONT_IMG;
	image.i[WHITEFONT_IMG] = WHITEFONT_IMG;
	image.i[GRAYFONT_IMG] = GRAYFONT_IMG;

	begin_image_font_frame();
	DrawImageFont(8,8,&dxg,image.i[FONT_IMG],"Now Loading...");
	dxg.DrawEnd();
}

// It’s the main thing!-----------------
void GameCtrl::Main(){
	begin_image_font_frame();
	dxg.DrawBegin(true);
	input.RenewKeyState();

	// If the scene and scene backup are different, it will be a mode change.
	if(scene!=scene_bk){
		ChangeScene();
	}scene_bk = scene;

	mode->Main();

	//For input check
	//::DrawImageFont(0,0,&dxg,image.i[FONT_IMG],"%d %d %d",input.GetKeyState(0,0),input.GetKeyState(0,1),input.GetKeyState(0,2));

	dxg.DrawEnd();

	// Atoshimatsu
	count++;
}
// Main end-------------------------------------

void GameCtrl::ChangeScene(){
	release_image_font_resources();
	release_font_sprite_resources();
	bn::blending::restore();
	delete mode;
	switch(scene){
	case LOGO_SCENE:
		mode = new Logo();
		break;
	case TITLE_SCENE:
		mode = new Title();
		break;
	case SCORE_SCENE:
		mode = new ScoreDisplay();
		break;
	case SINGLE_SCENE:
		mode = new SoloPlay();
		break;
	// case MULTI_SCENE:
	// 	mode = new MultiPlay();
	// 	break;
	// case OPTION_SCENE://Clockadded
	// 	mode = new Option();
	// 	break;
	default:
		mode = new Title();
		break;
	}
	mode->Initialize(&scene,&dxg,&image,&input);
}

void DrawImageFont(int x, int y, draw* dxg, int font, const char* format, ...){
	(void) dxg;
	char buffer[256];

	va_list ap;
	va_start(ap, format);
	compat_vsprintf(buffer, format, ap);
	va_end(ap);

	_draw_sprite_font_impl(x, y, font, 8, 8, -2, buffer);
}

void DrawImageFont(int x, int y, draw* dxg, int font, int trans, const char* format, ...){
	(void) dxg;
	(void) trans;
	char buffer[256];

	va_list ap;
	va_start(ap, format);
	compat_vsprintf(buffer, format, ap);
	va_end(ap);

	_draw_sprite_font_impl(x, y, font, 8, 8, -2, buffer);
}

void DrawImageFont(int x, int y, draw* dxg, int font, int sizex, int sizey, int offsetx, const char* format, ...){
	(void) dxg;
	char buffer[256];

	va_list ap;
	va_start(ap, format);
	compat_vsprintf(buffer, format, ap);
	va_end(ap);

	_draw_sprite_font_impl(x, y, font, sizex, sizey, offsetx, buffer);
}

void DrawString(int x, int y, draw* dxg, int font, const char* format, ...){
	(void) dxg;
	char buffer[256];

	va_list ap;
	va_start(ap, format);
	compat_vsprintf(buffer, format, ap);
	va_end(ap);

	_init_text_bg_layer(_font_bg_layer, bn::regular_bg_items::font_bg, font_bg_bn_gfxMap);

	if(font == WHITEFONT_IMG){
		_draw_string_bg_impl(_font_bg_layer, x, y, true, buffer);
	}else{
		_draw_string_bg_impl(_font_bg_layer, x, y, false, buffer);
	}
}

void DrawRankingFontSprite(int x, int y, draw* dxg, int font, int sizex, int sizey, int offsetx, const char* format, ...){
	(void) dxg;
	char buffer[256];

	va_list ap;
	va_start(ap, format);
	compat_vsprintf(buffer, format, ap);
	va_end(ap);

	_draw_sprite_font_impl(x, y, font, sizex, sizey, offsetx, buffer);
}

void ReleaseFontSpriteResources(){
	release_font_sprite_resources();
}

int CalcuFps(){
	static int fps = 0;
	static int fpscount = 0;
	static unsigned long timer = GetTickCount() / 1000;
	fpscount++;
	if(timer != GetTickCount() / 1000){
		fps = fpscount;
		timer = GetTickCount() / 1000;
		fpscount = 0;
	}
	return fps;
}
