#line 2 "app.cpp"
/*  app
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time12:03:23 PM
 *      Author: Lasty
 */

#include "app.h"
#include "opengl.h"
#include "vertexbuffer.h"

App::App()
{
	// TODO Auto-generated constructor stub

}

App::~App()
{
	// TODO Auto-generated destructor stub
}

void App::Update(float dt)
{

	//LOGf("dt = %f", dt);
}

void App::Render()
{

	glClearColor(0.3, 0.4, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);
	GLERR();

	VertexBuffer vb;
	vb.push_back( {0, 0, 0, 1, 1, 1, 0, 1});
	vb.Update();
}
