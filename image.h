#ifndef _LASTY_LD24_IMAGE_H_
#define _LASTY_LD24_IMAGE_H_
#line 4 "image.h"

/*  image
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time3:36:20 PM
 *      Author: Lasty
 */

#include "opengl.h"

#include <SDL2/SDL_image.h>

class Image
{
	GLuint tex_id = 0;
	bool blended = false;

	GLenum blendfunc1 = GL_SRC_ALPHA;
	GLenum blendfunc2 = GL_ONE_MINUS_SRC_ALPHA;


public:
	Image();
	virtual ~Image();

	void Generate();

	void Delete();

	void LoadFile(const char* filename);


	void Use();


	void SetBlend(GLenum f1, GLenum f2);

};

#endif /* _LASTY_LD24_IMAGE_H_ */
