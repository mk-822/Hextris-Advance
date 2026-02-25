// ScoreDisplay.h: ScoreDisplay クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOREDISPLAY_H__9D2D35BB_E039_412E_B0D0_42549471E8A6__INCLUDED_)
#define AFX_SCOREDISPLAY_H__9D2D35BB_E039_412E_B0D0_42549471E8A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameCtrl.h"
#include "ScoreManager.h"
static const int S_Fixed_PosX = -210;
static const int US_Fixed_PosX = 210;
static const int Fixed_PosY = -150;
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
	}move[5];
	int animateion(int Number);
	void GetBackAnimateion();
	ScoreManager Score;
	int num;
	int Speed;
	int Rank;
	int page;
	char* DrawRank;
	void Fade_In();
	int Fade_Out();
	void animateionReset();
	void ScoreDraw(int);
public:
	ScoreDisplay();
	void Main();
};

#endif // !defined(AFX_SCOREDISPLAY_H__9D2D35BB_E039_412E_B0D0_42549471E8A6__INCLUDED_)
