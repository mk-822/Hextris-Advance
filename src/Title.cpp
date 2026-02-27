#include "title.h"
#include "bn_regular_bg_items_title.h"
#include "bn_sprite_items_words.h"
#include "bn_blending.h"

void Title::Main(){
	switch(phase){
	case 0:	//出るとき
		if(!count){
			menu_pos = 320;
			cur_pos = 240;
			cur_count = 0;
			bg = bn::regular_bg_items::title.create_bg(8, 48);
			bg->set_blending_enabled(true);

			// カーソル用スプライト　幅1.5倍にして使う
			cursor[0] = bn::sprite_items::words.create_sprite(0, 0, CURSOR_PAT_INDEX);
			cursor[1] = bn::sprite_items::words.create_sprite(0, 0, CURSOR_PAT_INDEX + 1);
			cursor[0]->set_horizontal_scale(1.5);
			cursor[1]->set_horizontal_scale(1.5);
			cursor[0]->set_blending_enabled(true);
			cursor[1]->set_blending_enabled(true);

			// メニュー用スプライト 32x16を横3枚、縦4枚
			for(int i = 0; i < MENU_AMOUNT; ++i){
				for(int j = 0; j < 3; ++j){
					menu[i + j * MENU_AMOUNT] = bn::sprite_items::words.create_sprite(0, 0,
						MENU_PAT_INDEX + i * 8 + j);
					menu[i + j * MENU_AMOUNT]->set_blending_enabled(true);
				}
			}
			HighlightMenu();
		}

		Sound::ChangeBgm(0);
		//dxg->TexturePos();
		//dxg->Draw(image->i[TITLE_IMG], 0, 0, true, count*(256/FADETIME));
		bn::blending::set_fade_alpha((float)1 - (float)count / FADETIME);
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
		//dxg->TexturePos();
		//dxg->Draw(image->i[TITLE_IMG], 0, 0, true, 255-count*(256/FADETIME));
		bn::blending::set_fade_alpha((float)count / FADETIME);
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
			bg.reset();
			cursor[0].reset();
			cursor[1].reset();
			for(int i = 0; i < MENU_AMOUNT; ++i){
				for(int j = 0; j < 3; ++j){
					menu[i + j * MENU_AMOUNT].reset();
				}
			}
		}
		break;
	}
	count++;
	cur_count++;
}

// メニュースプライトをハイライトする
void Title::HighlightMenu(){
	for(int i=0; i<MENU_AMOUNT; i++){
		for(int j=0; j<3; j++){
			auto menuPattern = i == current ? MENU_SELECTED_PAT_INDEX : MENU_PAT_INDEX;
			menu[i + j * MENU_AMOUNT]->set_tiles(bn::sprite_items::words.tiles_item(), menuPattern + i * 8 + j);
		}
	}
}

// メニュースプライトの位置を変更する
void Title::OffsetMenuSprite(){
	for(int i=0; i<MENU_AMOUNT; i++){
		for(int j=0; j<3; j++){
			menu[i + j * MENU_AMOUNT]->set_position(menu_pos + j*32, (float)(MENUY+i*16));
		}
	}
}

// メニューが入ってくるメソッド
void Title::InMenu(){
	// メニューの位置を変更
	menu_pos = (MENUX + menu_pos*7) / 8;
	cur_pos = (MENUY + cur_pos*7) / 8;
	
	for(int i=0; i<MENU_AMOUNT; i++){
		dxg->TexturePos(0,i*16,80,16);
		dxg->Draw(image->i[WORDS_IMG], menu_pos, (float)(MENUY+i*16), true, count*(128/FADETIME));
	}
	OffsetMenuSprite();

	// カレントアニメ
	//dxg->TexturePos(160,16*((cur_count/ANIMSPEED)%10),96,16);
	//dxg->Draw(image->i[WORDS_IMG], MENUX-8, (float)cur_pos, true, count*(256/FADETIME));
	auto curPattern = (cur_count/ANIMSPEED)%10;
	cursor[0]->set_position(MENUX+CURSOR_OFFSET_X, (float)cur_pos);
	cursor[1]->set_position(MENUX+CURSOR_OFFSET_X+48, (float)cur_pos);
	cursor[0]->set_tiles(bn::sprite_items::words.tiles_item(), CURSOR_PAT_INDEX + curPattern * 256 / 32);
	cursor[1]->set_tiles(bn::sprite_items::words.tiles_item(), CURSOR_PAT_INDEX + 1 + curPattern * 256 / 32);
}

