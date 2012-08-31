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
#include <algorithm>

#include <math.h>
#include <gtc/matrix_transform.hpp>

#include "colours.h"

#include "entity.h"

#include "audio.h"

App::App()
: running(true)
, startup("startup.wav")
, noise("noise.wav")
, explosion("explosion.wav")
, laser("laser.wav")
, hit("hit.wav")
, pickup_coin("pickup_coin.wav")
, pickup_gem("gem.wav")
, prim(vb)
{
	//vb.push_back( {0, 0, 0, 1, 1, 1, 0, 1});
	//vb.Update();
	float sbh = 60;
	glm::vec3 c = gui_background;
	prim.Begin(GL_QUADS);
	prim.Add( {0.0, APP_HEIGHT - sbh, 0.0, c.r, c.g, c.b, 0.0, 0.0});
	prim.Add( {APP_WIDTH, APP_HEIGHT - sbh, 0.0, c.r, c.g, c.b, 16.0, 0.0});
	prim.Add( {APP_WIDTH, APP_HEIGHT, 0.0, c.r, c.g, c.b, 16.0, 1.0});
	prim.Add( {0.0, APP_HEIGHT, 0.0, c.r, c.g, c.b, 0.0, 1.0});
	prim.End();

	prog.LoadSource("basic.shader");
	i.LoadFile("tex1.png");
	i.SetBlend(GL_ONE_MINUS_SRC_COLOR, GL_SRC_COLOR);
	//i.LoadFile("font.png");
	prog.SetTexture(i);



	gamemap.Create(MAP_WIDTH, MAP_HEIGHT);

	ChangeLight(0);

	//Factory::GetInstance().program1.SetTexture(&gamemap.tiletexture);


	GenMap();

	audio.PlaySound(startup);
}


void App::GenMap()
{
	ClearEntities();
	dlights.clear();

	gamemap.GenerateTerrain();

	/*
	//add in some dynamic lights for effect
	for (int i = 1; i < 100; ++i)
	{
		float lx = RandFloat(1.0f, gamemap.mapsizex);
		float ly = RandFloat(1.0f, gamemap.mapsizey);

		int li = RandInt(0, lights_array_count-1);
		int type = RandInt(0, 2);

		float r = RandFloat(2.0f, 10.0f);

		AddDlight(glm::vec2{lx, ly}, lights_array[li], r, type);
	}
	*/

	const int NUM_GEMS = 100;
	const int NUM_COINS = 100;
	const int NUM_ROCKS = 1000;

	for (int i = 1; i < NUM_ROCKS; ++i)
	{
		int gx = RandInt(0, gamemap.mapsizex);
		int gy = RandInt(0, gamemap.mapsizey);

		entities.push_back(new Rock(gx, gy));
	}

	for (int i = 1; i < NUM_COINS; ++i)
	{
		int gx = RandInt(0, gamemap.mapsizex);
		int gy = RandInt(0, gamemap.mapsizey);

		entities.push_back(new Coin(gx, gy));
	}

	//add in some gems
	for (int i = 1; i < NUM_GEMS; ++i)
	{
		int gx = RandInt(0, gamemap.mapsizex);
		int gy = RandInt(0, gamemap.mapsizey);

		entities.push_back(new Gem(gx, gy));
	}


	if (player == nullptr)
	{
		player = new Player();
		player->x = 5.0f;
		player->y = 5.0f;
		//player->zoom = 1.0f;
		player->SetModelMatrix();
	}
	else
	{
		Player *p2 = new Player();
		p2->x = player->x;
		p2->y = player->y;
		p2->rot = player->rot;
		p2->SetModelMatrix();

		delete player;
		player = p2;
	}

	//entities.push_back(player);
	gamemap.ClearArea(player->x, player->y);


	//Gem *g = new Gem(player->x + 5,player->y);
	//entities.push_back( g );

}

void App::ClearEntities()
{
	for (Entity *i : entities)
	{
		delete i;
	}
	entities.clear();
}

App::~App()
{
	delete player;
	ClearEntities();  //Clean up vector of pointers
}

