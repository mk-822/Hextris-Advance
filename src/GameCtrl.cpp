#include "GameCtrl.h"
#include "logo.h"
#include "soloplay.h"
#include "title.h"
#include "ScoreDisplay.h"

GameCtrl::GameCtrl(){
	// ゲーム開始からのフレーム数
	count = 0;
	scene = LOGO_SCENE;
	mode = new Mode();
	mode->Initialize(&scene,&dxg,&image,&input);

	// 時計ライブラリの初期化＆画像読み込み
	dxg.Initialize(WINDOW_FULL,WINDOW_ZOOM,WINDOW_WIDE,WINDOW_HEIGHT);
	image.i[RANK_IMG] = dxg.LoadImage("./image/rank.bmp",1,0,0,0);//描画ロード(ファイルパスchar* , 描画モード , 透過色R , 透過色G , 透過色B)
	image.i[BLANK_IMG] = dxg.LoadImage("./image/blank.bmp",0,0,0,0);
	image.i[FONT_IMG] = dxg.LoadImage("./image/font.bmp",1,0,0,0);
	image.i[FONTEX_IMG] = dxg.LoadImage("./image/font_ex.bmp",1,0,0,0);
	image.i[BLOCK_IMG] = dxg.LoadImage("./image/blocks.bmp",1,0,0,0);
	image.i[LOGO_IMG] = dxg.LoadImage("./image/logo.bmp",1,0,0,0);
	image.i[TITLE_IMG] = dxg.LoadImage("./image/title.bmp",1,0,0,0);
	image.i[WORDS_IMG] = dxg.LoadImage("./image/words.bmp",1,0,0,0);
	image.i[FRAME_IMG] = dxg.LoadImage("./image/frame.bmp",1,0,0,0);
	image.i[BIGINT_IMG] = dxg.LoadImage("./image/font_big.bmp",1,0,0,0);
	image.i[WHITEFONT_IMG] = dxg.LoadImage("./image/whitefont.bmp",1,0,0,0);
	image.i[GRAYFONT_IMG] = dxg.LoadImage("./image/font_gray.bmp",1,0,0,0);
	image.i[OP_BG_IMG] = dxg.LoadImage("./image/logobg.bmp",1,0,0,0);
	image.i[SCOREBG_IMG] = dxg.LoadImage("./image/scorebg.bmp",1,0,0,0);
	image.i[BIGFONT_IMG] = dxg.LoadImage("./image/font16.bmp",1,0,0,0);
	image.i[MULTI_IMG] = dxg.LoadImage("./image/multi.bmp",1,0,0,0);

	image.i[BG_IMG[0]] = dxg.LoadImage("./image/bg00.bmp",1,0,0,0);
	image.i[BG_IMG[1]] = dxg.LoadImage("./image/bg01.bmp",1,0,0,0);
	image.i[BG_IMG[2]] = dxg.LoadImage("./image/bg02.bmp",1,0,0,0);
	image.i[BG_IMG[3]] = dxg.LoadImage("./image/bg03.bmp",1,0,0,0);
	image.i[BG_IMG[4]] = dxg.LoadImage("./image/bg04.bmp",1,0,0,0);
	image.i[BG_IMG[5]] = dxg.LoadImage("./image/bg05.bmp",1,0,0,0);
	image.i[BG_IMG[6]] = dxg.LoadImage("./image/bg06.bmp",1,0,0,0);
	image.i[BG_IMG[7]] = dxg.LoadImage("./image/bg07.bmp",1,0,0,0);
	image.i[BG_IMG[8]] = dxg.LoadImage("./image/bg08.bmp",1,0,0,0);
	image.i[BG_IMG[9]] = dxg.LoadImage("./image/bg09.bmp",1,0,0,0);
	image.i[BG_IMG[10]] = dxg.LoadImage("./image/bg10.bmp",1,0,0,0);
	image.i[BG_IMG[11]] = dxg.LoadImage("./image/bg11.bmp",1,0,0,0);

	DrawImageFont(8,8,&dxg,image.i[FONT_IMG],"Now Loading...");
	dxg.DrawEnd();
}

// メインだおおおおおおおおおおおおお-----------------
void GameCtrl::Main(){
	dxg.DrawBegin(true);
	input.RenewKeyState();

	// シーンとシーンバックアップが異なっていたらモードチェンジだろう
	if(scene!=scene_bk){
		ChangeScene();
	}scene_bk = scene;

	mode->Main();

	//入力チェック用
	//::DrawImageFont(0,0,&dxg,image.i[FONT_IMG],"%d %d %d",input.GetKeyState(0,0),input.GetKeyState(0,1),input.GetKeyState(0,2));

	dxg.DrawEnd();

	// あとしまつ
	count++;
}
// メインおわりい-------------------------------------

void GameCtrl::ChangeScene(){
	delete mode;
	switch(scene){
	case LOGO_SCENE:
		mode = new Logo();
		break;
	case TITLE_SCENE:
		mode = new Title();
		break;
	case SCORE_SCENE:
		mode = new ScoreDisplay();
		break;
	case SINGLE_SCENE:
		mode = new SoloPlay();
		break;
	// case MULTI_SCENE:
	// 	mode = new MultiPlay();
	// 	break;
	// case OPTION_SCENE://Clockが追加しますた
	// 	mode = new Option();
	// 	break;
	}
	mode->Initialize(&scene,&dxg,&image,&input);
}

void DrawImageFont(int x, int y, draw* dxg, int font, const char* format, ...){
	char buffer[256];

	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	va_end(ap);

	int sizex = 8;
	int sizey = 8;
	int wide = -2;

	int len = strlen(buffer);
	for(int i=0 ; i<len ; i++){
		dxg->TexturePos((buffer[i]-32)*sizex, 0, sizex, sizey);
		dxg->Draw(font, (float)(x + i*(sizex + wide)), (float)y, true);
	}
}

void DrawImageFont(int x, int y, draw* dxg, int font, int trans, const char* format, ...){
	char buffer[256];

	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	va_end(ap);

	int sizex = 8;
	int sizey = 8;
	int wide = -2;

	int len = strlen(buffer);
	for(int i=0 ; i<len ; i++){
		dxg->TexturePos((buffer[i]-32)*sizex, 0, sizex, sizey);
		dxg->Draw(font, (float)(x + i*(sizex + wide)), (float)y, true, trans);
	}
}

void DrawImageFont(int x, int y, draw* dxg, int font, int sizex, int sizey, int offsetx, const char* format, ...){
	char buffer[256];

	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	va_end(ap);

	int len = strlen(buffer);
	for(int i=0 ; i<len ; i++){
		dxg->TexturePos((buffer[i]-32)*sizex, 0, sizex, sizey);
		dxg->Draw(font, (float)(x + i*(sizex + offsetx)), (float)y, true);
	}
}

int CalcuFps(){
	static int fps = 0;
	static int fpscount = 0;
	static unsigned long timer = GetTickCount() / 1000;
	fpscount++;
	if(timer != GetTickCount() / 1000){
		fps = fpscount;
		timer = GetTickCount() / 1000;
		fpscount = 0;
	}
	return fps;
}