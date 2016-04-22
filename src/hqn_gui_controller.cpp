#include "hqn_gui_controller.h"
#include "hqn_main.h"
#include "hqn_util.h"
#include <SDL.h>

#define DEFAULT_WIDTH 256
#define DEFAULT_HEIGHT 240

namespace hqn
{

const char *DEFAULT_WINDOW_TITLE = "HeadlessQuickNES";

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


GUIController::GUIController(HQNState &state)
:m_state(state)
{
    m_tex = nullptr;
    m_texOverlay = nullptr;
    m_renderer = nullptr;
    m_window = nullptr;
    m_overlay = nullptr;
}

GUIController::~GUIController()
{
    if (m_tex)
        SDL_DestroyTexture(m_tex);
    if (m_texOverlay)
        SDL_DestroyTexture(m_texOverlay);
    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);
    if (m_window)
        SDL_DestroyWindow(m_window);
}

GUIController *GUIController::create(HQNState &state)
{
    GUIController *self = new GUIController(state);
    if (!self->init())
    {
        delete self;
        return nullptr;
    }
    else
    {
        return self;
    }
}

bool GUIController::init()
{
    // create the window
    if (!(m_window = SDL_CreateWindow(DEFAULT_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH,
        DEFAULT_HEIGHT, 0)))
        return false;
    if (!(m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED)))
        return false;
    if (!(m_tex = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, 256, 256)))
        return false;
    // Set the clear color now rather than later
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    // Default the scale to 1
    if (!setScale(1))
        return false;
    return true;
}

bool GUIController::setScale(int scale)
{
    if (scale < 1 || scale > 5)
        return false;
    int winW = DEFAULT_WIDTH * scale;
    int winH = DEFAULT_HEIGHT * scale;

    // Change the window size
    SDL_SetWindowSize(m_window, winW, winH);
    
    // Destroy windows-sized things
    if (m_overlay)
        delete m_overlay;
    if (m_texOverlay)
        SDL_DestroyTexture(m_texOverlay);
    // Now re-create them
    m_overlay = new Surface(winW, winH);
    if (!(m_texOverlay = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, winW, winW)))
        return false;
    SDL_SetTextureBlendMode(m_texOverlay, SDL_BLENDMODE_BLEND);
    m_texDest = { 0, 0, winW, winH };

    // Update internal scale variable
    m_scale = scale;
    return true;
}

int GUIController::getScale() const
{ return m_scale; }

void GUIController::update(bool readNES)
{
    void *nesPixels = nullptr;
    void *overlayPixels = nullptr;
    int pitch = 0;
    // Update the overlay
    if (SDL_LockTexture(m_texOverlay, nullptr, &overlayPixels, &pitch) < 0)
        return;
    memcpy(overlayPixels, m_overlay->getPixels(), m_overlay->getDataSize());
    SDL_UnlockTexture(m_texOverlay);

    // Only update the NES image if we have to
    if (readNES)
    {
        if (SDL_LockTexture(m_tex, nullptr, &nesPixels, &pitch) < 0)
            return;
        blit(m_state.emu(), (int32_t*)nesPixels, VideoPalette, 0, 0, 0, 0);
        SDL_UnlockTexture(m_tex);
    }
    
    // render to screen
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_tex, &NES_BLIT_RECT, nullptr);
    SDL_RenderCopy(m_renderer, m_texOverlay, &m_texDest, &m_texDest);
    SDL_RenderPresent(m_renderer);
    // Process any outstanding events
    processEvents();
}

void GUIController::onAdvanceFrame(HQNState *state)
{
    update(true);
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

void GUIController::setTitle(const char *title)
{
    SDL_SetWindowTitle(m_window, title);
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
