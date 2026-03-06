// HextrisCtrl.cpp: HextrisCtrl 繧ｯ繝ｩ繧ｹ縺ｮ繧､繝ｳ繝励Μ繝｡繝ｳ繝・・繧ｷ繝ｧ繝ｳ
//
//////////////////////////////////////////////////////////////////////

#include "HextrisCtrl.h"
#include "bn_bitmap_bg.h"
#include "bn_color.h"
#include "bn_sp_direct_bitmap_bg_painter.h"
#include "bn_sprite_items_block_cell.h"
#include "bn_sprite_items_block_cell_anim.h"
#include "bn_direct_bitmap_items_bg00_dbmp.h"
#include "bn_direct_bitmap_items_bg01_dbmp.h"
#include "bn_direct_bitmap_items_bg02_dbmp.h"
#include "bn_direct_bitmap_items_bg03_dbmp.h"
#include "bn_direct_bitmap_items_bg04_dbmp.h"
#include "bn_direct_bitmap_items_bg05_dbmp.h"
#include "bn_direct_bitmap_items_bg06_dbmp.h"
#include "bn_direct_bitmap_items_bg07_dbmp.h"
#include "bn_direct_bitmap_items_bg08_dbmp.h"
#include "bn_direct_bitmap_items_bg09_dbmp.h"
#include "bn_direct_bitmap_items_bg10_dbmp.h"
#include "bn_direct_bitmap_items_bg11_dbmp.h"
#include "bn_direct_bitmap_items_block_cell_dbmp.h"
#include "bn_direct_bitmap_items_font_dbmp.h"
#include "bn_direct_bitmap_items_frame_dbmp.h"
#include "bn_colors.h"

namespace
{
	constexpr int SCREEN_WIDTH = 240;
	constexpr int SCREEN_HEIGHT = 160;
	constexpr int VIRTUAL_OFFSET_X = 40;
	constexpr int VIRTUAL_OFFSET_Y = 40;
	constexpr int BLOCK_PIXEL_SIZE = 8;
	constexpr int FRAME_SRC_X = 0;
	constexpr int FRAME_SRC_Y = 0;
	constexpr int FRAME_WIDTH = 96;
	constexpr int FRAME_HEIGHT = 160;
	constexpr int FIELD_BLOCK_COLS = 11;
	constexpr int FIELD_LOGICAL_ROWS = 18;
	constexpr int FIELD_DRAW_ROWS = FIELD_LOGICAL_ROWS * 2;
	constexpr int FIELD_MAX_X = FIELD_BLOCK_COLS - 1;
	constexpr int FIELD_MAX_Y = FIELD_DRAW_ROWS - 1;

	constexpr int BG_SRC_OFFSET_X = 0;
	constexpr int BG_SRC_OFFSET_Y = 0;
	constexpr int HUD_BASE_X = 208;
	constexpr int HUD_BASE_Y = 80;
	constexpr int HUD_TIME_X = 40;
	constexpr int HUD_TIME_Y = 96;
	constexpr int HUD_LINE_STEP = 8;
	constexpr int HUD_CHAR_ADVANCE = 6;
	constexpr int HUD_CHAR_WIDTH = 8;
	constexpr int HUD_CHAR_HEIGHT = 8;
	constexpr int HUD_LINE_COUNT = 6;
	constexpr int HUD_CACHE_SLOTS = 7;
	constexpr int HUD_TEXT_MAX_CHARS = 24;
	constexpr int HUD_TIME_MAX_CHARS = 8;

	inline int virtual_to_screen_x(int value)
	{
		return value - VIRTUAL_OFFSET_X;
	}

	inline int virtual_to_screen_y(int value)
	{
		return value - VIRTUAL_OFFSET_Y;
	}

	inline int clamp_block_color(int color)
	{
		if(color < 0)
		{
			return 0;
		}

		if(color > 15)
		{
			return 15;
		}

		return color;
	}

	inline bool out_of_playfield(int x, int y)
	{
		if(x < 0 || x > FIELD_MAX_X)
		{
			return true;
		}

		int adjusted_y = y;
		if(x % 2)
		{
			adjusted_y -= 1;
		}

		// Spawn and rotation can legitimately place cells above the visible top.
		return adjusted_y > FIELD_MAX_Y;
	}

	const bn::direct_bitmap_item& field_background_item(int index)
	{
		switch(index)
		{
		case 1:
			return bn::direct_bitmap_items::bg01_dbmp;
		case 2:
			return bn::direct_bitmap_items::bg02_dbmp;
		case 3:
			return bn::direct_bitmap_items::bg03_dbmp;
		case 4:
			return bn::direct_bitmap_items::bg04_dbmp;
		case 5:
			return bn::direct_bitmap_items::bg05_dbmp;
		case 6:
			return bn::direct_bitmap_items::bg06_dbmp;
		case 7:
			return bn::direct_bitmap_items::bg07_dbmp;
		case 8:
			return bn::direct_bitmap_items::bg08_dbmp;
		case 9:
			return bn::direct_bitmap_items::bg09_dbmp;
		case 10:
			return bn::direct_bitmap_items::bg10_dbmp;
		case 11:
			return bn::direct_bitmap_items::bg11_dbmp;
		default:
			return bn::direct_bitmap_items::bg00_dbmp;
		}
	}

	void draw_background(
			bn::sp_direct_bitmap_bg_painter& painter,
			int background_index,
			int left,
			int top,
			int right,
			int bottom)
	{
		const bn::direct_bitmap_item& bg_item = field_background_item(background_index);
		const int src_width = bg_item.dimensions().width();
		const int src_height = bg_item.dimensions().height();

		for(int y = top; y < bottom; ++y)
		{
			const int src_y = y + BG_SRC_OFFSET_Y;
			if(src_y < 0 || src_y >= src_height)
			{
				continue;
			}

			for(int x = left; x < right; ++x)
			{
				const int src_x = x + BG_SRC_OFFSET_X;
				if(src_x < 0 || src_x >= src_width)
				{
					continue;
				}

				painter.unsafe_plot(x, y, bg_item.color(src_x, src_y));
			}
		}
	}

