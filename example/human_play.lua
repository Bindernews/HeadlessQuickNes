-- An example script which allows playing via normal keyboard controls.
-- Use the arrow keys to move, Enter = Start, Backspace = Select,
-- Z is A, X is B and S and A are save and load state respectively.
-- The current measured FPS will be shown in the title bar.
require("hqnes")

gui.enable() -- enable the gui, required if you want to do GUI stuff
emu.loadrom("smb.nes")
emu.setframerate(70) -- limit the framerate to 60 fps (kinda sorta)
gui.setscale(2)      -- set the gui to scale to twice the regular size

local kbprev = {}
while gui.isenabled() do
    local kb = input.get()  -- get the current keyboard state
    joypad.set{
        left = kb.Left or false,
        right = kb.Right or false,
        up = kb.Up or false,
        down = kb.Down or false,
        start = kb.Return or false,
        select = kb.Backspace or false,
        a = kb.Z or false,
        b = kb.X or false,
    }
    if kb.S and not kbprev.S then
        savestate.save("1.savestate")
    end
    if kb.A and not kbprev.A then
        savestate.load("1.savestate")
    end
    if kb.F11 and not kbprev.F11 then
        gui.setfullscreen(not gui.isfullscreen())
    end
    gui.settitle("Headless Quick NES " .. tostring(emu.getfps()))
    emu.frameadvance() -- advance the emulator
    kbprev = kb -- set prev keyboard state to current
end
