#ifndef _LASTY_LD24_APP_H_
#define _LASTY_LD24_APP_H_
#line 4 "app.h"

#include "vertexbuffer.h"
#include "primitives.h"
#include "program.h"
#include "font.h"

#include "tileboard.h"

#include "entity.h"

#include "audio.h"

#include <SDL2/SDL.h>

/*  app
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time12:03:23 PM
 *      Author: Lasty
 */

class DLight
{
public:
	glm::vec2 position;
	glm::vec3 colour;
	float radius;

	bool done = false;

	int type = 0;

	DLight(glm::vec2 position, glm::vec3 colour, float radius, int type = 0)
	: position(position), colour(colour), radius(radius), done(false), type(type)
	{ }
};

class App
{
public:
	App();
	virtual ~App();
	bool running = true;

	void Update(float dt);
	void Render();

	void RenderGUI();
	void RenderWorld();
	void UpdateWorld();

	Wave startup;
	Wave noise;
	Wave explosion;
	Wave laser;
	Wave hit;
	Wave pickup_coin;
	Wave pickup_gem;


	Audio audio;


	glm::mat4 ortho;

	VertexBuffer vb;
	Primitive prim;
	Program prog;

	Image i;

	Font font;

	TileBoard gamemap;
	void GenMap();

	void ClearEntities();
	std::vector<Entity*> entities;
	Player *player = nullptr;


	float runtime = 0.0f;
	glm::vec3 ambientcolour;

	std::vector<DLight> dlights;
	bool LightIsOnScreen(float x, float y, float radius);
	void AddDlight(glm::vec2 pos, glm::vec3 color, float radius, int type=0);
	void ChangeLight(int inc);
	glm::vec3 current_light_colour;
	float current_light_radius = 5.0f;
	int current_light_index = 0;
	std::string CurrentLightName = "TBA";
	int cull_count = 0;

	bool mouse_dragging = false;
	bool mouse_dragging_lights = false;

	void OnMouseDown(int x, int y, int button);
	void OnMouseUp(int x, int y, int button);
	void OnMouseWheel(int dx, int dy);
	void OnMouseMotion(int x, int y, int dx, int dy);

	void OnKeyDown(SDL_Keysym key);
	void OnKeyUp(SDL_Keysym key);

	void DropTorch();

	void UpdatePlayer(float dt);
	float player_move_speed = 4.0f;  //world units per second
	void PlayerMove(float dx, float dy, float dt);

	bool moving_left = false;
	bool moving_right = false;
	bool moving_up = false;
	bool moving_down = false;
	bool moving_running = false;

	bool NoClipping = false;

	float zoom = 30.0f;
	float target_zoom = 70.0f;
	float camx = 100.0f;
	float target_camx = 100.0f;
	float camy = 100.0f;
	float target_camy = 100.0f;

	glm::mat4 cam;
	glm::mat4 mapcam;
	glm::mat4 invmapcam;

	glm::vec2 topleft;  //topleft worldcoords on screen
	glm::vec2 botright; //bottom right worldcoords on screen

	glm::vec2 hover;  //mouse position in world coords
	void SetHover(int x, int y);
	glm::vec2 ScreenToWorld(int x, int y);


	bool MoveEntity(Entity *e, float dx, float dy);

	bool CollidePoint(float x, float y);
	bool CollidePoint(float x, float y, float radius);
	bool CollidePath(float x1, float y1, float x2, float y2, float radius);


};

#endif /* _LASTY_LD24_APP_H_ */
