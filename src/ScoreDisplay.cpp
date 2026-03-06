// ScoreDisplay.cpp: ScoreDisplay クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "ScoreDisplay.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
char* FrameCountToStr(int,char*);
ScoreDisplay :: ScoreDisplay()
{
	animateionReset();
	count = -51;
	num = 0;
	Speed = 0;
	page = 0;
	remove = 0;
	FadeLevel = 0;
}
/*
			&scoreList[0].record[i].score,
			&scoreList[0].record[i].time,
			&scoreList[0].record[i].name
*/
void ScoreDisplay :: Main()
{
	Sound::ChangeBgm(7);
	dxg->TexturePos();
	dxg->Draw(image->i[SCOREBG_IMG],0,0);
	DrawRankingFontSprite(64,10,dxg,image->i[BIGFONT_IMG],16,16,-2,"Score Ranking");
	DrawRankingFontSprite(40,60,dxg,image->i[BIGFONT_IMG],16,16,-4,"Name");
	DrawRankingFontSprite(96,60,dxg,image->i[BIGFONT_IMG],16,16,-6,"Score");
	DrawRankingFontSprite(156,60,dxg,image->i[BIGFONT_IMG],16,16,-6,"Level");
	DrawRankingFontSprite(212,60,dxg,image->i[BIGFONT_IMG],16,16,-4,"Time");
	if(count < 0)
		Fade_In();
	if(count == 0)
		FadeLevel = 254;
	if(count%500 == 0)
		animateionReset();
	if( 0 <= count && count < 1000){
		Rank = EASY;
		DrawRank = "EASY";
	}else if(count >= 1000 && count < 2000){
		Rank = NORMAL;
		DrawRank = "NORMAL";
	}else if(count >= 2000 && count <3000){
		Rank = MASTER;
		DrawRank = "MASTER";
	}else if(count > 3000){
		Rank = DEATH;
		DrawRank = "DEATH";
	}
	if(count%1000 == 0){////0,1000,2000,3000
		page = 0;
	}else if(count % 500 == 0){
		page = 5;
	}
	if(count >= 0)
		ScoreDraw(page);
	if(400+(Rank*1000) < count && count < 500+(Rank*1000) || 900+(Rank*1000) < count && count < 1000+(Rank*1000))
		GetBackAnimateion();
	else
		num = animateion(num);
	////////////////デバック用///////////////////////////////////////////
	//DrawImageFont(300, 10, dxg, image->i[GRAYFONT_IMG], "%d",count);
	//DrawImageFont(300, 100, dxg, image->i[GRAYFONT_IMG], "%d",Speed);
	//DrawImageFont(300, 200, dxg, image->i[GRAYFONT_IMG], "%d",num);
	if(input->GetKeyState(0,2) & BUTTON[0])
		remove = 1;
	if(remove){
		count = Fade_Out();
	}else{
		count = (count >= 3900)? Fade_Out() : ++count;
	}
}

int ScoreDisplay ::animateion(int num)
{
	if(move[num].x != 0){
		if(( 29 <= move[num].x && move[num].x <= 61) || (-61 <= move[num].x && move[num].x <= -29)){//32
			Speed = 4;
		}else if((13 <= move[num].x && move[num].x <= 29) || (-19 <= move[num].x && move[num].x <= -13)){//16
			Speed = 3;
		}else if((4 <= move[num].x && move[num].x <= 12) || (-12 <= move[num].x && move[num].x <= -4)){//8
			Speed = 2;
		}else if((0 < move[num].x && move[num].x <= 3) || ( -3<=move[num].x && move[num].x<0)){//4
			Speed = 1;
		}else{
			Speed = 5;
		}
		move[num].x += (num%2 == 1)? -1*Speed : Speed;
		move[num].y += Speed;		
	}else{
		Speed = 0;
		num++;
	}
	return num = (num>=5)? num = 0 : num;
}


void ScoreDisplay :: GetBackAnimateion()
{
	if(move[0].x <8){
		Speed = 1;
	}else if(move[0].x < 16){
		Speed = 2;
	}else if(move[0].x < 32){
		Speed = 4;
	}else if(move[0].x < 64){
		Speed = 8;
	}else{
		Speed = 16;
	}
	for(int i = 0 ; i < 5 ; i++ )
	{
		move[i].x+= Speed;
	}
}

void ScoreDisplay ::animateionReset()
{
	move[0].x=S_Fixed_PosX;
	move[0].y=Fixed_PosY;
	move[1].x=US_Fixed_PosX+10;
	move[1].y=Fixed_PosY+10;
	move[2].x=S_Fixed_PosX-20;
	move[2].y=Fixed_PosY+20;
	move[3].x=US_Fixed_PosX+30;
	move[3].y=Fixed_PosY+30;
	move[4].x=S_Fixed_PosX-40;
	move[4].y=Fixed_PosY+40;
	num = 0;
	Speed = 0;

}


void ScoreDisplay ::ScoreDraw(int s)
{
	char tmp[9];
	DrawRankingFontSprite(110 , 40 , dxg, image->i[BIGFONT_IMG],16,16,-2, "%s",DrawRank);
	for(int i = 0 ; i < 5 ; i++){
		DrawRankingFontSprite(8+move[i].x, move[i].y+25, dxg, image->i[BIGFONT_IMG],16,16,-6,"%2d", i+s+1);
		DrawRankingFontSprite(40+move[i].x, move[i].y+25, dxg, image->i[BIGFONT_IMG],16,16,-2,"%s", Score.scoreList[Rank].record[i+s].name);
		DrawRankingFontSprite(96+move[i].x, move[i].y+25, dxg,image->i[BIGFONT_IMG],16,16,-6, "%d", Score.scoreList[Rank].record[i+s].score);
		DrawRankingFontSprite(164+move[i].x, move[i].y+25, dxg,image->i[BIGFONT_IMG],16,16,-6, "%4d", Score.scoreList[Rank].record[i+s].level);
		DrawRankingFontSprite(212+move[i].x, move[i].y+25, dxg,image->i[BIGFONT_IMG],16,16,-6, "%s", FrameCountToStr(Score.scoreList[Rank].record[i+s].time,tmp));
	}
}


void ScoreDisplay ::Fade_In()
{
	dxg->TexturePos();
	dxg->ColorChange(1,true);
	dxg->Draw(image->i[BLANK_IMG],0,0,true,255-FadeLevel);
	FadeLevel+=5;
}


int ScoreDisplay::Fade_Out()
{
	dxg->TexturePos();
	dxg->ColorChange(1,true);
	dxg->Draw(image->i[BLANK_IMG],0,0,true,255-FadeLevel);
	FadeLevel-=5;
	if(FadeLevel <= 0){
		*scene = TITLE_SCENE;
	}
	return ++count;

}
