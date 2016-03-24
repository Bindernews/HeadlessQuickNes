
#include "hqn_lua.h"

namespace hqn_lua
{

	int emu_frameadvance(lua_State *L)
	{
		HQN_STATE(state);
		state->advanceFrame();
		return 0;
	}

	int emu_init_(lua_State *L)
	{
		luaL_Reg funcReg[] = {
				{ "frameadvance", &emu_frameadvance },
				{ nullptr, nullptr }
		};
		luaL_register(L, "emu", funcReg);
		return 0;
	}

}
