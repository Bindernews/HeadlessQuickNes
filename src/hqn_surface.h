#ifndef __HQN_SURFACE_H__
#define __HQN_SURFACE_H__

#include <cstdint>
#include <algorithm>

namespace hqn
{

extern const float pi;

float fastSin(float x);
float fastCos(float x);

struct Color
{
    uint8_t r, g, b, a;
};

enum BlendMode
{
    HQN_BLENDMODE_NONE,
    HQN_BLENDMODE_BLEND,
    HQN_BLENDMODE_ADD,
    HQN_BLENDMODE_MOD,
};

/**
 * A surface which provides drawing operations.
 * The format is always RGBA.
 */
class Surface
{
public:
    typedef Color (*BlendFunction)(Color src, Color dst);

    Surface(size_t width, size_t height);
    ~Surface();

    size_t getWidth() const;
    size_t getHeight() const;

    void drawRect(int x, int y, size_t w, size_t h, Color color);
    void fillRect(int x, int y, size_t w, size_t h, Color fg, Color bg);
    
    void drawCircle(int x, int y, size_t radius, Color color);
    void fillCircle(int x, int y, size_t radius, Color fg, Color bg);
    
    void drawOval(int x, int y, size_t w, size_t h, Color color);
    void fillOval(int x, int y, size_t w, size_t h, Color fg, Color bg);

    /**
     * Draw a line. If the line goes outside the edges of the screen it will
     * not be drawn.
     */
    void fastLine(int x1, int y1, int x2, int y2, Color color);

    /** 
     * Slower than fastLine but will still draw lines which are partially
     * offscreen. Note that safeLine is likely significantly slower than
     * fastLine because it checks if each pixel is in bounds.
     */
    void safeLine(int x1, int y1, int x2, int y2, Color color);

    void setPixel(int x, int y, Color color);
    int32_t getPixel(int x, int y);

    /**
     * Get a reference to the pixels.
     */
    inline Color *getPixels() const
    { return m_pixels; }

    void setBlendMode(BlendMode mode);
    BlendMode getBlendMode() const;

private:

    //////////////////
    // Private Data //
    //////////////////

    // RGBA formatted pixels
    Color *m_pixels;
    int m_width;
    int m_height;
    BlendMode m_blend;
    BlendFunction m_blendFunc;
    
    // Set the pixel directly. Performs blending.
    // Inlined because it's small and more efficient to inline it.
    inline void Surface::rawset(int x, int y, Color src)
    {
        // Yay for no branching
        Color *destPtr = &m_pixels[x + y * m_width];
        *destPtr = m_blendFunc(src, *destPtr);
    }

    void line(int x1, int y1, int x2, int y2, Color color);

    // Blend functions
    static Color blendNone(Color, Color);
    static Color blendBlend(Color, Color);
    static Color blendAdd(Color, Color);
    static Color blendMod(Color, Color);

    // inlined functions for bounds checking
    inline int boundLeft(int x)
    { return std::max(x, 0); }

    inline int boundRight(int x)
    { return std::min(x, (int)m_width - 1); }

    inline int boundTop(int y)
    { return std::max(y, 0); }

    inline int boundBottom(int y)
    { return std::min(y, (int)m_height - 1); }

};

}

#endif