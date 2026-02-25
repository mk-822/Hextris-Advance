// DataFileLoader.cpp: DataFileLoader クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataFileLoader.h"
#include "common.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

static const int CENTER_X = 6;
static const int CENTER_Y = 2;

DataFileLoader::DataFileLoader()
{
	//ブロック・ブロック補正情報を定義ファイルから読み込み
	static FILE *fp;
	fp = fopen("./Data/rotation.dat","r");
	for(int i=0 ; i<19 ; i++){
		fscanf(fp , "%d %d %d %d",
			&posData[i].x,
			&posData[i].y,
			&posData[i].next,
			&posData[i].previous
		);
	}
	fclose(fp);

	fp = fopen("./Data/blockdef.dat","r");
	for(int i=0 ; i<10 ; i++){
		fscanf(fp , "%d %d %d %d",
			&blockData[i].pos[0],
			&blockData[i].pos[1],
			&blockData[i].pos[2],
			&blockData[i].pos[3]
		);
		blockData[i].center_x = CENTER_X;
		blockData[i].center_y = CENTER_Y;
		blockData[i].color = i;
	}
	fclose(fp);

	fp = fopen("./Data/grade.dat","r");
	for(int i=0 ; i<8 ; i++){
		fscanf(fp,"%d",&borderScore[i]);
	}
	fclose(fp);

	for(int i=0 ; i<4 ; i++){
		switch(i){
		case 0:
			fp = fopen("./Data/easy.dat","r");
			break;
		case 1:
			fp = fopen("./Data/normal.dat","r");
			break;
		case 2:
			fp = fopen("./Data/master.dat","r");
			break;
		case 3:
			fp = fopen("./Data/death.dat","r");
			break;
		}
		for(int j=0 ; j<10 ; j++){
			fscanf(fp , "%d %d %d %d %d",
				&difficultyData[i][j].fix,
				&difficultyData[i][j].erase,
				&difficultyData[i][j].appear,
				&difficultyData[i][j].fall,
				&difficultyData[i][j].g
			);
		}
		fclose(fp);
	}
}
