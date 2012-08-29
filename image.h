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

#ifdef __APPLE__
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#include <glm.hpp>

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
	void SetBlend(bool onoff) { blended = onoff; }

	static glm::mat4x2 GetGridUV(int x, int y, int xtiles, int ytiles, int res);

};

#endif /* _LASTY_LD24_IMAGE_H_ */
