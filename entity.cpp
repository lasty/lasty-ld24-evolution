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

inline void TileQuad(Primitive &prim, float zoom, int x, int y, int gridx, int gridy, int res=512)
{
	glm::mat4x2 uv = Image::GetGridUV(x,y,gridx,gridy, res);
	glm::vec3 c1 = white;
	glm::vec3 c2 { 1, 0.5, 1 };

	prim.Begin(GL_QUADS);
	prim.Add( {-zoom, -zoom, 0.0, c2.r, c2.g, c2.b, uv[0][0], uv[0][1]});  //topleft
	prim.Add( {+zoom, -zoom, 0.0, c1.r, c1.g, c1.b, uv[1][0], uv[1][1]});  //topright
	prim.Add( {+zoom, +zoom, 0.0, c1.r, c1.g, c1.b, uv[2][0], uv[2][1]});  //botright
	prim.Add( {-zoom, +zoom, 0.0, c1.r, c1.g, c1.b, uv[3][0], uv[3][1]}); //botleft
	prim.End();
}


Factory::Factory()
: program1(), vb1(),  player_prim1(vb1)
{
	LOG("Factory()");

	program1.LoadSource("basic.shader");

	image1.LoadFile("tiles.png");
	image1.SetBlend(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	image1.SetBlend(GL_ONE, GL_ONE);
	image1.SetBlend(GL_SRC_COLOR, GL_ZERO);
	image1.SetBlend(GL_ONE_MINUS_SRC_COLOR, GL_ZERO);
	image1.SetBlend(GL_ONE_MINUS_SRC_COLOR, GL_ONE);
	image1.SetBlend(GL_ONE, GL_ZERO);
	image1.SetBlend(GL_ONE, GL_SRC_COLOR);
	image1.SetBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	program1.SetTexture(image1);
	//program1.UseTexture(&image1);

	TileQuad(player_prim1, 1.0, 0, 2, 4, 4, 512);
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
	LOGf("setmodelmatrix x= %.2f y=%.2f rot=%.2f", x, y, rot);

	glm::mat4 t = glm::translate( glm::mat4(), glm::vec3( x, y, 0.0f) );
	glm::mat4 r = glm::rotate( glm::mat4(), rot, glm::vec3(0.0f, 0.0f, 1.0f) );
	glm::mat4 s = glm::scale( glm::mat4(), glm::vec3(zoom, zoom, 1.0f));
	model_matrix =  t*r*s;
}

void Entity::Update(float dt)
{

}


Player::Player()
{


}

Player::~Player()
{

}

void Player::Draw(const glm::mat4 &proj, const glm::mat4 &view)
{
	Factory::GetInstance().program1.SetCamera(proj, view * model_matrix);
	Factory::GetInstance().program1.Draw( Factory::GetInstance().player_prim1);
}
