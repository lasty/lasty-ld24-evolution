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
	Image fonttexture;

public:
	Program program;

	Font() { SetupFont(); }

	virtual ~Font() { DestroyFont(); };


	Primitive * CreateGlyph(char ch, int xoff, int yoff, int xgrid = 8, int ygrid=8)
	{

		float xwidth = 1.0f / float(xgrid);
		float ywidth = 1.0f / float(ygrid);

		float u = float(xoff) / float(xgrid);
		float v = float(yoff) / float(ygrid);

		float glw = xgrid==8? 1.0f : 0.0f;

		Primitive *glyph = new Primitive(vb);

		glyph->Begin(GL_QUADS);
		glyph->Add( { -1.0f, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f, u, v } ); //topleft
		glyph->Add( { +glw, -1.0f, 0.0f,  1.0f, 1.0f, 1.0f, u+xwidth, v } ); //topright
		glyph->Add( { +glw, +1.0f, 0.0f,  1.0f, 1.0f, 1.0f, u+xwidth, v+ywidth } ); //botright
		glyph->Add( { -1.0f, +1.0f, 0.0f,  1.0f, 1.0f, 1.0f, u, v+ywidth } ); //botleft
		glyph->End();

		return glyph;

	}

	void SetupFont()
	{
		int x=0;
		GLERR();
		fonttexture.LoadFile("font.png");
		fonttexture.SetBlend(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

//		fonttexture.LoadFile("tex1.png");
		GLERR();

		program.LoadSource("basic.shader");
		program.SetTexture(fonttexture);
		GLERR();

		for(char ch = 'A'; ch <= 'Z'; ++ch)
		{
			glyphs[ch] = CreateGlyph(ch, x % 8, x / 8);
			x++;
		}


		for(char ch = '0'; ch <= '9'; ++ch)
		{
			glyphs[ch] = CreateGlyph(ch, x % 8, x / 8);
			x++;
		}

		for(char ch = 'a'; ch <= 'z'; ++ch)
		{
			glyphs[ch] = CreateGlyph(ch, x % 8, x / 8);
			x++;
		}

		x = 12;
		for(char ch : "(),.")
		{
			glyphs[ch] = CreateGlyph(ch, x, 7, 16);
			x++;

		}
	}

	void DestroyFont()
	{
		for (auto i : glyphs)
		{
			delete i.second;
		}
		glyphs.clear();
	}


	void Draw(char ch)
	{
		if (glyphs.count(ch) == 0) { if (ch != ' ') LOGf("GLYPH '%c' not in font", ch); return; }

		Primitive *g = glyphs[ch];

		program.Draw( *g );
	}

	void Draw(const glm::mat4 &proj, const glm::mat4 &mv_orig, const std::string &text)
	{
		glm::mat4 mv(mv_orig);

		program.Use();
		program.SetProjectionMatrix(proj);

		for (char ch : text)
		{
			program.SetModelViewMatrix(mv);
			Draw(ch);

			mv = glm::translate(mv, glm::vec3{1.0f, 0.0f, 0.0f} );
		}
	}

	void Draw(const glm::mat4 &proj, float x, float y, float zoom, const std::string &text)
	{
		glm::mat4 mv = glm::translate(glm::mat4(), glm::vec3(x, y, 0.0f));
		mv = glm::scale(mv, glm::vec3(zoom));
		return Draw(proj, mv, text);
	}
};

#endif /* _LASTY_LD24_FONT_H_ */
