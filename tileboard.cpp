#line 2 "tileboard.cpp"
/*  tileboard
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 26/08/2012 time6:15:18 AM
 *      Author: Lasty
 */

#include "tileboard.h"

#include "math.h"

#include "globals.h"

Tile::Tile(VertexBuffer* vb, int x, int y)
: Primitive(vb)
{
	const float size = 1.0;
	const float half = (size / 2.0) + 0.01;

	const float x1 = (x * size) - half;
	const float x2 = ((x+1) * size) - half;
	const float y1 = (y * size) - half;
	const float y2 = ((y+1) * size) - half;


	Begin(GL_TRIANGLE_FAN);
	Add( {x1, y1, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0});  //topleft
	Add( {x2, y1, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0});  //topright
	Add( {x2, y2, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0});  //botright
	Add( {x1, y2, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0});  //botleft
	//End();
}

void Tile::SetUV(const TileDef &t)
{
	for (unsigned i = 0; i < 4; ++i)
	{
		Vertex &v = vbuff->at(begin + i);
		v.u = t.uv[i][0];
		v.v = t.uv[i][1];
	}

	block_light = t.blocks_light;
	block_movement = t.block_movement;
	//dirty = true;
}

Vertex * Tile::GetV(int which) const
{
	return &(vbuff->at(begin + which - 1));
}

//////////////////////////////////////////////////////////////////////////////

LightPoint::LightPoint()
:dirty(false), colour(0.5), ambient_colour(0.2)
{}

LightPoint::~LightPoint()
{ }

void LightPoint::AddVertex(Vertex *v)
{
	vertex_refs.push_back(v);
}

void LightPoint::Clear()
{
	vertex_refs.clear();  //we don't own the pointers so dont delete
}


void LightPoint::ResetDynamic()
{
	colour = ambient_colour;
	dirty = true;
}

void LightPoint::SetAmbient(const glm::vec3 &col)
{
	ambient_colour = col;
	dirty = true;
}

void LightPoint::SetDynamic(const glm::vec3 &col)
{
	colour += col;
	dirty = true;
}

void LightPoint::Update()
{
	for (Vertex *v : vertex_refs)
	{
		v->r = colour.r;
		v->g = colour.g;
		v->b = colour.b;
	}
	dirty = false;
}


//////////////////////////////////////////////////////////////////////////////


TileBoard::TileBoard()
: vb(GL_DYNAMIC_DRAW)
{
	program.LoadSource("basic.shader");
	//tiletexture.LoadFile("tex1.png");
	tiletexture.LoadFile("tiles.png");
	tiletexture.SetBlend(false);
	program.SetTexture(tiletexture);

}

TileBoard::~TileBoard()
{
	Destroy();
}

void TileBoard::Create(int width, int height)
{
	Destroy();
	//width = 5; height = 5;
	mapsizex = width;
	mapsizey = height;

	for (int y = 0; y < height; y++)
	{
		std::vector<Tile*> row;
		for (int x = 0; x < width; x++)
		{
			row.push_back(new Tile(&vb, x, y));
		}
		map.push_back(row);
	}


	for (int y=0; y <= height; y++)  //light map is 1 more in width/height
	{
		std::vector<LightPoint> row;

		for (int x=0; x <= width; x++)
		{
			//LOGf("\n-------( %d , %d )----", x, y);
			LightPoint lp;
			float x2 = x - 0.5;  //adjust to the point to be in world coordinates
			float y2 = y - 0.5;

			Tile * tl = FindNearestTile( glm::vec2(x2-0.5, y2-0.5) );
			if (tl) lp.AddVertex(tl->GetV(3));
			//if (tl) LOG("top left");

			Tile * tr = FindNearestTile( glm::vec2(x2+0.5, y2-0.5) );
			if (tr) lp.AddVertex(tr->GetV(4));
			//if (tr) LOG("top right");

			Tile * br = FindNearestTile( glm::vec2(x2+0.5, y2+0.5) );
			if (br) lp.AddVertex(br->GetV(1));
			//if (br) LOG("bottom right");

			Tile * bl = FindNearestTile( glm::vec2(x2-0.5, y2+0.5) );
			if (bl) lp.AddVertex(bl->GetV(2));
			//if (bl) LOG("bottom left");

			//LOGf("x,y = %d, %d  -- size is %d", x, y, lp.vertex_refs.size());
			ASSERT (lp.vertex_refs.size() <=4,  "Too many vertexes");
			ASSERT (lp.vertex_refs.size() >0,  "Too few vertexes");
			row.push_back(lp);
		}
		lightmap.push_back(row);
	}

	//ASSERT(false, "break");
}

void TileBoard::Destroy()
{
	for (auto &row : map)
	{
		for (auto &col : row)
		{
			delete (col);
		}
	}
	map.clear();
	lightmap.clear();
}

void TileBoard::Draw(const glm::mat4 &camera, const glm::mat4 &mv)
{
	Draw(camera, mv, 0, mapsizex - 1, 0, mapsizey - 1);
}