	void draw_block_bitmap(
			bn::sp_direct_bitmap_bg_painter& painter,
			int x,
			int y,
			int color_index,
			int clip_left,
			int clip_top,
			int clip_right,
			int clip_bottom)
	{
		if(x + BLOCK_PIXEL_SIZE <= clip_left || x >= clip_right || y + BLOCK_PIXEL_SIZE <= clip_top || y >= clip_bottom)
		{
			return;
		}

		const int clamped_color = clamp_block_color(color_index);
		const bn::direct_bitmap_item& item = bn::direct_bitmap_items::block_cell_dbmp;
		const int src_x_base = clamped_color * BLOCK_PIXEL_SIZE;
		const bn::color transparent_key = item.color(0, 0);

		for(int py = 0; py < BLOCK_PIXEL_SIZE; ++py)
		{
			for(int px = 0; px < BLOCK_PIXEL_SIZE; ++px)
			{
				const int sx = x + px;
				const int sy = y + py;
				if(sx < clip_left || sx >= clip_right || sy < clip_top || sy >= clip_bottom)
				{
					continue;
				}

				const bn::color src_color = item.color(src_x_base + px, py);
				if(src_color != transparent_key)
				{
					painter.unsafe_plot(sx, sy, src_color);
				}
			}
		}
	}

	void draw_frame_overlay(
			bn::sp_direct_bitmap_bg_painter& painter,
			int frame_screen_x,
			int frame_screen_y,
			int clip_left,
			int clip_top,
			int clip_right,
			int clip_bottom)
	{
		const bn::direct_bitmap_item& frame_item = bn::direct_bitmap_items::frame_dbmp;
		const bn::color transparent_key = frame_item.color(FRAME_SRC_X, FRAME_SRC_Y);

		const int draw_left = frame_screen_x > clip_left ? frame_screen_x : clip_left;
		const int draw_top = frame_screen_y > clip_top ? frame_screen_y : clip_top;
		const int draw_right = frame_screen_x + FRAME_WIDTH < clip_right ? frame_screen_x + FRAME_WIDTH : clip_right;
		const int draw_bottom = frame_screen_y + FRAME_HEIGHT < clip_bottom ? frame_screen_y + FRAME_HEIGHT : clip_bottom;
		if(draw_left >= draw_right || draw_top >= draw_bottom)
		{
			return;
		}

		for(int sy = draw_top; sy < draw_bottom; ++sy)
		{
			const int src_y = FRAME_SRC_Y + (sy - frame_screen_y);
			for(int sx = draw_left; sx < draw_right; ++sx)
			{
				const int src_x = FRAME_SRC_X + (sx - frame_screen_x);
				const bn::color src_color = frame_item.color(src_x, src_y);
				if(src_color != transparent_key)
				{
					painter.unsafe_plot(sx, sy, src_color);
				}
			}
		}
	}

	void draw_hud_glyph(
			bn::sp_direct_bitmap_bg_painter& painter,
			int screen_x,
			int screen_y,
			char chr)
	{
		const bn::direct_bitmap_item& font_item = bn::direct_bitmap_items::font_dbmp;
		const bn::color transparent_key = font_item.color(0, 0);
		int glyph_index = static_cast<unsigned char>(chr) - 32;
		if(glyph_index < 0 || glyph_index > 127)
		{
			glyph_index = 0;
		}

		const int src_base_x = glyph_index * 8;
		for(int py = 0; py < HUD_CHAR_HEIGHT; ++py)
		{
			const int sy = screen_y + py;
			if(sy < 0 || sy >= SCREEN_HEIGHT)
			{
				continue;
			}

			for(int px = 0; px < HUD_CHAR_WIDTH; ++px)
			{
				const int sx = screen_x + px;
				if(sx < 0 || sx >= SCREEN_WIDTH)
				{
					continue;
				}

				const bn::color src_color = font_item.color(src_base_x + px, py);
				if(src_color != transparent_key)
				{
					painter.unsafe_plot(sx, sy, src_color);
				}
			}
		}
	}
}

enum PHASE{
	PHASE_APPEAR,
	PHASE_MOVING,
	PHASE_GROUNDED,
	PHASE_FIXED,
	PHASE_ERASED
};

enum SPECIALDRAW{
	BLOCK_FIX_EFFECT = 1,
};

//////////////////////////////////////////////////////////////////////
// 讒狗ｯ・豸域ｻ・
//////////////////////////////////////////////////////////////////////

HextrisCtrl::HextrisCtrl(){
}

void HextrisCtrl::Initialize(draw* Dxg,Image* Image,JoyPadCtrl* Input,DataFileLoader* BlockData){
	dxg = Dxg;
	image = Image;
	input = Input;
	blockData = BlockData;
	count = 0;
	phase = 0;
	eraseData.Clear();

	queErace = 0;
	queFix = 0;
	for(int i=0 ; i<FLOORUP_MAX ; i++){
		queFloorUp[i] = -1;
	}

	nextBlock = GenerateNext(10);
	effectFlag = 0;
	wait = 0;
	for(int i = 0; i < 4; ++i)
	{
		eraseEffectData[i].enable = false;
		eraseEffectData[i].effecttime = 0;
	}

	curBlock.center_x = 0;
	curBlock.center_y = 0;
	curBlock.color = 0;
	for(int i=0 ; i<4 ; i++)
		curBlock.pos[i] = 0;

	hField.Initialize();
	for(int i = 0; i < FIELD_LOGICAL_ROWS + 4; i++){
		hField.SetN(1,i,11);
		hField.SetN(13,i,11);
	}
	for(int j = 2; j < FIELD_BLOCK_COLS + 2; j++){
		hField.SetN(j,0,11);
		hField.SetN(j,FIELD_LOGICAL_ROWS + 2,11);
	}
	fieldBackgroundIndex = 0;
	fieldBitmapDirty = true;
	fieldBitmapFullRedraw = true;
	fieldDirtyCellsValid = false;
	scoreHudCacheValid = false;
	for(int i = 0; i < HUD_CACHE_SLOTS; ++i)
	{
		for(int j = 0; j <= HUD_TEXT_MAX_CHARS; ++j)
		{
			scoreHudLines[i][j] = '\0';
		}
	}

	if(! fieldBitmapBg)
	{
		fieldBitmapBg = bn::sp_direct_bitmap_bg_ptr::create();
		fieldBitmapBg->set_priority(3);
		fieldBitmapBg->set_blending_enabled(false);
	}

	HideCurrentBlockSprites();
	HideNextBlockSprites();
	HideEraseEffectSprites();
}

