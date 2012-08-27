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
	ProgramColour program2;

	VertexBuffer vb1;

	Image image1;


	Primitive prim_player1;
	Primitive prim_player2;
	Primitive prim_gem;

	Factory();
	~Factory();

	static Factory &GetInstance();
};

class DLight;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	bool done = false;

	glm::mat4 model_matrix;

	float x = 0.0f;
	float y = 0.0f;
	float rot = 0.0f;  //degrees not radians
	float zoom = 0.5f;
	float radius = 0.45f;

	virtual void SetModelMatrix();

	virtual void Update(float dt);

	virtual void Draw(const glm::mat4 &proj, const glm::mat4 &view, const glm::vec3 &backgroundcol){ THROW(Exception,"Shouldnt be here"); }

	virtual DLight* GetLight() { return nullptr; }
};


class Player : public Entity
{
public:
	int skin = 0;

	int score = 0;

	Player();
	virtual ~Player();
	void Update(float dt);
	void Draw(const glm::mat4 &proj, const glm::mat4 &view,  const glm::vec3 &backgroundcol);

	DLight* GetLight();

};


class Gem : public Entity
{
public:
	Gem(float x, float y);
	virtual ~Gem();

	void Update(float dt);
	void Draw(const glm::mat4 &proj, const glm::mat4 &view,  const glm::vec3 &backgroundcol);

	DLight* GetLight();
};

#endif /* _LASTY_LD24_ENTITY_H_ */
