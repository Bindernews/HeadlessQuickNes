#ifndef __HQN_GUI_CONTROLLER__
#define __HQN_GUI_CONTROLLER__

#include "hqn.h"
#include "hqn_surface.h"
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_pixels.h>

namespace hqn
{

bool nameToColor(const char *color, SDL_Color *out);

class GUIController : public HQNListener
{
public:
    GUIController(HQNState *state);
    ~GUIController();

    /**
     * Initialize the GUI controller. Call this right after
     * creating the controller. Returns true if all went well,
     * false if an error occured.
     */
    bool init();

    /** Set the window title. */
    void setTitle(const char *title);

    /** Set the size of the window. */
    void setSize(size_t width, size_t height);

    void setPosition(size_t x, size_t y);

    /** Get the window width. */
    size_t getWidth() const;

    /** Get the window height. */
    size_t getHeight() const;

    /** Get the pointer to the window. Use this to change settings. */
    SDL_Window *ptr();

    /**
     * Reference to the drawing surface. Use this to draw things on
     * top of the NES display.
     */
    inline Surface *getOverlay()
    { return m_overlay; }

    // Methods overriden from superclass.
    virtual void onLoadROM(HQNState *state, const char *filename);
    virtual void onAdvanceFrame(HQNState *state);
    virtual void onLoadState(HQNState *state);

private:
    /**
     * Process SDL events. Will exit the program (by calling endItAll())
     * if an SDL_QUIT message is recived.
     */
    void processEvents();

    // Copied directly from bizinterface.cpp in BizHawk/quicknes
    void blit(Nes_Emu *e, int32_t *dest, const int32_t *colors, int cropleft, int croptop, int cropright, int cropbottom);

    // Pointer to the state we're listening to
    HQNState *m_state;
    // Window pointer
    SDL_Window *m_window;
    // Renderer
    SDL_Renderer *m_renderer;
    // Pixel buffer
    int32_t m_pixels[256 * 240];
    // SDL Textures
    SDL_Texture *m_tex;
    // Overlay texture
    SDL_Texture *m_texOverlay;
    // Destination rect for the texture
    SDL_Rect m_texDest;
    // Overlay surface which will be drawn on top of the NES display
    Surface *m_overlay;
    // Should the emulator quit
    bool m_quit;
};

}

#endif //__HQN_GUI_CONTROLLER__