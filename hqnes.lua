-- hqnes.lua
-- Load the hqnes library and adds ffi functions.

local core
local ffi = require "ffi"
local ok,hqn = pcall(ffi.load, "hqnes")
if not ok then
    error("Failed to load hqnes library. Did you forget to set LD_LIBRARY_PATH?")
end

-- Because ffi requires the library to be named libhqnes.so on some platforms
-- temporarily modify package.cpath to suit our needs.
local oldCpath = package.cpath
package.cpath = oldCpath .. ";" .. oldCpath:gsub("(%?)", "lib%1")
local core = require "hqnes.core"
package.cpath = oldCpath

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
