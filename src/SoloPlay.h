#ifndef SOLOPLAYHEADER
#define SOLOPLAYHEADER

#include "ScoreManager.h"	// ClassView added by
#include "common.h"
#include "GameCtrl.h"
#include "HextrisCtrl.h"
#include "DataFileLoader.h"
#include "BackGroundImageCtrl.h"	// ClassView added by

class SoloPlay : public Mode{
private:
	int getscore;
	void EraceEffect();
	ScoreManager ScoreData;
	void DrawScore(bool updateEffects=true);
	int cur_diffic;
	float cur_pos;
	int SelectDifficulty();
	BackGroundImageCtrl BGImage;
	int difficulty;
	bool paused;
	void LevelupEffect();
	void GradeupEffect();
	void ComboEffect();
	struct EffectData{
		int levelup;
		int gradeup;
		int combo;
		int erace;
		int line;
		int backToBack;
	}effect;
	struct NameEntryData{
		char name[4];
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
	bool backToBackReady;
	int startTime;
	enum {
		READY_PAT_INDEX = 8 * 8,
		COUNTDOWN_PAT_INDEX = 6 + 8 * 11,
		DIFFICULTY_MENU_PAT_INDEX = 4 * 8,
		DIFFICULTY_SELECTED_PAT_INDEX = 8 * 8,
		ERASE_PAT_INDEX = 12 * 8,
		FADETIME = 64
	};
	HextrisCtrl hCtrl;
	HexFieldDrawData drawData;
	DataFileLoader gameData;
	
	bn::optional<bn::sprite_ptr> imgReady[3];
	bn::optional<bn::sprite_ptr> imgCountdown;
	bn::optional<bn::sprite_ptr> imgRank;
	bn::optional<bn::sprite_ptr> imgDifficulty[3 * 4];
	bn::optional<bn::sprite_ptr> imgErase[3];
	void DrawPauseOverlay();

public:
	int NameEntry();
	SoloPlay(){};
	void Main();
};

#endif
