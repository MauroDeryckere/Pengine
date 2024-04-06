#ifndef DEBUGOUTPUT
#define DEBUGOUTPUT

#include "Defines.h"

#include <iostream>

namespace Pengin
{
	//Allows file logging in future if necessary
	#ifdef DEBUG_MODE
		#define DEBUG_OUT(msg) std::wcout << "L[DEBUG]: " << msg << std::endl
	#else
		#define DEBUG_OUT(msg)
	#endif
}

#endif