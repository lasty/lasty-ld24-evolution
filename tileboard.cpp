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
	ambient_colour = col;
}

void Tile::UpdateAmbient()
{
	UpdateColour(ambient_colour);
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
			cell->UpdateAmbient();
		}
	}

	vb.Update();
}

