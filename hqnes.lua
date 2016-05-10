-- hqnes.lua
-- Load the hqnes library and adds ffi functions.

local core = require "hqnes.core"
local ffi = require "ffi"
local hqn = ffi.load("hqnes")

ffi.cdef[[
void *hqn_lua_emu_getpixels();
double hqn_lua_emu_getfps();
]]

core.emu.getpixels = function() return hqn.hqn_lua_emu_getpixels() end
core.emu.getfps = function() return hqn.hqn_lua_emu_getfps() end

return core