// 笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆繝｡繧､繝ｳ笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆
int HextrisCtrl::Main(int player){
	if(wait){
		wait--;
	}else{
		switch(phase){
////// 蜃ｺ迴ｾ蜃ｦ逅・////////////
		case PHASE_APPEAR:
			if(queFloorUp[0] != -1){	// 縺帙ｊ荳翫′繧・
				FloorUp(DequeFloorUp());
				break;
			}

			if(input->GetKeyState(player,0) & (BUTTON[0] | BUTTON[2])){	//蟾ｦ蝗櫁ｻ｢
				NextToCurrent(-1);
				Sound::PlaySe(8);
			}else if(input->GetKeyState(player,0) & BUTTON[1]){	//蜿ｳ蝗櫁ｻ｢
				NextToCurrent(1);
				Sound::PlaySe(8);
			}else{
				NextToCurrent(0);
			}

			nextBlock = GenerateNext(10);
			fallcount = delayData.fall;
			fixcount = delayData.fix;
			forcefixcount = 0;

			if(IsGrounded()){
				ShiftPhase(2);
			}else{
				ShiftPhase(1);
			}

			if(IsDead()){
				return true;
			}

			if(fallcount){
				fallcount--;
			}else{
				BlockMove(0,delayData.g);
				fallcount = delayData.fall;
				fixcount = delayData.fix;
			}
			break;
////// 繝悶Ο繝・け遘ｻ蜍穂ｸｭ ///////
		case PHASE_MOVING:
			if(IsGrounded()){
				ShiftPhase(1);
			}

			if(fallcount){
				fallcount--;
			}else{
				BlockMove(0,delayData.g);
				fallcount = delayData.fall;
				fixcount = delayData.fix;
			}

			//謫堺ｽ懊↓繧医ｋ遘ｻ蜍・
			if(input->GetKeyState(player,2) & UP){	//荳豌励↓關ｽ荳・
				BlockMove(0,20);
			}
			if(input->GetKeyState(player,1) & LEFT){	//蟾ｦ縺ｫ遘ｻ蜍・
				BlockMove(-1,0);
			}
			if(input->GetKeyState(player,1) & RIGHT){//蜿ｳ縺ｫ遘ｻ蜍・
				BlockMove(1,0);
			}
			if(input->GetKeyState(player,0) & DOWN){	//荳谿ｵ關ｽ荳・
				BlockMove(0,1);
			}
			//蝗櫁ｻ｢
			if(input->GetKeyState(player,2) & (BUTTON[0] | BUTTON[2])){	//蟾ｦ蝗櫁ｻ｢
				BlockSpin(-1);
				Sound::PlaySe(8);
			}
			if(input->GetKeyState(player,2) & BUTTON[1]){	//蜿ｳ蝗櫁ｻ｢
				BlockSpin(1);
				Sound::PlaySe(8);
			}
			break;
////// 遘ｻ蜍穂ｸｭ縺縺梧磁蝨ｰ縺励※縺・ｋ /////
		case PHASE_GROUNDED:
			if(fixcount){
				fixcount--;
			}else{
				BlockFix();
				ShiftPhase(1);
				break;
			}

			// 縺・ｍ縺｡繧・ｍ縺励※繧九→蠑ｷ蛻ｶ蝗ｺ螳・
			if(forcefixcount >= 30){
				BlockFix();
				ShiftPhase(1);
				break;
			}

			//謗･蝨ｰ荳ｭ縺縺｣縺溘ｉ蝗ｺ螳・
			if(input->GetKeyState(player,0) & DOWN){
				BlockFix();
				ShiftPhase(1);
				break;
			}
			if(input->GetKeyState(player,1) & LEFT){	//蟾ｦ縺ｫ遘ｻ蜍・
				BlockMove(-1,0);
				forcefixcount++;
			}
			if(input->GetKeyState(player,1) & RIGHT){//蜿ｳ縺ｫ遘ｻ蜍・
				BlockMove(1,0);
				forcefixcount++;
			}

			//蝗櫁ｻ｢
			if(input->GetKeyState(player,2) & (BUTTON[0] | BUTTON[2])){	//蟾ｦ蝗櫁ｻ｢
				BlockSpin(-1);
				Sound::PlaySe(8);
				forcefixcount++;
			}
			if(input->GetKeyState(player,2) & BUTTON[1]){	//蜿ｳ蝗櫁ｻ｢
				BlockSpin(1);
				Sound::PlaySe(8);
				forcefixcount++;
			}

			if(!IsGrounded()){
				if(fallcount){
					fallcount--;
				}else{
					BlockMove(0,delayData.g);
					fallcount = delayData.fall;
					fixcount = delayData.fix;
				}
				ShiftPhase(-1);
			}
			break;
////// 蝗ｺ螳壼ｾ後・蜃ｦ逅・/////////
		case PHASE_FIXED:
			Sound::PlaySe(5);
			effectFlag |= BLOCK_FIX_EFFECT;
			queFix = 1;
			if(queErace = BlockErase()){
				ShiftPhase(1);
				if(queErace==4){
					Sound::PlaySe(3);
				}else{
					Sound::PlaySe(4);
				}
			}else{
				ShiftPhase(2);
			}
			break;
		case PHASE_ERASED:
			Sound::PlaySe(0);
			for(int i=eraseData.GetFreq() ; i>0 ; i--){
				for(int j=eraseData.GetLine() ; j>=0 ; j--){
					for(int k=0 ; k<FIELD_BLOCK_COLS ; k++){
						if(j==0){
							hField.SetField(k,j,0);
						}else{
							hField.SetField(k,j,hField.GetField(k,j-1));
						}
					}
				}
			}
			MarkFieldRectDirty(0, 0, FIELD_MAX_X, eraseData.GetLine() * 2 + 1);
			eraseData.Clear();
			ShiftPhase(1);
			break;
		}
	}
	count++;
	return false;
}
//笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆繝｡繧､繝ｳ邨ゅｏ繧岩蔓笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆笆


void HextrisCtrl::Draw(HexFieldDrawData* drawData)
{
	DrawBitmapField(drawData);
	UpdateNextBlockSprites(drawData);
	UpdateCurrentBlockSprites(drawData);
	UpdateEraseEffectSprites(drawData);

	// Fixed block flash is now rendered in the bitmap field.
	effectFlag &= ~BLOCK_FIX_EFFECT;
}

void HextrisCtrl::DrawBitmapField(HexFieldDrawData* drawData)
{
	if(! fieldBitmapBg)
	{
		fieldBitmapBg = bn::sp_direct_bitmap_bg_ptr::create();
		fieldBitmapBg->set_priority(3);
		fieldBitmapDirty = true;
		fieldBitmapFullRedraw = true;
	}

	if(! fieldBitmapDirty)
	{
		return;
	}

	int clip_left = 0;
	int clip_top = 0;
	int clip_right = SCREEN_WIDTH;
	int clip_bottom = SCREEN_HEIGHT;
	int row_begin = 0;
	int row_end = FIELD_MAX_Y;
	int col_begin = 0;
	int col_end = FIELD_MAX_X;

	const int field_left = virtual_to_screen_x(drawData->game_pos_x + GAME_POS_OFFSET_X);
	const int field_top = virtual_to_screen_y(drawData->game_pos_y + GAME_POS_OFFSET_Y);
	if(! fieldBitmapFullRedraw && fieldDirtyCellsValid)
	{
		clip_left = field_left + fieldDirtyMinX * BLOCK_OFFSET_X;
		clip_top = field_top + fieldDirtyMinY * BLOCK_OFFSET_Y;
		clip_right = field_left + fieldDirtyMaxX * BLOCK_OFFSET_X + BLOCK_PIXEL_SIZE;
		clip_bottom = field_top + fieldDirtyMaxY * BLOCK_OFFSET_Y + BLOCK_PIXEL_SIZE;
		if(clip_left < 0)
		{
			clip_left = 0;
		}
		if(clip_top < 0)
		{
			clip_top = 0;
		}
		if(clip_right > SCREEN_WIDTH)
		{
			clip_right = SCREEN_WIDTH;
		}
		if(clip_bottom > SCREEN_HEIGHT)
		{
			clip_bottom = SCREEN_HEIGHT;
		}

		row_begin = fieldDirtyMinY - 2;
		if(row_begin < 0)
		{
			row_begin = 0;
		}
		row_end = fieldDirtyMaxY + 2;
		if(row_end > FIELD_MAX_Y)
		{
			row_end = FIELD_MAX_Y;
		}
		col_begin = fieldDirtyMinX - 2;
		if(col_begin < 0)
		{
			col_begin = 0;
		}
		col_end = fieldDirtyMaxX + 2;
		if(col_end > FIELD_MAX_X)
		{
			col_end = FIELD_MAX_X;
		}
	}

	bn::sp_direct_bitmap_bg_painter painter(*fieldBitmapBg);
	draw_background(painter, fieldBackgroundIndex, clip_left, clip_top, clip_right, clip_bottom);

	for(int i = row_begin; i <= row_end; ++i)
	{
		for(int j = col_begin; j <= col_end; ++j)
		{
			const int color = hField.Get(j, i);
			if(color)
			{
				const int x = field_left + j * BLOCK_OFFSET_X;
				const int y = field_top + i * BLOCK_OFFSET_Y;
				draw_block_bitmap(painter, x, y, color, clip_left, clip_top, clip_right, clip_bottom);
			}
		}
	}

	const int frame_x = virtual_to_screen_x(drawData->game_pos_x);
	const int frame_y = virtual_to_screen_y(drawData->game_pos_y + 40);
	draw_frame_overlay(painter, frame_x, frame_y, clip_left, clip_top, clip_right, clip_bottom);

	// Full background redraw can overwrite HUD text drawn in a previous phase/frame.
	// Repaint cached HUD strings immediately so they never remain erased.
	if(fieldBitmapFullRedraw && scoreHudCacheValid)
	{
		for(int cache_index = 0; cache_index < HUD_CACHE_SLOTS; ++cache_index)
		{
			const char* cached_text = scoreHudLines[cache_index].data();
			if(! cached_text[0])
			{
				continue;
			}

			int virtual_x = HUD_BASE_X;
			int virtual_y = HUD_BASE_Y + cache_index * HUD_LINE_STEP;
			int max_chars = HUD_TEXT_MAX_CHARS;
			if(cache_index == HUD_LINE_COUNT)
			{
				virtual_x = HUD_TIME_X;
				virtual_y = HUD_TIME_Y;
				max_chars = HUD_TIME_MAX_CHARS;
			}

			const int screen_y = virtual_to_screen_y(virtual_y);
			for(int i = 0; i < max_chars && cached_text[i]; ++i)
			{
				const int screen_x = virtual_to_screen_x(virtual_x + i * HUD_CHAR_ADVANCE);
				for(int py = 0; py < HUD_CHAR_HEIGHT; ++py)
				{
					const int sy = screen_y + py;
					if(sy < 0 || sy >= SCREEN_HEIGHT)
					{
						continue;
					}

					for(int px = 0; px < HUD_CHAR_WIDTH; ++px)
					{
						const int sx = screen_x + px;
						if(sx < 0 || sx >= SCREEN_WIDTH)
						{
							continue;
						}

						painter.unsafe_plot(sx, sy, bn::colors::black);
					}
				}

				if(cached_text[i] != ' ')
				{
					draw_hud_glyph(painter, screen_x, screen_y, cached_text[i]);
				}
			}
		}
	}

	fieldDirtyCellsValid = false;
	if(fieldBitmapFullRedraw)
	{
		scoreHudCacheValid = false;
	}
	fieldBitmapFullRedraw = false;
	fieldBitmapDirty = false;
}

