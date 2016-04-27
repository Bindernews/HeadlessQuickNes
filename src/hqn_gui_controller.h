#ifndef __HQN_GUI_CONTROLLER__
#define __HQN_GUI_CONTROLLER__

#include "hqn.h"
#include "hqn_surface.h"
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_pixels.h>

namespace hqn
{

    extern const char *DEFAULT_WINDOW_TITLE;

class GUIController : public HQNListener
{
public:
    virtual ~GUIController();

    /**
     * Create a new GUI controller. Returns a GUI Controller or nullptr
     * if an error occured during initialization.
     */
    static GUIController *create(HQNState &state);

    /**
     * Set the window title.
     */
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
     * Set the gui scale. Can be 1 - 5.
     * Returns true if it succeeds, false otherwise.
     */
    bool setScale(int scale);

    int getScale() const;

    /**
     * Redraw the screen and process window events without updating the
     * emulator. This can be used when drawing on the overlay.
     *
     * @param readNES should update() also redraw the nes screen (true) or
     *        trust that it didn't change (false).
     */
    void update(bool readNES);

    /**
     * Reference to the drawing surface. Use this to draw things on
     * top of the NES display.
     */
    inline Surface &getOverlay()
    { return *m_overlay; }

    // Methods overriden from superclass.
    virtual void onLoadROM(HQNState *state, const char *filename);
    virtual void onAdvanceFrame(HQNState *state);
    virtual void onLoadState(HQNState *state);

protected:
    GUIController(HQNState &state);

    /**
     * Called in create(). If this fails the GUI cannot be created.
     */
    bool init();

private:
    /**
     * Process SDL events. Will exit the program (by calling endItAll())
     * if an SDL_QUIT message is recived.
     */
    void processEvents();

    // Pointer to the state we're listening to
    HQNState &m_state;
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
    // Current scale. Can be 1, 2, 3, 4, 5
    int m_scale;
    // Should the emulator quit
    bool m_quit;
};

}

#endif //__HQN_GUI_CONTROLLER__