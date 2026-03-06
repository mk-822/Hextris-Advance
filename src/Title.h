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
	float cur_pos;	// For the current animation to move little by little
	float menu_pos;	// The menu is coming in little by little.
	float cur_move;	// Amount of movement when the current animation appears
	float cur_movemove;	// Amount of change in travel amount
	int cur_count;	// Dedicated counter for current anime
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