void App::Update(float dt)
{
	runtime += dt;

		zoom = (9*zoom + target_zoom) / 10.0f;

		if (not mouse_dragging)
		{
		target_camx = player->x;
		target_camy = player->y;
		}

		camx = (9*camx + target_camx) / 10.0f;
		camy = (9*camy + target_camy) / 10.0f;


	//Yep Im not sure what I did
	//Too tired to work it out :)

	//ortho -> 2d graphics, origin is top left of screen
	//cam -> centered on player, origin is center of screen


	//orthographic projection to map to pixels on the window (top left is 0,0)
	ortho = glm::ortho(0.0f, float(APP_WIDTH), float(APP_HEIGHT), 0.0f);

	//orthographic projection centered on the screen
	//cam = glm::translate( ortho, glm::vec3(float(APP_WIDTH)/2.0, float(APP_HEIGHT)/2.0f, 0.0f));
	glm::mat4 adj  = glm::translate( {} , glm::vec3(float(APP_WIDTH)/2.0, float(APP_HEIGHT)/2.0f, 0.0f));
	cam = ortho * adj;


	glm::mat4 s = glm::scale( glm::mat4(), glm::vec3 {zoom, zoom, 1.0f});
	glm::mat4 t = glm::translate(glm::mat4(), glm::vec3(-camx, -camy, 0.0f));

	//mapcam = glm::scale(glm::translate(glm::mat4(), glm::vec3(-camx, -camy, 0.0f)), glm::vec3 {zoom});
	mapcam = s * t;

	invmapcam =  mapcam._inverse() ;  //can't seem to find the proper access/function for this
	//invmapcam = (cam * mapcam)._inverse(); //s * t;


	//glm::mat4 is = glm::scale( {}, glm::vec3 {1.0/zoom});
	//glm::mat4 it = glm::translate({}, glm::vec3(camx*zoom, camy*zoom, 0.0f));

	//invmapcam = is* it;

	//invmapcam = glm::inverse(ortho * mapcam);// * adj;

	//invmapcam = glm::translate({}, glm::vec3(-float(APP_WIDTH)/2.0, -float(APP_HEIGHT)/2.0f, 0.0f)) * glm::inverse(cam);

	UpdatePlayer(dt);


	gamemap.ResetDynamicLights();  //Clear last frame's dynamic lights


	player->Update(dt);

	for (auto i = entities.begin();  i != entities.end(); )
	{
		Entity *e = *i;

		e->Update(dt);

		if (e->done)
		{
			delete e;
			i = entities.erase(i);
			continue;
		}

		DLight *l = e->GetLight();
		if (l and LightIsOnScreen(e->x, e->y, l->radius))
		{
			gamemap.DynamicLight(glm::vec2(e->x, e->y), l->colour, l->radius);
		}

		i++;
	}

	UpdateWorld();
}


bool App::LightIsOnScreen(float x, float y, float radius)
{
	const float left = (x - radius);
	const float right = (x + radius);

	if ((left > botright.x) or (right < topleft.x))
	{
		return false;
	}

	const float top = (y - radius);
	const float bot = (y + radius);

	if ((top > botright.y) or (bot < topleft.y))
	{
		return false;
	}

	return true;
}


void App::UpdateWorld()
{
	//Doing some rudimentary bounds checking in the case of huge maps
	topleft = ScreenToWorld(0, 0);
	botright = ScreenToWorld(APP_WIDTH, APP_HEIGHT);

	ambientcolour[0] = sinf(runtime);
	ambientcolour[1] = cosf(runtime);
	ambientcolour[2] = 0.8f;

	//Tile *ht = gamemap.FindNearest(hover);
	//if (ht) { ht->SetAmbient(ambientcolour * 2.0f); }


#ifdef debug_lights
	gamemap.DynamicLight(hover, current_light_colour, current_light_radius);
#else
	gamemap.DynamicLight(hover, light_torch, 4);
#endif

	gamemap.DynamicLight(glm::vec2(player->x, player->y), light_player, 9);

	cull_count = 0;

	for (auto &l : dlights)
	{
		//Cull lights we don't see on screen
		if (not LightIsOnScreen(l.position.x, l.position.y, l.radius))
		{
			cull_count++;
			continue;
		}

		switch (l.type)
		{
			case 0:  //solid
			default:
				gamemap.DynamicLight(l.position, l.colour, l.radius);
			break;

			case 1:  //flicker
				float brightflicker = RandFloat(-0.1, 0.1);
				//float radiusflicker = RandFloat(0.0, 0.2);
				gamemap.DynamicLight(l.position,
				                     l.colour+brightflicker,
				                     l.radius//+radiusflicker
				                     );
			break;
		}
	}

	gamemap.Update();
}

bool intersects(Entity *e1, Entity *e2)
{
	float radii = e1->radius + e2->radius;
	float distance = glm::distance( glm::vec2(e1->x, e1->y), glm::vec2(e2->x, e2->y));

	return (radii > distance);
}