void TileBoard::Draw(const glm::mat4 &camera, const glm::mat4 &mv, int x1, int x2, int y1, int y2)
{
	x1 = glm::max(0, x1);
	x2 = glm::min(mapsizex - 1, x2);
	y1 = glm::max(0, y1);
	y2 = glm::min(mapsizey - 1, y2);

	program.SetCamera(camera, mv);
	Tile * t = map[0][0];

	program.WarmUp(*t);

	for (int y = y1; y <= y2; ++y)
	{
		auto const &row = map[y];

		for (int x = x1; x <= x2; ++x)
		{
			auto const &cell = row[x];
			cell->Draw();
		}
	}
}

void TileBoard::ResetDynamicLights()
{
	for (auto &row : lightmap)
	{
		for (auto &cell : row)
		{
			cell.ResetDynamic();
		}
	}
}

void TileBoard::Update()
{
	for (auto &row : lightmap)
	{
		for (auto &cell : row)
		{
			if (cell.dirty)
			{
				cell.Update();
			}
		}
	}

	vb.Update();
}

Tile * TileBoard::FindNearestTile(const glm::vec2 &cursor)
{
	int x = roundf(cursor.x);
	int y = roundf(cursor.y);

	if (x < 0 or x >= mapsizex) return nullptr;
	if (y < 0 or y >= mapsizey) return nullptr;

	//LOGf("Finding map tile %d, %d", x, y);

	Tile *t = map[y][x];

	return t;
}


LightPoint * TileBoard::FindNearestLightPoint(const glm::vec2 &cursor)
{
	int x = roundf(cursor.x-0.5);
	int y = roundf(cursor.y-0.5);

	if (x < 0 or x >= mapsizex+1) return nullptr;
	if (y < 0 or y >= mapsizey+1) return nullptr;

	//LOGf("Finding map tile %d, %d", x, y);

	LightPoint *lp = &(lightmap[y][x]);

	return lp;
}


void TileBoard::DynamicLight(const glm::vec2 &position, const glm::vec3 colour, float radius)
{
	int irad = roundf(radius + 1);

	int xrange1 = glm::max(0, int(position.x - irad));
	int xrange2 = glm::min(mapsizex, int(position.x + irad));

	int yrange1 = glm::max(0, int(position.y - irad));
	int yrange2 = glm::min(mapsizey, int(position.y + irad));

	int cx = roundf(position.x);  //center tile
	int cy = roundf(position.y);

	for (int ix = xrange1; ix < xrange2; ++ix)
	{
		float x = ix;

		for (int iy = yrange1; iy < yrange2; ++iy)
		{
			float y = iy;
			glm::vec2 current(x, y);

			LightPoint &lp = lightmap[iy][ix];

			float distance = glm::distance(position, glm::vec2(x, y));

			if (distance > radius) continue;

//			int blocking = CheckBlockPath(x, y, cx, cy);
			int blocking = CheckBlockPath(cx, cy, x, y);

			if (blocking > 4) continue;

			float brightness = 1.0f - (distance / radius);  //brightness scale from 1 -> 0

			if (blocking) brightness /= blocking;

			glm::vec3 falloffcol = colour * brightness;

			lp.SetDynamic(falloffcol);
		}
	}

}


void TileBoard::GenerateTerrain()
{
	for (auto &y : map)
	{
		for (auto &i : y)
		{
			int r = RandInt(0, 6);
			switch (r)
			{
				case 0:
				case 1:
				case 2:
					i->SetUV(tile_floor1);
				break;

				case 3:
				case 4:
					i->SetUV(tile_floor2);
				break;

				case 5:
					i->SetUV(tile_wall1);
				break;

			}
		}
	}

	for (auto &y: lightmap)
	{
		for (auto &i : y)
		{
			float c = RandFloat(0.05f, 0.2f);
			i.SetAmbient(glm::vec3{c});
		}
	}

}

void TileBoard::ClearArea(float px, float py, int radius)
{
	int x = roundf(px);
	int y = roundf(py);

	for(int ix = x - radius; ix < x+radius; ++ix)
	{
		for (int iy = y-radius; iy < y+radius; ++iy)
		{
			if (ix < 0 or ix >= mapsizex) continue;
			if (iy < 0 or iy >= mapsizey) continue;

			auto i = map[iy][ix];

			int r = RandInt(0, 2);
			switch (r)
			{
				case 0:
					i->SetUV(tile_floor1);
				break;

				case 1:
					i->SetUV(tile_floor2);
				break;
			}
		}
	}

}


int TileBoard::CheckBlockPath(int x1, int y1, int x2, int y2)
{
	//returns the sign of a number
	#define sgn(x) ((x<0)?-1:((x>0)?1:0))

	int dx = x2 - x1;
	int dy = y2 - y1;

	int dxabs = abs(dx);
	int dyabs = abs(dy);

	int sdx = sgn(dx);
	int sdy = sgn(dy);

	int count_blocked = 0;

	if (dxabs >= dyabs)  //line is more horizontal
	{
		float slope = float(dy) / float(dx);
		for (int i = 0; i != dx; i += sdx)
		{
			int px = i + x1;
			int py = slope * i + y1;

			if (IsBlocking(px, py))
			{
				if (++count_blocked > 5) return 5;
			}
		}
	}
	else
	{
		float slope = float(dx) / float(dy);
		for (int i = 0; i != dy; i += sdy)
		{
			int py = i + y1;
			int px = slope * i + x1;

			if (IsBlocking(px, py))
			{
				if (++count_blocked > 5) return 5;
			}
		}
	}


	return count_blocked;
//	return RandInt(0, 10) == 0;
}



