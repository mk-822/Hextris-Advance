#pragma once
// BackGroundImageCtrl.h: BackGroundImageCtrl クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>

class BackGroundImageCtrl  
{
private:
	int speed;
	int count;
	int back;
	int current;

	bn::optional<bn::regular_bg_ptr> bg;
	bn::optional<bn::regular_bg_ptr> recentBg;
public:
	void Draw(draw* dxg);
	void Change(int image, int speed);
	BackGroundImageCtrl();
	~BackGroundImageCtrl();
};
