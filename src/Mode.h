#ifndef MODEHEADER
#define MODEHEADER

#include "DxGraph.h"
#include "Joypad.h"

void DrawImageFont(int,int,draw*,int,const char*,...);
void DrawImageFont(int x, int y, draw* dxg, int font, int sizex, int sizey, int offsetx, const char* format, ...);
void DrawImageFont(int x, int y, draw* dxg, int font, int trans, const char* format, ...);
char* FrameCountToStr(int count,char* timestr);

// åpè≥ÇµÇƒégÇ¡ÇƒÇ≠ÇæÇ≥Ç¢
class Mode{
protected:
	int* scene;
	draw* dxg;
	Image* image;
	JoyPadCtrl* input;
	int count;

public:
	int phase;
	Mode();
	void Initialize(int* Scene,draw* Dxg,Image* Image,JoyPadCtrl* Input);
	virtual void Main();
	virtual void Draw();
};

#endif
