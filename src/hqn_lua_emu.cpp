
#include "hqn_lua.h"

namespace hqn_lua
{

#define NES_PIXEL_COUNT (256 * 240)

	int emu_frameadvance(lua_State *L)
	{
		HQN_STATE(state);
		state->advanceFrame();
		return 0;
	}

	int emu_setframerate(lua_State *L)
	{
		HQN_STATE(state);
		int fps = lua_tointeger(L, 1);
		state->setFramerate(fps);
		return 0;
	}

	int emu_getframerate(lua_State *L)
	{
		HQN_STATE(state);
		lua_pushnumber(L, state->getFramerate());
		return 1;
	}

	int emu_getfps(lua_State *L)
	{
		HQN_STATE(state);
		lua_pushnumber(L, state->getFPS());
		return 1;
	}

	int emu_getpixels(lua_State *L)
	{
		HQN_STATE(state);
		int tbIndex;  // table index
		int32_t pixels[NES_PIXEL_COUNT];

		lua_createtable(L, NES_PIXEL_COUNT, 0);
		tbIndex = lua_gettop(L);

		// Check to prevent NPEs
		if (state->emu()->frame().pixels == nullptr)
		{
			// If there is no pixel data available return an empty table
			return 1;
		}

		// blit the pixels
		state->blit(pixels, HQNState::NES_VIDEO_PALETTE, 0, 0, 0, 0);
		// now add them to the table
		for (int i = 0; i < NES_PIXEL_COUNT; i++)
		{
			lua_pushnumber(L, pixels[i]);
			lua_rawseti(L, tbIndex, i + 1);
		}
		return 1;
	}

	int emu_loadrom(lua_State *L)
	{
		HQN_STATE(state);
		const char *romname = luaL_checkstring(L, 1);
		const char *err = state->loadROM(romname);
		if (err)
		{
			return luaL_error(L, "Error when loading ROM %s: %s", romname, err);
		}
		return 0;
	}

	int emu_init_(lua_State *L)
	{
		luaL_Reg funcReg[] = {
				{ "frameadvance", &emu_frameadvance },
				{ "setframerate", &emu_setframerate },
				{ "getframerate", &emu_getframerate },
				{ "getfps",       &emu_getfps },
				{ "getpixels",    &emu_getpixels },
				{ "loadrom",      &emu_loadrom },
				{ nullptr, nullptr }
		};
		luaL_register(L, "emu", funcReg);
		return 0;
	}

}
