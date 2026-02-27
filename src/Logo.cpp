#include "logo.h"
#include "bn_sprite_items_logo.h"
#include "bn_blending.h"

void Logo::Main(){
	switch(phase){
	case 0:	//出るとき
		if(!count){
			logo_pos = 64;
			spr = bn::sprite_items::logo.create_sprite(0, 0);
			spr->set_blending_enabled(true);
		}

		//dxg->TexturePos(0,0,64,32);
		//dxg->ColorChange(2);
		//dxg->Draw(image->i[LOGO_IMG], LOGOX, logo_pos, false, count*(256/FADETIME));
		bn::blending::set_fade_alpha((float)1 - (float)count/FADETIME);
		spr->set_position(LOGOX, logo_pos);

		// ロゴ位置を移動
		logo_pos = (LOGOY + logo_pos*(MOVESPEED-1)) / MOVESPEED;
		
		if(count>=FADETIME){
			phase = 1;
			count = 0;
		}
		break;
	case 1:	//静止してるとき
		//dxg->TexturePos(0,0,64,32);
		//dxg->Draw(image->i[LOGO_IMG], LOGOX,LOGOY, false, 255);
		spr->set_position(LOGOX,LOGOY);
		if((count>=FADETIME)||(input->GetKeyState(0))){
			phase = 2;
			count = 0;
		}
		break;
	case 2:	//消えるとき
		//dxg->TexturePos(0,0,64,32);
		//dxg->ColorChange(2);
		//dxg->Draw(image->i[LOGO_IMG], LOGOX,LOGOY, false, 255-count*(256/FADETIME));
		bn::blending::set_fade_alpha((float)count/FADETIME);
		spr->set_position(LOGOX,LOGOY);
		if(count>=FADETIME){
			*scene = TITLE_SCENE;
			phase = 0;
			count = 0;
			spr->set_blending_enabled(false);
			spr.reset();
		}
		break;
	}
	count++;
}
