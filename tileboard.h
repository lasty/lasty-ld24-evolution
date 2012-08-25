#ifndef _LASTY_LD24_TILEBOARD_H_
#define _LASTY_LD24_TILEBOARD_H_
#line 4 "tileboard.h"

/*  tileboard
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 26/08/2012 time6:15:18 AM
 *      Author: Lasty
 */

#include "opengl.h"
#include "primitives.h"
#include "program.h"
#include "image.h"

class Tile : public Primitive
{
	glm::vec3 ambient_colour;

public:

	Tile(VertexBuffer* vb, int x, int y);

	void SetAmbient(const glm::vec3 &col);
	void UpdateAmbient();

	void UpdateColour(const glm::vec3 &col);

};

class TileBoard
{
public:
	VertexBuffer vb;
	Program program;
	Image tiletexture;

	int mapsizex = 0;
	int mapsizey = 0;

	std::vector<std::vector<Tile*>> map;

	TileBoard();

	virtual ~TileBoard();

	void Create(int width, int height);

	void Destroy();

	void Draw(const glm::mat4 &camera, const glm::mat4 &mv);

	void SetAmbient(glm::vec3 col);
	void Update();

	Tile * FindNearest(glm::vec2 cursor);

};


#endif /* _LASTY_LD24_TILEBOARD_H_ */
