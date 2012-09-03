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

//#ifdef WIN32
//#define DATA_DIR "..\\data\\"
//#else
#define DATA_DIR "../data/"
//#endif //WIN32

#define APP_TITLE "Evolution (LD24)"

//no time to implement res changes
#ifdef boring
#define APP_WIDTH 640
#define APP_HEIGHT 480
#elif better
#define APP_WIDTH 800
#define APP_HEIGHT 600
#else //best! widescreen
#define APP_WIDTH 1280
#define APP_HEIGHT 700

#endif

constexpr int MAP_WIDTH = 160;  //in tiles
constexpr int MAP_HEIGHT = 90;

extern bool Draw_Collision_Radius;

#include <stdio.h>
#include <stdlib.h>


inline int RandInt(int start, int end)
{
	const int range = end - start;

	int r = rand() % range;

	return r + start;
}

inline float RandFloat(float start, float end)
{
	const float range = end - start;

	float r = (float(rand()) / float(RAND_MAX) )* range;

	return r + start;
}

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

	virtual const char* EName()
	{
		return "Exception";
	}
};

class Assert : public Exception
{
public:
	Assert(const char* what, const char* file, int line)
	: Exception(what, file, line)
	{
	}

	virtual const char* EName()
	{
		return "Assert";
	}
};

//make throw include file/line
#define THROW(exclass, what)throw(exclass(what, __FILE__, __LINE__));
#define ASSERT(test,what) if (not (test)) THROW(Assert,what)

//logging to stdout, disabled in optimized build
#ifndef NDEBUG
#define LOG(s){ printf("[%s:%d] %s\n",__FILE__,__LINE__,s); fflush(stdout); }
#define LOGf(fmt,...){ printf("[%s:%d] " fmt "\n",__FILE__,__LINE__,__VA_ARGS__); fflush(stdout); }
#else
#define LOG(s){}
#define LOGf(fmt,...){}
#endif

#endif /* _LASTY_LD24_GLOBALS_H_ */
