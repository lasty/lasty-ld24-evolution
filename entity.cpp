#line 2 "entity.cpp"
/*  entity
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 26/08/2012 time5:54:33 PM
 *      Author: Lasty
 */

#include "entity.h"
#include "colours.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

inline void TileQuad(Primitive &prim, float zoom, int x, int y, int gridx, int gridy, int res = 512)
{
	glm::mat4x2 uv = Image::GetGridUV(x, y, gridx, gridy, res);
	glm::vec3 c1 = white;
	glm::vec3 c2 {1, 0.5, 1};

	prim.Begin(GL_QUADS);
	prim.Add( {-zoom, -zoom, 0.0, c2.r, c2.g, c2.b, uv[0][0], uv[0][1]});  //topleft
	prim.Add( {+zoom, -zoom, 0.0, c1.r, c1.g, c1.b, uv[1][0], uv[1][1]});  //topright
	prim.Add( {+zoom, +zoom, 0.0, c1.r, c1.g, c1.b, uv[2][0], uv[2][1]});  //botright
	prim.Add( {-zoom, +zoom, 0.0, c1.r, c1.g, c1.b, uv[3][0], uv[3][1]});  //botleft
	prim.End();
}

Factory::Factory()
: program1(), vb1()
, prim_player1(vb1)
, prim_player2(vb1)
, prim_gem(vb1)
{
	LOG("Factory()");

	program1.LoadSource("basic.shader");
	program2.LoadSource("colour.shader");

	image1.LoadFile("tiles.png");
	image1.SetBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program1.SetTexture(image1);

	TileQuad(prim_player1, 1.0, 0, 2, 4, 4, 512);
	TileQuad(prim_player2, 1.0, 1, 2, 4, 4, 512);
	TileQuad(prim_gem, 1.0, 3, 2, 4, 4, 512);

}

Factory::~Factory()
{

}

Factory &Factory::GetInstance()
{
	static Factory theinstance;
	return theinstance;
}

Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::SetModelMatrix()
{
	//LOGf("setmodelmatrix x= %.2f y=%.2f rot=%.2f zoom=%.2f", x, y, rot, zoom);

	glm::mat4 t = glm::translate(glm::mat4(), glm::vec3(x, y, 0.0f));
	glm::mat4 r = glm::rotate(glm::mat4(), rot, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 s = glm::scale(glm::mat4(), glm::vec3(zoom, zoom, 1.0f));
	model_matrix = t * r * s;
}

void Entity::Update(float dt)
{

}

Player::Player()
{
	zoom = 0.5f;
}

Player::~Player()
{

}
void Player::Update(float dt)
{
}

void Player::Draw(const glm::mat4 &proj, const glm::mat4 &view)
{
	Factory::GetInstance().program1.SetCamera(proj, view * model_matrix);
	Factory::GetInstance().program1.Draw(Factory::GetInstance().prim_player1);
}

Gem::Gem(float x, float y)
{
	this->x = x;
	this->y = y;
	SetModelMatrix();
}
Gem::~Gem()
{
}

void Gem::Update(float dt)
{

}

void Gem::Draw(const glm::mat4 &proj, const glm::mat4 &view)
{
	glm::vec4 col{0.4f, 0.9f, 0.5f, 0.8f};

	Factory::GetInstance().program2.SetCamera(proj, view * model_matrix);
	Factory::GetInstance().program2.SetDrawColour(col);
	Factory::GetInstance().program2.Draw(Factory::GetInstance().prim_gem);
}

