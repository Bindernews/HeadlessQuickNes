
#define SDL_MAIN_HANDLED
#include <cstdio>
#include <SDL.h>
#include <SDL_ttf.h>
#include "gl_include.h"
#include "hqn.h"
#include "hqn_lua.h"
#include "hqn_util.h"

#define DEFAULT_FILENAME "hqnes"

namespace hqn
{

// Print the usage message.
void printUsage(const char *filename)
{
    const char *fname = filename ? filename : DEFAULT_FILENAME;
    printf("usage: %s <romfile> <lua_script>\n", fname);
}

bool luaCheck(lua_State *L, int errval)
{
    if (errval != 0)
    {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        return false;
    }
    return true;
}

int loadROM(lua_State *L, const char *romname)
{
    lua_pushcfunction(L, hqn_lua::emu_loadrom);
    lua_pushstring(L, romname);
    return lua_pcall(L, 1, 0, 0);
}

int hqn_main(int argc, char **argv)
{
#define LUASAFE(exp) if (!luaCheck(L, exp)) { goto init_fail; }
    // Delcare variables
    lua_State *L = nullptr;
    int exitval = 0;
    
    // We take two arguments, the rom file and a lua script to run.
    if (argc < 3)
    {
        printUsage(argv[0]);
        return 1;
    }

    // And set up our lua state
    L = luaL_newstate();
    luaL_openlibs(L);
    // Load the hqnes library
    lua_pushcfunction(L, luaopen_hqnes);
    LUASAFE(lua_pcall(L, 0, 0, 0));

    // Now run the main Lua script.
    luaCheck(L, luaL_dofile(L, argv[2]));
    goto close_down;

init_fail:   // label for if initializing fails
    // set the exit value because we failed
    exitval = 1;

close_down:  // label for when we want to shut down
    // Safely delete everything
    if (L)
    {
        lua_close(L);
    }

    TTF_Quit();
    SDL_Quit();

    return exitval;
}

} // end namespace hqn


// Should be the entry point
int main(int argc, char *argv[])
{
	SDL_SetMainReady();
    return hqn::hqn_main(argc, argv);
}

