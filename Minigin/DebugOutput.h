#ifndef DEBUGOUTPUT_H
#define DEBUGOUTPUT_H

#include "Defines.h"

#include <iostream>

namespace Pengin
{
	//Allows file logging in future if necessary
	#ifdef DEBUG_MODE
		#define DEBUG_OUT(msg) std::cout << "[DEBUG]: " << msg << std::endl
	#else
		#define DEBUG_OUT(msg)
	#endif
}

#endif