#ifndef H__RANDOM__H
#define H__RANDOM__H
#include "stdafx.h"
#include  <stdlib.h>

class random
{
public :
	int Decision();
	random();
private:
	struct CNT
	{
		int num;
		int count2;
	}cnt[10];
	int num;
	void countmalc(CNT Cnt , int NUM);
	int randnum;
};


#endif