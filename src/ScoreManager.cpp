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
	fp = fopen("./Data/easyscore.dat","r");
	for(int i=0 ; i<10 ; i++){
		fscanf(fp , "%d %d %d %s",
			&ScoreList[0].record[i].score,
			&ScoreList[0].record[i].time,
			&ScoreList[0].record[i].level,
			ScoreList[0].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/normalscore.dat","r");
	for(int i=0 ; i<10 ; i++){
		fscanf(fp , "%d %d %d %s",
			&ScoreList[1].record[i].score,
			&ScoreList[1].record[i].time,
			&ScoreList[1].record[i].level,
			ScoreList[1].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/masterscore.dat","r");
	for(int i=0 ; i<10 ; i++){
		fscanf(fp , "%d %d %d %s",
			&ScoreList[2].record[i].score,
			&ScoreList[2].record[i].time,
			&ScoreList[2].record[i].level,
			ScoreList[2].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/deathscore.dat","r");
	for(int i=0 ; i<10 ; i++){
		fscanf(fp , "%d %d %d %s",
			&ScoreList[3].record[i].score,
			&ScoreList[3].record[i].time,
			&ScoreList[3].record[i].level,
			ScoreList[3].record[i].name
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
		if(ScoreList[difficulty].record[i].score < score){
			return i;
		}
	}
	return -1;
}

void ScoreManager::EntryScore(int order, int difficulty, int score, int level, int time, char *name)
{
	for(int i=8 ; i>=order ; i--){
		ScoreList[difficulty].record[i+1] = ScoreList[difficulty].record[i];
	}
	ScoreList[difficulty].record[order].score = score;
	ScoreList[difficulty].record[order].level = level;
	ScoreList[difficulty].record[order].time = time;
	strcpy(ScoreList[difficulty].record[order].name,name);

	//スコア情報をファイルに書き込み
	static FILE *fp;
	fp = fopen("./Data/easyscore.dat","w");
	for(int i=0 ; i<10 ; i++){
		fprintf(fp , "%d %d %d %s\n",
			ScoreList[0].record[i].score,
			ScoreList[0].record[i].time,
			ScoreList[0].record[i].level,
			ScoreList[0].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/normalscore.dat","w");
	for(int i=0 ; i<10 ; i++){
		fprintf(fp , "%d %d %d %s\n",
			ScoreList[1].record[i].score,
			ScoreList[1].record[i].time,
			ScoreList[1].record[i].level,
			ScoreList[1].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/masterscore.dat","w");
	for(int i=0 ; i<10 ; i++){
		fprintf(fp , "%d %d %d %s\n",
			ScoreList[2].record[i].score,
			ScoreList[2].record[i].time,
			ScoreList[2].record[i].level,
			ScoreList[2].record[i].name
		);
	}
	fclose(fp);

	fp = fopen("./Data/deathscore.dat","w");
	for(int i=0 ; i<10 ; i++){
		fprintf(fp , "%d %d %d %s\n",
			ScoreList[3].record[i].score,
			ScoreList[3].record[i].time,
			ScoreList[3].record[i].level,
			ScoreList[3].record[i].name
		);
	}
	fclose(fp);
}