void HextrisCtrl::SetBitmapBackground(int image_id)
{
	int next_background_index = 0;
	for(int i = 0; i < 12; ++i)
	{
		if(BG_IMG[i] == image_id)
		{
			next_background_index = i;
			break;
		}
	}

	if(next_background_index != fieldBackgroundIndex)
	{
		fieldBackgroundIndex = next_background_index;
		MarkAllFieldDirty();
	}
}

void HextrisCtrl::MarkAllFieldDirty()
{
	fieldBitmapDirty = true;
	fieldBitmapFullRedraw = true;
	fieldDirtyCellsValid = false;
}

void HextrisCtrl::MarkFieldCellDirty(int x, int y)
{
	MarkFieldRectDirty(x, y, x, y);
}

void HextrisCtrl::MarkFieldRectDirty(int min_x, int min_y, int max_x, int max_y)
{
	if(min_x > max_x || min_y > max_y)
	{
		return;
	}

	if(max_x < 0 || max_y < 0 || min_x > FIELD_MAX_X || min_y > FIELD_MAX_Y)
	{
		return;
	}

	if(min_x < 0)
	{
		min_x = 0;
	}
	if(min_y < 0)
	{
		min_y = 0;
	}
	if(max_x > FIELD_MAX_X)
	{
		max_x = FIELD_MAX_X;
	}
	if(max_y > FIELD_MAX_Y)
	{
		max_y = FIELD_MAX_Y;
	}

	if(! fieldDirtyCellsValid)
	{
		fieldDirtyMinX = min_x;
		fieldDirtyMinY = min_y;
		fieldDirtyMaxX = max_x;
		fieldDirtyMaxY = max_y;
		fieldDirtyCellsValid = true;
	}
	else
	{
		if(min_x < fieldDirtyMinX)
		{
			fieldDirtyMinX = min_x;
		}
		if(min_y < fieldDirtyMinY)
		{
			fieldDirtyMinY = min_y;
		}
		if(max_x > fieldDirtyMaxX)
		{
			fieldDirtyMaxX = max_x;
		}
		if(max_y > fieldDirtyMaxY)
		{
			fieldDirtyMaxY = max_y;
		}
	}

	fieldBitmapDirty = true;
}

void HextrisCtrl::UpdateCurrentBlockSprites(HexFieldDrawData* drawData)
{
	if(! (phase & (PHASE_MOVING | PHASE_GROUNDED)))
	{
		HideCurrentBlockSprites();
		return;
	}

	for(int i = 0; i < 4; ++i)
	{
		const int block_x = drawData->game_pos_x + GAME_POS_OFFSET_X +
				(blockData->posData[curBlock.pos[i]].x + curBlock.center_x) * BLOCK_OFFSET_X;
		const int block_y = drawData->game_pos_y + GAME_POS_OFFSET_Y +
				(blockData->posData[curBlock.pos[i]].y + curBlock.center_y) * BLOCK_OFFSET_Y;
		const int sprite_x = block_x + BLOCK_PIXEL_SIZE / 2 - WINDOW_WIDE / 2;
		const int sprite_y = block_y + BLOCK_PIXEL_SIZE / 2 - WINDOW_HEIGHT / 2;
		const int tile_index = clamp_block_color(curBlock.color);

		if(! currentBlockSprites[i])
		{
			currentBlockSprites[i] = bn::sprite_items::block_cell.create_sprite(sprite_x, sprite_y, tile_index);
		}
		else
		{
			currentBlockSprites[i]->set_position(sprite_x, sprite_y);
			currentBlockSprites[i]->set_tiles(bn::sprite_items::block_cell.tiles_item(), tile_index);
		}

		currentBlockSprites[i]->set_visible(true);
	}
}

void HextrisCtrl::UpdateNextBlockSprites(HexFieldDrawData* drawData)
{
	const BlockData& next_data = blockData->blockData[nextBlock];
	for(int i = 0; i < 4; ++i)
	{
		const int block_x = drawData->game_pos_x + NEXT_OFFSET_X + 12 +
				(blockData->posData[next_data.pos[i]].x + next_data.center_x) * BLOCK_OFFSET_X;
		const int block_y = drawData->game_pos_y + NEXT_OFFSET_Y + 6 +
				(blockData->posData[next_data.pos[i]].y + next_data.center_y) * BLOCK_OFFSET_Y;
		const int sprite_x = block_x + BLOCK_PIXEL_SIZE / 2 - WINDOW_WIDE / 2;
		const int sprite_y = block_y + BLOCK_PIXEL_SIZE / 2 - WINDOW_HEIGHT / 2;
		const int tile_index = clamp_block_color(nextBlock + 1);

		if(! nextBlockSprites[i])
		{
			nextBlockSprites[i] = bn::sprite_items::block_cell.create_sprite(sprite_x, sprite_y, tile_index);
		}
		else
		{
			nextBlockSprites[i]->set_position(sprite_x, sprite_y);
			nextBlockSprites[i]->set_tiles(bn::sprite_items::block_cell.tiles_item(), tile_index);
		}

		nextBlockSprites[i]->set_visible(true);
	}
}

void HextrisCtrl::HideCurrentBlockSprites()
{
	for(int i = 0; i < 4; ++i)
	{
		if(currentBlockSprites[i])
		{
			currentBlockSprites[i]->set_visible(false);
		}
	}
}

