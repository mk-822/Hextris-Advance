// HextrisCtrl.h: HextrisCtrl class interface
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEXTRISCTRL_H__CC5D3365_B033_432D_9265_CF7269D35F38__INCLUDED_)
#define AFX_HEXTRISCTRL_H__CC5D3365_B033_432D_9265_CF7269D35F38__INCLUDED_

#include "EraseData.h"	// Included for ClassView and erase effect data
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

// Playfield size used by this controller: 96x216 pixels

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
	int BlockSpin(int spin);	// Rotate block: -1 = left, 1 = right
	int BlockMove(int x , int y);	// Move block
	int groundedTime;	// Time spent grounded
	int fixcount;
	enum DEFINE{
		BLOCK_OFFSET_X = 6,	// Horizontal spacing between block cells
		BLOCK_OFFSET_Y = 4,
		BLOCK_SIZE_X = 8,	// Block sprite width
		BLOCK_SIZE_Y = 8,
		GAME_POS_X = 112,
		GAME_POS_Y = 8,
		GAME_POS_OFFSET_X = 16,	// Field draw X offset inside the frame
		GAME_POS_OFFSET_Y = 44,
		BLOCK_IMAGE_OFFSET_Y = 24,	// Source Y offset for block images
		NEXT_OFFSET_X = -72,	// Next-piece preview X offset
		NEXT_OFFSET_Y = 40,	// Next-piece preview Y offset
		NEXT_SIZE_X = 40,	// Next-piece preview width
		NEXT_SIZE_Y = 24,
		NEXT_IMAGE_OFFSET_Y = 0,	// next image Y offset
		SPAWN_OFFSET_X = -1,
		FLOORUP_MAX = 8		// Maximum queued floor-up entries
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

