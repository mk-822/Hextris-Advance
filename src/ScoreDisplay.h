// ScoreDisplay.h: ScoreDisplay class interface
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOREDISPLAY_H__9D2D35BB_E039_412E_B0D0_42549471E8A6__INCLUDED_)
#define AFX_SCOREDISPLAY_H__9D2D35BB_E039_412E_B0D0_42549471E8A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameCtrl.h"
#include "ScoreManager.h"
#include "bn_optional.h"
#include "bn_sp_direct_bitmap_bg_ptr.h"
static const int S_Fixed_PosX = -210;
static const int US_Fixed_PosX = 210;
static const int Fixed_PosY = -150;
static const int SCORE_RECORD_COUNT = 10;
static const int SCORE_RECORDS_PER_PAGE = 3;
static const int SCORE_PAGE_DURATION = 250;
static const int SCORE_PAGE_EXIT_START = 200;
static const bool ComeIn = true;
static const bool GetBack = false;

enum{
	EASY,
	NORMAL,
	MASTER,
	DEATH
};

class ScoreDisplay : public Mode
{
private :
	int FadeLevel;
	int remove;
	struct Move{
		int x;
		int y;
	}move[SCORE_RECORDS_PER_PAGE];
	int animateion(int Number);
	void GetBackAnimateion();
	ScoreManager Score;
	bn::optional<bn::sp_direct_bitmap_bg_ptr> scoreBg;
	int num;
	int Speed;
	int Rank;
	int page;
	const char* DrawRank;
	void Fade_In();
	int Fade_Out();
	void animateionReset();
	void DrawBackground();
	void ScoreDraw(int);
public:
	ScoreDisplay();
	void Main();
};

#endif // !defined(AFX_SCOREDISPLAY_H__9D2D35BB_E039_412E_B0D0_42549471E8A6__INCLUDED_)
