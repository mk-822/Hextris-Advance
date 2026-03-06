// HextrisCtrl.h: HextrisCtrl 繧ｯ繝ｩ繧ｹ縺ｮ繧､繝ｳ繧ｿ繝ｼ繝輔ぉ繧､繧ｹ
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEXTRISCTRL_H__CC5D3365_B033_432D_9265_CF7269D35F38__INCLUDED_)
#define AFX_HEXTRISCTRL_H__CC5D3365_B033_432D_9265_CF7269D35F38__INCLUDED_

#include "EraseData.h"	// ClassView 縺ｫ繧医▲縺ｦ霑ｽ蜉縺輔ｌ縺ｾ縺励◆縲・
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DxGraph.h"
#include "Joypad.h"
#include "common.h"
#include "HexField.h"
#include "DataFileLoader.h"
#include "random.h"
#include "bn_array.h"
#include "bn_optional.h"
#include "bn_sp_direct_bitmap_bg_ptr.h"
#include "bn_sprite_ptr.h"

// 繧ｲ繝ｼ繝縺昴・繧ゅ・縺ｯ 96x216 繝峨ャ繝医ｒ蜊諡縺励∪縺・

struct HexFieldDrawData{
	int game_pos_x;
	int game_pos_y;
};

class HextrisCtrl{
private:
	void FloorUp(int color);
	int DequeFloorUp();
	int IsDead();
	class EraseEffectData{
	public:
		const int EFFECTTIME;
		EraseEffectData():EFFECTTIME(120){
			effecttime = 0;
			enable = false;
		};
		void GenerateMove(){
			effecttime = EFFECTTIME;
			enable = true;
			for(int i=0 ; i<11 ; i++){
				movex[i] = (float)(GameRandomInt(5)*100) / 100 -2;
				movey[i] = (float)(GameRandomInt(3)*100) / 100 -1;
				moveymove[i] = (float)(GameRandomInt(3)*100) / 10000 + (float)0.0125;
				animespeed[i] = GameRandomInt(8) +1;
			}
			gravity = (float)1.05;
		}
		int effecttime;
		bool enable;
		int color[11];
		float x[11];
		float y[11];
		float movex[11];
		float movey[11];
		float moveymove[11];
		int animespeed[11];
		float gravity;		
	}eraseEffectData[4];
	int AdjustX(int x=-256);
	int wait;
	int ShiftPhase(int diff);
	int BlockErase();
	int effectFlag;
	int BlockFix();
	int IsGrounded();
	int BlockSpin(int spin);	// 蝗櫁ｻ｢ spin = -1:蟾ｦ蝗櫁ｻ｢ 1:蜿ｳ蝗櫁ｻ｢
	int BlockMove(int x , int y);	// 遘ｻ蜍・
	int groundedTime;	// 謗･蝨ｰ譎る俣
	int fixcount;
	enum DEFINE{
		BLOCK_OFFSET_X = 6,	// 繝悶Ο繝・け縺昴・繧ゅ・縺ｮ繝輔ぅ繝ｼ繝ｫ繝我ｸ翫〒縺ｮ螟ｧ縺阪＆
		BLOCK_OFFSET_Y = 4,
		BLOCK_SIZE_X = 8,	// 繝悶Ο繝・け逕ｻ蜒上・繧ｵ繧､繧ｺ
		BLOCK_SIZE_Y = 8,
		GAME_POS_X = 112,
		GAME_POS_Y = 8,
		GAME_POS_OFFSET_X = 8,	// 繝悶Ο繝・け繧帝・鄂ｮ髢句ｧ九☆繧句ｺｧ讓・隕√☆繧九↓繝ｯ繧ｯ縺ｮ螟ｪ縺・
		GAME_POS_OFFSET_Y = 44,
		BLOCK_IMAGE_OFFSET_Y = 24,	// 縺ｩ縺ｮ Y 蠎ｧ讓吶・繝悶Ο繝・け逕ｻ蜒上ｒ菴ｿ縺・°
		NEXT_OFFSET_X = -72,	// 繝阪け繧ｹ繝育判蜒上ｒ縺ｩ繧後□縺大承縺ｫ縺壹ｉ縺吶°
		NEXT_OFFSET_Y = 40,	// 荳九↓縺壹ｉ縺吶°
		NEXT_SIZE_X = 40,	// 繝阪け繧ｹ繝育判蜒上・繧ｵ繧､繧ｺ
		NEXT_SIZE_Y = 24,
		NEXT_IMAGE_OFFSET_Y = 0,	// next image Y offset
		SPAWN_OFFSET_X = -2,
		FLOORUP_MAX = 8		// 譛鬮倥〒菴募・縺ｾ縺ｧ霑ｫ繧贋ｸ翫′繧九°
	};

	EraseData eraseData;
	int nextBlock;
	int GenerateNext(int max);
	BlockData curBlock;
	int NextToCurrent(int rotation);
	int phase;
	random rand;
	DelayData delayData;
	draw* dxg;
	Image* image;
	JoyPadCtrl* input;
	HexField hField;
	int blGraph;
	int count;
	DataFileLoader* blockData;
	int fallcount;
	int forcefixcount;
	bool fieldBitmapDirty;
	bool fieldBitmapFullRedraw;
	bool fieldDirtyCellsValid;
	int fieldDirtyMinX;
	int fieldDirtyMinY;
	int fieldDirtyMaxX;
	int fieldDirtyMaxY;
	int fieldBackgroundIndex;
	bn::optional<bn::sp_direct_bitmap_bg_ptr> fieldBitmapBg;
	bn::optional<bn::sprite_ptr> currentBlockSprites[4];
	bn::optional<bn::sprite_ptr> nextBlockSprites[4];
	bn::optional<bn::sprite_ptr> eraseEffectSprites[4][11];
	void MarkAllFieldDirty();
	void MarkFieldCellDirty(int x, int y);
	void MarkFieldRectDirty(int min_x, int min_y, int max_x, int max_y);
	void DrawBitmapField(HexFieldDrawData* drawData);
	void UpdateCurrentBlockSprites(HexFieldDrawData* drawData);
	void UpdateNextBlockSprites(HexFieldDrawData* drawData);
	void UpdateEraseEffectSprites(HexFieldDrawData* drawData);
	void HideCurrentBlockSprites();
	void HideNextBlockSprites();
	void HideEraseEffectSprites();
	void DrawCachedHudText(int cache_index, int virtual_x, int virtual_y, const char* text, int max_chars);
	bool scoreHudCacheValid;
	bn::array<bn::array<char, 25>, 7> scoreHudLines;

public:
	void EnqueFloorUp(int up, int color);
	void CountUp();
	int queFloorUp[FLOORUP_MAX];
	int queFix;
	int queErace;
	void SetBitmapBackground(int image_id);
	void ChangeLevel(DelayData* newLevel);
	HextrisCtrl();
	void Initialize(draw* Dxg,Image* Image,JoyPadCtrl* Input,DataFileLoader* BlockData);
	int Main(int player);
	virtual void DrawField(HexFieldDrawData* drawData, int trans, bool shadow=true);
	virtual void Draw(HexFieldDrawData* drawData);
	void DrawScoreHudLine(int line_index, const char* text);
	void DrawScoreTime(const char* text);
};

#endif // !defined(AFX_HEXTRISCTRL_H__CC5D3365_B033_432D_9265_CF7269D35F38__INCLUDED_)

