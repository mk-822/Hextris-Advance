#include "stdafx.h"
#include "common.h"
#include "bn_seed_random.h"
#include "random.h"

namespace
{
	bn::seed_random game_random;
}

void SeedGameRandom(unsigned seed)
{
	game_random = bn::seed_random(seed);
}

int GameRandomInt(int limit)
{
	return game_random.get_int(limit);
}

const int Sub=6;


random :: random()
{
	for(int i = 0; i < 10 ; i++)
	{
		cnt[i].num = i;
		cnt[i].count2 = Sub;//���
	}
}


int random::Decision(){
	do{
		num = GameRandomInt(10);
		cnt[num].count2++;
		switch(cnt[num].num)
		{
		case 0:
			if(cnt[num].count2 == Sub+1){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		case 1:
			if(cnt[num].count2 == Sub+3){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		case 2:
			if(cnt[num].count2 == Sub+2){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		case 3:
			if(cnt[num].count2 == Sub+3){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		case 4:
			if(cnt[num].count2 == Sub+3){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		case 5:
			if(cnt[num].count2 == Sub+3){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		case 6:
			if(cnt[num].count2 == Sub+3){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		case 7:
			if(cnt[num].count2 == Sub+3){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		case 8:
			if(cnt[num].count2 == Sub+3){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		case 9:
			if(cnt[num].count2 == Sub+3){
				cnt[num].count2 = 0;
				return cnt[num].num;
			}
			break;
		}
	}while(true);
}
