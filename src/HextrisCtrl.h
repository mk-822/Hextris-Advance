// HextrisCtrl.h: HextrisCtrl クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEXTRISCTRL_H__CC5D3365_B033_432D_9265_CF7269D35F38__INCLUDED_)
#define AFX_HEXTRISCTRL_H__CC5D3365_B033_432D_9265_CF7269D35F38__INCLUDED_

#include "EraseData.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DxGraph.h"
#include "Joypad.h"
#include "common.h"
#include "HexField.h"
#include "DataFileLoader.h"
#include "random.h"

// ゲームそのものは 96x216 ドットを占拠します

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
		};
		void GenerateMove(){
			effecttime = EFFECTTIME;
			enable = true;
			for(int i=0 ; i<13 ; i++){
				movex[i] = (float)((::rand()%5)*100) / 100 -2;
				movey[i] = (float)((::rand()%3)*100) / 100 -1;
				moveymove[i] = (float)((::rand()%3)*100) / 10000 + (float)0.0125;
				animespeed[i] = ::rand() % 8 +1;
			}
			gravity = (float)1.05;
		}
		int effecttime;
		bool enable;
		int color[13];
		float x[13];
		float y[13];
		float movex[13];
		float movey[13];
		float moveymove[13];
		int animespeed[13];
		float gravity;		
	}eraseEffectData[4];
	int AdjustX(int x=-256);
	int wait;
	int ShiftPhase(int diff);
	int BlockErase();
	int effectFlag;
	int BlockFix();
	int IsGrounded();
	int BlockSpin(int spin);	// 回転 spin = -1:左回転 1:右回転
	int BlockMove(int x , int y);	// 移動
	int groundedTime;	// 接地時間
	int fixcount;
	enum DEFINE{
		BLOCK_OFFSET_X = 6,	// ブロックそのもののフィールド上での大きさ
		BLOCK_OFFSET_Y = 4,
		BLOCK_SIZE_X = 8,	// ブロック画像のサイズ
		BLOCK_SIZE_Y = 8,
		GAME_POS_X = 112,
		GAME_POS_Y = 8,
		GAME_POS_OFFSET_X = 8,	// ブロックを配置開始する座標(要するにワクの太さ)
		GAME_POS_OFFSET_Y = 44,
		BLOCK_IMAGE_OFFSET_Y = 24,	// どの Y 座標のブロック画像を使うか
		NEXT_OFFSET_X = 28,	// ネクスト画像をどれだけ右にずらすか
		NEXT_OFFSET_Y = 8,	// 下にずらすか
		NEXT_SIZE_X = 40,	// ネクスト画像のサイズ
		NEXT_SIZE_Y = 24,
		NEXT_IMAGE_OFFSET_Y = 0,	// どの Y 座標のネクスト画像を使うか
		FLOORUP_MAX = 8		// 最高で何列まで迫り上がるか
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

public:
	void EnqueFloorUp(int up, int color);
	void CountUp();
	int queFloorUp[FLOORUP_MAX];
	int queFix;
	int queErace;
	void ChangeLevel(DelayData* newLevel);
	HextrisCtrl();
	void Initialize(draw* Dxg,Image* Image,JoyPadCtrl* Input,DataFileLoader* BlockData);
	int Main(int player);
	virtual void DrawField(HexFieldDrawData* drawData, int trans, bool shadow=true);
	virtual void Draw(HexFieldDrawData* drawData);
};

#endif // !defined(AFX_HEXTRISCTRL_H__CC5D3365_B033_432D_9265_CF7269D35F38__INCLUDED_)
