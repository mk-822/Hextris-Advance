// BackGroundImageCtrl.cpp: BackGroundImageCtrl Class implementation
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BackGroundImageCtrl.h"
#include "common.h"
#include "bn_regular_bg_items_blank.h"
#include "bn_regular_bg_items_bg00.h"
#include "bn_regular_bg_items_bg01.h"
#include "bn_regular_bg_items_bg02.h"
#include "bn_regular_bg_items_bg03.h"
#include "bn_regular_bg_items_bg04.h"
#include "bn_regular_bg_items_bg05.h"
#include "bn_regular_bg_items_bg06.h"
#include "bn_regular_bg_items_bg07.h"
#include "bn_regular_bg_items_bg08.h"
#include "bn_regular_bg_items_bg09.h"
#include "bn_regular_bg_items_bg10.h"
#include "bn_regular_bg_items_bg11.h"
#include "bn_bg_palettes.h"
#include "bn_colors.h"

//////////////////////////////////////////////////////////////////////
// construction/extinction
//////////////////////////////////////////////////////////////////////


bn::regular_bg_ptr LoadImageImpl(int image)
{
	// GBAcorrespondence
	switch(image){
		case BG_IMG_START:
		return bn::regular_bg_items::bg00.create_bg(8, 48);
		case BG_IMG_START + 1:
		return bn::regular_bg_items::bg01.create_bg(8, 48);
		case BG_IMG_START + 2:
		return bn::regular_bg_items::bg02.create_bg(8, 48);
		case BG_IMG_START + 3:
		return bn::regular_bg_items::bg03.create_bg(8, 48);
		case BG_IMG_START + 4:
		return bn::regular_bg_items::bg04.create_bg(8, 48);
		case BG_IMG_START + 5:
		return bn::regular_bg_items::bg05.create_bg(8, 48);
		case BG_IMG_START + 6:
		return bn::regular_bg_items::bg06.create_bg(8, 48);
		case BG_IMG_START + 7:
		return bn::regular_bg_items::bg07.create_bg(8, 48);
		case BG_IMG_START + 8:
		return bn::regular_bg_items::bg08.create_bg(8, 48);
		case BG_IMG_START + 9:
		return bn::regular_bg_items::bg09.create_bg(8, 48);
		case BG_IMG_START + 10:
		return bn::regular_bg_items::bg10.create_bg(8, 48);
		case BG_IMG_START + 11:
		return bn::regular_bg_items::bg11.create_bg(8, 48);
		case BLANK_IMG:
		default:
		return bn::regular_bg_items::blank.create_bg(8, 48);
	}
}

BackGroundImageCtrl::BackGroundImageCtrl(){
	current = back = -1;
}

BackGroundImageCtrl::~BackGroundImageCtrl(){
	if(bg.has_value())
		bg.reset();
}

void BackGroundImageCtrl::Change(int image, int sp)
{
	//back = current;
	current = image;
	speed = sp;
	count = 0;

	if(current == -1){
		back = current;
		bg = LoadImageImpl(current);
	}
}

void BackGroundImageCtrl::Draw(draw* dxg)
{
	count++;
	float blend = (float)count * speed / 255;
	if(blend > 1){
		blend = 1;
	}
	if(current != back)
	{
		// currentIf it has just been set, it will first fade out.
		bn::bg_palettes::set_fade_color(bn::colors::black);
		bn::bg_palettes::set_fade_intensity(blend); // Change the fade rate

		// Once the fade out is complete, replace it with the next image.
		if(blend == 1){
			count = 0;
			back = current;
			if(bg.has_value())
				bg.reset();
			bg = LoadImageImpl(current);
		}
	}
	else{
		bn::bg_palettes::set_fade_color(bn::colors::black);
		bn::bg_palettes::set_fade_intensity(1 - blend); // Change the fade rate
	}
	// dxg->TexturePos();
	// if((back != -1)&&(blend != 255)){
	// 	dxg->Draw(back,0,0);
	// }
	// dxg->Draw(current,0,0,true,blend);
}
