
#include "hqn_lua.h"
#include <SDL.h>
#include <SDL_ttf.h>

#define SDL_INIT_FAILED_MSG "Failed to initialize SDL: %s"

namespace hqn_lua
{

// Yep. Global state. Shared by the whole Lua interface.
static HQNState *globalState;


	/* Delcarations for all the init functions. */
	int emu_init_(lua_State *L);
    int gui_init_(lua_State *L);
    int input_init_(lua_State *L);
	int joypad_init_(lua_State *L);
	int mainmemory_init_(lua_State *L);
    int savestate_init_(lua_State *L);

void init_nes(lua_State *L, HQNState *state)
{
    globalState = state;

    // create new table to store all submodules
    lua_newtable(L);
    // initialize all submodules
	emu_init_(L);
    gui_init_(L);
    input_init_(L);
	joypad_init_(L);
	mainmemory_init_(L);
    savestate_init_(L);
    // end with hqnes table on top of stack
}

Nes_Emu *hqn_get_nes()
{
    HQN_STATE(state);
    return state->emu();
}

HQNState *hqn_get_state()
{
    return globalState;
}

void hqnL_register(lua_State *L, const char *k, const luaL_Reg *funcs)
{
    lua_newtable(L);
    luaL_register(L, nullptr, funcs);
    lua_setfield(L, lua_gettop(L) - 1, k);
}

} // end of hqn_lua namespace

extern "C"
int luaopen_hqnes_core(lua_State *L)
{
    if (SDL_Init(0) < 0)
    {
        return luaL_error(L, SDL_INIT_FAILED_MSG, SDL_GetError());
    }
    if (TTF_Init() < 0)
    {
        return luaL_error(L, SDL_INIT_FAILED_MSG, SDL_GetError());
    }
    hqn_lua::init_nes(L, new hqn::HQNState());
    return 1;
}
