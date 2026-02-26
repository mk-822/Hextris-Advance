#include "GameCtrl.h"
#include "common.h"
#include "sound.h"

#include "bn_core.h"

bool WINDOW_FULL = true;
int WINDOW_ZOOM = 1;

int main()
{
    bn::core::init();

    GameCtrl gctrl;
    Sound::Initialize();

    while(true)
    {
        gctrl.Main();
        bn::core::update();
    }
}
