#ifndef _LASTY_LD24_APP_H_
#define _LASTY_LD24_APP_H_
#line 4 "app.h"

/*  app
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time12:03:23 PM
 *      Author: Lasty
 */

class App
{
public:
	App();
	virtual ~App();
	bool running = true;

	void Update(float dt);
	void Render();

};

#endif /* _LASTY_LD24_APP_H_ */
