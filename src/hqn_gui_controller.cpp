#include "hqn_gui_controller.h"
#include "hqn_main.h"
#include "hqn_util.h"
#include <SDL.h>

#define DEFAULT_WINDOW_TITLE "HappyQuickNES"

namespace hqn
{

const SDL_Rect NES_BLIT_RECT = {0, 0, 256, 240};


// Function to initalize the video palette
int32_t *_initF_VideoPalette()
{
    static int32_t VideoPalette[512];
    const Nes_Emu::rgb_t *palette = Nes_Emu::nes_colors;
    for (int i = 0; i < 512; i++)
    {
        VideoPalette[i] = palette[i].red << 16 | palette[i].green << 8
            | palette[i].blue | 0xff000000;
    }
    return VideoPalette;
}

// Initialize the video palette
const int32_t *VideoPalette = _initF_VideoPalette();

// A map of color names to SDL_Color objects
// In alphabetical order so we can do binary search
struct {
    const char *name;
    SDL_Color color;
} COLOR_MAP = {
    // These are in alphabetical order so we can do a binary search
    { "black", {0, 0, 0, 255} },
    { "blue",  {0, 0, 255, 255} },
    { "green", {0, 255, 0, 255} },
    { "grey",  {50, 50, 50, 255} },
    { "orange", {100, 65, 0, 255} },
    { "pink", {100, 75, 80, 255} },
    { "purple", { 138, 43, 226, 255} },
    { "red", {255, 0, 0, 255} },
    { "white", {255, 255, 255, 255} },
    { "yellow", {255, 255, 0, 255} },
};
const int MAP_SIZE = 10;


bool nameToColor(const char *name, SDL_Color *out)
{
    int min = 0;
    int max = MAP_SIZE;
    int colorPos = -1;
    while (min <= max)
    {
        int med = (min + max) / 2;
        int r = hqn_util::stricmp(target, COLOR_MAP[med].name);
        if (r < 0)
        {
            max = med - 1;
        }
        else if (r > 0)
        {
            min = med + 1;
        }
        else // if (r == 0)
        {
            colorPos = med;
            break;
        }
    }
    if (colorPos == -1)
    {
        return false;
    }
    else
    {
        *out = COLOR_MAP[colorPos].color;
        return true;
    }
}


GUIController::GUIController(HQNState *state)
{
    m_tex = nullptr;
    m_renderer = nullptr;
    m_window = nullptr;
    m_state = state;
}

GUIController::~GUIController()
{
    if (m_tex)
        SDL_DestroyTexture(m_tex);
    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);
    if (m_window)
        SDL_DestroyWindow(m_window);
}

bool GUIController::init(size_t width, size_t height)
{
    // create the window
    if (!(m_window = SDL_CreateWindow(DEFAULT_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0)))
        return false;
    if (!(m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED)))
        return false;
    if (!(m_tex = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, 256, 256)))
        return false;

    // Set the clear color now rather than later
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

    return true;
}

void GUIController::onAdvanceFrame(HQNState *state)
{
    void *pixels = nullptr;
    int pitch = 0;
    // update texture
	if (SDL_LockTexture(m_tex, nullptr, &pixels, &pitch) < 0)
		return;
    blit(state->emu(), (int32_t*)pixels, VideoPalette, 0, 0, 0, 0);
    SDL_UnlockTexture(m_tex);
    // render to screen
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_tex, &NES_BLIT_RECT, &NES_BLIT_RECT);
    SDL_RenderPresent(m_renderer);
    // Process any outstanding events
    processEvents();
}

// Copied directly from bizinterface.cpp in BizHawk/quicknes
void GUIController::blit(Nes_Emu *e, int32_t *dest, const int32_t *colors, int cropleft, int croptop, int cropright, int cropbottom)
{
    // what is the point of the 256 color bitmap and the dynamic color allocation to it?
    // why not just render directly to a 512 color bitmap with static palette positions?

    const int srcpitch = e->frame().pitch;
    const unsigned char *src = e->frame().pixels;
    const unsigned char *const srcend = src + (e->image_height - cropbottom) * srcpitch;

    const short *lut = e->frame().palette;

    const int rowlen = 256 - cropleft - cropright;

    src += cropleft;
    src += croptop * srcpitch;

    for (; src < srcend; src += srcpitch)
    {
        for (int i = 0; i < rowlen; i++)
        {
            *dest++ = colors[lut[src[i]]];
        }
    }
}

void GUIController::processEvents()
{
    bool quit = false;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }
    }
    if (quit)
    {
        endItAll();
    }
}

void GUIController::onLoadROM(HQNState *state, const char *filename) {} // unimportant
void GUIController::onLoadState(HQNState *state) {} // also unimportant

}
