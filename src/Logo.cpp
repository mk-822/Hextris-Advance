#include "logo.h"

void Logo::Main(){
	switch(phase){
	case 0:	//o‚é‚Æ‚«
		if(!count)
			logo_pos = 64;

		dxg->TexturePos(0,0,64,32);
		dxg->ColorChange(2);
		dxg->Draw(image->i[LOGO_IMG], LOGOX, logo_pos, false, count*(256/FADETIME));

		// ƒƒSˆÊ’u‚ðˆÚ“®
		logo_pos = (LOGOY + logo_pos*(MOVESPEED-1)) / MOVESPEED;
		
		if(count>=FADETIME){
			phase = 1;
			count = 0;
		}
		break;
	case 1:	//ÃŽ~‚µ‚Ä‚é‚Æ‚«
		dxg->TexturePos(0,0,64,32);
		dxg->Draw(image->i[LOGO_IMG], LOGOX,LOGOY, false, 255);
		if((count>=FADETIME)||(input->GetKeyState(0))){
			phase = 2;
			count = 0;
		}
		break;
	case 2:	//Á‚¦‚é‚Æ‚«
		dxg->TexturePos(0,0,64,32);
		dxg->ColorChange(2);
		dxg->Draw(image->i[LOGO_IMG], LOGOX,LOGOY, false, 255-count*(256/FADETIME));
		if(count>=FADETIME){
			*scene = TITLE_SCENE;
			phase = 0;
			count = 0;
		}
		break;
	}
	count++;
}
