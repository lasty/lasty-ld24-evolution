#ifndef _LASTY_LD24_ENTITY_H_
#define _LASTY_LD24_ENTITY_H_
#line 4 "entity.h"

/*  entity
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 26/08/2012 time5:54:33 PM
 *      Author: Lasty
 */

#include "vertexbuffer.h"
#include "primitives.h"
#include "image.h"
#include "program.h"

#include <glm.hpp>

//For holding primitives which we can instance
class Factory
{
public:
	Program program1;

	VertexBuffer vb1;

	Image image1;


	Primitive player_prim1;

	Factory();
	~Factory();

	static Factory &GetInstance();
};


class Entity
{
public:
	Entity();
	virtual ~Entity();

	glm::mat4 model_matrix;

	float x = 0.0f;
	float y = 0.0f;
	float rot = 0.0f;  //degrees not radians
	float zoom = 1.0f;

	virtual void SetModelMatrix();

	virtual void Update(float dt);

	virtual void Draw(const glm::mat4 &proj, const glm::mat4 &view){ THROW(Exception,"Shouldnt be here"); }

};


class Player : public Entity
{
public:
	Player();
	virtual ~Player();
	float t;
	void Update(float dt)
	{
		//t += dt;
		//rot += 50 * dt;
		//zoom = sinf(t) + 2.0f;
	//SetModelMatrix();
	}

	void Draw(const glm::mat4 &proj, const glm::mat4 &view);
};

#endif /* _LASTY_LD24_ENTITY_H_ */
