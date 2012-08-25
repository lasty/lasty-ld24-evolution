#line 2 "main.cpp"
/*  main
 * 
 *  Created for Ludum Dare #24
 *
 *  Created on: 25/08/2012 time11:00:14 AM
 *      Author: Lasty
 */
#include "globals.h"
#include "app.h"

#include <SDL2/SDL.h>

#include "opengl.h"

int run(int argc, char *argv[])
{

	auto InitFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER;

	if (SDL_Init(InitFlags) < 0)
	{
		THROW(Exception, "Couldn't init SDL");
	}

	auto WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	SDL_Window *window = SDL_CreateWindow("test", 10, 10, 640, 480, WindowFlags);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	SDL_GL_SetSwapInterval(0);  //Disable vsync


#ifdef USE_GLEW
	if (glewInit() != GLEW_OK)
	{
		THROW(Exception, "Could not initialize GLEW library");
	}
#endif


	{
		App theapp;

		while (theapp.running)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{

					case SDL_QUIT:
						theapp.running = false;
					break;
				}

			}

			theapp.Update();

			theapp.Render();

			SDL_GL_SwapWindow(window);
		}

	}

	SDL_DestroyWindow(window);

	SDL_Quit();
	LOG("Quitting main");
	return 0;
}

#ifdef WIN32
#include <sstream>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
void DumpException(Exception &e)
{
	std::ostringstream etxt;
	etxt << e.EName() <<" caught! : ["<<e.file<<":"<<e.line<<"]";
	MessageBox(nullptr,  e.What(),etxt.str().c_str(), 0);
}
#else
void DumpException(Exception &e)
{
	printf("[%s:%d] %s caught! : %s\n", e.file, e.line, e.EName(), e.What());
}
#endif

int main(int argc, char *argv[])
{
	LOG("Hello LD world!");
	LOGf("Hi %s ", "there");

	int ret = 0;
	try
	{
		ret = run(argc, argv);

	}
	catch (Exception &e)
	{
		DumpException(e);
		return 1;
	}
	return ret;
}

