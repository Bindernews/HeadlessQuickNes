#ifndef __HQN_H__
#define __HQN_H__

#include <nes_emu/Nes_Emu.h>
#include <cstdint>

namespace hqn
{

/*
State which is maintained by the emulator driver.

This should normally be obtained using hqn_get_state() if you are un a lua
function.
*/
class HQNState
{
public:
    HQNState();
    ~HQNState();

    /*
    Call init() after loading a rom. Returns NULL if no errors
    occured or a string message otherwise.
    */
    const char *init();

    /*
    The joypad data for the two joypads available to an NES.
    This is directly available because I'm lazy.
    */
    uint32_t joypad[2];

    /* Get the emulator this state uses. */
    inline Nes_Emu *emu() const
    { return m_emu; }

    /*
    Load a NES rom from the named file.
    Returns NULL or error string.
    */
	blargg_err_t loadROM(const char *filename);

    /*
    Advance the emulator by one frame.
    Returns NULL or error string.
    */
	blargg_err_t advanceFrame();

    /*
    Save the game state. This can be restored at any time.
    */
    blargg_err_t saveState(void *dest, size_t size, size_t *size_out);

    /*
    Get the size (bytes) of a savestate.
    Use this to allocate enough space for the saveState method.
    */
    blargg_err_t saveStateSize(size_t *size) const;

    /*
    Load the emulator state from data.
    This should be data produced by saveState().
    */
    blargg_err_t loadState(const char *data, size_t size);


	blargg_err_t setSampleRate(int rate);

private:
    void unloadRom();

    /* A reference to the emulator instance. */
    Nes_Emu *m_emu;
    /* ROM file stored in memory because reasons */
    uint8_t *m_romData;
    size_t m_romSize;
};

/*
Print the usage message.
@param filename used to specify the name of the exe file, may be NULL.
*/
void printUsage(const char *filename);

} // end namespace hqn


#endif /* __HQN_H__ */