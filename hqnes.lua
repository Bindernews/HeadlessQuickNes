-- hqnes.lua
-- Load the hqnes library and adds ffi functions.

local core = require "hqnes.core"
local ffi = require "ffi"
local err,hqn = pcall(ffi.load, "hqnes")
if not err then
    error("Failed to load hqnes library. Did you forget to set LD_LIBRARY_PATH?")
end

ffi.cdef[[
int32_t *hqn_lua_emu_getpixels(const int32_t *);
double hqn_lua_emu_getfps();
const int32_t *hqn_lua_emu_defaultpalette();
]]

core.emu.getpixels = function(palette)
    if not palette then
        palette = hqn.hqn_lua_emu_defaultpalette()
    end
    return hqn.hqn_lua_emu_getpixels(palette)
end
core.emu.getfps = function() return hqn.hqn_lua_emu_getfps() end
core.emu.defaultpalette = function() return hqn.hqn_lua_emu_defaultpalette() end

return core
