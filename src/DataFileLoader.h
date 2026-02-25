// DataFileLoader.h: DataFileLoader クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFILELOADER_H__2CA5DEA7_8F85_42CE_AEA0_435EE2A4BEC2__INCLUDED_)
#define AFX_DATAFILELOADER_H__2CA5DEA7_8F85_42CE_AEA0_435EE2A4BEC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct BlockPositionData
{
	int x;
	int y;
	int next;
	int previous;
};

struct BlockData
{
	int center_x;
	int center_y;
	int pos[4];
	int color;
};

struct DelayData
{
	int fix;
	int erase;
	int appear;
	int g;
	int fall;
};

class DataFileLoader
{
public:
	enum{
		EASY,
		NORMAL,
		MASTER,
		DEATH
	};

	DelayData difficultyData[4][10];
	BlockPositionData posData[19];
	BlockData blockData[10];
	int borderScore[8];
	DataFileLoader();

};

#endif // !defined(AFX_DATAFILELOADER_H__2CA5DEA7_8F85_42CE_AEA0_435EE2A4BEC2__INCLUDED_)
