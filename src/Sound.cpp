#include "Sound.h"

#include <algorithm>

#include "bn_music.h"
#include "random.h"

#if __has_include("bn_music_items.h")
#include "bn_music_items.h"
#define HEXTRIS_HAS_BN_MUSIC_ITEMS 1
#else
#define HEXTRIS_HAS_BN_MUSIC_ITEMS 0
#endif

#if __has_include("bn_sound_items.h")
#include "bn_sound_items.h"
#define HEXTRIS_HAS_BN_SOUND_ITEMS 1
#else
#define HEXTRIS_HAS_BN_SOUND_ITEMS 0
#endif

int Sound::bgm_no;
int Sound::bgm[BGM_MAX];
int Sound::se_no[SE_MAX];
int Sound::bgm_mode;

namespace
{
	bool is_main_game_bgm(int no)
	{
		return no == 2 || no == 9 || no == 10 || no == 11 || no == 12 || no == 13;
	}

#if HEXTRIS_HAS_BN_MUSIC_ITEMS
	constexpr bn::fixed BGM_VOLUME = 0.3;
	constexpr int MAIN_GAME_BGM_COUNT = 15;

	void play_main_game_bgm()
	{
		switch(GameRandomInt(MAIN_GAME_BGM_COUNT)){
		case 0: bn::music_items::a_winter_kiss.play(BGM_VOLUME); break;
		case 1: bn::music_items::absolute_xtc.play(BGM_VOLUME); break;
		case 2: bn::music_items::ambient_starfield.play(BGM_VOLUME); break;
		case 3: bn::music_items::amanita_dream.play(BGM_VOLUME); break;
		case 4: bn::music_items::bionic_girl.play(BGM_VOLUME); break;
		case 5: bn::music_items::cabin_fever.play(BGM_VOLUME); break;
		case 6: bn::music_items::chip_overture.play(BGM_VOLUME); break;
		case 7: bn::music_items::dreamline.play(BGM_VOLUME); break;
		case 8: bn::music_items::drozerix_dream_candy.play(BGM_VOLUME); break;
		case 9: bn::music_items::drozerix_leisurely_voice.play(BGM_VOLUME); break;
		case 10: bn::music_items::falling_raindrops.play(BGM_VOLUME); break;
		case 11: bn::music_items::simple_chip_tune.play(BGM_VOLUME); break;
		case 12: bn::music_items::spectrum.play(BGM_VOLUME); break;
		case 13: bn::music_items::speed_chip.play(BGM_VOLUME); break;
		default: bn::music_items::twinkle_and_twinkle_rmx.play(BGM_VOLUME); break;
		}
	}
#endif
}

void Sound::Initialize(){
	bgm_no = -1;
	bgm_mode = 0;

	// GBA / Butano In the edition DXSince the library's sound handle is not used,
	// Initialize only the array to maintain compatibility.
	std::fill_n(bgm, BGM_MAX, -1);
	std::fill_n(se_no, SE_MAX, -1);
}

void Sound::ChangeBgm(int no){
	if(bgm_no == no){
		return;
	}

	bgm_no = no;

#if HEXTRIS_HAS_BN_MUSIC_ITEMS
	switch(no){
	case -1:
		bn::music::stop();
		break;
	case 0:
		bn::music_items::you_would_be_here.play(BGM_VOLUME);
		break;
	case 1:
		bn::music_items::past_never_come_back.play(BGM_VOLUME);
		break;
	default:
		if(is_main_game_bgm(no)){
			play_main_game_bgm();
		}else{
			bn::music::stop();
		}
		break;
	}
#else
	(void) no;
#endif
}

void Sound::PauseBgm(){
#if HEXTRIS_HAS_BN_MUSIC_ITEMS
	bn::music::pause();
#endif
}

void Sound::ResumeBgm(){
#if HEXTRIS_HAS_BN_MUSIC_ITEMS
	bn::music::resume();
#endif
}

void Sound::PlaySe(int no){
#if HEXTRIS_HAS_BN_SOUND_ITEMS
	switch(no){
		case 0: bn::sound_items::arrival.play(); break;
		case 1: bn::sound_items::cursor.play(); break;
		case 2: bn::sound_items::decide.play(); break;
		case 3: bn::sound_items::erese_combo.play(); break;
		case 4: bn::sound_items::erese.play(); break;
		case 5: bn::sound_items::fix.play(); break;
		case 6: bn::sound_items::hextris.play(); break;
		case 7: bn::sound_items::levelup.play(); break;
		case 8: bn::sound_items::spin.play(); break;
		case 9: bn::sound_items::shutout.play(); break;
		case 10: bn::sound_items::hiscore.play(); break;
		default:
			break;
	}
#else
	(void) no;
#endif
}
