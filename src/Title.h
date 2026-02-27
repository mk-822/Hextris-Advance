#ifndef TITLEHEADER
#define TITLEHEADER

#include "common.h"
#include "GameCtrl.h"
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>

const double OUTMOVESPEED = 1.05;

class Title : public Mode{
private:
	enum {
		FADETIME = 64,
		CURSOR_OFFSET_X = 0,
		MENUX = 32,
		MENUY = 6,
		MENU_AMOUNT = 4,
		ANIMSPEED = 3,
		MOVESPEED = 4,

		CURSOR_PAT_INDEX = 6,
		MENU_PAT_INDEX = 0,
		MENU_SELECTED_PAT_INDEX = 32,
	};
	float cur_pos;	// カレントアニメがちょっとづつ動く用
	float menu_pos;	// メニューがちょっとづつ入ってくる用
	float cur_move;	// カレントアニメが出てく時の移動量
	float cur_movemove;	// 移動量の変化量
	int cur_count;	// カレントアニメ用の専用カウンタ
	int current;
	int scene_tmp;
	bn::optional<bn::regular_bg_ptr> bg;
	bn::optional<bn::sprite_ptr> cursor[2];
	bn::optional<bn::sprite_ptr> menu[3*4];

public:
	Title(){};
	void Main();
	int Select();
	void InMenu();
	void OutMenu();
	void HighlightMenu();
	void OffsetMenuSprite();
};

#endif