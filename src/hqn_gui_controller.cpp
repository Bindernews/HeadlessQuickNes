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


GUIController::GUIController(HQNState &state)
:m_state(state)
{
    m_tex = nullptr;
    m_texOverlay = nullptr;
    m_renderer = nullptr;
    m_window = nullptr;
    m_overlay = nullptr;
    m_closeOp = CLOSE_QUIT;
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
    m_state.setListener(nullptr);
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
        m_state.blit((int32_t*)nesPixels, HQNState::NES_VIDEO_PALETTE, 0, 0, 0, 0);
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

void GUIController::setTitle(const char *title)
{
    SDL_SetWindowTitle(m_window, title);
}

void GUIController::setCloseOperation(CloseOperation closeop)
{
    m_closeOp = closeop;
}

GUIController::CloseOperation GUIController::getCloseOperation() const
{
    return m_closeOp;
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
        if (m_closeOp & CLOSE_QUIT)
        {
            exit(0);
        }
        if (m_closeOp & CLOSE_DELETE)
        {
            m_state.setListener(nullptr);
            delete this;
        }
    }
}

void GUIController::onLoadROM(HQNState *state, const char *filename) {} // unimportant
void GUIController::onLoadState(HQNState *state) {} // also unimportant

}
