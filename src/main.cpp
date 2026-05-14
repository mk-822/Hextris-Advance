#include "GameCtrl.h"
#include "common.h"
#include "sound.h"

#include "bn_core.h"
#include <bn_sprite_ptr.h>
#include "bn_sprite_items_logo.h"

bool WINDOW_FULL = true;
int WINDOW_ZOOM = 1;

int main()
{
    bn::core::init();
    GameCtrl gctrl;
    Sound::Initialize();
    //auto spr = bn::sprite_items::logo.create_sprite(0, 0);
    while(true)
    {
        gctrl.Main();
        bn::core::update();
    }
}
