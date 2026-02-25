// ScoreManager.h: ScoreManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOREMANAGER_H__F3758F90_794E_48A0_A70C_589E2B054CBB__INCLUDED_)
#define AFX_SCOREMANAGER_H__F3758F90_794E_48A0_A70C_589E2B054CBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"

static const char* gradename[]={
	"F",
	"E",
	"D",
	"C",
	"B",
	"A",
	"S",
	"SS",
};

class ScoreManager  
{
public:
	void EntryScore(int order, int difficulty, int score, int level, int time, char* name);
	int JudgeOrder(int score, int difficulty);
	struct record{
		int score;
		int time;
		int level;
		char name[4];
	};
	struct scorelist{
		record record[10];
	};
	ScoreManager();
	~ScoreManager();
	scorelist ScoreList[4];
};

#endif // !defined(AFX_SCOREMANAGER_H__F3758F90_794E_48A0_A70C_589E2B054CBB__INCLUDED_)
