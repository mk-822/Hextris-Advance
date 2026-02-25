#ifndef TITLEHEADER
#define TITLEHEADER

#include "common.h"
#include "GameCtrl.h"

const double OUTMOVESPEED = 1.05;

class Title : public Mode{
private:
	enum {
		FADETIME = 64,
		MENUX = 192,
		MENUY = 128,
		MENUMAX = 3,
		ANIMSPEED = 3,
		MOVESPEED = 4,
	};
	float cur_pos;	// カレントアニメがちょっとづつ動く用
	float menu_pos;	// メニューがちょっとづつ入ってくる用
	float cur_move;	// カレントアニメが出てく時の移動量
	float cur_movemove;	// 移動量の変化量
	int cur_count;	// カレントアニメ用の専用カウンタ
	int current;
	int scene_tmp;

public:
	Title(){};
	void Main();
	int Select();
	void InMenu();
	void OutMenu();
};

#endif