#include "Mode.h"
#include "common.h"

char* FrameCountToStr(int count,char* timestr){
	int csec = count*100/60;
	int sec = csec/100;
	int min = sec/60;

	csec %= 100;
	sec %= 60;
	min %= 60;
	compat_sprintf(timestr,"%02d:%02d:%02d",min,sec,csec);
	return timestr;
};

Mode::Mode(){
	count = 0;
	phase = 0;
}

Mode::~Mode(){
}

void Mode::Initialize(int* Scene,draw* Dxg,Image* Image,JoyPadCtrl* Input){
	dxg = Dxg;
	scene = Scene;
	image = Image;
	input = Input;
}

void Mode::Main(){
	count++;
	*scene = LOGO_SCENE;
}

void Mode::Draw(){
	DrawImageFont(32,32,dxg,image->i[FONT_IMG],"Initialized!!");
}
