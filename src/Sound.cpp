#include "Sound.h"
#include "common.h"

int Sound::bgm_no;
int Sound::bgm[];
int Sound::se_no[];
int Sound::bgm_mode;

void Sound::Initialize(){
	bgm_no = -1;
	bgm_mode = GetPrivateProfileInt("Sound","Streaming",1,WINDOW_INIPATH);

	// 効果音読み込み
	se_no[0] = LoadSoundMem("./se/arrival.wav");
	se_no[1] = LoadSoundMem("./se/cursor.wav");
	se_no[2] = LoadSoundMem("./se/decide.wav");
	se_no[3] = LoadSoundMem("./se/erese_combo.wav");
	se_no[4] = LoadSoundMem("./se/erese.wav");
	se_no[5] = LoadSoundMem("./se/fix.wav");
	se_no[6] = LoadSoundMem("./se/hextris.wav");
	se_no[7] = LoadSoundMem("./se/levelup.wav");
	se_no[8] = LoadSoundMem("./se/spin.wav");
	se_no[9] = LoadSoundMem("./se/shutout.wav");
	se_no[10] = LoadSoundMem("./bgm/hiscore.mp3");
	int sevolume = GetPrivateProfileInt("Sound","SEVolume",255,WINDOW_INIPATH);
	for(int i=0 ; i<=10 ; i++){
		ChangeVolumeSoundMem( sevolume , se_no[i]);
	}

	if(bgm_mode)
		SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);	// ストリーミングモード
	// BGM 読み込み
	bgm[0] = LoadSoundMem("./bgm/title.mp3");
	bgm[1] = LoadSoundMem("./bgm/modeselect.mp3");
	bgm[2] = LoadSoundMem("./bgm/soloplay.mp3");
	bgm[3] = LoadSoundMem("./bgm/gameover.mp3");
	bgm[5] = LoadSoundMem("./bgm/multiplay.mp3");
	bgm[6] = LoadSoundMem("./bgm/clear.mp3");	// nonloop
	bgm[7] = LoadSoundMem("./bgm/score.mp3");
	bgm[8] = LoadSoundMem("./bgm/hiscore.mp3");	// nonloop
	bgm[9] = LoadSoundMem("./bgm/solo_over200.mp3");
	bgm[10] = LoadSoundMem("./bgm/solo_over400.mp3");
	bgm[11] = LoadSoundMem("./bgm/solo_over600.mp3");
	bgm[12] = LoadSoundMem("./bgm/solo_over800.mp3");
	bgm[13] = LoadSoundMem("./bgm/solo_over900.mp3");
	int bgmvolume = GetPrivateProfileInt("Sound","BGMVolume",255,WINDOW_INIPATH);
	for(int i=0 ; i<=13 ; i++){
		ChangeVolumeSoundMem( bgmvolume , bgm[i]);
	}
}

void Sound::ChangeBgm(int no){
	if(no<0){
		StopSoundMem(bgm[bgm_no]);
		bgm_no = no;
	}else{
		if(no!=bgm_no){
			StopSoundMem(bgm[bgm_no]);
			bgm_no = no;
			switch(bgm_no){
				case 6:
				case 8:
					PlaySoundMem(bgm[bgm_no],DX_PLAYTYPE_BACK);	// ループなし
					break;
				default:
					PlaySoundMem(bgm[bgm_no],DX_PLAYTYPE_LOOP);	// ループあり
					break;
			}
		}
	}
}

void Sound::PlaySe(int no){
	PlaySoundMem(se_no[no],DX_PLAYTYPE_BACK);
}