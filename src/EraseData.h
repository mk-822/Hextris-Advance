// EraseData.h: EraseData class interface
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERASEDATA_H__794AE4B7_B489_4A94_87FA_22A4E4767076__INCLUDED_)
#define AFX_ERASEDATA_H__794AE4B7_B489_4A94_87FA_22A4E4767076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class EraseData  
{
public:
	int GetFreq();
	int GetLine();
	void GetLineRows(int* rows, int rows_count);
	void SetLine(int ln);
	void SetLineRows(const int* rows, int rows_count);
	void Clear();
	EraseData();
private:
	int linenum[4];
	int line_rows[4][11];
	int line;
};

#endif // !defined(AFX_ERASEDATA_H__794AE4B7_B489_4A94_87FA_22A4E4767076__INCLUDED_)
