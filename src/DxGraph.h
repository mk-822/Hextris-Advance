#ifndef DXGRAPHHEADER
#define DXGRAPHHEADER

#include <stdio.h>

const int IMG_MAX = 64; // 最大保存可能画像数
const float pai = 3.141592f;
struct Image{
	int i[IMG_MAX];
};

class draw
{
public:
	void MatchVirtualPosition(bool flag=true);
	void ShiftDrawPosition(int x=0, int y=0);
	typedef unsigned char SPIN;
	void ChangeEFXState(float scaleX = 1, float scaleY = 1, SPIN Spin = 0, float SPCX = 0, float SPCY = 0);
	draw();
	~draw();
	void TexturePos(int x = 0 , int y = 0 , int w = 320 , int h = 240);
	bool Initialize(bool,int,int,int);//フルかどうかbool , 解像度X , 解像度Y)
	void releaseDXG();
	int LoadImage(char*,int,int R, int G, int B);
	void Draw(int NUM, float transX, float transY, bool flag = true, int tr_all = 255, int r = 255 , int g = 255 , int b = 255);//rgb、tr_allに因数がない場合255になる
	void ColorChange(int,bool flag=false);	// ブレンド方法の変更。フラグを省略すると外部から呼び出す用
	//RGBを指定してtransも指定できる						//RGBを指定しないでtransを指定することはできない
	//RGBを指定してtransを指定しないことができる
	//RGBを指定しないでtransを指定しないことができる

	void DrawBegin(bool);//引数は何色で初期化するか
	void DrawEnd();
private:
	bool match_vpos;
	int shift_x, shift_y;
	float scale_x, scale_y;

	int mode;
	float spin;
	int X,Y,W,H;
};
#endif