void HextrisCtrl::HideNextBlockSprites()
{
	for(int i = 0; i < 4; ++i)
	{
		if(nextBlockSprites[i])
		{
			nextBlockSprites[i]->set_visible(false);
		}
	}
}

void HextrisCtrl::HideEraseEffectSprites()
{
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < FIELD_BLOCK_COLS; ++j)
		{
			if(eraseEffectSprites[i][j])
			{
				eraseEffectSprites[i][j]->set_visible(false);
			}
		}
	}
}

void HextrisCtrl::UpdateEraseEffectSprites(HexFieldDrawData* drawData)
{
	const int animation_frame = (count / 2) % 7;

	for(int i = 0; i < 4; ++i)
	{
		EraseEffectData& effect = eraseEffectData[i];
		if(! effect.enable)
		{
			for(int j = 0; j < FIELD_BLOCK_COLS; ++j)
			{
				if(eraseEffectSprites[i][j])
				{
					eraseEffectSprites[i][j]->set_visible(false);
				}
			}
			continue;
		}

		for(int j = 0; j < FIELD_BLOCK_COLS; ++j)
		{
			const int color = effect.color[j];
			if(color <= 0)
			{
				if(eraseEffectSprites[i][j])
				{
					eraseEffectSprites[i][j]->set_visible(false);
				}
				continue;
			}

			const int sprite_x = drawData->game_pos_x + int(effect.x[j]) + BLOCK_PIXEL_SIZE / 2 - WINDOW_WIDE / 2;
			const int sprite_y = drawData->game_pos_y + int(effect.y[j]) + BLOCK_PIXEL_SIZE / 2 - WINDOW_HEIGHT / 2;
			const int tile_index = animation_frame * 16 + clamp_block_color(color);

			if(! eraseEffectSprites[i][j])
			{
				eraseEffectSprites[i][j] = bn::sprite_items::block_cell_anim.create_sprite(sprite_x, sprite_y, tile_index);
			}
			else
			{
				eraseEffectSprites[i][j]->set_position(sprite_x, sprite_y);
				eraseEffectSprites[i][j]->set_tiles(bn::sprite_items::block_cell_anim.tiles_item(), tile_index);
			}
			eraseEffectSprites[i][j]->set_visible(true);
		}

		for(int j = 0; j < FIELD_BLOCK_COLS; ++j)
		{
			effect.x[j] += effect.movex[j];
			effect.y[j] += effect.movey[j];
			effect.movey[j] += effect.moveymove[j];
			effect.moveymove[j] *= effect.gravity;
		}

		effect.effecttime--;
		if(effect.effecttime <= 0)
		{
			effect.enable = false;
			for(int j = 0; j < FIELD_BLOCK_COLS; ++j)
			{
				if(eraseEffectSprites[i][j])
				{
					eraseEffectSprites[i][j]->set_visible(false);
				}
			}
		}
	}
}

void HextrisCtrl::ChangeLevel(DelayData *newLevel)
{
	delayData = *newLevel;
}

int HextrisCtrl::NextToCurrent(int rotation)
{
	curBlock.color = nextBlock + 1;
	curBlock.center_x = blockData->blockData[nextBlock].center_x + SPAWN_OFFSET_X;
	curBlock.center_y = blockData->blockData[nextBlock].center_y;
	for(int i=0 ; i<4 ; i++)
		curBlock.pos[i] = blockData->blockData[nextBlock].pos[i];

	BlockSpin(rotation);
	GenerateNext(10);

	return 0;
}// 霑斐ｊ蛟､縺・1 縺縺｣縺溘ｉ謇玖ｩｰ縺ｾ繧・

int HextrisCtrl::GenerateNext(int max)
{
	return rand.Decision();
}

int HextrisCtrl::BlockMove(int x, int y)
{
	// 縺ｾ縺壹・邨ｶ蟇ｾ蛟､繧堤ｮ怜・
	int abs_x = x; int abs_y = y;
	if(abs_x<0)
		abs_x *= -1;
	if(abs_y<0)
		abs_y *= -1;

	int adjust_y = 1;
	if(curBlock.center_x % 2)
		adjust_y = -1;

	// 蜍輔￠縺溷屓謨ｰ
	int movecount = 0;
	// 蜍輔￠縺ｪ縺九▲縺溘ｉ繧ｪ繝ｳ縺ｫ縺ｪ繧九ヵ繝ｩ繧ｰ・医ぜ繝ｬ縺ｦ蜍輔￠繧九°隧ｦ縺呻ｼ・
	bool movefailed = false;

	// 蜃ｦ逅・ｒ蠑ｷ蛻ｶ蛻・ｲ・x 霆ｸ y 霆ｸ繧貞酔譎ゅ↓蜍輔°縺吶％縺ｨ縺ｯ縺ｧ縺阪↑縺・
	if(abs_x){
		for(int i=0 ; i<abs_x ; i++){
			for(int j=0 ; j<=4 ; j++){
				if(j==4){
					curBlock.center_x += abs_x / x;
					curBlock.center_y += adjust_y;
					adjust_y = 1;
					if(curBlock.center_x % 2)
						adjust_y = -1;
					movecount++;
					break;
				}
				const int next_x = blockData->posData[curBlock.pos[j]].x + curBlock.center_x + abs_x / x;
				const int next_y = blockData->posData[curBlock.pos[j]].y + curBlock.center_y + adjust_y;
				if(out_of_playfield(next_x, next_y) || hField.Get(next_x, next_y)){
					if(movefailed){
						movefailed = false;
					}else{
						movefailed = true;
						adjust_y *= -1;
						i--;
					}
					break;
				}
			}
		}
	}else if(abs_y){
		for(int i=0 ; i<abs_y ; i++){
			for(int j=0 ; j<=4 ; j++){
				if(j==4){
					curBlock.center_y += abs_y / y * 2;
					movecount++;
					break;
				}
				const int next_x = blockData->posData[curBlock.pos[j]].x + curBlock.center_x;
				const int next_y = blockData->posData[curBlock.pos[j]].y + curBlock.center_y + abs_y / y * 2;
				if(out_of_playfield(next_x, next_y) || hField.Get(next_x, next_y)){
					break;
				}
			}
		}
	}
	return movecount;
}

