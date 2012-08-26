#ifndef _LASTY_LD24_APP_H_
#define _LASTY_LD24_APP_H_
#line 4 "app.h"

#include "vertexbuffer.h"
#include "primitives.h"
#include "program.h"
#include "font.h"

#include "tileboard.h"

#include <SDL2/SDL.h>

/*  app
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time12:03:23 PM
 *      Author: Lasty
 */

class App
{
public:
	App();
	virtual ~App();
	bool running = true;

	void Update(float dt);
	void Render();

	VertexBuffer vb;
	Primitive prim;
	Program prog;

	Image i;

	Font font;

	TileBoard gamemap;

	float runtime = 0.0f;
	glm::vec3 ambientcolour;




	bool mouse_dragging = false;

	void OnMouseDown(int x, int y, int button);
	void OnMouseUp(int x, int y, int button);
	void OnMouseWheel(int dx, int dy);
	void OnMouseMotion(int x, int y, int dx, int dy);

	void OnKeyDown(SDL_Keysym key);
	void OnKeyUp(SDL_Keysym key);



	float zoom = 30.0f;
	float camx = 100.0f;
	float camy = 100.0f;
	glm::mat4 mapcam;
	glm::mat4 invmapcam;


	glm::vec2 hover;  //mouse position in world coords
	void SetHover(int x, int y);
	glm::vec2 ScreenToWorld(int x, int y);

};

#endif /* _LASTY_LD24_APP_H_ */
