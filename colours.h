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


static std::vector<std::pair<std::string, glm::vec3>>
lights_array{
	{ "White", light_white },
	{ "OffWhite", light_offwhite},
	{ "Yellow", light_yellow},
	{ "Red", light_red}
};


#endif /* _LASTY_LD24_COLOURS_H_ */
