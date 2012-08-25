#ifndef _LASTY_LD24_FONT_H_
#define _LASTY_LD24_FONT_H_
#line 4 "font.h"

/*  font
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time5:46:01 PM
 *      Author: Lasty
 */

#include "opengl.h"
#include "primitives.h"
#include "program.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <map>


class Font
{
	VertexBuffer vb;
	std::map<char, Primitive*> glyphs;
	std::map<char, float> advancewidth;
	Image fonttexture;

public:
	Program program;

	Font();
	virtual ~Font();

	Primitive* CreateGlyph(char ch, int xoff, int yoff, int xgrid = 8, int ygrid = 8);

	void SetupFont();
	void DestroyFont();

	void Draw(char ch);
	void Draw(const glm::mat4& proj, const glm::mat4& mv_orig, const std::string& text);
	void Draw(const glm::mat4& proj, float x, float y, float zoom, const std::string& text);
};

#endif /* _LASTY_LD24_FONT_H_ */
