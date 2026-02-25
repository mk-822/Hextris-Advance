// MultiPlay.cpp: MultiPlay クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MultiPlay.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

/* コピペ用
for(int i=0 ; i<PLAYER_MAX ; i++){
*/

static const int DIFFICULTY = 1;

static const int PLAYER_COLOR[4] = {1,3,5,6};
static const int COLOR_PLAYER[8] = {-1,0,-1,1,-1,2,3,-1};

void MultiPlay::Main()
{
	switch(phase){
	case 0: // 初期化 /////////////////////////////////////
		Sound::ChangeBgm(5);
		dxg->MatchVirtualPosition();
		srand(GetTickCount());
		// とりあえず初期化する
		speed=0;
		for(int i=0 ; i<PLAYER_MAX ; i++){
			Player[i].hCtrl.Initialize(dxg,image,input,&blockdata);
			Player[i].Initialize(dxg,image,input,this);	// 超過激な事してますけどこうしないとめんどくさいので。
			Player[i].hCtrl.ChangeLevel(&blockdata.difficultyData[DIFFICULTY][speed]);
		}
		BGImage.Change(image->i[BG_IMG[9]],255);
		phase++; count=0;
		break;
	case 1: // フェードイン ////////////////////////////////
		BGImage.Draw(dxg);
		for(int i=0 ; i<PLAYER_MAX ; i++){
			dxg->ShiftDrawPosition(i*80,16);
			Player[i].Main(i);
			dxg->ShiftDrawPosition();
		}
		dxg->TexturePos();
		dxg->Draw(image->i[BLANK_IMG],0,0,true,255-count*8);
		if(count > 32){
			phase++; count=0;
		}
		break;
	case 2: // 参加意思待ち ////////////////////////////////
		BGImage.Draw(dxg);
		for(int i=0 ; i<PLAYER_MAX ; i++){
			dxg->ShiftDrawPosition(i*80,16);
			Player[i].Main(i);
			dxg->ShiftDrawPosition();
		}
		{
			int tmp=9-count/90;
			for(int i=0 ; i<=PLAYER_MAX ; i++){
				if(i == PLAYER_MAX){
					tmp = -1;
					break;
				}
				if(Player[i].phase == 0){
					break;
				}
			}
			if(tmp==-1){
				int game_ok = 0;
				for(int i=0 ; i<PLAYER_MAX ; i++){
					if(Player[i].phase == 1){
						game_ok++;
					}
				}
				if(game_ok>=2){
					phase++;
				}else{
					phase = 16;
				}
				count=0;
				tmp = 0;
			}		
			DrawImageFont(152,8,dxg,image->i[BIGFONT_IMG],16,16,0,"%d",tmp);
		}
		break;
	case 3: // カウントダウン ////////////////////////////////
		BGImage.Draw(dxg);
		for(int i=0 ; i<PLAYER_MAX ; i++){
			dxg->ShiftDrawPosition(i*80,16);
			Player[i].Main(i);
			dxg->ShiftDrawPosition();
		}
		dxg->TexturePos(80,112,80,16);
		if(count<6){
			dxg->TexturePos(80,64 + 16*(count/2),80,16);
		}
		dxg->Draw(image->i[WORDS_IMG],120,80);

		if(count < 90){
			DrawImageFont(
				152,
				96,
				dxg,
				image->i[BIGINT_IMG],
				16,16,-4,
				"%d",3-count/30
			);
		}
		if(count >= 90){
			dxg->TexturePos(160,176,24,16);
			if(count%4){
				dxg->TexturePos(160,160,24,16);
			}
			dxg->Draw(
				image->i[WORDS_IMG],
				148,
				96,
				true
			);
		}
		if(count == 120){
			for(int i=0 ; i<PLAYER_MAX ; i++){
				switch(Player[i].phase){
				case 0:
					Player[i].phase = 2;
					break;
				case 1:
					Player[i].phase = 4;
					break;
				}
			}
			for(int i=0 ; i<PLAYER_MAX ; i++){
				Player[i].target = i;
				Player[i].target = NextTarget(i,Player[i].target);
			}
			phase++; count=0;
		}
		break;
	case 4: // メインゲーム中 ////////////////////////////////
		BGImage.Draw(dxg);
		for(int i=0 ; i<PLAYER_MAX ; i++){
			dxg->ShiftDrawPosition(i*80,16);
			Player[i].Main(i);
			dxg->ShiftDrawPosition();
		}
		{
			int game_end = 0;
			for(int i=0 ; i<PLAYER_MAX ; i++){
				if(Player[i].phase == 4){
					game_end++;
				}
			}
			if(game_end == 1){
				for(int i=0 ; i<PLAYER_MAX ; i++){
					if(Player[i].phase == 4){
						Player[i].phase = 6;
					}
				}
				Sound::PlaySe(10);
				phase++; count=0;
			}
		}
		if(count/60 > 180){
			if(!(count%(60*10))){
				speed++;
				if(speed > 9){
					speed = 9;
				}
				for(int i=0 ; i<PLAYER_MAX ; i++){
					Player[i].hCtrl.ChangeLevel(&blockdata.difficultyData[DIFFICULTY][speed]);
				}
			}
		}
		break;
	case 5: // ゲームセット //////////////////////////////////
		BGImage.Draw(dxg);
		for(int i=0 ; i<PLAYER_MAX ; i++){
			dxg->ShiftDrawPosition(i*80,16);
			Player[i].Main(i);
			dxg->ShiftDrawPosition();
		}
		if(count > 240){
			speed=0;
			for(int i=0 ; i<PLAYER_MAX ; i++){
				Player[i].hCtrl.ChangeLevel(&blockdata.difficultyData[DIFFICULTY][speed]);
				Player[i].hCtrl.Initialize(dxg,image,input,&blockdata);
				switch(Player[i].phase){
				case 5:
					Player[i].phase = 0;
					break;
				case 6:
					Player[i].phase = 0;
					Player[i].win++;
				}
			}
			phase=2; count=0;
		}
		break;
	case 16: // おわる //////////////////////////////////////
		BGImage.Draw(dxg);
		dxg->TexturePos();
		dxg->Draw(image->i[BLANK_IMG],0,0,true,count*8);
		if(count>32){
			phase++; count=0;
		}
		break;
	case 17: // 本当におわる ////////////////////////
		*scene = TITLE_SCENE;
		break;
	}
	count++;
}

