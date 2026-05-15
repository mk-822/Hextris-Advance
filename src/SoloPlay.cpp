#include "soloplay.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <bn_sprite_items_words.h>
#include <bn_sprite_items_font_big.h>
#include <bn_sprite_items_rank.h>

static const char* difficname[]={
	"EASY",
	"NORMAL",
	"MASTER",
	"DEATH"
};

static const char* rankname[]={
	"1st",
	"2nd",
	"3rd",
	"4th",
	"5th",
	"6th",
	"7th",
	"8th",
	"9th",
	"10th",
};

namespace
{
	constexpr bool SOLO_USE_REGULAR_BG = false;
	constexpr int GAMEOVER_STONE_TILE = 21;

	unsigned BuildGameStartRandomSeed(int count, int difficulty, INPUT input_state)
	{
		unsigned seed = static_cast<unsigned>(GetTickCount());
		seed ^= static_cast<unsigned>(count) << 16;
		seed ^= static_cast<unsigned>(difficulty + 1) << 24;
		seed ^= static_cast<unsigned>(input_state) * 2654435761u;
		return seed ? seed : 1u;
	}
}

void SoloPlay::Main(){
	switch(phase){
	case 0: //pre-initialization
		Sound::ChangeBgm(-1);
		drawData.game_pos_x = 112;
		drawData.game_pos_y = 0;
	
		if(SOLO_USE_REGULAR_BG){
			BGImage.Change(image->i[BLANK_IMG], 255);
			BGImage.Draw(dxg);
			BGImage.Change(image->i[BG_IMG[0]], 8);
		}

		hCtrl.Initialize(dxg,image,input,&gameData);
		hCtrl.SetBitmapBackground(image->i[BG_IMG[0]]);

		phase++; count = 0;
		// Darebreak not included
	case 1:	// fade in
		// background drawing
		if(SOLO_USE_REGULAR_BG){
			BGImage.Draw(dxg);
		}

		// main game drawing
		hCtrl.DrawField(&drawData,count*8);

		if(count == 32){
			phase++; count = 0;
		}
		break;
	case 2:	// Initialization
		Sound::ChangeBgm(1);

		score = 0;
		grade = 0;

		effect.combo = 0;
		effect.gradeup = 0;
		effect.levelup = 0;
		effect.erace = 0;

		cntCombo = 0;
		cntErace = 0;
		cntFall = 0;
		cntTime = 0;
		cntLevel = 0;
		paused = false;
		phase++;

		// Dare break not included
		cur_diffic = 0;
		difficulty = -1;
		for(int i = 0; i < 3 * 4; ++i){
			imgDifficulty[i].reset();
		}
		for(int i = 0; i < 3; ++i){
			imgErase[i].reset();
		}
		imgCountdown.reset();
	case 3: // Difficulty selection
		// background drawing
		if(SOLO_USE_REGULAR_BG){
			BGImage.Draw(dxg);
		}
		// main game drawing
		hCtrl.DrawField(&drawData,255);
		// Score-related drawings
		DrawScore();

		difficulty = SelectDifficulty();
		if(difficulty != -1){
			SeedGameRandom(BuildGameStartRandomSeed(count, difficulty, input->GetKeyState(0,0)));
			hCtrl.Initialize(dxg,image,input,&gameData);
			for(int i = 0; i < 3 * 4; ++i){
				imgDifficulty[i].reset();
			}
			hCtrl.ChangeLevel(&gameData.difficultyData[difficulty][0]);
			hCtrl.SetBitmapBackground(image->i[BG_IMG[1]]);
			if(SOLO_USE_REGULAR_BG){
				BGImage.Change(image->i[BG_IMG[1]],8);
			}
			phase++; count = 0; cur_pos = 32;

			imgReady[0] = bn::sprite_items::words.create_sprite(-32, 0, READY_PAT_INDEX);
			imgReady[1] = bn::sprite_items::words.create_sprite(0, 0, READY_PAT_INDEX + 1);
			imgReady[2] = bn::sprite_items::words.create_sprite(32, 0, READY_PAT_INDEX + 2);
		}
		break;
	case 4: // Ready ...?
		Sound::ChangeBgm(-1);
		// background drawing
		if(SOLO_USE_REGULAR_BG){
			BGImage.Draw(dxg);
		}
		// main game drawing
		hCtrl.DrawField(&drawData,255);
		// Score-related drawings
		DrawScore();

		dxg->TexturePos(80,112,80,16);
		if(count<6){
			dxg->TexturePos(80,64 + 16*(count/2),80,16);
		}
		dxg->Draw(image->i[WORDS_IMG],(float)drawData.game_pos_x + 8,(float)drawData.game_pos_y + 96);

		if(count < 90){
			// ReadyPosition/pattern update
			int readyPattern = count/2;
			if(readyPattern > 3)
				readyPattern = 3;
			imgReady[0]->set_tiles(bn::sprite_items::words.tiles_item(), READY_PAT_INDEX + readyPattern * 8);
			imgReady[1]->set_tiles(bn::sprite_items::words.tiles_item(), READY_PAT_INDEX + readyPattern * 8 + 1);
			imgReady[2]->set_tiles(bn::sprite_items::words.tiles_item(), READY_PAT_INDEX + readyPattern * 8 + 2);
			
			if(!(count%30)){
				Sound::PlaySe(1);
				cur_pos = 32;
			}
			cur_pos /= 1.25;

			const int countdown_pattern = COUNTDOWN_PAT_INDEX + (count / 30) * 8;
			const int countdown_x = drawData.game_pos_x + 32 + 16 - WINDOW_WIDE / 2;
			const int countdown_y = drawData.game_pos_y + 128 - (int)cur_pos + 8 - WINDOW_HEIGHT / 2;
			if(! imgCountdown){
				imgCountdown = bn::sprite_items::words.create_sprite(countdown_x, countdown_y, countdown_pattern);
			}else{
				imgCountdown->set_position(countdown_x, countdown_y);
				imgCountdown->set_tiles(bn::sprite_items::words.tiles_item(), countdown_pattern);
				imgCountdown->set_visible(true);
			}
		}
		if(count >= 90){
			if(count == 90){
				Sound::PlaySe(1);
				cur_pos = 0.0625;
				
				// Readyrelease
				imgReady[1].reset();
				imgReady[2].reset();
				imgCountdown.reset();
			}
			cur_pos *= 1.25;
			int flash = (count%4) ? 0 : 1;
			imgReady[0]->set_tiles(bn::sprite_items::words.tiles_item(), 6 + 8*(14+flash));
			imgReady[0]->set_position(0, -cur_pos);
		}
		if(count == 120){
			imgReady[0].reset();
			imgCountdown.reset();
			phase++;
			Sound::ChangeBgm(2);
		}

		break;
	case 5:	// in game
		if(input->GetKeyState(0,2) & START){
			paused = !paused;
			if(paused){
				Sound::PauseBgm();
			}else{
				Sound::ResumeBgm();
			}
			Sound::PlaySe(1);
		}

		// background drawing
		if(SOLO_USE_REGULAR_BG){
			BGImage.Draw(dxg);
		}

		if(paused){
			hCtrl.Draw(&drawData, false);
			DrawScore(false);
			DrawPauseOverlay();
			break;
		}

		// main game drawing
		if(hCtrl.Main(0)){
			hCtrl.StartGameOverPetrify(GAMEOVER_STONE_TILE);
			cur_pos = 120;
			Sound::ChangeBgm(-1);
			paused = false;
			phase++;
		}
		hCtrl.Draw(&drawData);

		cntTime++;
		// Processing of scores etc.
		if(hCtrl.queFix){
			hCtrl.queFix = 0;
			cntFall++;
			cntLevel++;
			if(!(cntLevel%100)&&(cntLevel!=0)){	// The difficulty level will not increase unless you erase the line.
				cntLevel--;
			}
			if(hCtrl.queErace){	// Score addition rate when erasing combo is the score increase rate when erasing consecutively
				effect.erace = 1;
				switch(hCtrl.queErace){
				case 1:
					getscore = 100 + cntCombo*300;
					break;
				case 2:
					getscore = 250 + cntCombo*450;
					break;
				case 3:
					getscore = 750 + cntCombo*600;
					break;
				case 4:
					getscore = 2000 + cntCombo*750;
					break;
				}
				score += getscore;
				cntErace += hCtrl.queErace;
				cntLevel += hCtrl.queErace;
				if(cntLevel%100 < (cntLevel-hCtrl.queErace)%100){ // Increased difficulty
					Sound::PlaySe(7);
					switch(cntLevel/100){
					case 2:
						Sound::ChangeBgm(9);break;
					case 4:
						Sound::ChangeBgm(10);break;
					case 6:
						Sound::ChangeBgm(11);break;
					case 8:
						Sound::ChangeBgm(12);break;
					case 9:
						if(difficulty==3)
							Sound::ChangeBgm(13);
						break;
					default:;
					}
					if(SOLO_USE_REGULAR_BG){
						BGImage.Change(image->i[BG_IMG[cntLevel/100+1]],8);
					}
					hCtrl.SetBitmapBackground(image->i[BG_IMG[cntLevel/100+1]]);
					effect.levelup = 1;
					hCtrl.ChangeLevel(&gameData.difficultyData[difficulty][cntLevel/100]);
					if((difficulty==0)&&(cntLevel>=300)){ // EASYclear
						Sound::ChangeBgm(6);
						cntLevel = 300;
						cur_pos = 240;
						phase++;
					}
					if(cntLevel>=1000){	//clear
						Sound::ChangeBgm(6);
						cntLevel = 1000;
						cur_pos = 240;
						phase++;
					}
				}
				cntCombo++;
				if(cntCombo>=2){ // 2combo or more
					effect.combo = 1;
				}
				if(score >= gameData.borderScore[grade]){ // grade(SS,S,ABCDEF)up
					Sound::PlaySe(10);
					grade++;
					effect.gradeup = 1;
				}
				effect.line = hCtrl.queErace;
				hCtrl.queErace = 0;
			}else{
				cntCombo = 0;
			}
		}

		// Drawing scores etc.
		DrawScore(true);

		break;
	case 6:	// death
		hCtrl.CountUp();
		hCtrl.StepGameOverPetrify(GAMEOVER_STONE_TILE);

		// background drawing
		if(SOLO_USE_REGULAR_BG){
			BGImage.Draw(dxg);
		}
		// main game drawing
		hCtrl.Draw(&drawData);
		// Drawing scores etc.
		DrawScore();

		cur_pos--;

		// game over image
		dxg->TexturePos(96,0,96,160-(int)(160/60*cur_pos)-8);
		dxg->Draw(image->i[FRAME_IMG],(float)drawData.game_pos_x + 8,(float)drawData.game_pos_y+40+(160/60*cur_pos));

		// main game drawing
		hCtrl.DrawField(&drawData,255,false);

		if(cur_pos <= 0){
			Sound::PlaySe(9);
			phase ++;
			cur_pos = 120;
			if((cntLevel == 1000)||((cntLevel == 300)&&(difficulty == 0))){
				cur_pos = 360;
			}
		}
		break;
	case 7: // Wow or congratulations
		// background drawing
		if(SOLO_USE_REGULAR_BG){
			BGImage.Draw(dxg);
		}
		// main game drawing
		hCtrl.Draw(&drawData);
		// Drawing scores etc.
		DrawScore();
		// game over image
		dxg->TexturePos(96,0,96,160);
		dxg->Draw(image->i[FRAME_IMG],(float)drawData.game_pos_x + 8,(float)drawData.game_pos_y+40);
		// main game drawing
		hCtrl.DrawField(&drawData,255,false);
		
		if((cntLevel == 1000)||((cntLevel == 300)&&(difficulty == 0))){
			int x=32,y=88,font =  image->i[WHITEFONT_IMG],count2 = (int)cur_pos;
			if(count2%4){
				font = image->i[FONT_IMG];
			}
			dxg->TexturePos(0,0,320,56);
			dxg->Draw(image->i[BLANK_IMG],0,(float)y-8,true,192);
			DrawImageFont(x,y,dxg,font,		"              Congratulations!!");
			y += 16;
			switch(difficulty){
			case 0:
				DrawImageFont(x,y,dxg,font,	"Please challenge a normal mode next time!");
				break;
			case 1:
				DrawImageFont(x,y,dxg,font,	"Please challenge a master mode next time!");
				break;
			case 2:
				DrawImageFont(x,y,dxg,font,	"          You are a super player!");
				break;
			case 3:
				DrawImageFont(x,y,dxg,font,	"        You are a perfect player!!!");
				break;
			}
			y += 16;
			DrawImageFont(x,y,dxg,font,		"          Thank you for playing.");
		}else{
			int x=128,y=88,font =  image->i[WHITEFONT_IMG],count2 = (int)cur_pos;
			if(count2%4){
				font = image->i[FONT_IMG];
			}
			dxg->TexturePos(0,0,320,24);
			dxg->Draw(image->i[BLANK_IMG],0,(float)y-8,true,192);
			DrawImageFont(x,y,dxg,font,		"GAME OVER");
		}

		cur_pos--;
		if(cur_pos <= 0){
			if(ScoreData.JudgeOrder(score,difficulty) != -1){
				phase += 1;
					// Name processing
				compat_strcpy(name.name,"   ");
				name.cur_pos = 0;
				name.cur_word = 65;
				cur_pos = 96-24;
				if(cntLevel == 1000){
					cur_pos = 96-56;
				}
			}else{
				phase += 2;
				count = 0;
			}
		}
		break;
	case 8: // Looks like I'll be able to get a high score
		Sound::ChangeBgm(8);
		// background drawing
		if(SOLO_USE_REGULAR_BG){
			BGImage.Draw(dxg);
		}
		// Drawing scores etc.
		DrawScore();
		// Thread the wall
		hCtrl.DrawField(&drawData,255);
		// game over image
		dxg->TexturePos(96,0,96,160);
		dxg->Draw(image->i[FRAME_IMG],(float)drawData.game_pos_x + 8,(float)drawData.game_pos_y+40);
		// main game drawing
		hCtrl.DrawField(&drawData,255,false);

		{
			int x=32,y=88;
			dxg->TexturePos(0,0,320,96-(int)cur_pos);
			dxg->Draw(image->i[BLANK_IMG],0,(float)y-8,true,192);
		}

		{
			int font =  image->i[WHITEFONT_IMG];
			if(count%4){
				font = image->i[FONT_IMG];
			}
			char tmp[9];
			char tmp2[5];
			compat_strcpy(tmp2,rankname[ScoreData.JudgeOrder(score,difficulty)]);
			DrawImageFont(90,88,dxg,font,"Please input your name.");
			DrawImageFont(80,100,dxg,font,"%s [%s]%d %d %s",tmp2,gradename[grade],score,cntLevel,FrameCountToStr(cntTime,tmp));
			if(NameEntry()){
				ScoreData.EntryScore(ScoreData.JudgeOrder(score,difficulty), difficulty, score, cntLevel, cntTime, name.name);
				phase++; count=0;
			}
		}

		if(cur_pos > 0.25){
			cur_pos /= 1.25;
		}else{
			cur_pos = 0;
		}
		break;
	case 9: // Next game
		Sound::ChangeBgm(3);
		// background drawing
		if(SOLO_USE_REGULAR_BG){
			BGImage.Draw(dxg);
		}
		// Drawing scores etc.
		DrawScore();
		// Thread the wall
		hCtrl.DrawField(&drawData,255);

		// game over image
		cur_pos = (float)count * 2;
		if(cur_pos > 160){
			cur_pos = 160;
		}
		dxg->TexturePos(96,0,96,(int)cur_pos);
		dxg->Draw(image->i[FRAME_IMG],(float)drawData.game_pos_x + 8,(float)drawData.game_pos_y+40);

		hCtrl.DrawField(&drawData,255,false);
		DrawImageFont(132,116,dxg,image->i[FONT_IMG],"CONTINUE?");
		DrawImageFont(152,128,dxg,image->i[BIGINT_IMG],16,16,0,"%d",9-count/100);

		if(input->GetKeyState(0,0)&BUTTON[1]){
			count += 8;
		}
		if(count >= 999){
			count=0;
			phase++;
		}
		if(input->GetKeyState(0,2)&BUTTON[0]){
			// 1Initialize again for when you come back after playing
			hCtrl.Initialize(dxg,image,input,&gameData);
			hCtrl.SetBitmapBackground(image->i[BG_IMG[0]]);
			// BGImage
			if(SOLO_USE_REGULAR_BG){
				BGImage.Change(image->i[BG_IMG[0]],8);
			}
			count = 0;
			phase = 2;
		}
		break;
	case 10: // fade out
		// background drawing
		if(SOLO_USE_REGULAR_BG){
			BGImage.Draw(dxg);
		}

		dxg->TexturePos();
		dxg->Draw(image->i[BLANK_IMG],0,0,true,count*8);
		// main game drawing
		hCtrl.DrawField(&drawData,255-count*8);

		if(count == 32){
			*scene = SCORE_SCENE;
		}
		break;
	}
	count++;
}

