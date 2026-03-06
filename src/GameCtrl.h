#ifndef GAMECTRLHEADER
#define GAMECTRLHEADER

#include "DxGraph.h"
#include "Joypad.h"
#include "common.h"
#include "Mode.h"

// BMP フォントを使って文字を描画。英数字だけ。
void DrawImageFont(int x, int y, draw* dxg, int font, const char*, ...);
void DrawString(int x, int y, draw* dxg, int font, const char*, ...);
void DrawRankingFontSprite(int x, int y, draw* dxg, int font, int sizex, int sizey, int offsetx, const char* format, ...);

// FPS カウンタ。返値は FPS
int CalcuFps();

// ゲーム管理のいちばんだいじなところ
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
