#ifndef LOGOHEADER
#define LOGOHEADER

#include "common.h"
#include "GameCtrl.h"
#include <bn_sprite_ptr.h>

class Logo : public Mode{
private:
	static constexpr int FADETIME = 64;
	static constexpr int LOGOX = 0;
	static constexpr int LOGOY = 0;
	static constexpr int MOVESPEED = 16;
	float logo_pos;
	bn::optional<bn::sprite_ptr> spr;

public:
	Logo(){}
	void Main();
};

#endif
