// MultiPlay.h: MultiPlay クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIPLAY_H__DA79FCB8_D7F6_4609_AA7D_511F305CE30D__INCLUDED_)
#define AFX_MULTIPLAY_H__DA79FCB8_D7F6_4609_AA7D_511F305CE30D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "GameCtrl.h"
#include "HextrisCtrl.h"
#include "DataFileLoader.h"	// ClassView によって追加されました。
#include "BackGroundImageCtrl.h"	// ClassView によって追加されました。

class MultiPlay : public Mode  
{
private:
	int speed;
	BackGroundImageCtrl BGImage;
	DataFileLoader blockdata;
	enum{
		PLAYER_MAX = 4
	};
public:
	class PlayerOperation{
	public:
		int target;
		int win;
		void Initialize(draw* Dxg, Image* Image, JoyPadCtrl* Input, MultiPlay* PMulti);
		int phase;
		void Main(int player);
		PlayerOperation(){
			phase=0;
			target=0;
			win=0;
		};
		HextrisCtrl hCtrl;
	private:
		int combo;
		draw* dxg;
		Image* image;
		JoyPadCtrl* input;
		MultiPlay* pMulti;
	}Player[4];
	int NextTarget(int player,int target);
	void Main();
	MultiPlay(){};
};

#endif // !defined(AFX_MULTIPLAY_H__DA79FCB8_D7F6_4609_AA7D_511F305CE30D__INCLUDED_)
