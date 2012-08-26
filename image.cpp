#line 2 "image.cpp"
/*  image
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time3:36:20 PM
 *      Author: Lasty
 */

#include "image.h"
#include <string>
#include <glm.hpp>

Image::Image()
{
	Generate();
}

Image::~Image()
{
	Delete();
}

void Image::Generate()
{
	glGenTextures(1, &tex_id);
	GLERR();
}

void Image::Delete()
{
	try
	{
		glDeleteTextures(1, &tex_id);
		GLERR();
	}
	catch (GLError &e)
	{
	}
}

void Image::LoadFile(const char* filename)
{
	std::string FN = std::string(DATA_DIR) + std::string(filename);
	SDL_Surface* img = IMG_Load(FN.c_str());
	ASSERT(img != nullptr, "SDL_Image could not load image");
	GLenum format, iformat;
	if (img->format->BitsPerPixel == 24)
	{
		iformat = format = GL_RGB;
		blended = false;
	}
	else if (img->format->BitsPerPixel == 32)
	{
		iformat = format = GL_RGBA;
		blended = true;
	}
	else
	{
		THROW(Exception, "PNG Format should be 24 bit RGB or 32 bit RGBA");
	}

	Use();

	glTexImage2D(GL_TEXTURE_2D, 0, iformat, img->w, img->h, 0, format, GL_UNSIGNED_BYTE, img->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(img);
}

void Image::Use()
{
	glBindTexture(GL_TEXTURE_2D, tex_id);
	GLERR();
	glEnable(GL_TEXTURE_2D);
	GLERR();
	if (blended)
	{
		glEnable(GL_BLEND);
		glBlendFunc(blendfunc1, blendfunc2);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	GLERR();
}

void Image::SetBlend(GLenum f1, GLenum f2)
{
	blended = true;
	blendfunc1 = f1;
	blendfunc2 = f2;
}

glm::mat4x2 Image::GetGridUV(int xoff, int yoff, int xtiles, int ytiles, int res)
{
	float xwidth = 1.0f / float(xtiles);
	float ywidth = 1.0f / float(ytiles);

	float u = float(xoff) / float(xtiles);
	float v = float(yoff) / float(ytiles);


	//theres some overlap with bilinear filtering, so move to center of pixels
	float pixelsize = 1.0f / float(res);
	float nudge = pixelsize;// / 2.0f;

	xwidth -= 2*nudge;
	ywidth -= 2*nudge;

	u += nudge;
	v += nudge;

	return glm::mat4x2( u, v, u + xwidth, v,  u + xwidth, v + ywidth, u, v + ywidth);
}