void SoloPlay::ComboEffect()
{
	int tmp = (32-effect.combo);
	if(tmp<0){
		tmp=0;
	}else{
		tmp *= tmp;
	}

	int font =  image->i[WHITEFONT_IMG];
	if(count%4){
		font = image->i[FONT_IMG];
	}
	DrawImageFont(48-tmp,128,dxg,font,"%d COMBO!!",cntCombo);
	if(effect.combo>64){
		effect.combo = 0;
	}else{
		effect.combo++;
	}
}

void SoloPlay::GradeupEffect()
{
	if(imgRank){
		const int rank_index = (effect.gradeup / 2) % 2 ? 0 : grade + 1;
		imgRank->set_tiles(bn::sprite_items::rank.tiles_item(), rank_index);
		imgRank->set_visible(true);
	}

	if(effect.gradeup>32){
		effect.gradeup = 0;
	}else{
	effect.gradeup++;
	}
}

void SoloPlay::LevelupEffect()
{
	if(effect.levelup>64){
		effect.levelup = 0;
	}else{
	effect.levelup++;
	}
}

void SoloPlay::EraceEffect()
{
	int tmp = (32-effect.erace);
	if(tmp<0){
		tmp=0;
	}else{
		tmp *= tmp;
	}

	int line_index = effect.line - 1;
	if(line_index < 0){
		line_index = 0;
	}else if(line_index > 3){
		line_index = 3;
	}

	int erase_pattern = ERASE_PAT_INDEX + line_index * 8;
	if(! (count % 4)){
		erase_pattern += 3;
	}

	const int erase_left = 40 - tmp;
	const int erase_top = 104;
	for(int i = 0; i < 3; ++i){
		const int sprite_x = erase_left + 16 + i * 32 - WINDOW_WIDE / 2;
		const int sprite_y = erase_top + 8 - WINDOW_HEIGHT / 2;
		if(! imgErase[i]){
			imgErase[i] = bn::sprite_items::words.create_sprite(sprite_x, sprite_y, erase_pattern + i);
		}else{
			imgErase[i]->set_position(sprite_x, sprite_y);
			imgErase[i]->set_tiles(bn::sprite_items::words.tiles_item(), erase_pattern + i);
			imgErase[i]->set_visible(true);
		}
	}

	int font =  image->i[WHITEFONT_IMG];
	if(count%4){
		font = image->i[FONT_IMG];
	}
	DrawImageFont(72-tmp,120,dxg,font,"%d pts",getscore);

	if(effect.erace>64){
		effect.erace = 0;
		for(int i = 0; i < 3; ++i){
			imgErase[i].reset();
		}
	}else{
	effect.erace++;
	}
}

