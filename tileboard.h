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
//	glm::vec3 avg_colour;

public:

//	glm::vec3 GetCol() const { return avg_colour; }

	bool block_light = false;
	bool block_movement = false;

	Tile(VertexBuffer* vb, int x, int y);

//	void SetAverageColour(const glm::vec3 &col) { avg_colour = col; }

	void SetUV(const TileDef &t);

	Vertex * GetV(int which) const;
};

class LightPoint
{
protected:
	friend class TileBoard;

	bool dirty = true;

	glm::vec3 colour;
	glm::vec3 ambient_colour;


	std::vector<Vertex *> vertex_refs;

public:
	LightPoint();
	~LightPoint();

	void AddVertex(Vertex *v);
	void Clear();

	void ResetDynamic();
	void SetAmbient(const glm::vec3 &col);
	void SetDynamic(const glm::vec3 &col);

	void Update();

	const glm::vec3 &GetCol() const { return colour; }
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

	std::vector<std::vector<LightPoint>> lightmap;

	TileBoard();

	virtual ~TileBoard();

	void Create(int width, int height);

	void Destroy();

	void Draw(const glm::mat4 &camera, const glm::mat4 &mv);
	void Draw(const glm::mat4 &camera, const glm::mat4 &mv, int x1, int x2, int y1, int y2);

	void ResetDynamicLights();  //Call this to reset to static background before applying dynamic lights
	void Update();

	Tile * FindNearestTile(const glm::vec2 &cursor);  //find nearest tile from world coordinates cursor
	LightPoint * FindNearestLightPoint(const glm::vec2 &cursor);
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
