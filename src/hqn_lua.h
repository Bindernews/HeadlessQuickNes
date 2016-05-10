#ifndef __HQN_LUA_H__
#define __HQN_LUA_H__

#include <lua.hpp>
#include "hqn.h"

namespace hqn_lua
{

// Basically import HQNState into this namespace as well
using HQNState = hqn::HQNState;

/**
 * Prerequisite: a table is on the top of the stack.
 *
 * Create a new table and register funcs into the table.
 * Then put the new table into the table which was at the top of the
 * stack with name k.
 */
void hqnL_register(lua_State *L, const char *k, const luaL_Reg *funcs);

/*
 * Get the current HQNState, but whatever means necessary.
 * You should probably use the macro.
 */
DLLEXPORT HQNState *hqn_get_state();

/*
 * Get the NES emulator from the Lua state.
 * Used by the lua C api.
 */
DLLEXPORT Nes_Emu *hqn_get_nes();

/*
 * Initialize the lua state with functions for working with the NES
 * This should be the first function you call. This leaves a table
 * on top of the Lua stack which should be used to access the library.
 */
void init_nes(lua_State *L, HQNState *state);

/*
 * Use this for loading roms.
 */
int emu_loadrom(lua_State *L);

/* Macro for lua calls which are not yet implemented. */
#define HQN_UNIMPLEMENTED(L) luaL_error(L, "NOT YET IMPLEMENTED")

/* Macro for getting the HQNState */
#define HQN_STATE(var) HQNState *var = hqn_get_state()
/* Macro for getting the Nes_Emu */
#define HQN_EMULATOR(var) Nes_Emu *var = hqn_get_nes()

}

extern "C" DLLEXPORT int luaopen_hqnes_core(lua_State *L);

#endif // __HQN_LUA_H__