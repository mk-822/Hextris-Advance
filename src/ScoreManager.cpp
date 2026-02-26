// ScoreManager.cpp: ScoreManager クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScoreManager.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ScoreManager::ScoreManager()
{
	//スコア情報をファイルから読み込み
	static FILE *fp;
	fp = fopen("./Data/easyscore.bin","r");
	for(int i=0 ; i<10 ; i++){
		fscanf(fp , "%d %d %d %s",
			&scoreList[0].record[i].score,
			&scoreList[0].record[i].time,
			&scoreList[0].record[i].level,
			scoreList[0].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/normalscore.bin","r");
	for(int i=0 ; i<10 ; i++){
		fscanf(fp , "%d %d %d %s",
			&scoreList[1].record[i].score,
			&scoreList[1].record[i].time,
			&scoreList[1].record[i].level,
			scoreList[1].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/masterscore.bin","r");
	for(int i=0 ; i<10 ; i++){
		fscanf(fp , "%d %d %d %s",
			&scoreList[2].record[i].score,
			&scoreList[2].record[i].time,
			&scoreList[2].record[i].level,
			scoreList[2].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/deathscore.bin","r");
	for(int i=0 ; i<10 ; i++){
		fscanf(fp , "%d %d %d %s",
			&scoreList[3].record[i].score,
			&scoreList[3].record[i].time,
			&scoreList[3].record[i].level,
			scoreList[3].record[i].name
		);
	}
	fclose(fp);
}

ScoreManager::~ScoreManager()
{
}

int ScoreManager::JudgeOrder(int score, int difficulty)
{
	for(int i=0 ; i<10 ; i++){
		if(scoreList[difficulty].record[i].score < score){
			return i;
		}
	}
	return -1;
}

void ScoreManager::EntryScore(int order, int difficulty, int score, int level, int time, char *name)
{
	for(int i=8 ; i>=order ; i--){
		scoreList[difficulty].record[i+1] = scoreList[difficulty].record[i];
	}
	scoreList[difficulty].record[order].score = score;
	scoreList[difficulty].record[order].level = level;
	scoreList[difficulty].record[order].time = time;
	compat_strcpy(scoreList[difficulty].record[order].name,name);

	//スコア情報をファイルに書き込み
	static FILE *fp;
	fp = fopen("./Data/easyscore.bin","w");
	for(int i=0 ; i<10 ; i++){
		fprintf(fp , "%d %d %d %s\n",
			scoreList[0].record[i].score,
			scoreList[0].record[i].time,
			scoreList[0].record[i].level,
			scoreList[0].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/normalscore.bin","w");
	for(int i=0 ; i<10 ; i++){
		fprintf(fp , "%d %d %d %s\n",
			scoreList[1].record[i].score,
			scoreList[1].record[i].time,
			scoreList[1].record[i].level,
			scoreList[1].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/masterscore.bin","w");
	for(int i=0 ; i<10 ; i++){
		fprintf(fp , "%d %d %d %s\n",
			scoreList[2].record[i].score,
			scoreList[2].record[i].time,
			scoreList[2].record[i].level,
			scoreList[2].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/deathscore.bin","w");
	for(int i=0 ; i<10 ; i++){
		fprintf(fp , "%d %d %d %s\n",
			scoreList[3].record[i].score,
			scoreList[3].record[i].time,
			scoreList[3].record[i].level,
			scoreList[3].record[i].name
		);
	}
	fclose(fp);
}