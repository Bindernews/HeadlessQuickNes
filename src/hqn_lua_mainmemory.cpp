/*
 * Lua functions mimicing BizHawk's mainmemory api.
 */
#include "hqn_lua.h"

namespace hqn_lua
{

/// Read a signed byte from RAM
int mainmemory_read_s8(lua_State *L)
{
    HQN_EMULATOR(emu);
    int index = luaL_optint(L, 1, 0);
    int8_t data = (int8_t)emu->low_mem()[index];
    lua_pushinteger(L, data);
    return 1;
}

/// Read an unsigned byte from RAM
int mainmemory_read_u8(lua_State *L)
{
    HQN_EMULATOR(emu);
    // First get the index requested
    int index = luaL_optint(L, 1, 0);
    // Now get the byte at that index and return it
    lua_pushinteger(L, emu->low_mem()[index]);
    return 1;
}

// Write an unsigned byte to ram
int mainmemory_write_u8(lua_State *L)
{
    HQN_EMULATOR(emu);
    int index = luaL_checkint(L, 1);
    int value = luaL_checkint(L, 2);
    emu->low_mem()[index] = (uint8_t)value;
    return 0;
}

int mainmemory_init_(lua_State *L)
{
    luaL_Reg functionList[] = {
        { "readbyte",  &mainmemory_read_u8 },
        { "writebyte", &mainmemory_write_u8 },
        { "read_s8",   &mainmemory_read_s8 },
        { "read_u8",   &mainmemory_read_u8 },
        { "write_u8",  &mainmemory_write_u8 },
		{ nullptr, nullptr }
    };

    hqnL_register(L, "mainmemory", functionList);
    return 0;
}

} // end namespace hqn_lua
