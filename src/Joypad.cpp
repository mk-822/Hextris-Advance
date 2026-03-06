// Joypad operation JoyPadCtrl class

#include "Joypad.h"
#include "bn_keypad.h"

namespace
{
    INPUT _read_butano_input()
    {
        INPUT input = 0;

        if(bn::keypad::up_held())
        {
            input |= UP;
        }

        if(bn::keypad::down_held())
        {
            input |= DOWN;
        }

        if(bn::keypad::left_held())
        {
            input |= LEFT;
        }

        if(bn::keypad::right_held())
        {
            input |= RIGHT;
        }

        // Fixed assignment (ini Custom functions omitted)
        if(bn::keypad::a_held())
        {
            input |= BUTTON[0];
        }

        if(bn::keypad::b_held())
        {
            input |= BUTTON[1];
        }

        if(bn::keypad::l_held())
        {
            input |= BUTTON[2];
        }

        if(bn::keypad::r_held())
        {
            input |= BUTTON[3];
        }

        return input;
    }
}

INPUT JoyPadCtrl::keystate;
INPUT JoyPadCtrl::keystate_old[KEYDELAY];
INPUT JoyPadCtrl::keystate_once;
INPUT JoyPadCtrl::keystate_leg;

JoyPadCtrl::JoyPadCtrl() :
    count(0)
{
    IniFileLoad();
    GetJoyStickDeviceState();

    keystate = 0;
    keystate_once = 0;
    keystate_leg = 0;

    for(int i = 0; i < KEYDELAY; ++i)
    {
        keystate_old[i] = 0;
    }

    for(int i = 0; i < MAXPLAYERS; ++i)
    {
        joystate[i].input = 0;
        joystate[i].inputleg = 0;
        joystate[i].inputonce = 0;
        joystate[i].enable = (i == 0);

        for(int j = 0; j < KEYDELAY; ++j)
        {
            joystate[i].inputold[j] = 0;
        }
    }
}

void JoyPadCtrl::SetKeyState(bool in, int key)
{
    // Butano Well then bn::keypad Get every frame input from .
    // For compatibility with existing interfaces, when explicitly calling keystate Please update.
    if(in)
    {
        keystate |= key;
    }
    else
    {
        keystate &= ~key;
    }
}

INPUT JoyPadCtrl::GetKeyState(int player, int flag)
{
    if(player != 0)
    {
        return 0;
    }

    switch(flag)
    {
    case 0:
        return keystate;
    case 1:
        return keystate_leg;
    case 2:
        return keystate_once;
    case 3:
        return keystate_old[KEYDELAY - 1];
    default:
        return 0;
    }
}

void JoyPadCtrl::RenewKeyState()
{
    const INPUT current_input = _read_butano_input();

    keystate_once = current_input & ~keystate_old[0];
    keystate_leg = keystate_once;

    if(count % REPEATTIME == 0)
    {
        keystate_leg |= keystate_old[KEYDELAY - 1];
    }

    keystate = current_input;
    keystate_old[0] = current_input;

    for(int i = KEYDELAY - 1; i > 0; --i)
    {
        keystate_old[i] = keystate_old[i - 1] & current_input;
    }

    ++count;
}

void JoyPadCtrl::IniFileLoad()
{
    // GBA/Butano In the edition ini Do not use custom assignments.
}

void JoyPadCtrl::GetJoyStickDeviceState()
{
    // GBA The input of the actual machine is 1P Fixed.
    for(int i = 0; i < MAXPLAYERS; ++i)
    {
        joystate[i].enable = (i == 0);
    }
}
