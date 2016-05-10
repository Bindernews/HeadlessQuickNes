#include "hqn_surface.h"
#include "hqn_lua.h"
#include <SDL.h>
#include "hqn_gui_controller.h"

using hqn::Color;

#define NO_GUI_ERROR "GUI not available"
#define GET_GUI(state, name) hqn::GUIController *name = static_cast<hqn::GUIController*>(state->getListener())
#define CHECK_GUI(state, name) GET_GUI(state, name); if (!name) { return luaL_error(L, "%s", NO_GUI_ERROR); }
// Shortcut macro to get the state and call CHECK_GUI
#define STATE_GUI(state, gui) HQN_STATE(state); CHECK_GUI(state, gui)

namespace hqn_lua
{

const Color ALPHA = {0, 0, 0, 0};
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
    STATE_GUI(state, gui);
    int x, y, w, h;
    Color fgColor;
    Color bgColor;

    x = lua_tointeger(L, 1);
    y = lua_tointeger(L, 2);
    w = lua_tointeger(L, 3);
    h = lua_tointeger(L, 4);
    // parse colors
    fgColor = parseColor(L, 5, BLACK);
    bgColor = parseColor(L, 6, ALPHA);
    // draw to buffer
    gui->getOverlay().fillRect(x, y, w, h, fgColor, bgColor);
    return 0;
}

int gui_drawBox(lua_State *L)
{
    STATE_GUI(state, gui);
    int x, y, w, h;
    Color fgColor;
    Color bgColor;

    x = lua_tointeger(L, 1);
    y = lua_tointeger(L, 2);
    w = lua_tointeger(L, 3);
    h = lua_tointeger(L, 4);
    // parse colors
    fgColor = parseColor(L, 5, BLACK);
    bgColor = parseColor(L, 6, ALPHA);
    // draw to buffer
    gui->getOverlay().fillRect(x, y, x + w, y + h, fgColor, bgColor);
    return 0;
}

int gui_drawLine(lua_State *L)
{
    STATE_GUI(state, gui);
    int x1, y1, x2, y2;
    Color fgColor;

    x1 = lua_tointeger(L, 1);
    y1 = lua_tointeger(L, 2);
    x2 = lua_tointeger(L, 3);
    y2 = lua_tointeger(L, 4);
    fgColor = parseColor(L, 5, BLACK);
    gui->getOverlay().fastLine(x1, y1, x2, y2, fgColor);
    return 0;
}

int gui_drawText(lua_State *L)
{
    STATE_GUI(state, gui);
    int x, y, ptsize;
    const char *text;
    Color fg;

    x = luaL_checkint(L, 1);
    y = luaL_checkint(L, 2);
    text = lua_tostring(L, 3);
    fg = parseColor(L, 4, BLACK);
    ptsize = luaL_optint(L, 5, HQN_DEFAULT_PTSIZE);
    gui->getOverlay().drawText(x, y, text, fg, ptsize);
    return 0;
}

int gui_clear(lua_State *L)
{
    STATE_GUI(state, gui);
    Color clearColor = parseColor(L, 1, ALPHA);
    gui->getOverlay().clear(clearColor);
    return 0;
}

int gui_screenwidth(lua_State *L)
{
    STATE_GUI(state, gui);
    lua_pushnumber(L, gui->getOverlay().getWidth());
    return 1;
}

int gui_screenheight(lua_State *L)
{
    STATE_GUI(state, gui);
    lua_pushnumber(L, gui->getOverlay().getHeight());
    return 1;
}

int gui_setscale(lua_State *L)
{
	STATE_GUI(state, gui);
	int scale = 1;
	if (!lua_isnil(L, 1))
	{
		scale = lua_tointeger(L, 1);
	}
	lua_pushboolean(L, gui->setScale(scale));
	return 1;
}

int gui_getscale(lua_State *L)
{
    STATE_GUI(state, gui);
	lua_pushnumber(L, gui->getScale());
	return 1;
}

int gui_settitle(lua_State *L)
{
    STATE_GUI(state, gui);

    const char *title = lua_tostring(L, 1);
    if (!title)
        title = hqn::DEFAULT_WINDOW_TITLE;
    gui->setTitle(title);
    return 0;
}

int gui_setfullscreen(lua_State *L)
{
    STATE_GUI(state, gui);
    bool full = lua_toboolean(L, 1);
    gui->setFullscreen(full, false);
    return 0;
}

int gui_isfullscreen(lua_State *L)
{
    STATE_GUI(state, gui);
    lua_pushboolean(L, gui->isFullscreen());
    return 1;
}

int gui_update(lua_State *L)
{
    STATE_GUI(state, gui);
    gui->update(false);
    return 0;
}

// Enable the GUI from code
int gui_enable(lua_State *L)
{
    HQN_STATE(state);
    
    // We can only call SDL_InitSubSystem once
    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
        {
            return luaL_error(L, "Failed to initialize video: %s", SDL_GetError());
        }
    }

    // But maybe the user can close the window and code can re-open it
    GET_GUI(state, gui);
    if (!gui)
    {
        hqn::GUIController *controller = nullptr;

        // Now try to create the gui controller
        if (controller = hqn::GUIController::create(*state))
        {
            // The guiController needs to listen to events from HQNState
            state->setListener(controller);
            controller->setCloseOperation(hqn::GUIController::CLOSE_DELETE);
        }
        else
        {
            return luaL_error(L, "Failed to create window: %s", SDL_GetError());
        }
    }
    return 0;
}

int gui_isenabled(lua_State *L)
{
    HQN_STATE(state);
    GET_GUI(state, gui);
    lua_pushboolean(L, gui != nullptr);
    return 1;
}

int gui_init_(lua_State *L)
{
    luaL_Reg funcReg[] = {
            { "drawRectangle", gui_drawRectangle },
            { "drawBox",   gui_drawBox },
            { "drawLine",  gui_drawLine },
            { "drawText",  gui_drawText },
            { "clear",     gui_clear },
            { "screenwidth",   gui_screenwidth },
            { "screenheight",  gui_screenheight },
			{ "setscale",      gui_setscale },
			{ "getscale",      gui_getscale },
            { "setfullscreen", gui_setfullscreen },
            { "isfullscreen",  gui_isfullscreen },
            { "settitle",   gui_settitle },
            { "update",     gui_update },
            { "enable",     gui_enable },
            { "isenabled",  gui_isenabled },
            { nullptr, nullptr }
    };
    hqnL_register(L, "gui", funcReg);
    return 0;
}

}
