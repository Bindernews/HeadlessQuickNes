
#define SDL_MAIN_HANDLED
#include <cstdio>
#include <csetjmp>
#include <SDL.h>
#include <SDL_ttf.h>
#include "gl_include.h"
#include "hqn.h"
#include "hqn_lua.h"
#include "options.h"
#include "hqn_util.h"
#include "hqn_gui_controller.h"

#ifndef CONFIG_FILENAME
#define CONFIG_FILENAME "hqnes.cfg"
#endif

#define DEFAULT_FILENAME "hqn"

// Import json so we can read the config file.
// using json = nlohmann::json;

namespace hqn
{

// Global setjmp state
jmp_buf endItAllBuffer;

// Print the usage message.
void printUsage(const char *filename)
{
    const char *fname = filename ? filename : DEFAULT_FILENAME;
    printf("usage: %s <romfile> <lua_script>\n", fname);
}


int hqn_main(int argc, char **argv)
{
    // Delcare variables
    blargg_err_t err;
    Options opts;
    HQNState *hstate = nullptr;
    GUIController *guiController = nullptr;
    lua_State *lstate = nullptr;
	bool useGui = false;
    int sdlInitFlags;
    int exitval = 0;
    
    // We take two arguments, the rom file and a lua script to run.
    if (argc < 3)
    {
        printUsage(argv[0]);
        return 1;
    }

    if (hqn_util::file_exists(CONFIG_FILENAME))
    {
        opts.load(CONFIG_FILENAME);
    }
	useGui = opts.getBool("gui", false);

	// Initialize SDL
	/* Enable standard application logging */
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

	sdlInitFlags = useGui ? SDL_INIT_VIDEO : 0;
    if (SDL_Init(sdlInitFlags) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return 1;
	}
    if (TTF_Init() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }

    // Now we create our emulator state, allocated on the heap just because
    hstate = new HQNState();
    // Create the GUI if we have to
    if (useGui)
    {
        // Now try to create the gui controller
        if (!(guiController = GUIController::create(*hstate)))
            goto init_fail;
        // if we want to use a GUI but we can't load OpenGL then fail
        // Load the OpenGL extensions, etc.
        // if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
        //     goto init_fail;
        // if (ogl_ext_ARB_framebuffer_object != ogl_LOAD_FAILED)
        //     goto init_fail;
        // The guiController needs to listen to events from HQNState
        hstate->setListener(guiController);
    }

    // Now that most things are set up let's read the options
    hstate->setFramerate(opts.getInt("framerate", 0));

	// Load the ROM
	err = hstate->loadROM(argv[1]);
	if (err)
	{
		fprintf(stderr, "Failed to load rom %s: %s\n", argv[1], err);
        goto init_fail;
	}

    // And set up our lua state
    lstate = luaL_newstate();
    luaL_openlibs(lstate);
    hqn_lua::init_nes(lstate, hstate);

    if (!setjmp(endItAllBuffer))
    {
        // Now run the Lua script.
        int luaErr = luaL_dofile(lstate, argv[2]);
        if (luaErr != 0)
        {
            fprintf(stderr, "ERROR: %s\n", lua_tostring(lstate, -1));
        }
    }

    goto close_down;

init_fail:   // label for if initializing fails
    // set the exit value because we failed
    exitval = 1;

close_down:  // label for when we want to shut down
    // Safely delete everything
    if (lstate)
        lua_close(lstate);
    if (guiController)
    {
        hstate->setListener(nullptr);
        delete guiController;
    }
    if (hstate)
        delete hstate;

    TTF_Quit();
    SDL_Quit();

    return exitval;
}


void endItAll()
{
    // Go back into hqn_main to the setjmp and continue from there
    longjmp(endItAllBuffer, 1);
}

} // end namespace hqn


// Should be the entry point
int main(int argc, char *argv[])
{
	SDL_SetMainReady();
    return hqn::hqn_main(argc, argv);
}

