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
const char* Sound::current_bgm_name;

namespace
{
	bool is_main_game_bgm(int no)
	{
		return no == 2 || no == 9 || no == 10 || no == 11 || no == 12 || no == 13;
	}

#if HEXTRIS_HAS_BN_MUSIC_ITEMS
	constexpr bn::fixed BGM_VOLUME = 0.3;
	constexpr int MAIN_GAME_BGM_COUNT = 20;
	int main_game_bgm_order[MAIN_GAME_BGM_COUNT];
	int main_game_bgm_index = MAIN_GAME_BGM_COUNT;

	void shuffle_main_game_bgms()
	{
		for(int index = 0; index < MAIN_GAME_BGM_COUNT; ++index){
			main_game_bgm_order[index] = index;
		}

		for(int index = MAIN_GAME_BGM_COUNT - 1; index > 0; --index){
			const int swap_index = GameRandomInt(index + 1);
			std::swap(main_game_bgm_order[index], main_game_bgm_order[swap_index]);
		}

		main_game_bgm_index = 0;
	}

	void reset_main_game_bgm_selection()
	{
		main_game_bgm_index = MAIN_GAME_BGM_COUNT;
	}

	const char* play_main_game_bgm()
	{
		if(main_game_bgm_index >= MAIN_GAME_BGM_COUNT){
			shuffle_main_game_bgms();
		}

		switch(main_game_bgm_order[main_game_bgm_index++]){
		case 0: bn::music_items::a_winter_kiss.play(BGM_VOLUME); return "a_winter_kiss.xm";
		case 1: bn::music_items::absolute_xtc.play(BGM_VOLUME); return "absolute_xtc.mod";
		case 2: bn::music_items::ambient_starfield.play(BGM_VOLUME); return "ambient_starfield.xm";
		case 3: bn::music_items::amanita_dream.play(BGM_VOLUME); return "amanita_dream.xm";
		case 4: bn::music_items::bionic_girl.play(BGM_VOLUME); return "bionic_girl.xm";
		case 5: bn::music_items::cabin_fever.play(BGM_VOLUME); return "cabin_fever.xm";
		case 6: bn::music_items::dreamline.play(BGM_VOLUME); return "dreamline.it";
		case 7: bn::music_items::drozerix_alone.play(BGM_VOLUME); return "drozerix_alone.xm";
		case 8: bn::music_items::drozerix_bubble_machine.play(BGM_VOLUME); return "drozerix_bubble_machine.xm";
		case 9: bn::music_items::drozerix_leisurely_voice.play(BGM_VOLUME); return "drozerix_leisurely_voice.xm";
		case 10: bn::music_items::drozerix_lost_without.play(BGM_VOLUME); return "drozerix_lost_without.xm";
		case 11: bn::music_items::drozerix_master_select.play(BGM_VOLUME); return "drozerix_master_select.xm";
		case 12: bn::music_items::drozerix_my_dearest.play(BGM_VOLUME); return "drozerix_my_dearest.xm";
		case 13: bn::music_items::nerve_intro.play(BGM_VOLUME); return "nerve_intro.xm";
		case 14: bn::music_items::nerve_intro2.play(BGM_VOLUME); return "nerve_intro2.xm";
		case 15: bn::music_items::october_chip.play(BGM_VOLUME); return "october_chip.xm";
		case 16: bn::music_items::spc.play(BGM_VOLUME); return "spc.xm";
		case 17: bn::music_items::spectrum.play(BGM_VOLUME); return "spectrum.xm";
		case 18: bn::music_items::speed_chip.play(BGM_VOLUME); return "speed_chip.s3m";
		default: bn::music_items::the_dim_dungeon.play(BGM_VOLUME); return "the_dim_dungeon.xm";
		}
	}
#endif
}

void Sound::Initialize(){
	bgm_no = -1;
	bgm_mode = 0;
	current_bgm_name = "none";

	// GBA / Butano In the edition DXSince the library's sound handle is not used,
	// Initialize only the array to maintain compatibility.
	std::fill_n(bgm, BGM_MAX, -1);
	std::fill_n(se_no, SE_MAX, -1);
#if HEXTRIS_HAS_BN_MUSIC_ITEMS
	reset_main_game_bgm_selection();
#endif
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
		current_bgm_name = "none";
		break;
	case 0:
		bn::music_items::falling_raindrops.play(BGM_VOLUME);
		current_bgm_name = "falling_raindrops.xm";
		break;
	case 1:
		bn::music_items::past_never_come_back.play(BGM_VOLUME);
		current_bgm_name = "past_never_come_back.xm";
		break;
	case 7:
		bn::music_items::you_would_be_here.play(BGM_VOLUME);
		current_bgm_name = "you_would_be_here.xm";
		break;
	case 8:
		bn::music_items::twinkle_and_twinkle_rmx.play(BGM_VOLUME);
		current_bgm_name = "twinkle_and_twinkle_rmx.it";
		break;
	default:
		if(is_main_game_bgm(no)){
			if(no == 2){
				reset_main_game_bgm_selection();
			}
			current_bgm_name = play_main_game_bgm();
		}else{
			bn::music::stop();
			switch(no){
			case 3: current_bgm_name = "unmapped continue BGM"; break;
			case 6: current_bgm_name = "unmapped clear BGM"; break;
			case 7: current_bgm_name = "unmapped score BGM"; break;
			case 8: current_bgm_name = "unmapped hiscore BGM"; break;
			default: current_bgm_name = "unmapped BGM"; break;
			}
		}
		break;
	}
#else
	(void) no;
	current_bgm_name = "music items missing";
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

const char* Sound::CurrentBgmName()
{
	return current_bgm_name ? current_bgm_name : "none";
}
