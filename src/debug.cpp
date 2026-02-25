#include "debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

// ‘®•t‚«‚Ì‹Lq‚É‘Î‰‚µ‚½ MBox
/*
void MBox(const char* format, ...){
	char buffer[256];
	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	va_end(ap);

	if(MessageBox(GetForegroundWindow(),
		("%s",buffer),"",MB_OKCANCEL) == IDCANCEL)
		PostQuitMessage(0);
}
*/