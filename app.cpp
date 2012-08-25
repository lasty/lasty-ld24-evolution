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
#include "primitives.h"
#include "program.h"

#include "tileboard.h"

#include <math.h>
#include <gtc/matrix_transform.hpp>

App::App()
: prim(vb)
{
	//vb.push_back( {0, 0, 0, 1, 1, 1, 0, 1});
	//vb.Update();

	prim.Begin(GL_QUADS);
	prim.Add( {-1.0, -1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0});
	prim.Add( {1.0, -1.0, 0.0, 1.0, 1.0, 1.0, 4.0, 0.0});
	prim.Add( {1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 4.0, 4.0});
	prim.Add( {-1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 4.0});
	prim.End();

	prog.LoadSource("basic.shader");
	i.LoadFile("tex1.png");
	//i.LoadFile("font.png");

	prog.SetTexture(i);

	gamemap.Create(10, 10);
}

App::~App()
{

}

void App::Update(float dt)
{
	runtime += dt;

	ambientcolour[0] = sinf(runtime);
	ambientcolour[1] = cosf(runtime);
	ambientcolour[2] = 0.8f;

	//LOGf("dt = %f", dt);

	gamemap.SetAmbient(ambientcolour);
	gamemap.Update();

}

void App::Render()
{

	glClearColor(0.3, 0.4, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);
	GLERR();

	glm::mat4 p, m;
	prog.SetCamera(p, m);
	//prog.Draw(prim);

	//orthographic projection to map to pixels on the window (top left is 0,0)
	glm::mat4 ortho = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f);

	glm::mat4 mapcam = glm::scale(glm::translate( {}, glm::vec3(camx, camy, 0.0f)), glm::vec3 {zoom});
	gamemap.Draw(ortho, mapcam);

//	font.Draw(ortho, 300, 20, 24, "SOMETHING Something Something Complete");
//	font.Draw(ortho, 300, 40, 16, "the quick brown fox jumps over the lazy dog,");
//	font.Draw(ortho, 300, 60, 10, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.");
//	font.Draw(ortho, 50, 100, 30, "12345679810, 11, 12");

	font.Draw(ortho, 220, 25, 30, "(Evolution)");

	font.Draw(ortho, 10, 470, 12, "This is some status line text.");

//	char ch = '0';
//	for (int y = 10;  ch <= '9'; y+= 30, ch++)
//	{
//		font.Draw(ortho, 20, y, 30, std::string(6, ch));
//	}

//	ch = 'r';
//	for (int y = -9;  y < 9 and ch <= 'z'; y++, ch++)
//	{
//		font.Draw(ortho, -9, y, 1, std::string(6, ch));
//	}

}

void App::OnMouseDown(int x, int y, int button)
{
	if (button == 2 or button == 3)
	{
		mouse_dragging = true;
	}
}

void App::OnMouseUp(int x, int y, int button)
{
	if (button == 2 or button == 3)
	{
		mouse_dragging = false;
	}

}

void App::OnMouseMotion(int x, int y, int dx, int dy)
{
	if (mouse_dragging)
	{
		camx += dx;
		camy += dy;
	}
}

void App::OnMouseWheel(int dx, int dy)
{
	//LOGf("Mouse wheel dx %d  dy %d", dx, dy);
	float zoomdelta = dy / -40.0f;
	zoom += zoomdelta;
}

void App::OnKeyDown(SDL_Keysym key)
{
	if (key.sym == SDLK_ESCAPE)
	{
		running = false;
	}
}

void App::OnKeyUp(SDL_Keysym key)
{


}
