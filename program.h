#ifndef _LASTY_LD24_PROGRAM_H_
#define _LASTY_LD24_PROGRAM_H_
#line 4 "program.h"

/*  program
 * 
 *  OpenGL GLSL Shaders, and programs
 *
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time2:12:13 PM
 *      Author: Lasty
 */

#include "globals.h"
#include "opengl.h"

#include "image.h"
#include "primitives.h"

#include "glm.hpp"

#include <iostream>
#include <string>

class ShaderError : public GLError
{
public:
	std::string log;

	ShaderError(const char* what, const std::string &log, const char*file, int line)
	: GLError(what, file, line), log(log)
	{
	}

	const char* EName()
	{
		return "ShaderError";
	}

	const char* What()
	{
		return (std::string(what) + std::string("\n") + log).c_str();
	}
};


class Shader
{
	friend class Program;

	GLenum type;
	GLuint shader_id;

public:
	Shader(GLenum type);
	virtual ~Shader();
	void Generate();
	void Delete();

	void LoadSource(const std::string& src);
	void Compile();
	std::string GetLog();
};

class Program
{
	GLuint program_id;
	std::string filename = "Unknown";

	Shader vs;
	Shader fs;

	//uniforms
	GLint projection_matrix;
	GLint modelview_matrix;
	GLint texture_diffuse;

	//attributes
	GLint vertex_position;
	GLint vertex_colour;
	GLint vertex_texcoords;

	Image* imgref;

public:
	Program();
	virtual ~Program();
	void Generate();
	void Delete();
	void Use();

	void LoadSource(const std::string &filename);
	void LoadSource(std::istream &in);

	void Link();
	void Validate();
	std::string GetLog();

	void DiscoverUniforms();
	void DiscoverAttributes();

	void SetAttributes(Primitive &p);

	void SetCamera(const glm::mat4 &projection_matrix, const glm::mat4 &modelview_matrix);
	void SetProjectionMatrix(const glm::mat4 &projection_matrix);
	void SetModelViewMatrix(const glm::mat4 &modelview_matrix);

	void SetTexture(Image *img);
	void SetTexture(Image &img);
	void UseTexture(Image *img);


	void Draw(Primitive &p);


};


#endif /* _LASTY_LD24_PROGRAM_H_ */
