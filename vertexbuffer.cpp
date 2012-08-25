#line 2 "vertexbuffer.cpp"
/*  vertexbuffer
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time1:22:50 PM
 *      Author: Lasty
 */

#include "vertexbuffer.h"

#include "opengl.h"

VertexBuffer::VertexBuffer()
{
	Generate();
}

VertexBuffer::~VertexBuffer()
{
	Destroy();
}

void VertexBuffer::Generate()
{
	glGenBuffers(1, &vbuf_id);
	GLERR();
}

void VertexBuffer::Destroy()
{
	try {

	glDeleteBuffers(1, &vbuf_id);
	GLERR();
	}
	catch (GLError &e)
	{
		//pass
	}
}

void VertexBuffer::Use()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbuf_id);
}

void VertexBuffer::Update()
{
	Use();
	glBufferData(GL_ARRAY_BUFFER, stride * size(), data(), GL_STATIC_DRAW);
	GLERR();
}

