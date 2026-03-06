#ifndef JOYPADHEADER
#define JOYPADHEADER

#include "keydefine.h"

typedef unsigned int INPUT;

class JoyPadCtrl{
private:	
	struct GamePadState
	{
		INPUT input;		// which button is pressed(keydefine.h There is a list in)
		INPUT inputleg;		// Tototototototototototototototototototototototototototototototototototo
		INPUT inputonce;	// to.
		INPUT inputold[KEYDELAY];	// Backup of input state

		int player;					// Which player to assign(ini read from)
		int button[MAXBUTTONS];				// Which button to assign(ini read from)
		bool enable;					// Is it available?
	}joystate[MAXPLAYERS];
	static INPUT keystate;
	static INPUT keystate_leg;
	static INPUT keystate_once;
	static INPUT keystate_old[KEYDELAY];

	int count;							// Counter (just for key repeat)
	void IniFileLoad();					// ini Internal method for loading files
	void GetJoyStickDeviceState();
public:
	JoyPadCtrl();
	static void SetKeyState(bool in, int key);	// WM_KEYDOWN WM_KEYUP For updating keyboard information when
	void RenewKeyState();				// Update input status
	INPUT GetKeyState(int player, int flag=0);		// Get input status for each player
};

#endif