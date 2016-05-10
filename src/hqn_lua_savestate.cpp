
#include "hqn_lua.h"
#include "hqn_util.h"

#define ERR_CANNOT_READ_FILE "Cannot read file %s"
#define ERR_SAVESTATE_LOAD_FAILED "Savestate load failed: %s"

namespace hqn_lua
{

/*
Load a savestate from a file.
 - savestate.load(string filename)
*/
int savestate_load(lua_State *L)
{
    HQN_STATE(state);
    const char *filename;
    char *dataRef;
    size_t dataSize;
    const char *err;

    // Get the name of the file we're loading
    filename = luaL_checkstring(L, 1);
    // load the file into memory
    if (!hqn::load_file(filename, &dataRef, &dataSize) != 0)
    {
        return luaL_error(L, ERR_CANNOT_READ_FILE, filename);
    }

    err = state->loadState(dataRef, dataSize);
    if (err)
    {
        // make sure to clean up after ourselves
        delete[] dataRef;
        return luaL_error(L, ERR_SAVESTATE_LOAD_FAILED, err);
    }

    // clean up after ourselves
    delete[] dataRef;
    return 0;
}

/*
Load savestate data from a string. This string must have come from
savestate.savememory().
 - savestate.loadmemory(string savedata)
*/
int savestate_loadmemory(lua_State *L)
{
    HQN_STATE(state);
    const char *dataRef;
    size_t dataSize;
    const char *err;

    dataRef = lua_tolstring(L, 1, &dataSize);
    err = state->loadState(dataRef, dataSize);
    if (err)
    {
        return luaL_error(L, ERR_SAVESTATE_LOAD_FAILED, err);
    }
    return 0;
}

/*
Savestate to ram. If it fails throw a Lua error.
This is a utility function used by both the savestate_* functions.
*/
bool savestate_to_ram(lua_State *L, char **data, size_t *size)
{
    HQN_STATE(state);
    const char *err;

    // Store the savestate
    err = state->saveStateSize(size);
    if (err)
    {
        return luaL_error(L, "Failed to determine savestate size");
    }
    *data = new char[*size];
    err = state->saveState(*data, *size, nullptr);
    if (err)
    {
        delete[] *data;
        return luaL_error(L, "Failed to save state: %s", err);
    }
    return true;
}

/*
Save the state of the emulator to a file.
 - savestate.save(string filename)
*/
int savestate_save(lua_State *L)
{
    const char *filename;
    char *data;
    size_t size;

    // Get the filename
    filename = luaL_checkstring(L, 1);
    // Get the data
    savestate_to_ram(L, &data, &size);
    // Write it to a file
    bool success = hqn::save_file(filename, data, size);
    // delete our data either way
    delete[] data;
    // now return success or throw an error
    if (success)
        return 0;
    else
        return luaL_error(L, "Things went wrong!");
}

/*
Save state and return it as a lua string.
 - savestate.savememory() -> string
*/
int savestate_savememory(lua_State *L)
{
    char *data;
    size_t size;

    // Store the savestate
    savestate_to_ram(L, &data, &size);
    // Give it to the Lua caller as a string
    lua_pushlstring(L, data, size);
    return 1;
}

int savestate_init_(lua_State *L)
{
    luaL_Reg reg[] = {
        { "load",       &savestate_load },
        { "loadmemory", &savestate_loadmemory },
        { "save",       &savestate_save },
        { "savememory", &savestate_savememory },
        { nullptr, nullptr }
    };
    hqnL_register(L, "savestate", reg);
    return 0;
}

}
