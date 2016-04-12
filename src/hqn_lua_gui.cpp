#include "hqn_lua.h"
#include "hqn_gui_controller.h"

#define NO_GUI_ERROR "GUI not available"
#define CHECK_GUI(state, name) hqn::GUIController *name = static_cast<hqn::GUIController*>(state->getListener()); \
    if (!name) { return luaL_error(L, "%s", NO_GUI_ERROR); }

namespace hqn_lua
{

SDL_Color defaultFG;
SDL_Color defaultBG;

SDL_Color parseColor(lua_State *L, int index)
{
    SDL_Color out;
    int rgba = lua_tointeger(L, index);
    out.r = 0xff & (rgba >> 24);
    out.g = 0xff & (rgba >> 16);
    out.b = 0xff & (rgba >>  8);
    out.a = 0xff & rgba;
    return out;
}
    
int gui_drawRectangle(lua_State *L)
{
    HQN_STATE(state);
    CHECK_GUI(state, gui);
    int x, y, w, h;
    SDL_Color lineColor;
    SDL_Color bgColor;

    x = lua_tointeger(L, 1);
    y = lua_tointeger(L, 2);
    w = lua_tointeger(L, 3);
    h = lua_tointeger(L, 4);
    // parse colors
    lineColor = lua_isnil(L, 5) ? defaultFG : parseColor(L, 5);
    bgColor = lua_isnil(L, 6) ? defaultBG : parseColor(L, 6);
    // draw to buffer
    // TODO actually finish this
    return 0;
}

}