void App::UpdatePlayer(float dt)
{
	const int VALUE_COIN = 1;
	const int VALUE_GEM = 10;

	float dx = 0.0f;
	float dy = 0.0f;

	if (moving_left) dx -= 1.0f;
	if (moving_right) dx += 1.0f;
	if (moving_up) dy -= 1.0f;
	if (moving_down) dy += 1.0f;

	//hacky but
	float origx = player->x;
	float origy = player->y;

	if (dx or dy) PlayerMove(dx, dy, dt);

	for (Entity *e : entities)
	{
		if (intersects(player, e))
		{
			if (Gem *gem = dynamic_cast<Gem*>(e))
			{
				gem->done = true;
				player->score+=VALUE_GEM;
				audio.PlaySound(pickup_gem);
			}
			else if (Coin *coin = dynamic_cast<Coin*>(e))
			{
				coin->done = true;
				player->score+=VALUE_COIN;
				audio.PlaySound(pickup_coin);
			}
			else if (Rock *rock = dynamic_cast<Rock*>(e))
			{
				float dist = 1;
				//bump the entity
				MoveEntity(rock, dx*dist, dy*dist);

				//undo the move
				player->x = origx;
				player->y = origy;
			}
		}
	}
}


//does this point intersect the world?
bool App::CollidePoint(float x, float y)
{
	Tile *t = gamemap.FindNearestTile(glm::vec2(x, y));

	if (t == nullptr) return true;  //clip on edge of world

	return t->block_movement;
}

//does this point (with radius) intersect the world?
bool App::CollidePoint(float x, float y, float radius)
{
	bool p;
	p = CollidePoint(x+(radius * 0.0f), y+(radius * 0.0f));
	if (p) return true;

	p = CollidePoint(x+(radius * 1.0f), y+(radius * 0.0f));
	if (p) return true;
	p = CollidePoint(x+(radius * -1.0f), y+(radius * 0.0f));
	if (p) return true;

	p = CollidePoint(x+(radius * 0.0f), y+(radius * 1.0f));
	if (p) return true;
	p = CollidePoint(x+(radius * 0.0f), y+(radius * -1.0f));
	if (p) return true;


	return false;
}

//does this line intersect the world?
bool App::CollidePath(float x1, float y1, float x2, float y2, float radius)
{
	//divide the test line into smaller chunks TODO

	bool t1 = CollidePoint(x1, y1, radius);
	if (t1) return true;

	float xmid = (x1 + x2) / 2.0f;
	float ymid = (y1 + y2) / 2.0f;

	bool tmid = CollidePoint(xmid, ymid, radius);
	if (tmid) return true;

	bool t2 = CollidePoint(x2, y2, radius);
	if (t2) return true;

	//else, free path
	return false;
}

//move an entitiy while checking for clipping
bool App::MoveEntity(Entity *e, float dx, float dy)
{
	bool canmove = not CollidePath(e->x, e->y, e->x+dx, e->y+dy, e->radius);
	if (NoClipping) canmove = true;
	if (canmove)
	{
		e->x = e->x+dx;
		e->y = e->y+dy;

		e->SetModelMatrix();
	}
	return canmove;
}

void App::PlayerMove(float dx, float dy, float dt)
{
	float runmod = moving_running ? 3.0f : 1.0f;
	float dist = (runmod * player_move_speed) * dt;

	if (not MoveEntity(player, dx*dist, dy*dist))
	{
		MoveEntity(player, dx*dist, 0.0f);
		MoveEntity(player, 0.0f, dy*dist);
	}

	//player->x += dx * dist;
	//player->y += dy * dist;

	//smooth change the rotation (not perfect, needs wrap around)
	float a1 = player->rot;
	float a2;
	if (dx>0)
	{
		a2 = 90;
		if (dy != 0) a2 += 45*dy;
	}
	else if (dx < 0)
	{
		a2 = 270;
		if (dy != 0) a2 -= 45*dy;
	}
	else if (dy > 0) a2 = 180;
	else a2 = 0;

	float a = (a1*9 + a2) / 10.0f;

	player->rot = a;


	player->SetModelMatrix();
}

void App::AddDlight(glm::vec2 pos, glm::vec3 color, float radius, int type)
{
	DLight d (pos, color, radius, type);
	dlights.push_back(d);
}

void App::ChangeLight(int inc)
{
	current_light_index = (current_light_index + inc) % lights_array_count;
	CurrentLightName = lights_array_names[current_light_index];
	current_light_colour = lights_array[current_light_index];
}

void App::RenderWorld()
{

	gamemap.Draw(cam, mapcam, topleft.x - 2, botright.x + 2, topleft.y - 2, botright.y + 2);

}