int HextrisCtrl::BlockSpin(int spin)
{
	// 蝗櫁ｻ｢縺梧・蜉溘＠縺溘°縺ｩ縺・°
	int spincount=0;
	int adjust_x , adjust_y;

	if(spin){
		for(int k=0 ; k<=7 ; k++){
			switch(k){
			case 0:	//縺ｾ縺壹・譎ｮ騾壹↓
				adjust_x = 0; adjust_y = 0;
				break;
			case 1: //荳九□
				adjust_x = 0; adjust_y = 2;
				break;
			case 2:	//縺昴ｌ縺ｧ鬧・岼縺ｪ繧俄ｦ蟾ｦ・・
				adjust_x = -1; adjust_y = AdjustX();
				break;
			case 3: //縺上▲窶ｦ蜿ｳ・・
				adjust_x = 1; adjust_y = AdjustX();
				break;
			case 4: //蟾ｦ荳・
				adjust_x = -1; adjust_y = 2 + AdjustX();
				break;
			case 5: //蜿ｳ荳・
				adjust_x = 1; adjust_y = 2 + AdjustX();
				break;
			case 6:	//荳倶ｸ・
				adjust_x = 0; adjust_y = 4;
				break;
			case 7: //縺・♀縺奇ｼ√≠縺ｨ縺ｯ荳翫＠縺九・縺茨ｼ・
				adjust_x = 0; adjust_y = -2;
				break;
			}
			if(spincount){
				break;
			}
			for(int i=0 ; i<=4 ; i++){
				if(i==4){
					for(int j=0 ; j<4 ; j++){
						if(spin == -1){
							curBlock.pos[j] = blockData->posData[curBlock.pos[j]].next;
						}else{
							curBlock.pos[j] = blockData->posData[curBlock.pos[j]].previous;
						}
					}
					curBlock.center_x += adjust_x;
					curBlock.center_y += adjust_y;
					spincount++;
					break;
				}
				if(spin == -1){
					const int next_x = blockData->posData[ blockData->posData[curBlock.pos[i]].next ].x + curBlock.center_x + adjust_x;
					const int next_y = blockData->posData[ blockData->posData[curBlock.pos[i]].next ].y + curBlock.center_y + adjust_y;
					if(out_of_playfield(next_x, next_y) || hField.Get(next_x, next_y)){
						break;
					}
				}else{
					const int next_x = blockData->posData[ blockData->posData[curBlock.pos[i]].previous ].x + curBlock.center_x + adjust_x;
					const int next_y = blockData->posData[ blockData->posData[curBlock.pos[i]].previous ].y + curBlock.center_y + adjust_y;
					if(out_of_playfield(next_x, next_y) || hField.Get(next_x, next_y)){
						break;
					}
				}
			}
		}
	}
	return spincount;
}

int HextrisCtrl::IsGrounded()
{
	for(int i=0 ; i<=4 ; i++){
		if(i==4){
			return 0;
			break;
		}
		const int next_x = blockData->posData[curBlock.pos[i]].x + curBlock.center_x;
		const int next_y = blockData->posData[curBlock.pos[i]].y + curBlock.center_y + 2;
		if(out_of_playfield(next_x, next_y) || hField.Get(next_x, next_y)){
			break;
		}
	}
	return 1;
}

int HextrisCtrl::BlockFix()
{
	for(int i=0 ; i<4 ; i++){
		const int raw_x = blockData->posData[curBlock.pos[i]].x + curBlock.center_x;
		const int raw_y = blockData->posData[curBlock.pos[i]].y + curBlock.center_y;
		MarkFieldCellDirty(raw_x, raw_y);
		hField.Set(raw_x, raw_y, curBlock.color);
	}

	return 0;
}

int HextrisCtrl::BlockErase()
{
	int erasecount = 0;
	int erased_min_line = FIELD_LOGICAL_ROWS - 1;
	int erased_max_line = 0;
	for(int i = FIELD_LOGICAL_ROWS - 1 ; i >= 0 ; i--){
		for(int j = 0 ; j <= FIELD_BLOCK_COLS ; j++){
			if(j == FIELD_BLOCK_COLS){
				for(int k = 0 ; k < FIELD_BLOCK_COLS ; k++){
			// 繧ｨ繝輔ぉ繧ｯ繝医∈縺ｮ繝・・繧ｿ貂｡縺・///////////////
					eraseEffectData[erasecount].GenerateMove();
					eraseEffectData[erasecount].color[k] = hField.GetField(k,i);
					eraseEffectData[erasecount].x[k] = (float)(GAME_POS_OFFSET_X + k*BLOCK_OFFSET_X);
					eraseEffectData[erasecount].y[k] = (float)(GAME_POS_OFFSET_Y + i*BLOCK_OFFSET_Y*2 + (k % 2 ? BLOCK_OFFSET_Y : 0));
			///////////////////////////////////////////
					hField.SetField(k,i,0);
				}
				erasecount++;
				if(i < erased_min_line){
					erased_min_line = i;
				}
				if(i > erased_max_line){
					erased_max_line = i;
				}
				eraseData.SetLine(i);
				break;
			}
			if(!hField.GetField(j,i)){
				break;
			}
		}
	}
	if(erasecount){
		MarkFieldRectDirty(0, erased_min_line * 2, FIELD_MAX_X, erased_max_line * 2 + 1);
	}
	return erasecount;
}

int HextrisCtrl::ShiftPhase(int diff)
{
	phase += diff;
	if(phase > PHASE_ERASED){
		phase = PHASE_APPEAR;
	}
	switch(phase){
	case PHASE_APPEAR:
		wait = delayData.appear;
		break;
	case PHASE_ERASED:
		wait = delayData.erase;
		break;
	}
	return phase;
}

