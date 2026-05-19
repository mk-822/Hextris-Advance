// ScoreDisplay.h: ScoreDisplay class interface
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOREDISPLAY_H__9D2D35BB_E039_412E_B0D0_42549471E8A6__INCLUDED_)
#define AFX_SCOREDISPLAY_H__9D2D35BB_E039_412E_B0D0_42549471E8A6__INCLUDED_

#if defined(_MSC_VER) && _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameCtrl.h"
#include "ScoreManager.h"
#include "bn_array.h"
#include "bn_optional.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"
static const int SCORE_RECORD_COUNT = 10;

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
	ScoreManager Score;
	bn::optional<bn::regular_bg_ptr> scoreBg;
	bn::optional<bn::regular_bg_ptr> textBg;
	bn::array<bn::regular_bg_map_cell, 128> glyphCells;
	bn::regular_bg_map_cell spaceCell;
	void Fade_In();
	int Fade_Out();
	void DrawBackground();
	void DrawScoreText();
	void InitializeTextBg();
	void ClearTextMap();
	void PopulateTextMap();
	void DrawTextRow(int row, const char* text);
	void BuildScrollLine(int contentRow, char* line);
public:
	ScoreDisplay();
	void Main();
};

#endif // !defined(AFX_SCOREDISPLAY_H__9D2D35BB_E039_412E_B0D0_42549471E8A6__INCLUDED_)
