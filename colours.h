#ifndef _LASTY_LD24_COLOURS_H_
#define _LASTY_LD24_COLOURS_H_
#line 4 "colours.h"

/*  colours
 * 
 *  Some colour definitions
 *
 *  Created for Ludum Dare #24
 *
 *  Created on: 26/08/2012 time12:31:11 PM
 *      Author: Lasty
 */

#include <glm.hpp>
#include <map>
#include <vector>
#include <string>

static glm::vec3 white(1.0f, 1.0f, 1.0f);

static glm::vec3 gui_background(0.4f, 0.8f, 0.9f);


static glm::vec3 light_white(5.0f, 5.0f, 5.0f);
static glm::vec3 light_offwhite(4.9f, 4.8f, 5.0f);
static glm::vec3 light_yellow(5.0f, 4.0f, 0.5f);
static glm::vec3 light_red(4.9f, 0.5f, 0.5f);
static glm::vec3 light_blue(1.9f, 1.5f, 4.9f);
static glm::vec3 light_cyan(1.5f, 4.9f, 4.9f);

static glm::vec3 light_torch(2.0f, 2.0f, 1.5f);

static glm::vec3 light_player(2.0f, 2.0f, 2.0f);


static glm::vec3 light_gem(0.7, 1.0, 0.7);
static glm::vec3 light_coin(0.9, 0.9, 0.1);

static glm::vec4 tint_player1(1.0f, 1.0f, 1.0f, 1.0f);
static glm::vec4 tint_player2(1.0f, 1.0f, 1.0f, 1.0f);

static glm::vec4 tint_gem(0.4f, 0.9f, 0.5f, 1.0f);
static glm::vec4 tint_coin(1.0f, 1.0f, 1.0f, 1.0f);
static glm::vec4 tint_rock(86/256.0f, 67/256.0f, 32/256.0f, 1.0f);


static std::string
lights_array_names[]=
{
	 "White",
	 "OffWhite",
	 "Yellow",
	 "Red",

	 "Blue",
	 "Cyan",

	 "Torchlight",
	 "PlayerLight"
};


static glm::vec3
lights_array[]=
{
	 light_white,
	 light_offwhite,
	 light_yellow,
	 light_red,

	 light_blue,
	 light_cyan,

	 light_torch,
	 light_player
};

constexpr size_t lights_array_count = 8;

#endif /* _LASTY_LD24_COLOURS_H_ */
