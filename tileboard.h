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


struct TileDef
{
	glm::mat4x2 uv;
	bool blocks_light;
	bool block_movement;

	TileDef(int x, int y, int xtiles, int ytiles, bool blockslight)
	:uv(Image::GetGridUV(x,y,xtiles,ytiles, 512)), blocks_light(blockslight), block_movement(blockslight)
	{}
};

static TileDef tile_floor1(0,0,4,4, false);
static TileDef tile_floor2(1,0,4,4, false);
static TileDef tile_wall1(2,0,4,4, true);

class Tile : public Primitive
{
protected:
	friend class TileBoard;

	glm::vec3 colour;
	glm::vec3 ambient_colour;

	bool dirty = true;


public:
	bool block_light = false;
	bool block_movement = false;

	Tile(VertexBuffer* vb, int x, int y);

	void SetDynamic(const glm::vec3 &col);
	void SetAmbient(const glm::vec3 &col);
	void ReSetAmbient();

	void Update();
	void UpdateColour(const glm::vec3 &col);


	void SetUV(const TileDef &t);

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
	void Draw(const glm::mat4 &camera, const glm::mat4 &mv, int x1, int x2, int y1, int y2);

	void ReSetAmbient();  //Call this to reset to static background before applying dynamic lights
	void Update();

	Tile * FindNearest(const glm::vec2 &cursor);  //find nearest tile from world coordinates cursor
	inline bool IsBlocking(int px, int py);

	int CheckBlockPath(int x1, int y1, int x2, int y2);  //find any blocked tiles in a line

	void DynamicLight(const glm::vec2 &position, const glm::vec3 colour, float radius);

	void GenerateTerrain();

	void ClearArea(float x, float y, int radius = 3);
};

bool TileBoard::IsBlocking(int px, int py)
{
	if (px < 0 or px >= mapsizex or py < 0 or py >= mapsizey)
	{
		return true;
	}
	else
	{
		Tile * t = map[py][px];
		return t->block_light;
	}
}

#endif /* _LASTY_LD24_TILEBOARD_H_ */
