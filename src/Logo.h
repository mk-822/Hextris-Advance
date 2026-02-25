#ifndef LOGOHEADER
#define LOGOHEADER

#include "common.h"
#include "GameCtrl.h"

class Logo : public Mode{
private:
	enum {
		FADETIME = 64,
		LOGOX = 128,
		LOGOY = 96,
		MOVESPEED = 16
	};
	float logo_pos;

public:
	Logo(){};
	void Main();
};

#endif