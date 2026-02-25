#ifndef SOLOPLAYHEADER
#define SOLOPLAYHEADER

#include "ScoreManager.h"	// ClassView Ç…ÇÊÇ¡Çƒí«â¡Ç≥ÇÍÇ‹ÇµÇΩÅB
#include "common.h"
#include "GameCtrl.h"
#include "HextrisCtrl.h"
#include "DataFileLoader.h"
#include "BackGroundImageCtrl.h"	// ClassView Ç…ÇÊÇ¡Çƒí«â¡Ç≥ÇÍÇ‹ÇµÇΩÅB

class SoloPlay : public Mode{
private:
	int getscore;
	void EraceEffect();
	ScoreManager ScoreData;
	void DrawScore();
	int cur_diffic;
	float cur_pos;
	int SelectDifficulty();
	BackGroundImageCtrl BGImage;
	int difficulty;
	void LevelupEffect();
	void GradeupEffect();
	void ComboEffect();
	struct EffectData{
		int levelup;
		int gradeup;
		int combo;
		int erace;
		int line;
	}effect;
	struct NameEntryData{
		char name[3];
		int cur_word;
		int cur_pos;
	}name;
	int score;
	int grade;
	int cntLevel;
	int cntTime;
	int cntFall;
	int cntCombo;
	int cntErace;
	int startTime;
	enum {FADETIME = 64};
	HextrisCtrl hCtrl;
	HexFieldDrawData drawData;
	DataFileLoader gameData;

public:
	int NameEntry();
	SoloPlay(){};
	void Main();
};

#endif