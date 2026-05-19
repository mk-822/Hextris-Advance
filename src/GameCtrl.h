#ifndef GAMECTRLHEADER
#define GAMECTRLHEADER

#include "DxGraph.h"
#include "Joypad.h"
#include "common.h"
#include "Mode.h"

// BMP Draw characters using fonts. Only alphanumeric characters.
void DrawImageFont(int x, int y, draw* dxg, int font, const char*, ...);
void DrawImageFontBlended(int x, int y, draw* dxg, int font, const char*, ...);
void DrawString(int x, int y, draw* dxg, int font, const char*, ...);
void DrawRankingFontSprite(int x, int y, draw* dxg, int font, int sizex, int sizey, int offsetx, const char* format, ...);
void ReleaseFontSpriteResources();

// FPS counter. The return value is FPS
int CalcuFps();

// The most important part of game management
class GameCtrl{
private:
	float posX, posY;
	draw dxg;
	Mode* mode;
	JoyPadCtrl input;
	Image image;
	int count;
	int scene;
	int scene_bk = -1;

public:
	GameCtrl();
	void ChangeScene();
	void Main();
	
};

#endif
