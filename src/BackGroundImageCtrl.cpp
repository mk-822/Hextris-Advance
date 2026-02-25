// BackGroundImageCtrl.cpp: BackGroundImageCtrl クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BackGroundImageCtrl.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BackGroundImageCtrl::BackGroundImageCtrl(){
	current = -1;
}

void BackGroundImageCtrl::Change(int image, int sp)
{
	back = current;
	current = image;
	speed = sp;
	count = 0;
}

void BackGroundImageCtrl::Draw(draw* dxg)
{
	count++;
	int blend = count * speed;
	if(blend > 255){
		blend = 255;
	}
	dxg->TexturePos();
	if((back != -1)&&(blend != 255)){
		dxg->Draw(back,0,0);
	}
	dxg->Draw(current,0,0,true,blend);
}
