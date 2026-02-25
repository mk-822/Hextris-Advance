// EraseData.cpp: EraseData クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EraseData.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

EraseData::EraseData()
{

}

void EraseData::Clear()
{
	line = 0;
	for(int i=0 ; i<4 ; i++){
		linenum[i] = 0;
	}
}

void EraseData::SetLine(int ln)
{
	if(line<4){
		linenum[line] = ln;
		line++;
	}
}

int EraseData::GetLine()
{
	if(line<0){
		return -1;
	}

	line--;
	return linenum[line];
}

int EraseData::GetFreq()
{
	return line;
}