void App::Render()
{

	glClearColor(0.3, 0.4, 0.5, 1.0);

	glClear(GL_COLOR_BUFFER_BIT);
	GLERR();

	//glm::mat4 p, m;
	//prog.SetCamera(ortho, m);
	//prog.Draw(prim);

	RenderWorld();

	for (Entity *e : entities)
	{
		LightPoint *t = gamemap.FindNearestLightPoint(glm::vec2(e->x, e->y));
		glm::vec3 col = t ? t->GetCol() : glm::vec3(1.0, 1.0, 1.0);
		e->Draw(cam, mapcam, col);
	}

	LightPoint *t = gamemap.FindNearestLightPoint(glm::vec2(player->x, player->y));
	glm::vec3 col = t ? t->GetCol() : glm::vec3(1.0, 1.0, 1.0);
	player->Draw(cam, mapcam, col);

	RenderGUI();

}

void App::RenderGUI()
{

	font.Draw(ortho, APP_WIDTH / 2 - 100, 25, 30, "(Evolution)");

	//render the bottom panel to make text easier to read
	prog.SetCamera(ortho, {});
	prog.Draw(prim);

	std::stringstream cursorpos;
	cursorpos << std::setprecision(1) << std::fixed;  //this is a lot of code to replace %.2f
	cursorpos << "(" << hover.x << "," << hover.y << ")  Dynamic Lights " << dlights.size() << " (Culled " << cull_count
				<< " offscreen lights)  Entities " << entities.size();


	std::stringstream statusline;
	statusline << "Score " << player->score << "    ";

	font.Draw(ortho, 10, APP_HEIGHT - 12, 12, statusline.str() + cursorpos.str());

	std::stringstream status_light;
	status_light << "(Current light Colour) " << CurrentLightName << ", Radius " << current_light_radius;

	font.Draw(ortho, 10, APP_HEIGHT - 42, 12, status_light.str());

}

void App::DropTorch()
{
	AddDlight(hover, light_torch, 4);
	audio.PlaySound(noise);

}

void App::OnMouseDown(int x, int y, int button)
{
	if (button == 1 or button == 3)
	{
		mouse_dragging = true;
	}
	if (button == 3)
	{
#ifdef debug_lights
		mouse_dragging_lights = true;
		AddDlight(hover, current_light_colour, current_light_radius);
#else
		DropTorch();
#endif

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
		//glm::vec2 tmp = ScreenToWorld(dy, dx);

		target_camx += dx * 4 / zoom;
		target_camy += dy * 4/ zoom;
	}

	if (mouse_dragging_lights)
	{
		AddDlight(hover, current_light_colour, current_light_radius);
	}

}

void App::OnMouseWheel(int dx, int dy)
{
	//LOGf("Mouse wheel dx %d  dy %d", dx, dy);
	float zoomdelta = dy / -40.0f;
	target_zoom += zoomdelta;
}

void App::OnKeyDown(SDL_Keysym key)
{
	if (key.sym == SDLK_ESCAPE)
	{
		running = false;
	}

	if (key.sym == SDLK_KP_PLUS) ChangeLight(+1);
	if (key.sym == SDLK_KP_MINUS) ChangeLight(-1);
	if (key.sym == SDLK_MINUS and current_light_radius > 0) current_light_radius -= 1.0f;
	if (key.sym == SDLK_EQUALS) current_light_radius += 1.0f;

	if (key.sym == SDLK_LEFT or key.sym == SDLK_a) moving_left = true;
	if (key.sym == SDLK_RIGHT or key.sym == SDLK_d) moving_right = true;
	if (key.sym == SDLK_UP or key.sym == SDLK_w) moving_up = true;
	if (key.sym == SDLK_DOWN or key.sym == SDLK_s) moving_down = true;

	if (key.sym == SDLK_LSHIFT or key.sym == SDLK_RSHIFT) moving_running = true;

	if (key.sym == SDLK_c) NoClipping = not NoClipping;

	if (key.sym == SDLK_n) GenMap();

}

void App::OnKeyUp(SDL_Keysym key)
{
	if (key.sym == SDLK_LEFT or key.sym == SDLK_a) moving_left = false;
	if (key.sym == SDLK_RIGHT or key.sym == SDLK_d) moving_right = false;
	if (key.sym == SDLK_UP or key.sym == SDLK_w) moving_up = false;
	if (key.sym == SDLK_DOWN or key.sym == SDLK_s) moving_down = false;

	if (key.sym == SDLK_LSHIFT or key.sym == SDLK_RSHIFT) moving_running = false;
}

glm::vec2 App::ScreenToWorld(int x, int y)
{
	//must convert to xyzw vector first.. argh
	glm::vec4 tmp(float(x) - (APP_WIDTH/2.0f), float(y) - (APP_HEIGHT/2.0f), 0.0f, 1.0f);

	tmp = tmp * invmapcam;


	return glm::vec2(tmp);
}

void App::SetHover(int x, int y)
{

	hover = ScreenToWorld(x, y);

	//LOGf("Hovering at world coords %.2f x %.2f", hover.x, hover.y);
}

