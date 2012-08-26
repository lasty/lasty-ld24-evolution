#line 2 "font.cpp"
/*  font
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time5:46:01 PM
 *      Author: Lasty
 */

#include "font.h"

Font::Font()
{
	SetupFont();
}

Font::~Font()
{
	DestroyFont();
}

Primitive* Font::CreateGlyph(char ch, int xoff, int yoff, int xgrid, int ygrid)
{
	float xwidth = 1.0f / float(xgrid);
	float ywidth = 1.0f / float(ygrid);

	float u = float(xoff) / float(xgrid);
	float v = float(yoff) / float(ygrid);

	float glw = xgrid == 8 ? 1.0f : 0.5f;

	Primitive* glyph = new Primitive(vb);

	glyph->Begin(GL_QUADS);
	glyph->Add( {-glw, -1.0f, 0.0f, 1.1f, 1.1f, 1.1f, u, v});  //topleft
	glyph->Add( {+glw, -1.0f, 0.0f, 1.1f, 1.1f, 1.1f, u + xwidth, v});  //topright
	glyph->Add( {+glw, +1.0f, 0.0f, 0.8f, 0.8f, 0.9f, u + xwidth, v + ywidth});  //botright
	glyph->Add( {-glw, +1.0f, 0.0f, 0.8f, 0.8f, 0.9f, u, v + ywidth});  //botleft
	glyph->End();

	//adjust advance width  (still needs work but its close enough)
	float aw = 0.8f;
	//if (std::string(",.").find(ch) != std::string::npos) aw = 0.5f;
	if (std::string("abh").find(ch) != std::string::npos) aw = 0.8f;
	if (std::string("IJZE1()").find(ch) != std::string::npos) aw = 0.7f;
	if (std::string("tpfr").find(ch) != std::string::npos) aw = 0.6f;
	if (std::string("lji").find(ch) != std::string::npos) aw = 0.6f;
	if (std::string(",.WMwm").find(ch) != std::string::npos) aw = 1.0f;

	advancewidth[ch] = aw;
	return glyph;
}

void Font::SetupFont()
{
	int x = 0;
	GLERR();
	fonttexture.LoadFile("font.png");
	//Texture is white on black, set blend to use colour not alpha
	fonttexture.SetBlend(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	GLERR();

	program.LoadSource("basic.shader");
	program.SetTexture(fonttexture);

	GLERR();

	for (char ch = 'A'; ch <= 'Z'; ++ch)
	{
		glyphs[ch] = CreateGlyph(ch, x % 8, x / 8);
		x++;
	}
	for (char ch = '0'; ch <= '9'; ++ch)
	{
		glyphs[ch] = CreateGlyph(ch, x % 8, x / 8);
		x++;
	}
	for (char ch = 'a'; ch <= 'z'; ++ch)
	{
		glyphs[ch] = CreateGlyph(ch, x % 8, x / 8);
		x++;
	}
	x = 12;
	for (char ch : "(),.")
	{
		glyphs[ch] = CreateGlyph(ch, x, 7, 16);
		x++;
	}
}

void Font::DestroyFont()
{
	for (auto i : glyphs)
	{
		delete i.second;
	}

	glyphs.clear();
}

void Font::Draw(char ch)
{
	if (glyphs.count(ch) == 0)
	{
		if (ch != ' ' and ch != '-') LOGf("GLYPH '%c' not in font", ch);
		return;
	}

	Primitive* g = glyphs[ch];

	program.Draw(*g);
}

void Font::Draw(const glm::mat4& proj, float x, float y, float zoom, const std::string& text)
{
	glm::mat4 mv = glm::translate(glm::mat4(), glm::vec3(x, y, 0.0f));
	mv = glm::scale(mv, glm::vec3(zoom));

	return Draw(proj, mv, text);
}

void Font::Draw(const glm::mat4& proj, const glm::mat4& mv_orig, const std::string& text)
{
	glm::mat4 mv(mv_orig);

	program.Use();
	program.SetProjectionMatrix(proj);

	for (char ch : text)
	{
		program.SetModelViewMatrix(mv);
		Draw(ch);
		float aw = advancewidth[ch];
		if (aw == 0.0f) aw = 0.8f;

		mv = glm::translate(mv, glm::vec3 {aw, 0.0f, 0.0f});
	}
}