int SoloPlay::SelectDifficulty()
{
	for(int i=0; i<=3; i++){
		int menu_pattern = 3 + i * 8;
		if(cur_diffic == i){
			menu_pattern += 8 * 4;
			if(count % 4){
				menu_pattern += 8 * 4;
			}
		}

		for(int j = 0; j < 3; ++j){
			const int sprite_x = drawData.game_pos_x + 8 + 16 + j * 32 - WINDOW_WIDE / 2;
			const int sprite_y = drawData.game_pos_y + 80 + i * 16 + 8 - WINDOW_HEIGHT / 2;
			const int sprite_index = i + j * 4;
			if(! imgDifficulty[sprite_index]){
				imgDifficulty[sprite_index] = bn::sprite_items::words.create_sprite(sprite_x, sprite_y, menu_pattern + j);
			}else{
				imgDifficulty[sprite_index]->set_position(sprite_x, sprite_y);
				imgDifficulty[sprite_index]->set_tiles(bn::sprite_items::words.tiles_item(), menu_pattern + j);
				imgDifficulty[sprite_index]->set_visible(true);
			}
		}
	}

	if((input->GetKeyState(0,1)) & UP){
		Sound::PlaySe(1);
		cur_diffic--;
	}
	if((input->GetKeyState(0,1)) & DOWN){
		Sound::PlaySe(1);
		cur_diffic++;
	}
	if(cur_diffic<0)
		cur_diffic = 3;
	if(cur_diffic>3)
		cur_diffic = 0;

	if((input->GetKeyState(0,1)) & BUTTON[0]){
		Sound::PlaySe(2);
		return cur_diffic;
	}
	return -1;
}

