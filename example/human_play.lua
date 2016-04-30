-- An example script which allows playing via normal keyboard controls.
-- Use the arrow keys to move, Z is A, X is B and S and A are save and load
-- state respectively. The current measured FPS will be shown in the title bar.
-- Note this requires the GUI to be enabled. See the example hqnes.cfg.

emu.setframerate(60) -- limit the framerate to 60 fps (kinda sorta)
gui.setscale(2)      -- set the gui to scale to twice the regular size

while true do
    local kb = input.get()  -- get the current keyboard state
    joypad.set{
        left = kb.Left or false,
        right = kb.Right or false,
        up = kb.Up or false,
        down = kb.Down or false,
        a = kb.Z or false,
        b = kb.X or false,
    }
    if kb.S then
        savestate.save("1.savestate")
    end
    if kb.A then
        savestate.load("1.savestate")
    end
    gui.settitle("Headless Quick NES " .. tostring(emu.getfps()))
    emu.frameadvance() -- advance the emulator
end
