#ifndef _LASTY_LD24_VERTEXBUFFER_H_
#define _LASTY_LD24_VERTEXBUFFER_H_
#line 4 "vertexbuffer.h"

/*  VertexBuffer
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time1:22:50 PM
 *      Author: Lasty
 */

#include "opengl.h"

#include <vector>

struct Vertex
{
	float x,y,z;
	float r,g,b;
	float u,v;
};


class VertexBuffer : public std::vector<Vertex>
{
public:
	GLuint vbuf_id = 0;

	VertexBuffer();
	virtual ~VertexBuffer();

	void Generate();
	void Destroy();

	void Use();
	void Update();

	constexpr static size_t stride = sizeof(Vertex);
};


#endif /* _LASTY_LD24_VERTEXBUFFER_H_ */
