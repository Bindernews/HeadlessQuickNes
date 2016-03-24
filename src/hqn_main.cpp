
#include <cstdio>
#include "hqn.h"
#include "hqn_lua.h"

#ifndef CONFIG_FILENAME
#define CONFIG_FILENAME "config.json"
#endif

#define DEFAULT_FILENAME "hqn"

// Import json so we can read the config file.
// using json = nlohmann::json;

namespace hqn
{

// Print the usage message.
void printUsage(const char *filename)
{
    const char *fname = filename ? filename : DEFAULT_FILENAME;
    printf("usage: %s <romfile> <lua_script>\n", fname);
}


int hqn_main(int argc, char **argv)
{
    // error
    blargg_err_t err;

    // We take two arguments, the rom file and a lua script to run.
    if (argc < 3)
    {
        printUsage(argv[0]);
        return 1;
    }

    // Now we create our emulator state, allocated on the heap just because
    HQNState *hstate = new HQNState();

	// Load the ROM
	err = hstate->loadROM(argv[1]);
	if (err)
	{
		fprintf(stderr, "Failed to load rom %s: %s\n", argv[1], err);
		// cleanup
		delete hstate;
		return 1;
	}

    // And set up our lua state
    lua_State *lstate = luaL_newstate();
    luaL_openlibs(lstate);
    hqn_lua::init_nes(lstate, hstate);

    // Now run the Lua script.
    int luaErr = luaL_dofile(lstate, argv[2]);
	if (luaErr != 0)
	{
        fprintf(stderr, "ERROR: %s\n", lua_tostring(lstate, -1));
	}

    // Always delete for good measure
    lua_close(lstate);
    delete hstate;

    return 0;
}

} // end namespace hqn


// Should be the entry point
int main(int argc, char *argv[])
{
    return hqn::hqn_main(argc, argv);
}
