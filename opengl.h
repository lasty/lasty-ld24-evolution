#ifndef _LASTY_LD24_OPENGL_H_
#define _LASTY_LD24_OPENGL_H_
#line 4 "opengl.h"

/*  opengl
 * 
 *  Header for getting OpenGL interface
 *
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time11:59:35 AM
 *      Author: Lasty
 */

#include "globals.h"

#ifdef USE_GLEW

#ifdef __APPLE__
#include <glew.h>
#else
#include <GL/glew.h>
#endif

#else

//Otherwise you could use your sysetm's opengl headers
#include <GL/gl.h>

#endif

class GLError : public Exception
{
public:
	GLError(const char*what, const char*file, int line)
	: Exception(what, file, line) { }

	const char* EName() { return "GLError"; }
};

inline void GLCheckError(const char* file, int line)
{
	auto e = glGetError();
	if (e != GL_NO_ERROR)
	{
		throw GLError((const char*)gluErrorString(e), file, line);
	}
}

#define GLERR()GLCheckError(__FILE__,__LINE__)


#endif /* _LASTY_LD24_OPENGL_H_ */
