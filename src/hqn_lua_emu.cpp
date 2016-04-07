
#include "hqn_lua.h"

namespace hqn_lua
{

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

	int emu_init_(lua_State *L)
	{
		luaL_Reg funcReg[] = {
				{ "frameadvance", &emu_frameadvance },
				{ "setframerate", &emu_setframerate },
				{ "getframerate", &emu_getframerate },
				{ nullptr, nullptr }
		};
		luaL_register(L, "emu", funcReg);
		return 0;
	}

}