void SoloPlay::DrawPauseOverlay()
{
	int font = image->i[WHITEFONT_IMG];
	if(count % 32 >= 16){
		font = image->i[FONT_IMG];
	}

	dxg->TexturePos(0,0,96,24);
	dxg->Draw(image->i[BLANK_IMG],112,104,true,160);
	DrawImageFont(136,112,dxg,font,"PAUSED");
}

void SoloPlay::DrawScore(bool updateEffects)
{
	// Preparing the score display string
	char time[9];
	char line0[32];
	char line1[32];
	char line2[32];
	char line3[32];
	char line4[32];
	char line5[32];

	compat_sprintf(line0, "SCORE:%5d", score);
	compat_sprintf(line1, "NEXT :%5d", gameData.borderScore[grade]);
	compat_sprintf(line2, "ERASE:%5d", cntErace);
	compat_sprintf(line3, "FALL :%5d", cntFall);

	// Rank drawing
	const int rank_x = 224 + 16 - WINDOW_WIDE / 2;
	const int rank_y = 48 + 16 - WINDOW_HEIGHT / 2;
	const int rank_index = grade + 1;
	if(! imgRank){
		imgRank = bn::sprite_items::rank.create_sprite(rank_x, rank_y, rank_index);
	}else{
		imgRank->set_position(rank_x, rank_y);
		imgRank->set_tiles(bn::sprite_items::rank.tiles_item(), rank_index);
		imgRank->set_visible(true);
	}
	// Drawing scores etc.
	int nextlevel = cntLevel-cntLevel%100+99;
	if(nextlevel > 1000){
		nextlevel = 1000;
	}
	if((nextlevel > 300)&&(difficulty == 0)){
		nextlevel = 300;
	}
	compat_sprintf(line4, "LEVEL:%d/%d", cntLevel, nextlevel);
	if(difficulty == -1){
		compat_sprintf(line5, "MODE :SELECTING");
	}else{
		compat_sprintf(line5, "MODE :%s", difficname[difficulty]);
	}

	// Draw effects etc.
	hCtrl.DrawScoreTime(FrameCountToStr(cntTime, time));
	hCtrl.DrawScoreHudLine(0, line0);
	hCtrl.DrawScoreHudLine(1, line1);
	hCtrl.DrawScoreHudLine(2, line2);
	hCtrl.DrawScoreHudLine(3, line3);
	hCtrl.DrawScoreHudLine(4, line4);
	hCtrl.DrawScoreHudLine(5, line5);

	if(! updateEffects){
		return;
	}

	if(effect.combo){
		ComboEffect();
	}
	if(effect.gradeup){
		GradeupEffect();
	}
	if(effect.levelup){
		LevelupEffect();
	}
	if(effect.erace){
		EraceEffect();
	}else{
		for(int i = 0; i < 3; ++i){
			imgErase[i].reset();
		}
	}
}

