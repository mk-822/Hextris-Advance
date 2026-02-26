#include "title.h"

void Title::Main(){
	switch(phase){
	case 0:	//出るとき
		if(!count){
			menu_pos = 320;
			cur_pos = 240;
			cur_count = 0;
		}

		Sound::ChangeBgm(0);
		dxg->TexturePos();
		dxg->Draw(image->i[TITLE_IMG], 0, 0, true, count*(256/FADETIME));
		InMenu();
		
		if(count>=FADETIME){
			phase = 1;
			count = 0;
			current = 0;
			cur_pos = 0;
		}
		break;
	case 1:	//選ぶとき
		dxg->TexturePos();
		dxg->Draw(image->i[TITLE_IMG], 0, 0);
		scene_tmp = Select();
		if(scene_tmp != -1){
			phase = 2;
			count = 0;
			cur_pos = MENUX;
			cur_move = 2;
			cur_movemove = (float)-0.1;
		}
		break;
	case 2:	// 消えるとき
		dxg->TexturePos();
		dxg->Draw(image->i[TITLE_IMG], 0, 0, true, 255-count*(256/FADETIME));
		OutMenu();

		if(count>=FADETIME){
			switch(scene_tmp){
			case 0:
				*scene = SINGLE_SCENE;
				break;
			case 1:
				//*scene = MULTI_SCENE;
				break;
			case 2:
				*scene = SCORE_SCENE;
				break;
			case 3:
				//*scene = OPTION_SCENE;
				break;
			case 5:
				*scene = SCORE_SCENE;
				break;
			default:
				break;
			}
		}
		break;
	}
	count++;
	cur_count++;
}

// メニューが入ってくるメソッド
void Title::InMenu(){
	// メニューの位置を変更
	menu_pos = (MENUX + menu_pos*7) / 8;
	cur_pos = (MENUY + cur_pos*7) / 8;
	
	for(int i=0; i<=MENUMAX; i++){
		dxg->TexturePos(0,i*16,80,16);
		dxg->Draw(image->i[WORDS_IMG], menu_pos, (float)(MENUY+i*16), true, count*(128/FADETIME));
	}

	// カレントアニメ
	dxg->TexturePos(160,16*((cur_count/ANIMSPEED)%10),96,16);
	dxg->Draw(image->i[WORDS_IMG], MENUX-8, (float)cur_pos, true, count*(256/FADETIME));
}

// 選ぶメソッド
int Title::Select(){
	// カレントアニメの位置を変更
	cur_pos = (current + cur_pos*(MOVESPEED-1)) / (MOVESPEED);

	dxg->ColorChange(1);
	for(int i=0; i<=MENUMAX; i++){
		if(current!=i){
			dxg->TexturePos(0,i*16,80,16);
			dxg->Draw(image->i[WORDS_IMG], MENUX, (float)(MENUY+i*16), false, 128);
		}
	}
	// カレント画像
	dxg->TexturePos(80,16*current,80,16);
	dxg->Draw(image->i[WORDS_IMG], MENUX, (float)(MENUY+16*current), true, 255);
	// カレントアニメ
	dxg->TexturePos(160,16*((cur_count/ANIMSPEED)%10),96,16);
	dxg->Draw(image->i[WORDS_IMG], MENUX-8, (float)(MENUY+(int)(16*cur_pos+0.5)), true, 255);

	if((input->GetKeyState(0,1)) & UP){
		Sound::PlaySe(1);
		current--;
	}
	if((input->GetKeyState(0,1)) & DOWN){
		Sound::PlaySe(1);
		current++;
	}
	if(current<0)
		current = MENUMAX;
	if(current>MENUMAX)
		current = 0;

	if((input->GetKeyState(0,1)) & BUTTON[0]){
		Sound::PlaySe(2);
		return current;
	}
	return -1;
}

// メニューが出てくメソッド
void Title::OutMenu(){
	for(int i=0; i<=MENUMAX; i++){
		dxg->TexturePos(0,i*16,80,16);
		dxg->Draw(image->i[WORDS_IMG], MENUX, (float)(MENUY+i*16), true, 127-count*(128/FADETIME));
	}
	
	// カレントアニメ
	cur_movemove *= (float)OUTMOVESPEED;
	cur_move += cur_movemove;
	cur_pos += cur_move;
	dxg->TexturePos(160,16*((cur_count/ANIMSPEED)%10),96,16);
	if(scene_tmp != 5){
		dxg->Draw(image->i[WORDS_IMG], cur_pos, (float)(MENUY+(int)(16*scene_tmp)), true, 255-count*(256/FADETIME));
	}
}