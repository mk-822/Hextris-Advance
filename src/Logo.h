#ifndef LOGOHEADER
#define LOGOHEADER

#include "common.h"
#include "GameCtrl.h"
#include <bn_sprite_ptr.h>

class bn::sprite_ptr;

class Logo : public Mode{
private:
	enum {
		FADETIME = 64,
		LOGOX = 0,
		LOGOY = 0,
		MOVESPEED = 16
	};
	float logo_pos;
	bn::optional<bn::sprite_ptr> spr;

public:
	Logo(){}
	void Main();
};

#endif