
#include "hqn_lua.h"
#include <SDL.h>
#include <SDL_ttf.h>

#define HQN_STATE_REF "_hqn_state"
#define SDL_INIT_FAILED_MSG "Failed to initialize SDL: %s"

namespace hqn_lua
{
	/* Delcarations for all the init functions. */
	int emu_init_(lua_State *L);
    int gui_init_(lua_State *L);
    int input_init_(lua_State *L);
	int joypad_init_(lua_State *L);
	int mainmemory_init_(lua_State *L);
    int savestate_init_(lua_State *L);

void init_nes(lua_State *L, HQNState *state)
{
    lua_pushlightuserdata(L, state);
    lua_setfield(L, LUA_GLOBALSINDEX, HQN_STATE_REF);

	emu_init_(L);
    gui_init_(L);
    input_init_(L);
	joypad_init_(L);
	mainmemory_init_(L);
    savestate_init_(L);
}

Nes_Emu *hqn_get_nes(lua_State *L)
{
    return hqn_get_state(L)->emu();
}

HQNState *hqn_get_state(lua_State *L)
{
    lua_getfield(L, LUA_GLOBALSINDEX, HQN_STATE_REF);
    HQNState *ref = (HQNState*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return ref;
}

}

extern "C"
int luaopen_hqn(lua_State *L)
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
    return 0;
}