int HextrisCtrl::AdjustX(int x)
{
	if(x != -256){
		if(x % 2){
			return -1;
		}else{
			return 1;
		}
	}
	if(curBlock.center_x % 2){
		return -1;
	}else{
		return 1;
	}
}

void HextrisCtrl::DrawField(HexFieldDrawData* drawData, int trans, bool shadow)
{
	(void) trans;
	(void) shadow;
	DrawBitmapField(drawData);
	UpdateNextBlockSprites(drawData);
	HideCurrentBlockSprites();
	UpdateEraseEffectSprites(drawData);
}

int HextrisCtrl::IsDead()
{
	for(int i=0 ; i<=4 ; i++){
		if(i==4){
			return 0;
			break;
		}
		const int cur_x = blockData->posData[curBlock.pos[i]].x + curBlock.center_x;
		const int cur_y = blockData->posData[curBlock.pos[i]].y + curBlock.center_y;
		if(out_of_playfield(cur_x, cur_y) || hField.Get(cur_x, cur_y)){
			break;
		}
	}
	return 1;
}

void HextrisCtrl::CountUp()	// 蜉帶･ｭ縺ｧ繧ｴ繝｡繝ｳ
{
	count++;
}

void HextrisCtrl::EnqueFloorUp(int up, int color)
{
	for(int i=0 ; i<FLOORUP_MAX ; i++){
		if(!up){
			break;
		}
		if(queFloorUp[i] == -1){
			queFloorUp[i] = color;
			up--;
		}
	}
}

int HextrisCtrl::DequeFloorUp()
{
	int tmp = queFloorUp[0];
	for(int i=0 ; i<FLOORUP_MAX-1 ; i++){
		queFloorUp[i] = queFloorUp[i+1];
	}
	queFloorUp[FLOORUP_MAX-1] = -1;
	return tmp;
}

void HextrisCtrl::FloorUp(int color)
{
	for(int i = 0 ; i < FIELD_LOGICAL_ROWS - 1 ; i++){
		for(int j = 0 ; j < FIELD_BLOCK_COLS ; j++){
			hField.SetField(j,i,hField.GetField(j,i+1));
		}
	}
	for(int i = 0 ; i < FIELD_BLOCK_COLS ; i++){
		if(hField.GetField(i, FIELD_LOGICAL_ROWS - 2)){
			hField.SetField(i, FIELD_LOGICAL_ROWS - 1, color);
		}
	}
	MarkFieldRectDirty(0, 0, FIELD_MAX_X, FIELD_MAX_Y);
}

void HextrisCtrl::DrawCachedHudText(int cache_index, int virtual_x, int virtual_y, const char* text, int max_chars)
{
	if(cache_index < 0 || cache_index >= HUD_CACHE_SLOTS || ! text || max_chars <= 0 || max_chars > HUD_TEXT_MAX_CHARS)
	{
		return;
	}

	if(! fieldBitmapBg)
	{
		fieldBitmapBg = bn::sp_direct_bitmap_bg_ptr::create();
		fieldBitmapBg->set_priority(3);
		fieldBitmapBg->set_blending_enabled(false);
		scoreHudCacheValid = false;
	}

	char new_text[HUD_TEXT_MAX_CHARS + 1];
	int new_len = 0;
	while(new_len < max_chars && text[new_len])
	{
		new_text[new_len] = text[new_len];
		++new_len;
	}
	new_text[new_len] = '\0';

	char* old_text = scoreHudLines[cache_index].data();
	int old_len = 0;
	while(old_len < HUD_TEXT_MAX_CHARS && old_text[old_len])
	{
		++old_len;
	}

	int redraw_len = old_len > new_len ? old_len : new_len;
	if(redraw_len <= 0 && scoreHudCacheValid)
	{
		return;
	}

	bn::sp_direct_bitmap_bg_painter painter(*fieldBitmapBg);
	const bn::color bg_color = bn::colors::black;
	const int screen_y = virtual_to_screen_y(virtual_y);

	for(int i = 0; i < redraw_len; ++i)
	{
		const char old_chr = scoreHudCacheValid ? old_text[i] : '\0';
		const char new_chr = i < new_len ? new_text[i] : '\0';
		if(scoreHudCacheValid && old_chr == new_chr)
		{
			continue;
		}

		const int screen_x = virtual_to_screen_x(virtual_x + i * HUD_CHAR_ADVANCE);
		for(int py = 0; py < HUD_CHAR_HEIGHT; ++py)
		{
			const int sy = screen_y + py;
			if(sy < 0 || sy >= SCREEN_HEIGHT)
			{
				continue;
			}

			for(int px = 0; px < HUD_CHAR_WIDTH; ++px)
			{
				const int sx = screen_x + px;
				if(sx < 0 || sx >= SCREEN_WIDTH)
				{
					continue;
				}

				painter.unsafe_plot(sx, sy, bg_color);
			}
		}

		if(new_chr && new_chr != ' ')
		{
			draw_hud_glyph(painter, screen_x, screen_y, new_chr);
		}
	}

	for(int i = 0; i < new_len; ++i)
	{
		old_text[i] = new_text[i];
	}
	old_text[new_len] = '\0';
	scoreHudCacheValid = true;
}

void HextrisCtrl::DrawScoreHudLine(int line_index, const char* text)
{
	if(line_index < 0 || line_index >= HUD_LINE_COUNT)
	{
		return;
	}

	DrawCachedHudText(line_index, HUD_BASE_X, HUD_BASE_Y + line_index * HUD_LINE_STEP, text, HUD_TEXT_MAX_CHARS);
}

void HextrisCtrl::DrawScoreTime(const char* text)
{
	DrawCachedHudText(HUD_LINE_COUNT, HUD_TIME_X, HUD_TIME_Y, text, HUD_TIME_MAX_CHARS);
}


