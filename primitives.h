#ifndef _LASTY_LD24_PRIMITIVES_H_
#define _LASTY_LD24_PRIMITIVES_H_
#line 4 "primitives.h"

/*  primitives
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time1:51:37 PM
 *      Author: Lasty
 */

#include "opengl.h"
#include "vertexbuffer.h"

class Primitive
{
protected:
	friend class Program;

	VertexBuffer *vbuff;  //reference to vertex buffer

	unsigned begin;  //offset to begin from
	unsigned count;  //number of elements to draw (vertexes)

	GLenum prim_type; //opengl draw type
public:
	Primitive(VertexBuffer *vbref);
	Primitive(VertexBuffer &vbref);

	virtual ~Primitive();

	void Begin(GLenum primitive_type);
	void Add(const Vertex &v);
	void End();

	void Use() const;
	void Draw() const;
};

#endif /* _LASTY_LD24_PRIMITIVES_H_ */
