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

#include <sstream>
#include <iomanip>

#include <math.h>
#include <gtc/matrix_transform.hpp>

#include "colours.h"

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

	gamemap.Create(160, 90);

	ChangeLight(0);
}

App::~App()
{

}



void App::Update(float dt)
{
	runtime += dt;

	ortho = glm::ortho(0.0f, float(APP_WIDTH), float(APP_HEIGHT), 0.0f);

	mapcam = glm::scale(glm::translate( glm::mat4(), glm::vec3(camx, camy, 0.0f)), glm::vec3 {zoom});
	invmapcam = mapcam._inverse();  //can't seem to find the proper access/function for this

	UpdateWorld();
}

void App::UpdateWorld()
{
	ambientcolour[0] = sinf(runtime);
	ambientcolour[1] = cosf(runtime);
	ambientcolour[2] = 0.8f;

	//LOGf("dt = %f", dt);

	gamemap.SetAmbient(ambientcolour);

	Tile *ht = gamemap.FindNearest(hover);
	if (ht) { ht->SetAmbient(ambientcolour * 2.0f); }

	gamemap.DynamicLight(hover, current_light_colour, current_light_radius);

	for (auto &l : dlights)
	{
		gamemap.DynamicLight(l.position, l.colour, l.radius);
	}

	gamemap.Update();
}



void App::AddDlight(glm::vec2 pos)
{
	DLight d{pos, current_light_colour, current_light_radius};
	dlights.push_back(d);
}

void App::ChangeLight(int inc)
{
	current_light_index = (current_light_index + inc) % lights_array.size();
	CurrentLightName = lights_array[current_light_index].first;
	current_light_colour = lights_array[current_light_index].second;
}

void App::RenderWorld()
{

	//Doing some rudimentary bounds checking in the case of huge maps
	glm::vec2 topleft = ScreenToWorld(0,0);
	glm::vec2 botright = ScreenToWorld(APP_WIDTH, APP_HEIGHT);

	gamemap.Draw(ortho, mapcam, topleft.x-2, botright.x+2, topleft.y-2, botright.y+2);
//	gamemap.Draw(ortho, mapcam);

}

void App::Render()
{

	glClearColor(0.3, 0.4, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);
	GLERR();

//	glm::mat4 p, m;
//	prog.SetCamera(p, m);
	//prog.Draw(prim);

	//orthographic projection to map to pixels on the window (top left is 0,0)

	RenderWorld();


//	font.Draw(ortho, 300, 20, 24, "SOMETHING Something Something Complete");
//	font.Draw(ortho, 300, 40, 16, "the quick brown fox jumps over the lazy dog,");
//	font.Draw(ortho, 300, 60, 10, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.");
//	font.Draw(ortho, 50, 100, 30, "12345679810, 11, 12");


	font.Draw(ortho, APP_WIDTH / 2 - 100, 25, 30, "(Evolution)");

	std::stringstream cursorpos;
	cursorpos << std::setprecision(1) << std::fixed;  //this is a lot of code to replace %.2f
	cursorpos << "("<<hover.x<<","<<hover.y<<")  Dynamic Lights " << dlights.size();

	font.Draw(ortho, 10, APP_HEIGHT - 12 , 12, "This is some status line text.  42  "+cursorpos.str());

	std::stringstream status_light;
	status_light << "(Current light Colour) " << CurrentLightName  <<  "Current radius " << current_light_radius;

	font.Draw(ortho, 10, APP_HEIGHT - 42 , 12, status_light.str());


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

//	//debugging hover
//	glm::mat4 hovercam = glm::translate(mapcam, glm::vec3(hover, 0.0f));
//	prog.SetCamera(ortho, hovercam);
//	prog.Draw(prim);

}

void App::OnMouseDown(int x, int y, int button)
{
	if (button == 2 or button == 3)
	{
		mouse_dragging = true;
	}
	if (button == 1)
	{
		mouse_dragging_lights = true;

		AddDlight(hover);
	}
}

void App::OnMouseUp(int x, int y, int button)
{
//	if (button == 2 or button == 3)
//	{
	mouse_dragging = false;
	mouse_dragging_lights = false;
//	}

}

void App::OnMouseMotion(int x, int y, int dx, int dy)
{
	SetHover(x, y);

	if (mouse_dragging)
	{
		camx += dx;
		camy += dy;
	}

	if (mouse_dragging_lights)
	{
		AddDlight(hover);
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

	if (key.sym == SDLK_KP_PLUS) ChangeLight(+1);
	if (key.sym == SDLK_KP_MINUS) ChangeLight(-1);
	if (key.sym == SDLK_MINUS and current_light_radius > 0 ) current_light_radius-= 1.0f;
	if (key.sym == SDLK_EQUALS) current_light_radius += 1.0f;


}

void App::OnKeyUp(SDL_Keysym key)
{


}

glm::vec2 App::ScreenToWorld(int x, int y)
{
	//must convert to xyzw vector first.. argh
	glm::vec4 tmp(float(x), float(y), 0.0, 1.0);
	tmp = tmp * invmapcam;

	return glm::vec2(tmp);
}

void App::SetHover(int x, int y)
{

	hover = ScreenToWorld(x,y);

	//LOGf("Hovering at world coords %.2f x %.2f", hover.x, hover.y);
}

