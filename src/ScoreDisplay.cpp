// ScoreDisplay.cpp: ScoreDisplay Class implementation
//
//////////////////////////////////////////////////////////////////////
#include "ScoreDisplay.h"
#include "bn_direct_bitmap_items_scorebg.h"
#include "bn_sp_direct_bitmap_bg_painter.h"

namespace
{
	constexpr int RANK_COLUMN_X = 42;
	constexpr int NAME_COLUMN_X = 64;
	constexpr int SCORE_COLUMN_X = 112;
	constexpr int LEVEL_HEADER_X = 156;
	constexpr int LEVEL_COLUMN_X = 164;
	constexpr int TIME_COLUMN_X = 208;
}

//////////////////////////////////////////////////////////////////////
// construction/extinction
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
	DrawBackground();
	DrawRankingFontSprite(64,10,dxg,image->i[BIGFONT_IMG],16,16,-4,"Score Ranking");
	DrawRankingFontSprite(NAME_COLUMN_X,60,dxg,image->i[BIGFONT_IMG],16,16,-6,"Name");
	DrawRankingFontSprite(SCORE_COLUMN_X,60,dxg,image->i[BIGFONT_IMG],16,16,-8,"Score");
	DrawRankingFontSprite(LEVEL_HEADER_X,60,dxg,image->i[BIGFONT_IMG],16,16,-8,"Level");
	DrawRankingFontSprite(TIME_COLUMN_X,60,dxg,image->i[BIGFONT_IMG],16,16,-6,"Time");
	if(count < 0)
		Fade_In();
	if(count == 0)
		FadeLevel = 254;
	if( 0 <= count && count < 1000){
		Rank = EASY;
		DrawRank = "EASY";
	}else if(count >= 1000 && count < 2000){
		Rank = NORMAL;
		DrawRank = "NORMAL";
	}else if(count >= 2000 && count <3000){
		Rank = MASTER;
		DrawRank = "MASTER";
	}else if(count >= 3000){
		Rank = DEATH;
		DrawRank = "DEATH";
	}
	if(count >= 0){
		const int rank_count = count - Rank * 1000;
		const int page_index = rank_count / SCORE_PAGE_DURATION;

		page = SCORE_RECORD_COUNT - 1 - page_index * SCORE_RECORDS_PER_PAGE;
		if(page < 0){
			page = 0;
		}

		if(rank_count % SCORE_PAGE_DURATION == 0){
			animateionReset();
		}
	}
	if(count >= 0)
		ScoreDraw(page);
	if(count >= 0){
		const int rank_count = count - Rank * 1000;
		if(rank_count % SCORE_PAGE_DURATION > SCORE_PAGE_EXIT_START)
			GetBackAnimateion();
		else
			num = animateion(num);
	}else{
		num = animateion(num);
	}
	////////////////For debugging///////////////////////////////////////////
	//DrawImageFont(300, 10, dxg, image->i[GRAYFONT_IMG], "%d",count);
	//DrawImageFont(300, 100, dxg, image->i[GRAYFONT_IMG], "%d",Speed);
	//DrawImageFont(300, 200, dxg, image->i[GRAYFONT_IMG], "%d",num);
	if(input->GetKeyState(0,2) & BUTTON[0])
		remove = 1;
	if(remove){
		count = Fade_Out();
	}else{
		if(count >= 3900)
			count = Fade_Out();
		else
			++count;
	}
}

int ScoreDisplay ::animateion(int number)
{
	if(move[number].x != 0){
		if(( 29 <= move[number].x && move[number].x <= 61) || (-61 <= move[number].x && move[number].x <= -29)){//32
			Speed = 4;
		}else if((13 <= move[number].x && move[number].x <= 29) || (-19 <= move[number].x && move[number].x <= -13)){//16
			Speed = 3;
		}else if((4 <= move[number].x && move[number].x <= 12) || (-12 <= move[number].x && move[number].x <= -4)){//8
			Speed = 2;
		}else if((0 < move[number].x && move[number].x <= 3) || ( -3<=move[number].x && move[number].x<0)){//4
			Speed = 1;
		}else{
			Speed = 5;
		}
		move[number].x += (number%2 == 1)? -1*Speed : Speed;
		move[number].y += Speed;		
	}else{
		Speed = 0;
		number++;
	}
	if(number >= SCORE_RECORDS_PER_PAGE)
		number = 0;

	return number;
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
	for(int i = 0 ; i < SCORE_RECORDS_PER_PAGE ; i++ )
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
	num = 0;
	Speed = 0;

}

void ScoreDisplay ::DrawBackground()
{
	if(! scoreBg){
		scoreBg = bn::sp_direct_bitmap_bg_ptr::create();
		scoreBg->set_priority(3);

		bn::sp_direct_bitmap_bg_painter painter(*scoreBg);
		painter.blit(0, 0, bn::direct_bitmap_items::scorebg);
	}
}

void ScoreDisplay ::ScoreDraw(int s)
{
	char tmp[9];
	DrawRankingFontSprite(110 , 40 , dxg, image->i[BIGFONT_IMG],16,16,-4, "%s",DrawRank);
	for(int i = 0 ; i < SCORE_RECORDS_PER_PAGE && s - i >= 0 ; i++){
		const int record_index = s - i;
		DrawRankingFontSprite(RANK_COLUMN_X+move[i].x, move[i].y+25, dxg, image->i[BIGFONT_IMG],16,16,-8,"%2d", record_index+1);
		DrawRankingFontSprite(NAME_COLUMN_X+move[i].x, move[i].y+25, dxg, image->i[BIGFONT_IMG],16,16,-6,"%s", Score.scoreList[Rank].record[record_index].name);
		DrawRankingFontSprite(SCORE_COLUMN_X+move[i].x, move[i].y+25, dxg,image->i[BIGFONT_IMG],16,16,-8, "%d", Score.scoreList[Rank].record[record_index].score);
		DrawRankingFontSprite(LEVEL_COLUMN_X+move[i].x, move[i].y+25, dxg,image->i[BIGFONT_IMG],16,16,-8, "%4d", Score.scoreList[Rank].record[record_index].level);
		DrawRankingFontSprite(TIME_COLUMN_X+move[i].x, move[i].y+25, dxg,image->i[BIGFONT_IMG],16,16,-8, "%s", FrameCountToStr(Score.scoreList[Rank].record[record_index].time,tmp));
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
