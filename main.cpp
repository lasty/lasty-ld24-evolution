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

#include <time.h>

int run(int argc, char *argv[])
{
	auto InitFlags = SDL_INIT_VIDEO  |  SDL_INIT_AUDIO  | SDL_INIT_TIMER;

	if (SDL_Init(InitFlags) < 0)
	{
		THROW(Exception, "Couldn't init SDL");
	}

	//auto WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	auto WindowFlags = SDL_WINDOW_OPENGL;  // | SDL_WINDOW_RESIZABLE;

	SDL_Window *window = SDL_CreateWindow(APP_TITLE, SDL_WINDOWPOS_CENTERED, 40, APP_WIDTH, APP_HEIGHT, WindowFlags);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);

	SDL_GL_SetSwapInterval(0);  //Disable vsync

#ifdef USE_GLEW
	if (glewInit() != GLEW_OK)
	{
		THROW(Exception, "Could not initialize GLEW library");
	}
#endif

	long last_ticks = SDL_GetTicks();

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

					case SDL_MOUSEBUTTONDOWN:
						theapp.OnMouseDown(event.button.x, event.button.y, event.button.button);
					break;

					case SDL_MOUSEBUTTONUP:
						theapp.OnMouseUp(event.button.x, event.button.y, event.button.button);
					break;

					case SDL_MOUSEMOTION:
						theapp.OnMouseMotion(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
					break;

					case SDL_MOUSEWHEEL:
						theapp.OnMouseWheel(event.wheel.x, event.wheel.y);
					break;

					case SDL_KEYDOWN:
						theapp.OnKeyDown((SDL_Keysym)event.key.keysym);
					break;

					case SDL_KEYUP:
						theapp.OnKeyUp((SDL_Keysym)event.key.keysym);
					break;
				}
			}

			long this_ticks = SDL_GetTicks();
			float dt = (this_ticks - last_ticks) / 1000.0f;
			if (dt > 1.0) dt = 1.0;
			last_ticks = this_ticks;

			theapp.Update(dt);

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
	printf("[%s:%d] %s caught! : %s\n", e.file, e.line, e.EName(), e.What());

	std::ostringstream etxt;
	etxt << "[" << e.file << ":" << e.line << "]\n" << e.What();
	MessageBox(nullptr, etxt.str().c_str(), e.EName(), 0);
}
#else
void DumpException(Exception &e)
{
	printf("[%s:%d] %s caught! : %s\n", e.file, e.line, e.EName(), e.What());
}
#endif

#ifdef __WIN32__
#include <unistd.h>
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <windows.h>
#include <psapi.h>
#include <Shlwapi.h>

void ChangeDirectoryToHere(char* argv0)
{
	char path[128];

	GetModuleFileName(nullptr, path, sizeof(path));

	*strrchr(path, '\\') = '\0';  //Chop off filename part

	LOGf("My filename is %s", path);

	chdir(path);
}

#else
#include <unistd.h>

//Add Change Directory for your platform here if necessary

void ChangeDirectoryToHere(char* argv0)
{
	LOGf("Argv0 is  %s", argv0);
	char *path = strdup(argv0);
	char *pos = strrchr(path, '/');
	if (pos != nullptr)
	{
		*pos = '\0';  //Chop off filename part
		LOGf("My path is %s", path);
		chdir(path);
	}
	free(path);
}

#endif


int main(int argc, char *argv[])
{
	LOG("Hello LD world!");
	LOGf("Hi %s ", "there");
	LOGf("Argv[0] is %s ", argv[0]);

	srand(time(nullptr));

	int ret = 0;
	try
	{
		ChangeDirectoryToHere(argv[0]);

		ret = run(argc, argv);

	}
	catch (Exception &e)
	{
		DumpException(e);
		return 1;
	}
	return ret;
}

