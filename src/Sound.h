#pragma once

#define SE_MAX 11
#define BGM_MAX 14

class Sound{
public:
	static int bgm_no;
	static int bgm[BGM_MAX];
	static int se_no[SE_MAX];
	static void Initialize();
	static void ChangeBgm(int no);
	static void PlaySe(int no);
	static int bgm_mode;
};
