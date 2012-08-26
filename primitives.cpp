#line 2 "primitives.cpp"
/*  primitives
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time1:51:37 PM
 *      Author: Lasty
 */

#include "primitives.h"
#include "opengl.h"
#include "vertexbuffer.h"

Primitive::Primitive(VertexBuffer *vbref)
: vbuff(vbref), begin(0), count(0), prim_type(0)
{

}

Primitive::Primitive(VertexBuffer &vbref)
: Primitive(&vbref)
{
}

Primitive::~Primitive()
{
	// TODO Auto-generated destructor stub
}

void Primitive::Begin(GLenum primitive_type)
{
	prim_type = primitive_type;
	begin = vbuff->size();
}

void Primitive::Add(const Vertex &v)
{
	vbuff->push_back(v);
	count++;
}

void Primitive::End()
{
	vbuff->Update();  //Copy vertex buffer to graphics memory
}

void Primitive::Use() const
{
	vbuff->Use();
}

void Primitive::Draw() const
{
	glDrawArrays(prim_type, begin, count);
}

