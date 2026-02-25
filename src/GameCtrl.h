#ifndef GAMECTRLHEADER
#define GAMECTRLHEADER

#include "DxGraph.h"
#include "Joypad.h"
#include "common.h"
#include "Mode.h"

// BMP フォントを使って文字を描画。英数字だけ。
void DrawImageFont(int x, int y, draw* dxg, int font, const char*, ...);

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
	int scene_bk;

public:
	GameCtrl();
	void ChangeScene();
	void Main();
	
};

#endif