void MultiPlay::PlayerOperation::Main(int player)
{
	HexFieldDrawData drawData={0,0};
	switch(phase){
	case 0: // 参加意思確認中
		dxg->ChangeEFXState(0.85f,0.85f);
		hCtrl.DrawField(&drawData,255,false);
		dxg->ChangeEFXState();
		DrawImageFont(22,96,dxg,image->i[FONT_IMG],"READY?");
		if(input->GetKeyState(player,2) & BUTTON[0]){
			Sound::PlaySe(2);
			phase++;
		}
		break;
	case 1: // 参加表明済み
		dxg->ChangeEFXState(0.85f,0.85f);
		hCtrl.DrawField(&drawData,255,true);
		dxg->ChangeEFXState();
		DrawImageFont(22,96,dxg,image->i[FONT_IMG]," OK!");
		break;
	case 2: // 不参加
		dxg->ChangeEFXState(0.85f,0.85f);
		hCtrl.DrawField(&drawData,255,false);
		dxg->ChangeEFXState();
		DrawImageFont(22,96,dxg,image->i[FONT_IMG],"ENTER?");
		if(input->GetKeyState(player,2) & BUTTON[0]){
			if(pMulti->phase == 1){
				Sound::PlaySe(2);
				phase = 1;
			}else{
				phase++;
			}
		}
		break;
	case 3: // 参加意思確認フェーズ待ち
		dxg->ChangeEFXState(0.85f,0.85f);
		hCtrl.DrawField(&drawData,255,false);
		dxg->ChangeEFXState();
		DrawImageFont(22,96,dxg,image->i[FONT_IMG],"WAIT...");
		if(pMulti->phase == 2){
			phase = 1;
		}
		break;
	case 4: // メインゲーム中
		dxg->ChangeEFXState(0.85f,0.85f);
		if(hCtrl.Main(player)){
			phase++;
		}
		hCtrl.Draw(&drawData);
		dxg->ChangeEFXState();
		for(int i=0 ; i<8 ; i++){
			if(hCtrl.queFloorUp[i] == -1){
				continue;
			}
			dxg->TexturePos(COLOR_PLAYER[hCtrl.queFloorUp[i]]*8,32,8,8);
			dxg->Draw(image->i[MULTI_IMG],(float)i*8 + 8,186);
		}
		if(hCtrl.queFix){
			hCtrl.queFix = 0;
			if(hCtrl.queErace){
				if(hCtrl.queErace+combo>=2){
					pMulti->Player[target].hCtrl.EnqueFloorUp(hCtrl.queErace-1+combo,PLAYER_COLOR[player]);
					if(hCtrl.queErace+combo==4){
						pMulti->Player[target].hCtrl.EnqueFloorUp(1,PLAYER_COLOR[player]);
					}
				}
				target = pMulti->NextTarget(player,target);
				combo = 1;
				hCtrl.queErace = 0;
			}else{
				combo = 0;
			}
		}
		
		dxg->TexturePos(32,32,16,16);
		dxg->Draw(image->i[MULTI_IMG],40,198);
		dxg->TexturePos(target*16,0,16,16);
		dxg->Draw(image->i[MULTI_IMG],56,198);
		break;
	case 5: // 死んだ
		dxg->ChangeEFXState(0.85f,0.85f);
		hCtrl.Draw(&drawData);
		hCtrl.DrawField(&drawData,255);
		hCtrl.CountUp();
		dxg->ChangeEFXState();
		DrawImageFont(16,96,dxg,image->i[FONT_IMG],"YOU LOSE");
		break;
	case 6: // 勝った
		dxg->ChangeEFXState(0.85f,0.85f);
		hCtrl.Draw(&drawData);
		hCtrl.CountUp();
		dxg->ChangeEFXState();
		DrawImageFont(16,96,dxg,image->i[FONT_IMG],"YOU WIN!");
		break;
	}
	DrawImageFont(2,26,dxg,image->i[FONTEX_IMG],"!");
	DrawImageFont(10,26,dxg,image->i[FONT_IMG],"%d",win);
	dxg->TexturePos(player*32,16,32,16);
	dxg->Draw(image->i[MULTI_IMG],8,198);
}

void MultiPlay::PlayerOperation::Initialize(draw *Dxg, Image *Image, JoyPadCtrl *Input, MultiPlay *PMulti)
{
	dxg = Dxg;
	image = Image;
	input = Input;
	pMulti = PMulti;
}

int MultiPlay::NextTarget(int player, int target)
{
	for(int i=target+1 ; i!=target ; i++){
		if(i>=PLAYER_MAX){
			i=0;
		}
		if((Player[i].phase == 4)&&(i != player)){
			return i;
		}
	}
	return target;
}
