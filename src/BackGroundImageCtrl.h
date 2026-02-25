// BackGroundImageCtrl.h: BackGroundImageCtrl クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUNDIMAGECTRL_H__8DC0922C_32BA_4590_9CC5_0F540306F939__INCLUDED_)
#define AFX_BACKGROUNDIMAGECTRL_H__8DC0922C_32BA_4590_9CC5_0F540306F939__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BackGroundImageCtrl  
{
private:
	int speed;
	int count;
	int back;
	int current;
	enum{
		image_max = 10
	};
public:
	void Draw(draw* dxg);
	void Change(int image, int speed);
	BackGroundImageCtrl();
};

#endif // !defined(AFX_BACKGROUNDIMAGECTRL_H__8DC0922C_32BA_4590_9CC5_0F540306F939__INCLUDED_)
