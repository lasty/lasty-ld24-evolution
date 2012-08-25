#ifndef _LASTY_LD24_GLOBALS_H_
#define _LASTY_LD24_GLOBALS_H_
#line 4 "globals.h"

/*  globals
 * 
 *  Contains random assortment of classes, and #define switches
 *
 *
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time10:59:59 AM
 *      Author: Lasty
 */

//////////////////////////////////////////////////////////////////////////////

//use GLEW library for opengl headers; see opengl.h
#define USE_GLEW

//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

class Exception
{
public:
	const char* what;
	const char* file;
	int line;

	Exception(const char* what, const char* file, int line)
	: what(what), file(file), line(line)
	{
	}

	virtual ~Exception()
	{
	}

	virtual const char* What()
	{
		return what;
	}

	virtual const char* EName() { return "Exception"; }
};

//make throw include file/line
#define THROW(exclass, what)throw(exclass(what, __FILE__, __LINE__));

//logging to stdout, disabled in optimized build
#ifndef NDEBUG
#define LOG(s){ printf("[%s:%d] %s\n",__FILE__,__LINE__,s); fflush(stdout); }
#define LOGf(fmt,...){ printf("[%s:%d] " fmt "\n",__FILE__,__LINE__,__VA_ARGS__); fflush(stdout); }
#else
#define LOG(s){}
#define LOGf(fmt,...){}
#endif

#endif /* _LASTY_LD24_GLOBALS_H_ */
