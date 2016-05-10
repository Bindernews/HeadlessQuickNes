#include "hqn_lua.h"
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <SDL_mouse.h>

namespace hqn_lua
{

// Although it should be SDL_NUM_SCANCODES the actual max is 284 so 300 is closer
#define MAX_SCANCODE 300

int input_get(lua_State *L)
{
    // Create a table in Lua and start with space for 5 keys
    lua_createtable(L, 0, 5);
    int tableIndex = lua_gettop(L);
    int numkeys = MAX_SCANCODE;

    const Uint8 *kb = SDL_GetKeyboardState(&numkeys);
    for(int i = 0; i < numkeys; i++)
    {
        int sdlk = SDL_GetKeyFromScancode((SDL_Scancode)i);
        if (sdlk && kb[i])
        {
            lua_pushboolean(L, 1);
            lua_setfield(L, tableIndex, SDL_GetKeyName(sdlk));
        }
    }
    return 1;
}

int input_getmouse(lua_State *L)
{
    int x, y;
    Uint32 state = SDL_GetMouseState(&x, &y);
    lua_createtable(L, 0, 7);
    int tbl = lua_gettop(L);
    lua_pushnumber(L, x);
    lua_setfield(L, tbl, "X");
    lua_pushnumber(L, y);
    lua_setfield(L, tbl, "Y");
    lua_pushboolean(L, state & SDL_BUTTON(SDL_BUTTON_LEFT));
    lua_setfield(L, tbl, "Left");
    lua_pushboolean(L, state & SDL_BUTTON(SDL_BUTTON_MIDDLE));
    lua_setfield(L, tbl, "Middle");
    lua_pushboolean(L, state & SDL_BUTTON(SDL_BUTTON_RIGHT));
    lua_setfield(L, tbl, "Right");
    lua_pushboolean(L, state & SDL_BUTTON(SDL_BUTTON_X1));
    lua_setfield(L, tbl, "XButton1");
    lua_pushboolean(L, state & SDL_BUTTON(SDL_BUTTON_X2));
    lua_setfield(L, tbl, "XButton2");
    return 1;
}

int input_init_(lua_State *L)
{
    luaL_Reg funcReg[] = {
            { "get",      &input_get },
            { "getmouse", &input_getmouse },
            { nullptr, nullptr }
    };
    hqnL_register(L, "input", funcReg);
    return 0;
}

}
