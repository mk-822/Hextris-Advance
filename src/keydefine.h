#ifndef KEYDEFINEHEADER
#define KEYDEFINEHEADER

static const char* JOYINIPATH = "./setting.ini";	// Load settings ini path
static const int MAXPLAYERS = 4;					// Highest number of players
static const int MAXBUTTONS = 4;					// Maximum number of buttons
static const int KEYDELAY = 15;						// Time until repeat
static const int REPEATTIME = 2;					// Repeat interval (n in the frame 1 times)
static const char* JOYNAME[MAXPLAYERS] = {"JoyPad1","JoyPad2","JoyPad3","JoyPad4"};
													// ini Item name to read from
static const char* KEYNAME[MAXBUTTONS+1] = {"Player","Button1","Button2","Button3","Button4"};
													// Item name

static const int UP = 1;							// up
static const int DOWN = 2;							// did
static const int LEFT = 4;							// Hidari
static const int RIGHT = 8;							// right
static const int BUTTON[] = {16,32,64,128};			// Peony

#endif