int SoloPlay::NameEntry()
{
	DrawImageFont(136,120,dxg,image->i[BIGFONT_IMG],16,16,0,"%s",name.name);
	if(count/15%2){
		DrawImageFont(136+name.cur_pos*16,120,dxg,image->i[BIGFONT_IMG],16,16,0,"~");
	}else{
		DrawImageFont(136+name.cur_pos*16,120,dxg,image->i[BIGFONT_IMG],16,16,0,"%c",name.cur_word);
	}

	if(name.cur_word < 32){
		name.cur_word = 124;
	}
	if(name.cur_word > 124){
		name.cur_word = 32;
	}
	for(int i=0, tmp=name.cur_word ; i<15 ;  i++, tmp++){
		if(!i){
			tmp-=7;
			if(tmp < 32){
				tmp = 125 + tmp-32;
			}
		}
		if(tmp == 125){
			tmp = 32;
		}
		DrawImageFont(86 + i*10,152,dxg,image->i[FONT_IMG],255-abs(i-7)*32,"%c",tmp);
	}
	if(!(count%4)){
		DrawImageFont(156,152,dxg,image->i[WHITEFONT_IMG],"%c",name.cur_word);
		DrawImageFont(152,148,dxg,image->i[BIGFONT_IMG],16,16,0,"}");
	}

	if(input->GetKeyState(0,1) & LEFT){
		Sound::PlaySe(1);
		name.cur_word--;
	}
	if(input->GetKeyState(0,1) & RIGHT){
		Sound::PlaySe(1);
		name.cur_word++;
	}
	if(input->GetKeyState(0,2) & BUTTON[0]){
		Sound::PlaySe(2);
		switch(name.cur_word){
		case 123:
			if(name.cur_pos){
				name.cur_pos--;
			}
			name.name[name.cur_pos] = 32;
			break;
		case 124:
			return 1;
		default:
			if(name.cur_pos==3){
				name.cur_pos--;
			}
			name.name[name.cur_pos] = name.cur_word;
			name.cur_pos++;
			if(name.cur_pos==3){
				name.cur_word = 124;
			}
			break;
		}
	}
	return 0;
}
