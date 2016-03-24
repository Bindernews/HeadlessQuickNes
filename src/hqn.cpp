
#include "hqn.h"
#include "hqn_util.h"

namespace hqn
{

// simulate the write so we'll know how long the buffer needs to be
class Sim_Writer : public Data_Writer
{
	long size_;
public:
	Sim_Writer() :size_(0) { }
	error_t write(const void *, long size)
	{
		size_ += size;
		return 0;
	}
	long size() const { return size_; }
};

// Constructor
HQNState::HQNState()
{
    m_emu = new Nes_Emu();
    joypad[0] = 0x00;
    joypad[1] = 0x00;

    m_romData = nullptr;
    m_romSize = 0;

	m_emu->set_tracecb(nullptr);
	m_emu->set_sample_rate(44100);
}

// Destructor
HQNState::~HQNState()
{
    delete m_emu;
}

const char *HQNState::setSampleRate(int rate)
{
	const char *ret = m_emu->set_sample_rate(rate);
	if (!ret)
		m_emu->set_equalizer(Nes_Emu::nes_eq);
	return ret;
}

// Load a ROM image
const char *HQNState::loadROM(const char *filename)
{
    // unload any existing rom data
    unloadRom();

    if (!hqn_util::load_file(filename, (char**)(&m_romData), &m_romSize))
    {
        return "Failed to open file";
    }

    // Now finally load the rom. Ugh
    Mem_File_Reader r(m_romData, (int)m_romSize);
    Auto_File_Reader a(r);
    return m_emu->load_ines(a);
}

blargg_err_t HQNState::saveState(void *dest, size_t size, size_t *size_out)
{
    Mem_Writer w(dest, size, 0);
    Auto_File_Writer a(w);
    blargg_err_t ret = m_emu->save_state(a);
	if (size_out)
		*size_out = w.size();
    if (!ret && w.size() != size)
        return "Buffer Underrun!";
    return ret;
}

blargg_err_t HQNState::saveStateSize(size_t *size) const
{
    Sim_Writer w;
    Auto_File_Writer a(w);
    const char *ret = m_emu->save_state(a);
    if (size)
        *size = w.size();
    return ret;
}

blargg_err_t HQNState::loadState(const char *data, size_t size)
{
    Mem_File_Reader r(data, size);
    Auto_File_Reader a(r);
    return m_emu->load_state(a);
}

void HQNState::unloadRom()
{
    if (m_romData)
    {
        delete[] m_romData;
        m_romData = nullptr;
        m_romSize = 0;
    }
}

// Advance the emulator
const char *HQNState::advanceFrame()
{
    return m_emu->emulate_frame(joypad[0], joypad[1]);
}
    
} // end namespace hqn