// 選ぶメソッド
int Title::Select(){
	// カレントアニメの位置を変更
	cur_pos = (current + cur_pos*(MOVESPEED-1)) / (MOVESPEED);

	dxg->ColorChange(1);
	for(int i=0; i<MENU_AMOUNT; i++){
		if(current!=i){
			dxg->TexturePos(0,i*16,80,16);
			dxg->Draw(image->i[WORDS_IMG], MENUX, (float)(MENUY+i*16), false, 128);
		}
	}
	OffsetMenuSprite();
	// カレント画像
	dxg->TexturePos(80,16*current,80,16);
	dxg->Draw(image->i[WORDS_IMG], MENUX, (float)(MENUY+16*current), true, 255);

	// カレントアニメ
	//dxg->TexturePos(160,16*((cur_count/ANIMSPEED)%10),96,16);
	//dxg->Draw(image->i[WORDS_IMG], MENUX-8, (float)(MENUY+(int)(16*cur_pos+0.5)), true, 255);
	auto curPattern = (cur_count/ANIMSPEED)%10;
	cursor[0]->set_position(MENUX+CURSOR_OFFSET_X, (float)(MENUY+(int)(16*cur_pos+0.5)));
	cursor[1]->set_position(MENUX+CURSOR_OFFSET_X+48, (float)(MENUY+(int)(16*cur_pos+0.5)));
	cursor[0]->set_tiles(bn::sprite_items::words.tiles_item(), CURSOR_PAT_INDEX + curPattern * 256 / 32);
	cursor[1]->set_tiles(bn::sprite_items::words.tiles_item(), CURSOR_PAT_INDEX + 1 + curPattern * 256 / 32);

	bool moved = false;
	if((input->GetKeyState(0,1)) & UP){
		Sound::PlaySe(1);
		current--;
		moved = true;
	}
	if((input->GetKeyState(0,1)) & DOWN){
		Sound::PlaySe(1);
		current++;
		moved = true;
	}
	if(current<0)
		current = MENU_AMOUNT - 1;
	if(current>=MENU_AMOUNT)
		current = 0;
	if(moved)
		HighlightMenu();

	if((input->GetKeyState(0,1)) & BUTTON[0]){
		Sound::PlaySe(2);
		return current;
	}
	return -1;
}

// メニューが出てくメソッド
void Title::OutMenu(){
	for(int i=0; i<MENU_AMOUNT; i++){
		dxg->TexturePos(0,i*16,80,16);
		dxg->Draw(image->i[WORDS_IMG], MENUX, (float)(MENUY+i*16), true, 127-count*(128/FADETIME));
	}
	OffsetMenuSprite();
	
	// カレントアニメ
	cur_movemove *= (float)OUTMOVESPEED;
	cur_move += cur_movemove;
	cur_pos += cur_move;
	//dxg->TexturePos(160,16*((cur_count/ANIMSPEED)%10),96,16);
	//if(scene_tmp != 5){
	//	dxg->Draw(image->i[WORDS_IMG], cur_pos, (float)(MENUY+(int)(16*scene_tmp)), true, 255-count*(256/FADETIME));
	//}
	
	auto curPattern = (cur_count/ANIMSPEED)%10;
	cursor[0]->set_position(cur_pos+CURSOR_OFFSET_X, (float)(MENUY+(int)(16*scene_tmp)));
	cursor[1]->set_position(cur_pos+CURSOR_OFFSET_X+48, (float)(MENUY+(int)(16*scene_tmp)));
	cursor[0]->set_tiles(bn::sprite_items::words.tiles_item(), CURSOR_PAT_INDEX + curPattern * 256 / 32);
	cursor[1]->set_tiles(bn::sprite_items::words.tiles_item(), CURSOR_PAT_INDEX + 1 + curPattern * 256 / 32);
}