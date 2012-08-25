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

#include "GL/glew.h"

#else

//Otherwise you could use your sysetm's opengl headers
#include <GL/gl.h>

#endif


#endif /* _LASTY_LD24_OPENGL_H_ */
