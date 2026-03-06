#include "Sound.h"

#include <algorithm>

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

void Sound::Initialize(){
	bgm_no = -1;
	bgm_mode = 0;

	// GBA / Butano In the edition DXSince the library's sound handle is not used,
	// Initialize only the array to maintain compatibility.
	std::fill_n(bgm, BGM_MAX, -1);
	std::fill_n(se_no, SE_MAX, -1);
}

void Sound::ChangeBgm(int no){
	// MP3 BGM teeth GBA / Butano This is a dummy implementation because it cannot be played as is.
	// To maintain consistency with existing logic, only the current number will be updated.
	bgm_no = no;
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
