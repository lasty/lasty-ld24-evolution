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


class Circle : public Primitive
{
	//VertexBuffer vb;
public:
	Circle(VertexBuffer &vbref, int segments=8);
};


//For holding primitives which we can instance
class Factory
{
public:
	Program program1;
	ProgramColour program2;
	ProgramColour program3;

	VertexBuffer vb1;

	Image image1;


	Primitive prim_player1;
	Primitive prim_player2;
	Primitive prim_gem;
	Primitive prim_coin;
	Primitive prim_rock;
	Primitive prim_bullet;

	Circle prim_circle;

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

	virtual void Draw(const glm::mat4 &proj, const glm::mat4 &view, const glm::vec3 &backgroundcol);

	virtual DLight* GetLight() { return nullptr; }


	void DrawRadius(const glm::mat4 &proj, const glm::mat4 &view, const glm::vec4 &col);
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

class Coin : public Entity
{
public:
	Coin(float x, float y);
	virtual ~Coin();

	void Update(float dt);
	void Draw(const glm::mat4 &proj, const glm::mat4 &view,  const glm::vec3 &backgroundcol);

	DLight* GetLight();
};


class Rock : public Entity
{
public:
	int hitpoints = 10;

	Rock(float x, float y);
	virtual ~Rock();

	void Update(float dt);
	void Draw(const glm::mat4 &proj, const glm::mat4 &view,  const glm::vec3 &backgroundcol);
};


class Bullet : public Entity
{
public:
	float velocityx;
	float velocityy;
	int damage = 5;

	Bullet(float x, float y, float vx, float vy);
	virtual ~Bullet();

	void Update(float dt);
	void Draw(const glm::mat4 &proj, const glm::mat4 &view,  const glm::vec3 &backgroundcol);

	DLight* GetLight();

};



#endif /* _LASTY_LD24_ENTITY_H_ */
