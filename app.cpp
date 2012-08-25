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

App::App()
{
	// TODO Auto-generated constructor stub

}

App::~App()
{
	// TODO Auto-generated destructor stub
}

void App::Update() {}

void App::Render() {

	glClearColor(0.3, 0.4, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);

}
