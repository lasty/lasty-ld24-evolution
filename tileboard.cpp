#line 2 "tileboard.cpp"
/*  tileboard
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 26/08/2012 time6:15:18 AM
 *      Author: Lasty
 */

#include "tileboard.h"

Tile::Tile(VertexBuffer* vb, int x, int y)
: Primitive(vb)
{
	const float size = 1.0f;
	const float half = size / 2.0f;
	const float xoff = x * size;
	const float yoff = y * size;

	Begin(GL_QUADS);
	Add( {xoff - half, yoff - half, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0});  //topleft
	Add( {xoff + half, yoff - half, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0});  //topright
	Add( {xoff + half, yoff + half, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0});  //botright
	Add( {xoff - half, yoff + half, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0});  //botleft
	End();
}

void Tile::SetAmbient(const glm::vec3 &col)
{
	ambient_colour = colour = col;
}

void Tile::SetDynamic(const glm::vec3 &col)
{
	colour += col;
}

void Tile::Update()
{
	UpdateColour(colour);
}

void Tile::UpdateColour(const glm::vec3 &col)
{
	for (unsigned i = 0; i < count; ++i)
	{
		Vertex &v = vbuff->at(begin + i);
		v.r = col[0];
		v.g = col[1];
		v.b = col[2];
	}
}

//////////////////////////////////////////////////////////////////////////////


TileBoard::TileBoard()
{
	program.LoadSource("basic.shader");
	tiletexture.LoadFile("tex1.png");
	program.SetTexture(tiletexture);

}


TileBoard::~TileBoard()
{
	Destroy();
}


void TileBoard::Create(int width, int height)
{
	Destroy();
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
}

void TileBoard::Destroy()
{
	for (auto row : map)
	{
		for (auto col : row)
		{
			delete(col);
		}
	}
	map.clear();
}


void TileBoard::Draw(const glm::mat4 &camera, const glm::mat4 &mv)
{
	program.SetCamera(camera, mv);

	int count = 0;
	for (auto row: map)
	{
		for (auto cell: row)
		{
			count++;
			program.Draw(*cell);
		}
	}
}


void TileBoard::SetAmbient(glm::vec3 col)
{
	for (auto row : map)
	{
		for (auto cell : row)
		{
			cell->SetAmbient(col);
		}
	}
}


void TileBoard::Update()
{
	for (auto row : map)
	{
		for (auto cell : row)
		{
			cell->Update();
		}
	}

	vb.Update();
}


Tile * TileBoard::FindNearest(const glm::vec2 &cursor)
{
	int x = roundf(cursor.x);
	int y = roundf(cursor.y);

	if (x < 0 or x >= mapsizex) return nullptr;
	if (y < 0 or y >= mapsizey) return nullptr;

	//LOGf("Finding map tile %d, %d", x, y);

	Tile *t = map[y][x];

	return t;
}

void TileBoard::DynamicLight(const glm::vec2 &position, const glm::vec3 colour, float radius)
{
	float maxb = 0.0f, minb = 1.0f;

	for (int ix = 0;  ix < mapsizex;  ++ix)
	{
		float x = ix;

		for (int iy = 0; iy < mapsizey; ++iy)
		{
			float y = iy;
			glm::vec2 current(x,y);

			Tile *t = map[y][x];

			float distance = glm::distance(position, glm::vec2(x,y));

			if (distance > radius) continue;

			float brightness = 1.0f - (distance / radius);  //brightness scale from 1 -> 0

			glm::vec3 falloffcol = colour * brightness;

			t->SetDynamic(falloffcol);
			t->Update();

			maxb = glm::max(maxb, brightness);
			minb = glm::min(minb, brightness);

		}
	}

	LOGf("min brigtness %.2f   max brightness %.2f", minb, maxb);
}

