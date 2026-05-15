// EraseData.cpp: EraseData Class implementation
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EraseData.h"

//////////////////////////////////////////////////////////////////////
// construction/extinction
//////////////////////////////////////////////////////////////////////

EraseData::EraseData()
{

}

void EraseData::Clear()
{
	line = 0;
	for(int i=0 ; i<4 ; i++){
		linenum[i] = 0;
		for(int j=0 ; j<11 ; j++){
			line_rows[i][j] = 0;
		}
	}
}

void EraseData::SetLine(int ln)
{
	if(line<4){
		linenum[line] = ln;
		for(int i=0 ; i<11 ; i++){
			line_rows[line][i] = ln;
		}
		line++;
	}
}

void EraseData::SetLineRows(const int* rows, int rows_count)
{
	if(line<4){
		int max_row = 0;
		for(int i=0 ; i<11 ; i++){
			const int row = i < rows_count ? rows[i] : 0;
			line_rows[line][i] = row;
			if(row > max_row){
				max_row = row;
			}
		}
		linenum[line] = max_row;
		line++;
	}
}

int EraseData::GetLine()
{
	if(line<=0){
		return -1;
	}

	line--;
	return linenum[line];
}

void EraseData::GetLineRows(int* rows, int rows_count)
{
	if(line<=0 || ! rows){
		return;
	}

	line--;
	for(int i=0 ; i<rows_count && i<11 ; i++){
		rows[i] = line_rows[line][i];
	}
}

int EraseData::GetFreq()
{
	return line;
}
