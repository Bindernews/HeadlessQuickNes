
#include "hqn_lua.h"

namespace hqn_lua
{

#define NES_PIXEL_COUNT (256 * 240)

	// static buffer used in hqn_lua_getpixels()
	static int32_t pixelBuffer[NES_PIXEL_COUNT];

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
				{ "loadrom",      &emu_loadrom },
				{ nullptr, nullptr }
		};
		// add it to the previous table
		hqnL_register(L, "emu", funcReg);
		return 0;
	}

	extern "C" DLLEXPORT
	double hqn_lua_emu_getfps()
	{
		HQN_STATE(state);
		return state->getFPS();
	}

	extern "C" DLLEXPORT
	void *hqn_lua_emu_getpixels()
	{
		HQN_STATE(state);
		state->blit(pixelBuffer, HQNState::NES_VIDEO_PALETTE, 0, 0, 0, 0);
		return (void*)pixelBuffer;
	}

}

