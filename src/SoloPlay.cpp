#include "soloplay.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <bn_sprite_items_words.h>
#include <bn_sprite_items_font_big.h>

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

void SoloPlay::Main(){
	switch(phase){
	case 0: //前初期化
		Sound::ChangeBgm(-1);
		drawData.game_pos_x = 112;
		drawData.game_pos_y = 0;
	
		BGImage.Change(image->i[BLANK_IMG], 255);
		BGImage.Draw(dxg);
		BGImage.Change(image->i[BG_IMG[0]], 8);

		hCtrl.Initialize(dxg,image,input,&gameData);

		phase++; count = 0;
		// あえてbreak を入れていません
	case 1:	// フェードイン
		// 背景の描画
		BGImage.Draw(dxg);

		// メインゲームの描画
		hCtrl.DrawField(&drawData,count*8);

		if(count == 32){
			phase++; count = 0;
		}
		break;
	case 2:	// 初期化
		Sound::ChangeBgm(1);
		SeedGameRandom(GetTickCount());

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
		phase++;

		// あえて break を入れていません
		cur_diffic = 0;
		difficulty = -1;
	case 3: // 難易度選択
		// 背景の描画
		BGImage.Draw(dxg);
		// メインゲームの描画
		hCtrl.DrawField(&drawData,255);
		// スコア関連の描画
		DrawScore();

		difficulty = SelectDifficulty();
		if(difficulty != -1){
			hCtrl.ChangeLevel(&gameData.difficultyData[difficulty][0]);
			BGImage.Change(image->i[BG_IMG[1]],8);
			phase++; count = 0; cur_pos = 32;

			imgReady[0] = bn::sprite_items::words.create_sprite(-32, 0, READY_PAT_INDEX);
			imgReady[1] = bn::sprite_items::words.create_sprite(0, 0, READY_PAT_INDEX + 1);
			imgReady[2] = bn::sprite_items::words.create_sprite(32, 0, READY_PAT_INDEX + 2);

			imgCountDown = bn::sprite_items::words.create_sprite(0, 16, 6 + 8*10); // 透明にしておく
		}
		break;
	case 4: // Ready ...?
		Sound::ChangeBgm(-1);
		// 背景の描画
		BGImage.Draw(dxg);
		// メインゲームの描画
		hCtrl.DrawField(&drawData,255);
		// スコア関連の描画
		DrawScore();

		dxg->TexturePos(80,112,80,16);
		if(count<6){
			dxg->TexturePos(80,64 + 16*(count/2),80,16);
		}
		dxg->Draw(image->i[WORDS_IMG],(float)drawData.game_pos_x + 8,(float)drawData.game_pos_y + 96);

		if(count < 90){
			// Ready位置・パターン更新
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

			DrawImageFont(
				drawData.game_pos_x + 40,
				drawData.game_pos_y + 128 - (int)cur_pos,
				dxg,
				image->i[BIGINT_IMG],
				16,16,-4,
				"%d",3-count/30
			);
			imgCountDown->set_tiles(bn::sprite_items::words.tiles_item(), 6 + 8*11 + (count/30)*8);
		}
		if(count >= 90){
			if(count == 90){
				Sound::PlaySe(1);
				cur_pos = 0.0625;
				
				// Ready解放
				imgReady[0].reset();
				imgReady[1].reset();
				imgReady[2].reset();
			}
			cur_pos *= 1.25;
			dxg->TexturePos(160,176,24,16);
			if(count%4){
				dxg->TexturePos(160,160,24,16);
			}
			dxg->Draw(
				image->i[WORDS_IMG],
				(float)drawData.game_pos_x + 36,
				(float)drawData.game_pos_y + 128 - cur_pos,
				true
			);
			int flash = (count%4) ? 0 : 1;
			imgCountDown->set_tiles(bn::sprite_items::words.tiles_item(), 6 + 8*(14+flash));
			imgCountDown->set_y(-cur_pos);
		}
		if(count == 120){
			imgCountDown.reset();
			phase++;
			Sound::ChangeBgm(2);
		}

		break;
	case 5:	// ゲーム中
		// 背景の描画
		BGImage.Draw(dxg);

		// メインゲームの描画
		if(hCtrl.Main(0)){
			cur_pos = 120;
			Sound::ChangeBgm(-1);
			phase++;
		}
		hCtrl.Draw(&drawData);

		cntTime++;
		// スコアなどの処理
		if(hCtrl.queFix){
			hCtrl.queFix = 0;
			cntFall++;
			cntLevel++;
			if(!(cntLevel%100)&&(cntLevel!=0)){	// ライン消しじゃないと難易度はアップしない
				cntLevel--;
			}
			if(hCtrl.queErace){	// 消去時のスコア加算率 combo は連続で消したときの得点上昇倍率
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
				if(cntLevel%100 < (cntLevel-hCtrl.queErace)%100){ // 難易度上昇
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
					BGImage.Change(image->i[BG_IMG[cntLevel/100+1]],8);
					effect.levelup = 1;
					hCtrl.ChangeLevel(&gameData.difficultyData[difficulty][cntLevel/100]);
					if((difficulty==0)&&(cntLevel>=300)){ // EASYクリア
						Sound::ChangeBgm(6);
						cntLevel = 300;
						cur_pos = 240;
						phase++;
					}
					if(cntLevel>=1000){	//クリアー
						Sound::ChangeBgm(6);
						cntLevel = 1000;
						cur_pos = 240;
						phase++;
					}
				}
				cntCombo++;
				if(cntCombo>=2){ // 2コンボ以上
					effect.combo = 1;
				}
				if(score >= gameData.borderScore[grade]){ // グレード(SS,S,ABCDEF)アップ
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

		// スコアなど描画
		DrawScore();

		break;
	case 6:	// 死亡
		hCtrl.CountUp();

		// 背景の描画
		BGImage.Draw(dxg);
		// メインゲームの描画
		hCtrl.Draw(&drawData);
		// スコアなど描画
		DrawScore();

		cur_pos--;

		// ゲームオーバ画像
		dxg->TexturePos(96,0,96,176-(int)(176/60*cur_pos)-8);
		dxg->Draw(image->i[FRAME_IMG],(float)drawData.game_pos_x,(float)drawData.game_pos_y+40+(176/60*cur_pos));

		// メインゲームの描画
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
	case 7: // うぇいと もしくはおめでとう
		// 背景の描画
		BGImage.Draw(dxg);
		// メインゲームの描画
		hCtrl.Draw(&drawData);
		// スコアなど描画
		DrawScore();
		// ゲームオーバ画像
		dxg->TexturePos(96,0,96,176);
		dxg->Draw(image->i[FRAME_IMG],(float)drawData.game_pos_x,(float)drawData.game_pos_y+40);
		// メインゲームの描画
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
					// 名前入れ用処理
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
	case 8: // ハイスコアにはいれるようだ
		Sound::ChangeBgm(8);
		// 背景の描画
		BGImage.Draw(dxg);
		// スコアなど描画
		DrawScore();
		// 壁をびょうが
		hCtrl.DrawField(&drawData,255);
		// ゲームオーバ画像
		dxg->TexturePos(96,0,96,176);
		dxg->Draw(image->i[FRAME_IMG],(float)drawData.game_pos_x,(float)drawData.game_pos_y+40);
		// メインゲームの描画
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
	case 9: // 次のゲームへ
		Sound::ChangeBgm(3);
		// 背景の描画
		BGImage.Draw(dxg);
		// スコアなど描画
		DrawScore();
		// 壁をびょうが
		hCtrl.DrawField(&drawData,255);

		// ゲームオーバ画像
		dxg->TexturePos(96,0,96,(int)cur_pos-8);
		dxg->Draw(image->i[FRAME_IMG],(float)drawData.game_pos_x,(float)drawData.game_pos_y+40+176-cur_pos);

		cur_pos = 176 - (float)count*2;

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
			// 1プレイして戻ってきた時用にもう一度初期化
			hCtrl.Initialize(dxg,image,input,&gameData);
			// BGImage
			BGImage.Change(image->i[BG_IMG[0]],8);
			count = 0;
			phase = 2;
		}
		break;
	case 10: // フェードアウト
		// 背景の描画
		BGImage.Draw(dxg);

		dxg->TexturePos();
		dxg->Draw(image->i[BLANK_IMG],0,0,true,count*8);
		// メインゲームの描画
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
	if(effect.gradeup/2%2){
		dxg->TexturePos(0,0,32,32);
		dxg->Draw(image->i[RANK_IMG],224,40);
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
	dxg->TexturePos(96, 192 + (effect.line-1)*16, 96, 16);
	if(count%4){
		dxg->TexturePos(0, 192 + (effect.line-1)*16, 96, 16);
	}
	dxg->Draw(image->i[WORDS_IMG],16-(float)tmp,104);

	int font =  image->i[WHITEFONT_IMG];
	if(count%4){
		font = image->i[FONT_IMG];
	}
	DrawImageFont(48-tmp,120,dxg,font,"%d pts",getscore);

	if(effect.erace>64){
		effect.erace = 0;
	}else{
	effect.erace++;
	}
}

int SoloPlay::SelectDifficulty()
{
	dxg->ColorChange(1);
	for(int i=0; i<=3; i++){
		if(cur_diffic!=i){
			dxg->TexturePos(0,i*16+64,80,16);
			dxg->Draw(image->i[WORDS_IMG], (float)drawData.game_pos_x+8, (float)(drawData.game_pos_y+ 80 +i*16), false, 128);
		}
	}
	// カレント画像
	if(count%4){
		dxg->TexturePos(0,16*cur_diffic+128,80,16);
	}else{
		dxg->TexturePos(80,16*cur_diffic+128,80,16);
	}
	dxg->Draw(image->i[WORDS_IMG], (float)drawData.game_pos_x+8, (float)(drawData.game_pos_y+ 80 +16*cur_diffic), true, 255);

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

void SoloPlay::DrawScore()
{
	// プレイ時間描画
	char time[9];
	DrawImageFont(
		drawData.game_pos_x-2, 220, dxg,
		image->i[BIGINT_IMG],
		16, 16, -4,
		"%s", FrameCountToStr(cntTime,time)
	);

	// スコアなどの背景を暗くする処理
	dxg->TexturePos(0,0,96,64);
	dxg->Draw(image->i[BLANK_IMG],212,76,true,128);
	// ランクの描画
	dxg->TexturePos((grade+1)*32,0,32,32);
	dxg->Draw(image->i[RANK_IMG],224,40);
	// スコアなどの描画
	DrawImageFont(216,80,dxg,image->i[FONT_IMG],"SCORE:%d",score);
	DrawImageFont(216,88,dxg,image->i[FONT_IMG],"NEXT :%d",gameData.borderScore[grade]);
	DrawImageFont(216,96,dxg,image->i[FONT_IMG],"ERASE:%d",cntErace);
	DrawImageFont(216,104,dxg,image->i[FONT_IMG],"FALL :%d",cntFall);
	int nextlevel = cntLevel-cntLevel%100+99;
	if(nextlevel > 1000){
		nextlevel = 1000;
	}
	if((nextlevel > 300)&&(difficulty == 0)){
		nextlevel = 300;
	}
	DrawImageFont(216,112,dxg,image->i[FONT_IMG],"LEVEL:%d/%d",cntLevel,nextlevel);
	if(difficulty == -1){
		DrawImageFont(216,120,dxg,image->i[FONT_IMG],"MODE :SELECTING");
	}else{
		DrawImageFont(216,120,dxg,image->i[FONT_IMG],"MODE :%s",difficname[difficulty]);
	}

	// エフェクトなど描画
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
