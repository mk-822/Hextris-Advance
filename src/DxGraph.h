#ifndef DXGRAPHHEADER
#define DXGRAPHHEADER

#include <stdio.h>

const int IMG_MAX = 64; // Maximum number of images that can be saved
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
	bool Initialize(bool,int,int,int);//Full or not?bool , resolutionX , resolutionY)
	void releaseDXG();
	int LoadImage(const char*,int,int R, int G, int B);
	void Draw(int NUM, float transX, float transY, bool flag = true, int tr_all = 255, int r = 255 , int g = 255 , int b = 255);//rgb、tr_allIf there is no factor in255become
	void ColorChange(int,bool flag=false);	// Change blending method. If the flag is omitted, it will be called from outside.
	//RGBspecifytranscan also be specified//RGBdo not specifytranscannot be specified
	//RGBspecifytransmay not be specified
	//RGBdo not specifytransmay not be specified

	void DrawBegin(bool);//What color should the argument be initialized with?
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
