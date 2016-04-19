#include "hqn_lua.h"
#include "hqn_gui_controller.h"
#include "hqn_surface.h"

using hqn::Color;

#define NO_GUI_ERROR "GUI not available"
#define CHECK_GUI(state, name) hqn::GUIController *name = static_cast<hqn::GUIController*>(state->getListener()); \
    if (!name) { return luaL_error(L, "%s", NO_GUI_ERROR); }

namespace hqn_lua
{

const Color TRANSPARENT {0, 0, 0, 0};
const Color BLACK = {0, 0, 0, 255};

Color parseColor(lua_State *L, int index, Color def)
{
    if (lua_isnil(L, index)) {
        return def;
    } else {
        Color out;
        int rgba = lua_tointeger(L, index);
        out.r = 0xff & (rgba >> 24);
        out.g = 0xff & (rgba >> 16);
        out.b = 0xff & (rgba >>  8);
        out.a = 0xff & rgba;
        return out;
    }
}
    
int gui_drawRectangle(lua_State *L)
{
    HQN_STATE(state);
    CHECK_GUI(state, gui);
    int x, y, w, h;
    Color fgColor;
    Color bgColor;

    x = lua_tointeger(L, 1);
    y = lua_tointeger(L, 2);
    w = lua_tointeger(L, 3);
    h = lua_tointeger(L, 4);
    // parse colors
    fgColor = parseColor(L, 5, BLACK);
    bgColor = parseColor(L, 6, TRANSPARENT);
    // draw to buffer
    gui->getOverlay()->fillRect(x, y, w, h, fgColor, bgColor);
    return 0;
}

int gui_drawBox(lua_State *L)
{
    HQN_STATE(state);
    CHECK_GUI(state, gui);
    int x, y, w, h;
    Color fgColor;
    Color bgColor;

    x = lua_tointeger(L, 1);
    y = lua_tointeger(L, 2);
    w = lua_tointeger(L, 3);
    h = lua_tointeger(L, 4);
    // parse colors
    fgColor = parseColor(L, 5, BLACK);
    bgColor = parseColor(L, 6, TRANSPARENT);
    // draw to buffer
    gui->getOverlay()->fillRect(x, y, x + w, y + h, fgColor, bgColor);
    return 0;
}

int gui_drawLine(lua_State *L)
{
    HQN_STATE(state);
    CHECK_GUI(state, gui);
    int x1, y1, x2, y2;
    Color fgColor;

    x1 = lua_tointeger(L, 1);
    y1 = lua_tointeger(L, 2);
    x2 = lua_tointeger(L, 3);
    y2 = lua_tointeger(L, 4);
    fgColor = parseColor(L, 5, BLACK);
    gui->getOverlay()->fastLine(x1, y1, x2, y2, fgColor);
    return 0;
}

int gui_drawText(lua_State *L)
{
    HQN_STATE(state);
    CHECK_GUI(state, gui);
    int x, y;
    const char *text;
    Color fg;

    x = lua_tointeger(L, 1);
    y = lua_tointeger(L, 2);
    text = lua_tostring(L, 3);
    fg = parseColor(L, 4, BLACK);
    gui->getOverlay()->drawText(x, y, text, fg);
    return 0;
}

int gui_clear(lua_State *L)
{
    HQN_STATE(state);
    CHECK_GUI(state, gui);
    Color clearColor = parseColor(L, 1, TRANSPARENT);
    gui->getOverlay()->clear(clearColor);
    return 0;
}

int gui_screenwidth(lua_State *L)
{
    HQN_STATE(state);
    CHECK_GUI(state, gui);
    lua_pushnumber(L, gui->getOverlay()->getWidth());
    return 1;
}

int gui_screenheight(lua_State *L)
{
    HQN_STATE(state);
    CHECK_GUI(state, gui);
    lua_pushnumber(L, gui->getOverlay()->getHeight());
    return 1;
}

int gui_init_(lua_State *L)
{
    luaL_Reg funcReg[] = {
            { "drawRectangle", &gui_drawRectangle },
            { "drawBox",  &gui_drawBox },
            { "drawLine", &gui_drawLine },
            { "drawText", &gui_drawText },
            { "clear",    &gui_clear },
            { "screenwidth",   &gui_screenwidth },
            { "screenheight",  &gui_screenheight },
            { nullptr, nullptr }
    };
    luaL_register(L, "gui", funcReg);
    return 0;
}

}
