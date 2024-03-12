/***
 *
 *
 *    MMMMMMMM               MMMMMMMMIIIIIIIIIINNNNNNNN        NNNNNNNNIIIIIIIIII             GGGGGGGGGGGGGFFFFFFFFFFFFFFFFFFFFFFXXXXXXX       XXXXXXX
 *    M:::::::M             M:::::::MI::::::::IN:::::::N       N::::::NI::::::::I          GGG::::::::::::GF::::::::::::::::::::FX:::::X       X:::::X
 *    M::::::::M           M::::::::MI::::::::IN::::::::N      N::::::NI::::::::I        GG:::::::::::::::GF::::::::::::::::::::FX:::::X       X:::::X
 *    M:::::::::M         M:::::::::MII::::::IIN:::::::::N     N::::::NII::::::II       G:::::GGGGGGGG::::GFF::::::FFFFFFFFF::::FX::::::X     X::::::X
 *    M::::::::::M       M::::::::::M  I::::I  N::::::::::N    N::::::N  I::::I        G:::::G       GGGGGG  F:::::F       FFFFFFXXX:::::X   X:::::XXX
 *    M:::::::::::M     M:::::::::::M  I::::I  N:::::::::::N   N::::::N  I::::I       G:::::G                F:::::F                X:::::X X:::::X
 *    M:::::::M::::M   M::::M:::::::M  I::::I  N:::::::N::::N  N::::::N  I::::I       G:::::G                F::::::FFFFFFFFFF       X:::::X:::::X
 *    M::::::M M::::M M::::M M::::::M  I::::I  N::::::N N::::N N::::::N  I::::I       G:::::G    GGGGGGGGGG  F:::::::::::::::F        X:::::::::X
 *    M::::::M  M::::M::::M  M::::::M  I::::I  N::::::N  N::::N:::::::N  I::::I       G:::::G    G::::::::G  F:::::::::::::::F        X:::::::::X
 *    M::::::M   M:::::::M   M::::::M  I::::I  N::::::N   N:::::::::::N  I::::I       G:::::G    GGGGG::::G  F::::::FFFFFFFFFF       X:::::X:::::X
 *    M::::::M    M:::::M    M::::::M  I::::I  N::::::N    N::::::::::N  I::::I       G:::::G        G::::G  F:::::F                X:::::X X:::::X
 *    M::::::M     MMMMM     M::::::M  I::::I  N::::::N     N:::::::::N  I::::I        G:::::G       G::::G  F:::::F             XXX:::::X   X:::::XXX
 *    M::::::M               M::::::MII::::::IIN::::::N      N::::::::NII::::::II       G:::::GGGGGGGG::::GFF:::::::FF           X::::::X     X::::::X
 *    M::::::M               M::::::MI::::::::IN::::::N       N:::::::NI::::::::I        GG:::::::::::::::GF::::::::FF           X:::::X       X:::::X
 *    M::::::M               M::::::MI::::::::IN::::::N        N::::::NI::::::::I          GGG::::::GGG:::GF::::::::FF           X:::::X       X:::::X
 *    MMMMMMMM               MMMMMMMMIIIIIIIIIINNNNNNNN         NNNNNNNIIIIIIIIII             GGGGGG   GGGGFFFFFFFFFFF           XXXXXXX       XXXXXXX
 *
 *
 *
 *
 * minigfx is a tiny graphics library, providing a unified API for Windows and Linux.
 * It was inspired by the tigr graphics library, but has been stripped down on one side
 * but has been extended with other useful things on the other side.
 * -----------------------------------------------------------------------------------
 *
 *  • Bitmap-backed windows.
 *  • Direct access to bitmaps, no locking.
 *  • Basic drawing helpers (plot, line, blitter).
 *  • Text output using bitmap fonts.
 *  • Mouse, touch and keyboard input.
 *  • PNG loading, drawing and saving.
 *  • Easy pixel shader access.
 *
 *
 * How to set up minigfx
 * ----------------------
 *
 * minigfx is supplied as a single .h file.
 * To use it, you just drop it right into your project.
 *
 * 1. Grab minigfx.h
 * 2. Throw it into your project
 * 3. Link with:
 *       • -lopengl32 and -lgdi32 on Windows
 *       • -lGLU -lGL -lX11 on Linux
 *
 *
 * Custom fonts
 * -------------
 *
 * minigfx comes with a built-in bitmap font, accessed by the tfont variable. Custom fonts can be loaded from bitmaps
 * using mgLoadFont. A font bitmap contains rows of characters separated by same-colored borders. minigfx assumes
 * that the borders use the same color as the top-left pixel in the bitmap. Each character is assumed to be drawn in
 * white on a transparent background to make tinting work.
 *
 *
 * Custom pixel shaders
 * ---------------------
 *
 * minigfx uses a built-in pixel shader that provides a couple of stock effects as controlled by mgSetPostFX. These stock
 * effects can be replaced by calling mgSetPostShader with a custom shader. The custom shader is in the form of a
 * shader function: void fxShader(out vec4 color, in vec2 uv) and has access to the four parameters from
 * mgSetPostFX as a uniform vec4 called parameters.
 *
 * Example:
 *
 *     #include "minigfx.h"
 *     #include "math.h"
 *
 *     const char fxShader[] =
 *         "void fxShader(out vec4 color, in vec2 uv) {"
 *         "   vec2 tex_size = vec2(textureSize(image, 0));"
 *         "   vec4 c = texture(image, (floor(uv * tex_size) + 0.5 * sin(parameters.x)) / tex_size);"
 *         "   color = c;"
 *         "}";
 *
 *     int main(int argc, char* argv[]) {
 *         MgSurface* screen = mgWindow(320, 240, "Shady", 0);
 *         mgSetPostShader(screen, fxShader, sizeof(fxShader) - 1);
 *
 *         float duration = 1;
 *         float phase = 0;
 *         while (!mgClosed(screen) && !mgKeyDown(screen, TK_ESCAPE)) {
 *             phase += mgTime();
 *             while (phase > duration) {
 *                 phase -= duration;
 *             }
 *             float p = 6.28 * phase / duration;
 *             mgSetPostFX(screen, p, 0, 0, 0);
 *             mgClear(screen, mgRGB(0x80, 0x90, 0xa0));
 *             mgPrint(screen, tfont, 120, 110, mgRGB(0xff, 0xff, 0xff), "Shady business");
 *             mgUpdate(screen);
 *         }
 *         mgFree(screen);
 *         return 0;
 *     }
 *
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// #######################################################################################################################################
// #######################################################################################################################################
// #######################################################################################################################################

// mg_header.h

// Comment the following line if the window position should be saved in the registry
#define MG_DO_NOT_PRESERVE_WINDOW_POSITION

// Comment the following if you want the window to maximize to fullscreen when user maximizes the window
#define MG_DO_NOT_MAXIMIZE_TO_FULLSCREEN

    // Compiler configuration ----------------------------------------------------------------
#ifdef _MSC_VER
#define MG_INLINE static __forceinline
#else
#define MG_INLINE static inline
#endif

    // Bitmaps ----------------------------------------------------------------

    // This struct contains one pixel.
    typedef struct
    {
        unsigned char r, g, b, a;
    } MgPixel;

// Window flags.
#define MG_FIXED 0       // window's bitmap is a fixed size (default)
#define MG_AUTO 1        // window's bitmap is scaled with the window
#define MG_2X 2          // always enforce (at least) 2X pixel scale
#define MG_3X 4          // always enforce (at least) 3X pixel scale
#define MG_4X 8          // always enforce (at least) 4X pixel scale
#define MG_NOCURSOR 16   // hide cursor
#define MG_FULLSCREEN 32 // start in full-screen mode

    // A Mg bitmap.
    typedef struct MgSurface
    {
        int w, h;           // width/height (unscaled)
        int cx, cy, cw, ch; // clip rect
        MgPixel *pixels;    // pixel data
        void *handle;       // OS window handle, NULL for off-screen bitmaps.
        int blitMode;       // Target bitmap blit mode
    } MgSurface;

    // Creates a new empty window with a given bitmap size.
    //
    // Title is UTF-8.
    //
    // In MG_FIXED mode, the window is made as large as possible to contain an integer-scaled
    // version of the bitmap while still fitting on the screen. Resizing the window will adapt
    // the scale in integer steps to fit the bitmap.
    //
    // In MG_AUTO mode, the initial window size is set to the bitmap size times the pixel
    // scale. Resizing the window will resize the bitmap using the specified scale.
    // For example, in forced 2X mode, the window will be twice as wide (and high) as the bitmap.
    //
    MgSurface *mgWindow(int w, int h, const char *title, int flags);

    // Creates an empty off-screen bitmap.
    MgSurface *mgBitmap(int w, int h);

    // Deletes a window/bitmap.
    void mgFree(MgSurface *surface);

    // Returns non-zero if the user requested to close a window.
    int mgClosed(MgSurface *surface);

    // Displays a window's contents on-screen and updates input.
    void mgUpdate(MgSurface *surface);

    // Called before doing direct OpenGL calls and before mgUpdate.
    // Returns non-zero if OpenGL is available.
    int mgBeginOpenGL(MgSurface *surface);

    // Sets post shader for a window.
    // This replaces the built-in post-FX shader.
    void mgSetPostShader(MgSurface *surface, const char *code, int size);

    // Sets post-FX properties for a window.
    //
    // The built-in post-FX shader uses the following parameters:
    // p1: hblur - use bilinear filtering along the x-axis (pixels)
    // p2: vblur - use bilinear filtering along the y-axis (pixels)
    // p3: scanlines - CRT scanlines effect (0-1)
    // p4: contrast - contrast boost (1 = no change, 2 = 2X contrast, etc)
    void mgSetPostFX(MgSurface *surface, float p1, float p2, float p3, float p4);

    // Drawing ----------------------------------------------------------------

    // Helper for reading pixels.
    // For high performance, just access surface->pixels directly.
    MgPixel mgGet(MgSurface *surface, int x, int y);

    // Plots a pixel.
    // Clips and blends.
    // For high performance, just access surface->pixels directly.
    void mgPlot(MgSurface *surface, int x, int y, MgPixel pix);

    // Clears a bitmap to a color.
    // No blending, no clipping.
    void mgClear(MgSurface *surface, MgPixel color);

    // Fills a rectangular area.
    // No blending, no clipping.
    void mgFill(MgSurface *surface, int x, int y, int w, int h, MgPixel color);

    // Draws a line.
    // Start pixel is drawn, end pixel is not.
    // Clips and blends.
    void mgLine(MgSurface *surface, int x0, int y0, int x1, int y1, MgPixel color);

    // Draws an empty rectangle.
    // Drawing a 1x1 rectangle yields the same result as calling mgPlot.
    // Clips and blends.
    void mgRect(MgSurface *surface, int x, int y, int w, int h, MgPixel color);

    // Fills a rectangle.
    // Fills the inside of the specified rectangular area.
    // Calling mgRect followed by mgFillRect using the same arguments
    // causes no overdrawing.
    // Clips and blends.
    void mgFillRect(MgSurface *surface, int x, int y, int w, int h, MgPixel color);

    // Draws a circle.
    // Drawing a zero radius circle yields the same result as calling mgPlot.
    // Drawing a circle with radius one draws a circle three pixels wide.
    // Clips and blends.
    void mgCircle(MgSurface *surface, int x, int y, int r, MgPixel color);

    // Fills a circle.
    // Fills the inside of the specified circle.
    // Calling mgCircle followed by mgFillCircle using the same arguments
    // causes no overdrawing.
    // Filling a circle with zero radius has no effect.
    // Clips and blends.
    void mgFillCircle(MgSurface *surface, int x, int y, int r, MgPixel color);

    // Sets clip rect.
    // Set to (0, 0, -1, -1) to reset clipping to full bitmap.
    void mgClip(MgSurface *surface, int cx, int cy, int cw, int ch);

    // Copies bitmap data.
    // dx/dy = dest co-ordinates
    // sx/sy = source co-ordinates
    // w/h   = width/height
    //
    // RGBAdest = RGBAsrc
    // Clips, does not blend.
    void mgBlit(MgSurface *dest, MgSurface *src, int dx, int dy, int sx, int sy, int w, int h);

    // Same as mgBlit, but alpha blends the source bitmap with the
    // target using per pixel alpha and the specified global alpha.
    //
    // Ablend = Asrc * alpha
    // RGBdest = RGBsrc * Ablend + RGBdest * (1 - Ablend)
    //
    // Blit mode == MG_KEEP_ALPHA:
    // Adest = Adest
    //
    // Blit mode == MG_BLEND_ALPHA:
    // Adest = Asrc * Ablend + Adest * (1 - Ablend)
    // Clips and blends.
    void mgBlitAlpha(MgSurface *dest, MgSurface *src, int dx, int dy, int sx, int sy, int w, int h, float alpha);

    // Same as mgBlit, but tints the source bitmap with a color
    // and alpha blends the resulting source with the destination.
    //
    // Rblend = Rsrc * Rtint
    // Gblend = Gsrc * Gtint
    // Bblend = Bsrc * Btint
    // Ablend = Asrc * Atint
    //
    // RGBdest = RGBblend * Ablend + RGBdest * (1 - Ablend)
    //
    // Blit mode == MG_KEEP_ALPHA:
    // Adest = Adest
    //
    // Blit mode == MG_BLEND_ALPHA:
    // Adest = Ablend * Ablend + Adest * (1 - Ablend)
    // Clips and blends.
    void mgBlitTint(MgSurface *dest, MgSurface *src, int dx, int dy, int sx, int sy, int w, int h, MgPixel tint);

    enum MGBlitMode
    {
        MG_KEEP_ALPHA = 0,  // Keep destination alpha value
        MG_BLEND_ALPHA = 1, // Blend destination alpha (default)
    };

    // Set destination bitmap blend mode for blit operations.
    void mgBlitMode(MgSurface *dest, int mode);

    // Helper for making colors.
    MG_INLINE MgPixel mgRGB(unsigned char r, unsigned char g, unsigned char b)
    {
        MgPixel p;
        p.r = r;
        p.g = g;
        p.b = b;
        p.a = 0xff;
        return p;
    }

    // Helper for making colors.
    MG_INLINE MgPixel mgRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        MgPixel p;
        p.r = r;
        p.g = g;
        p.b = b;
        p.a = a;
        return p;
    }

    // Font printing ----------------------------------------------------------

    typedef struct MgGlyph
    {
        int code, x, y, w, h;
    } MgGlyph;

    typedef struct MgFont
    {
        MgSurface *surface;
        int numGlyphs;
        MgGlyph *glyphs;
    } MgFont;

    typedef enum TCodepage
    {
        TCP_ASCII = 0,
        TCP_1252 = 1252,
        TCP_UTF32 = 12001
    } TCodepage;

    // Loads a font.
    //
    // Codepages:
    //
    //  TCP_ASCII   - Regular 7-bit ASCII
    //  TCP_1252    - Windows 1252
    //  TCP_UTF32   - Unicode subset
    //
    // For ASCII and 1252, the font bitmap should contain all characters
    // for the given codepage, excluding the first 32 control codes.
    //
    // For UTF32 - the font bitmap contains a subset of Unicode characters
    // and must be in the format generated by mgFont for UTF32.
    //
    MgFont *mgLoadFont(MgSurface *bitmap, int codepage);

    // Frees a font.
    void mgFreeFont(MgFont *font);

    // Prints UTF-8 text onto a bitmap.
    // NOTE:
    //  This uses the target bitmap blit mode.
    //  See mgBlitTint for details.
    void mgPrint(MgSurface *dest, MgFont *font, int x, int y, MgPixel color, const char *text, ...);

    // Returns the width/height of a string.
    int mgTextWidth(MgFont *font, const char *text);
    int mgTextHeight(MgFont *font, const char *text);

    // The built-in font.
    extern MgFont *mgfont;

    // User Input -------------------------------------------------------------

    // Key scancodes. For letters/numbers, use ASCII ('A'-'Z' and '0'-'9').
    typedef enum
    {
        MK_PAD0 = 128,
        MK_PAD1,
        MK_PAD2,
        MK_PAD3,
        MK_PAD4,
        MK_PAD5,
        MK_PAD6,
        MK_PAD7,
        MK_PAD8,
        MK_PAD9,
        MK_PADMUL,
        MK_PADADD,
        MK_PADENTER,
        MK_PADSUB,
        MK_PADDOT,
        MK_PADDIV,
        MK_F1,
        MK_F2,
        MK_F3,
        MK_F4,
        MK_F5,
        MK_F6,
        MK_F7,
        MK_F8,
        MK_F9,
        MK_F10,
        MK_F11,
        MK_F12,
        MK_BACKSPACE,
        MK_TAB,
        MK_RETURN,
        MK_SHIFT,
        MK_CONTROL,
        MK_ALT,
        MK_PAUSE,
        MK_CAPSLOCK,
        MK_ESCAPE,
        MK_SPACE,
        MK_PAGEUP,
        MK_PAGEDN,
        MK_END,
        MK_HOME,
        MK_LEFT,
        MK_UP,
        MK_RIGHT,
        MK_DOWN,
        MK_INSERT,
        MK_DELETE,
        MK_LWIN,
        MK_RWIN,
        MK_NUMLOCK,
        MK_SCROLL,
        MK_LSHIFT,
        MK_RSHIFT,
        MK_LCONTROL,
        MK_RCONTROL,
        MK_LALT,
        MK_RALT,
        MK_SEMICOLON,
        MK_EQUALS,
        MK_COMMA,
        MK_MINUS,
        MK_DOT,
        MK_SLASH,
        MK_BACKTICK,
        MK_LSQUARE,
        MK_BACKSLASH,
        MK_RSQUARE,
        MK_TICK
    } MgKey;

    // Returns mouse input for a window.
    // Example:
    // int mx, my, buttons = 0;
    // mgMouse(surface, &mx, &my, &buttons);
    // if (buttons & 1 || buttons & 2 || buttons & 4) printf("Left/Right/Middle button pressed");
    void mgMouse(MgSurface *surface, int *x, int *y, int *buttons);

    typedef struct
    {
        int x;
        int y;
    } MgTouchPoint;

    // Reads touch input for a window.
    // Returns number of touch points read.
    int mgTouch(MgSurface *surface, MgTouchPoint *points, int maxPoints);

    // Reads the keyboard for a window.
    // Returns non-zero if a key is pressed/held.
    // mgKeyDown tests for the initial press, mgKeyHeld repeats each frame.
    int mgKeyDown(MgSurface *surface, int key);
    int mgKeyHeld(MgSurface *surface, int key);

    // Reads character input for a window.
    // Returns the Unicode value of the last key pressed, or 0 if none.
    int mgReadChar(MgSurface *surface);

    // Bitmap I/O -------------------------------------------------------------

    // Loads a PNG, from either a file or memory. (fileName is UTF-8)
    // On error, returns NULL and sets errno.
    MgSurface *mgLoadImage(const char *fileName);
    MgSurface *mgLoadImageMem(const void *data, int length);

    // Saves a PNG to a file. (fileName is UTF-8)
    // On error, returns zero and sets errno.
    int mgSaveImage(const char *fileName, MgSurface *surface);

    // Helpers ----------------------------------------------------------------

    // Returns the amount of time elapsed since mgTime was last called,
    // or zero on the first call.
    float mgTime(void);

    // Displays an error message and quits. (UTF-8)
    // 'bmp' can be NULL.
    void mgError(MgSurface *surface, const char *message, ...);

    // Reads an entire file into memory. (fileName is UTF-8)
    // Free it yourself after with 'free'.
    // On error, returns NULL and sets errno.
    // MG will automatically append a NUL terminator byte
    // to the end (not included in the length)
    void *mgReadFile(const char *fileName, int *length);

    // Decompresses DEFLATEd zip/zlib data into a buffer.
    // Returns non-zero on success.
    int mgInflate(void *out, unsigned outlen, const void *in, unsigned inlen);

    // Decodes a single UTF8 codepoint and returns the next pointer.
    const char *mgDecodeUTF8(const char *text, int *cp);

    // Encodes a single UTF8 codepoint and returns the next pointer.
    char *mgEncodeUTF8(char *text, int cp);

    // Extensions ----------------------------------------------------------------

    // Returns the frames per second based on the given delta time.
    // Delta time can be received by calling 'mgTime()' once in the main loop.
    int mgFps(float dt);

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

#ifdef __cplusplus
}
#endif

/***
 * ████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
 * ██                                                                                                                                ██
 * ██                                                                                                                                ██
 * ██       ██ ███    ███ ██████  ██      ███████ ███    ███ ███████ ███    ██ ████████  █████  ████████ ██  ██████  ███    ██       ██
 * ██       ██ ████  ████ ██   ██ ██      ██      ████  ████ ██      ████   ██    ██    ██   ██    ██    ██ ██    ██ ████   ██       ██
 * ██       ██ ██ ████ ██ ██████  ██      █████   ██ ████ ██ █████   ██ ██  ██    ██    ███████    ██    ██ ██    ██ ██ ██  ██       ██
 * ██       ██ ██  ██  ██ ██      ██      ██      ██  ██  ██ ██      ██  ██ ██    ██    ██   ██    ██    ██ ██    ██ ██  ██ ██       ██
 * ██       ██ ██      ██ ██      ███████ ███████ ██      ██ ███████ ██   ████    ██    ██   ██    ██    ██  ██████  ██   ████       ██
 * ██                                                                                                                                ██
 * ██                                                                                                                                ██
 * ████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
 */

#ifdef MINIGFX_IMPLEMENTATION
#ifdef __cplusplus
extern "C"
{
#endif

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_font.h

    const unsigned char mg_font[] = {
        0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
        0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x5c, 0x08, 0x03, 0x00, 0x00, 0x00, 0x92, 0xab, 0x43,
        0x85, 0x00, 0x00, 0x03, 0x00, 0x50, 0x4c, 0x54, 0x45, 0x5f, 0x53, 0x87, 0x00, 0x00, 0x00, 0x54,
        0x54, 0x54, 0xff, 0xff, 0xfa, 0x31, 0x2a, 0x42, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
        0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
        0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x4a, 0x44, 0xb7, 0x5a, 0x00, 0x00, 0x00,
        0x01, 0x74, 0x52, 0x4e, 0x53, 0x00, 0x40, 0xe6, 0xd8, 0x66, 0x00, 0x00, 0x00, 0x01, 0x62, 0x4b,
        0x47, 0x44, 0x00, 0x88, 0x05, 0x1d, 0x48, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00,
        0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00, 0x9a, 0x9c, 0x18, 0x00, 0x00, 0x00, 0x07,
        0x74, 0x49, 0x4d, 0x45, 0x07, 0xe5, 0x04, 0x1c, 0x16, 0x0f, 0x2b, 0x56, 0x9d, 0x54, 0x3b, 0x00,
        0x00, 0x0b, 0x7c, 0x49, 0x44, 0x41, 0x54, 0x78, 0xda, 0xed, 0x5c, 0x89, 0x9a, 0xec, 0xac, 0x0a,
        0x04, 0xf5, 0xfd, 0x9f, 0xf9, 0x4e, 0x94, 0xa5, 0x40, 0x93, 0x98, 0xee, 0x3e, 0x73, 0xe7, 0xfb,
        0x67, 0xce, 0xd2, 0x66, 0x31, 0x6a, 0x29, 0x51, 0x4a, 0x20, 0xd4, 0x7e, 0xf3, 0x1f, 0x6a, 0x34,
        0xfe, 0xb4, 0x5a, 0xf9, 0xf8, 0x7f, 0x24, 0x54, 0xf9, 0xf8, 0xf7, 0x75, 0xeb, 0xeb, 0x97, 0xe0,
        0xd4, 0x73, 0x92, 0x64, 0xd7, 0xab, 0x90, 0xa4, 0xc3, 0x91, 0x59, 0xb2, 0x56, 0x29, 0x50, 0xce,
        0xed, 0xfa, 0x71, 0x74, 0x94, 0xd7, 0x2f, 0xd8, 0xf5, 0x7c, 0x0e, 0xf9, 0xb1, 0x74, 0x69, 0x15,
        0x13, 0x14, 0xfd, 0x09, 0xf4, 0xd2, 0x08, 0x3e, 0xfe, 0xf6, 0x53, 0x86, 0x9c, 0xa5, 0x8d, 0xcc,
        0xd2, 0x45, 0x9a, 0x2c, 0xd0, 0x7f, 0x65, 0x63, 0x45, 0xc1, 0xa3, 0x00, 0xae, 0x78, 0x7d, 0xb4,
        0xb9, 0x9f, 0xf3, 0x57, 0xb9, 0x7a, 0xfd, 0x38, 0xc3, 0x7c, 0x90, 0x36, 0xad, 0xba, 0x68, 0x75,
        0x1c, 0x8a, 0x7e, 0x05, 0x3d, 0x1f, 0xcf, 0xfa, 0x18, 0x0c, 0xd8, 0xad, 0xff, 0x96, 0x9e, 0xa3,
        0xd7, 0xc0, 0x36, 0xec, 0x90, 0xe9, 0x0c, 0x7d, 0x1d, 0x68, 0x6c, 0x6c, 0x7b, 0x01, 0x5f, 0xa5,
        0x0d, 0x74, 0x23, 0x25, 0xe9, 0xf5, 0xd1, 0xbb, 0xbd, 0x22, 0x62, 0xc1, 0xa5, 0xd7, 0x53, 0x2a,
        0x02, 0x38, 0x9a, 0x35, 0x04, 0xa4, 0x8f, 0x90, 0x16, 0xf9, 0x0a, 0x7a, 0xfa, 0x7a, 0xb6, 0x8f,
        0xe9, 0xd1, 0x09, 0x63, 0x6c, 0x8f, 0x46, 0x70, 0xd1, 0x7e, 0x1d, 0x08, 0x07, 0x20, 0x41, 0xdf,
        0x81, 0x15, 0x6b, 0x03, 0x11, 0x1e, 0xf6, 0x3e, 0xb1, 0xde, 0xec, 0xc3, 0x22, 0x12, 0x31, 0x46,
        0x4d, 0x9f, 0xfd, 0xaa, 0x41, 0x7b, 0x5d, 0xf3, 0xb3, 0xe7, 0xe3, 0x5a, 0x3d, 0x1d, 0xbd, 0xd6,
        0xff, 0x1f, 0xe0, 0xa9, 0x4b, 0x48, 0x2f, 0x6c, 0x0c, 0xc2, 0x78, 0xe4, 0x25, 0xf4, 0x32, 0x6a,
        0x2a, 0xf9, 0xac, 0xaf, 0xeb, 0xa8, 0xa6, 0x8e, 0x54, 0x87, 0x27, 0x8c, 0xfd, 0xd1, 0x5f, 0x63,
        0xb8, 0xbc, 0x39, 0xe4, 0xa3, 0x63, 0x12, 0x2e, 0x6f, 0x49, 0xbf, 0xfe, 0xd5, 0x60, 0xc6, 0xf7,
        0x3d, 0x4b, 0xba, 0xe6, 0xb3, 0x73, 0xd6, 0xeb, 0xa3, 0x11, 0x2a, 0xec, 0x15, 0x5f, 0x83, 0xf1,
        0xc8, 0x53, 0xf4, 0xec, 0xaf, 0x50, 0x92, 0xfc, 0xa3, 0xdc, 0x31, 0x24, 0x7a, 0x8e, 0xe8, 0x0f,
        0xc1, 0x18, 0xd5, 0xae, 0x24, 0x5f, 0x85, 0xa6, 0x04, 0xf4, 0xde, 0x8d, 0x84, 0xa8, 0xb3, 0xa4,
        0x6b, 0x3e, 0x3b, 0xf7, 0xfb, 0x0d, 0x85, 0x7d, 0x74, 0xb6, 0x74, 0xe8, 0xa8, 0xe2, 0x8d, 0xb1,
        0x07, 0xc9, 0xaf, 0xa3, 0xdc, 0xca, 0x3a, 0x24, 0x32, 0xa8, 0x8e, 0x9e, 0x48, 0x90, 0x68, 0x43,
        0x1b, 0x4e, 0xa3, 0xa9, 0xf5, 0x36, 0xeb, 0x15, 0x5a, 0xa3, 0xca, 0x69, 0xc8, 0x37, 0xbd, 0xf7,
        0x2a, 0xec, 0x63, 0xce, 0x97, 0x66, 0xf4, 0xf4, 0x33, 0x92, 0xdf, 0xfb, 0x13, 0x3b, 0x43, 0xc6,
        0x9e, 0x6d, 0xe2, 0x8f, 0x2b, 0xde, 0x84, 0x5e, 0x32, 0xfb, 0xdc, 0x80, 0x92, 0x2e, 0xd7, 0x4d,
        0x06, 0xf0, 0xfd, 0x96, 0xc9, 0x05, 0x9f, 0xb7, 0x74, 0xcc, 0x4d, 0xc7, 0x73, 0x30, 0xe7, 0x17,
        0xa2, 0xd7, 0xc7, 0x1e, 0x24, 0x7f, 0x4c, 0x7e, 0x36, 0x9e, 0x76, 0x7a, 0x48, 0x98, 0xbe, 0xd0,
        0x32, 0xf3, 0xe4, 0xf5, 0x1e, 0x06, 0xa3, 0xe7, 0x28, 0x92, 0xb9, 0x67, 0x65, 0x11, 0x52, 0x4d,
        0xf5, 0xfa, 0x91, 0x3d, 0x9c, 0x5b, 0xfe, 0x42, 0xf8, 0x3c, 0xe4, 0x6f, 0xd6, 0xf1, 0xa1, 0x01,
        0x2f, 0xa3, 0xcf, 0x05, 0x9d, 0x9d, 0x7a, 0xce, 0x36, 0x67, 0x6a, 0x51, 0x1a, 0xda, 0x69, 0x01,
        0xef, 0xa6, 0xcd, 0x3a, 0x3e, 0xa3, 0x7f, 0x4d, 0xf2, 0x3f, 0x83, 0x3e, 0x4a, 0xc3, 0x3f, 0x44,
        0x9f, 0x4b, 0x4f, 0x55, 0xfd, 0x7f, 0xd0, 0xaf, 0x0e, 0x7f, 0x32, 0xfa, 0xdf, 0xce, 0x72, 0x54,
        0x01, 0xa5, 0xac, 0x6a, 0xa5, 0xf3, 0x3c, 0x65, 0xfb, 0x7d, 0x5d, 0x88, 0xe0, 0x7a, 0x5a, 0xc0,
        0x74, 0xe1, 0xca, 0xeb, 0xf9, 0x65, 0x85, 0x34, 0x55, 0x3c, 0xad, 0x7c, 0x34, 0x1f, 0x60, 0x53,
        0xaa, 0x2f, 0x3c, 0x35, 0x90, 0x92, 0x21, 0x20, 0x63, 0x3d, 0x63, 0xd5, 0x66, 0xce, 0x48, 0x05,
        0x67, 0x12, 0x82, 0x64, 0x44, 0xab, 0x64, 0xd3, 0x8c, 0xd9, 0x16, 0xac, 0x88, 0xde, 0xf5, 0xfd,
        0xdb, 0x8a, 0x30, 0xd5, 0x82, 0xf7, 0xd1, 0x83, 0xd6, 0x58, 0x7c, 0xd9, 0xe5, 0x26, 0x7a, 0x9b,
        0xa3, 0xef, 0x7a, 0x98, 0x6a, 0x33, 0x53, 0xe1, 0xbc, 0x44, 0x11, 0xc9, 0x48, 0x56, 0x66, 0xc6,
        0xea, 0x4d, 0xc2, 0x18, 0x0b, 0xa2, 0x8f, 0xc2, 0xb3, 0xaa, 0x30, 0x8d, 0xa0, 0xe8, 0xcc, 0xa6,
        0x25, 0xad, 0xd1, 0x33, 0x56, 0x55, 0x55, 0xdd, 0x57, 0x32, 0xc6, 0xa6, 0x60, 0x1e, 0x60, 0x61,
        0x4d, 0x6e, 0x83, 0xbf, 0xaa, 0x2e, 0x07, 0x8d, 0x0b, 0xad, 0x76, 0x71, 0x52, 0xc5, 0x9a, 0x43,
        0x2f, 0x98, 0xa6, 0x9d, 0xd0, 0x9b, 0x9a, 0xae, 0xe5, 0xd8, 0x98, 0xdf, 0xa0, 0x77, 0xed, 0x86,
        0x61, 0x11, 0x3f, 0x45, 0x4f, 0x2c, 0x55, 0xb9, 0xfe, 0xe4, 0x10, 0x1a, 0x39, 0xfa, 0x63, 0x35,
        0xf4, 0x35, 0x59, 0xd0, 0x93, 0x6b, 0xb2, 0x31, 0x4d, 0xe8, 0x59, 0x15, 0x57, 0x4d, 0xeb, 0x44,
        0x4a, 0xa6, 0xb1, 0x0f, 0xe8, 0x95, 0xcb, 0xc5, 0x57, 0x06, 0xbb, 0x3b, 0x14, 0xc8, 0x71, 0x57,
        0x64, 0x81, 0xde, 0xe9, 0x93, 0xd0, 0x4f, 0xe1, 0x4f, 0xd0, 0xf1, 0x42, 0x07, 0x08, 0x95, 0x59,
        0x40, 0xdf, 0xe5, 0xe6, 0xec, 0xb5, 0x4a, 0x95, 0x26, 0xc9, 0x0f, 0x20, 0x16, 0xe8, 0xf9, 0x72,
        0xd6, 0x43, 0xb4, 0xda, 0xd6, 0x58, 0x20, 0xd7, 0xc4, 0x0f, 0x16, 0x73, 0x9b, 0x6f, 0x10, 0x60,
        0x3f, 0x39, 0xc3, 0x10, 0x9a, 0xbd, 0x46, 0xcf, 0x75, 0x28, 0xb0, 0xb4, 0xf1, 0x1a, 0xce, 0xef,
        0xfd, 0x0a, 0x3d, 0x8e, 0xf9, 0x2e, 0x7a, 0x7b, 0x35, 0x72, 0xaf, 0xac, 0xd0, 0x03, 0x6f, 0xb0,
        0x07, 0x0a, 0x9f, 0xf1, 0x28, 0x25, 0x43, 0x4b, 0xf4, 0xc4, 0x75, 0xb5, 0x72, 0x05, 0xb2, 0x91,
        0x49, 0x4a, 0x24, 0x1f, 0x79, 0x99, 0x61, 0x10, 0xbb, 0x3c, 0xc9, 0xc5, 0x99, 0xf9, 0x84, 0xcd,
        0xe4, 0xfb, 0x8a, 0xc9, 0xce, 0xd9, 0x2a, 0x28, 0x1c, 0x27, 0x00, 0x1f, 0x7b, 0x5f, 0xf1, 0x74,
        0x7b, 0x0d, 0x09, 0x58, 0x3f, 0x6b, 0x43, 0x33, 0x76, 0x52, 0x61, 0x24, 0xa4, 0x24, 0x72, 0x12,
        0x48, 0x4a, 0x24, 0x1f, 0xe1, 0x39, 0x21, 0x2b, 0x85, 0xe6, 0xeb, 0xfd, 0xaa, 0x1e, 0x20, 0xab,
        0xc1, 0x02, 0x75, 0xc7, 0x2b, 0xb2, 0x20, 0x16, 0xba, 0x15, 0xcf, 0x43, 0x81, 0xb2, 0x17, 0xc6,
        0xbe, 0xdf, 0x33, 0xaa, 0x2e, 0x42, 0x05, 0x19, 0x68, 0x9c, 0xdc, 0x75, 0x4d, 0xf7, 0xdf, 0x28,
        0xa4, 0xab, 0xf4, 0x3a, 0x83, 0xb7, 0xf1, 0x79, 0x81, 0xeb, 0x1c, 0x8d, 0xce, 0xd4, 0xf2, 0x9f,
        0x87, 0xfe, 0x9d, 0x02, 0xff, 0xd0, 0x3f, 0x44, 0xff, 0xbb, 0x59, 0x8e, 0x4f, 0x85, 0xbe, 0x96,
        0x41, 0x17, 0xe1, 0x6d, 0x51, 0x1f, 0x73, 0x6e, 0x53, 0x2d, 0xe3, 0xe2, 0xf6, 0xea, 0x58, 0x37,
        0x57, 0x1b, 0x9e, 0x88, 0x4e, 0x0b, 0x3b, 0x9e, 0x78, 0xf7, 0x53, 0xe8, 0x6b, 0xc5, 0x49, 0xc9,
        0x16, 0x2a, 0x99, 0x57, 0x93, 0xc2, 0xf4, 0xed, 0xe8, 0x75, 0xb5, 0x6c, 0x1c, 0x87, 0xec, 0x12,
        0x7d, 0xdf, 0xb5, 0x5d, 0x51, 0x4b, 0xa3, 0xaa, 0xf1, 0x36, 0x30, 0x58, 0xd3, 0x13, 0x60, 0xec,
        0x51, 0xbf, 0x36, 0xb5, 0xe7, 0x82, 0xc1, 0x42, 0x41, 0x14, 0xaa, 0xb9, 0x64, 0x74, 0xb3, 0xa2,
        0xdc, 0xc8, 0x1b, 0xe3, 0x64, 0xf6, 0x0e, 0x7d, 0x57, 0x1b, 0x16, 0x06, 0xb3, 0x48, 0x61, 0x59,
        0xfb, 0x74, 0x71, 0x39, 0xa0, 0x07, 0xfd, 0x1a, 0x38, 0x1d, 0xef, 0x33, 0x5a, 0xac, 0x66, 0x0f,
        0x3d, 0xb0, 0x59, 0x35, 0x49, 0xd5, 0x3a, 0xec, 0x6f, 0x9d, 0xd6, 0xdd, 0xec, 0x6c, 0x99, 0x89,
        0x28, 0xaa, 0x9e, 0xab, 0x6d, 0x8b, 0x13, 0xb2, 0x01, 0xe8, 0x5d, 0xbf, 0x46, 0x4e, 0x77, 0xab,
        0x42, 0x33, 0x58, 0xce, 0xee, 0xf9, 0x6c, 0xa2, 0xb3, 0x66, 0x53, 0x85, 0xb1, 0x2f, 0x9d, 0xcc,
        0x1e, 0xdb, 0xca, 0x77, 0xe8, 0x67, 0xd2, 0xc5, 0x99, 0xdc, 0x5d, 0x37, 0x2b, 0x8c, 0x3d, 0xa0,
        0x37, 0x4e, 0xb7, 0x45, 0x20, 0xd4, 0x72, 0x76, 0xc7, 0xe8, 0x92, 0x66, 0x4e, 0xc6, 0xeb, 0xa2,
        0xe4, 0x93, 0x6c, 0xcb, 0x6f, 0xa0, 0xa7, 0x6b, 0xf4, 0xb4, 0x3b, 0xf6, 0x4e, 0x70, 0xae, 0x59,
        0xcd, 0x92, 0xe5, 0x98, 0xa1, 0xfb, 0x84, 0xd3, 0x4c, 0x2f, 0x12, 0x98, 0x7c, 0x56, 0xe8, 0xef,
        0xdf, 0xfb, 0x28, 0xf9, 0xc8, 0x6f, 0xdc, 0x66, 0xaa, 0xec, 0x02, 0xc8, 0xc6, 0x29, 0x7a, 0x9b,
        0x21, 0xae, 0x58, 0x4d, 0xb4, 0xc9, 0x22, 0xcb, 0x11, 0x12, 0x83, 0xb5, 0xb2, 0xed, 0x5c, 0xf0,
        0x4c, 0x82, 0x60, 0xd6, 0x43, 0xdb, 0x7e, 0x79, 0x80, 0x9e, 0xd0, 0xc8, 0x62, 0xfc, 0xc6, 0x2e,
        0x83, 0x2d, 0xa5, 0x2c, 0xc8, 0x88, 0xac, 0x78, 0x89, 0x9f, 0x5c, 0xb0, 0x1a, 0xe5, 0xf3, 0x58,
        0x90, 0x36, 0x43, 0x48, 0xcc, 0x9a, 0xe3, 0x94, 0xc0, 0xaa, 0xfc, 0x71, 0x36, 0x86, 0xd3, 0x92,
        0x8d, 0x69, 0x0b, 0xfd, 0xf6, 0x46, 0x7e, 0x36, 0x3e, 0x92, 0x5b, 0xb4, 0xd3, 0xa5, 0x8b, 0x05,
        0x1a, 0xd5, 0x8a, 0xfc, 0xd4, 0x5d, 0xed, 0xa9, 0x98, 0x4c, 0xda, 0x5b, 0xae, 0xfd, 0xe3, 0xe8,
        0xd1, 0xf8, 0xe8, 0xa6, 0xf4, 0x7c, 0xe9, 0xdf, 0xa3, 0x8f, 0xed, 0xf8, 0x2e, 0xf4, 0x68, 0x7c,
        0xb4, 0x0a, 0xa7, 0x4b, 0x17, 0xe8, 0x4d, 0xd2, 0x97, 0x8c, 0x76, 0x1b, 0x7d, 0x6c, 0xc7, 0x6b,
        0xe8, 0xff, 0x3c, 0xd6, 0xa4, 0x8f, 0x80, 0x2c, 0xa4, 0xfe, 0x86, 0x6d, 0x65, 0x77, 0xc0, 0x60,
        0xb3, 0xf4, 0x8f, 0x53, 0xd5, 0xed, 0x45, 0x83, 0x1d, 0xc5, 0xf8, 0x8d, 0x16, 0x1d, 0x5e, 0x2a,
        0xa3, 0x3a, 0xbc, 0x30, 0x23, 0xca, 0x56, 0xb4, 0x39, 0xbe, 0x24, 0x6b, 0x04, 0xb4, 0x2d, 0x0c,
        0xb5, 0xfa, 0x97, 0x0d, 0x17, 0x84, 0x2d, 0x4b, 0x16, 0x03, 0xfa, 0xd9, 0x27, 0x61, 0x89, 0x5e,
        0xd4, 0xc8, 0xe1, 0xc9, 0xe4, 0x9e, 0x0e, 0x3e, 0x05, 0x0e, 0x99, 0xf6, 0x1b, 0xcf, 0xd1, 0xbb,
        0xcf, 0x58, 0x51, 0xa7, 0x19, 0xa8, 0xe5, 0x04, 0xbd, 0xe4, 0x10, 0x9f, 0x83, 0x5b, 0xf4, 0x5d,
        0xc9, 0xe7, 0x80, 0x9e, 0x16, 0xe8, 0x27, 0xc7, 0x8b, 0x32, 0x3c, 0x28, 0x64, 0xf7, 0xd4, 0xfa,
        0xd0, 0x37, 0x75, 0x15, 0xb0, 0x49, 0x01, 0xba, 0x7c, 0x70, 0x05, 0xcb, 0x86, 0xba, 0x44, 0x24,
        0xf4, 0xa2, 0xab, 0x8e, 0x71, 0x3c, 0x7a, 0x20, 0xd4, 0x32, 0x54, 0xbe, 0x68, 0x64, 0x9c, 0x28,
        0xd2, 0x3d, 0xbf, 0x17, 0x52, 0xc1, 0xcd, 0x50, 0xae, 0x24, 0x5f, 0x8d, 0x69, 0x66, 0xad, 0x13,
        0x2f, 0x0f, 0xdc, 0xdc, 0x1f, 0xcc, 0x4a, 0x76, 0xa6, 0x87, 0xa3, 0xa3, 0x71, 0x90, 0xfe, 0x0f,
        0x5c, 0x3e, 0xc6, 0xea, 0xdd, 0x74, 0x11, 0x07, 0xb5, 0x05, 0xde, 0x1b, 0x82, 0x71, 0x24, 0x25,
        0x58, 0xc3, 0x2e, 0x29, 0x9a, 0x95, 0x29, 0x7e, 0xc3, 0x1a, 0x09, 0x06, 0xbc, 0x2d, 0xf4, 0x2c,
        0x96, 0x19, 0x05, 0xc4, 0x9b, 0x92, 0x4f, 0xe6, 0x8d, 0x68, 0x8f, 0x89, 0xe7, 0x94, 0x4a, 0x7e,
        0x91, 0x42, 0xab, 0x90, 0x0d, 0xeb, 0xb2, 0x93, 0x55, 0xb0, 0x05, 0xc7, 0x96, 0x65, 0x23, 0x74,
        0x04, 0x5c, 0xcd, 0x77, 0xff, 0xb2, 0xbe, 0x71, 0xcd, 0xc9, 0xc5, 0x6f, 0x6b, 0xec, 0x89, 0x6e,
        0x25, 0x3f, 0x6f, 0x95, 0x1b, 0x7a, 0x99, 0x66, 0x44, 0xc1, 0x57, 0xa7, 0x4e, 0x93, 0x7c, 0xf3,
        0xcd, 0x6b, 0x0b, 0xa7, 0x87, 0xb5, 0xdb, 0xd0, 0x7a, 0xe1, 0x45, 0x1e, 0xe4, 0x46, 0x57, 0x33,
        0x28, 0x2b, 0x7a, 0xa9, 0x8e, 0x77, 0xfc, 0xf6, 0x64, 0xec, 0x75, 0x7a, 0xb6, 0x4d, 0x82, 0x31,
        0x19, 0xe9, 0xdb, 0x89, 0x86, 0x82, 0x28, 0xf9, 0xd5, 0x69, 0xb4, 0x28, 0xf8, 0x3c, 0xbd, 0xf7,
        0xe2, 0x4a, 0x76, 0x8d, 0x7e, 0x39, 0xe7, 0xcf, 0x93, 0x8f, 0xf2, 0x20, 0x53, 0xf7, 0x8d, 0x6c,
        0x08, 0x7a, 0xab, 0x6e, 0xc7, 0x7b, 0x89, 0xd4, 0xfe, 0xb9, 0x8d, 0x3e, 0x4a, 0xbe, 0xd8, 0x7f,
        0x0b, 0x30, 0x14, 0xa5, 0x1f, 0x32, 0xd7, 0x85, 0x59, 0xef, 0x5d, 0xf4, 0xea, 0x16, 0xeb, 0xf6,
        0xda, 0xba, 0xf2, 0x86, 0xe3, 0x7d, 0xf4, 0xa6, 0xa6, 0x35, 0x73, 0x9a, 0x35, 0xbd, 0xbd, 0x8c,
        0x49, 0x09, 0xcd, 0x24, 0x9e, 0x9a, 0x33, 0x9b, 0xb9, 0xb3, 0x0d, 0x27, 0x37, 0x61, 0x47, 0xb2,
        0xe2, 0xf9, 0xd1, 0x07, 0xd0, 0x4b, 0x2d, 0x60, 0xd9, 0x09, 0xc4, 0xcc, 0x88, 0xda, 0xa6, 0xef,
        0x56, 0xd0, 0x76, 0xdc, 0xac, 0xd4, 0x0c, 0xf2, 0xc6, 0x56, 0x79, 0x6e, 0x69, 0xb8, 0xda, 0xf0,
        0xe8, 0x7d, 0xf4, 0xab, 0x5a, 0xae, 0x94, 0xf4, 0xe7, 0xba, 0xde, 0x55, 0xc5, 0x7f, 0xe8, 0xff,
        0x4b, 0xe8, 0xff, 0x58, 0x0e, 0x86, 0x52, 0x10, 0xaa, 0xfe, 0x94, 0xdd, 0x14, 0xec, 0x9e, 0xfa,
        0xae, 0xc3, 0x2e, 0x66, 0x5f, 0x72, 0x2c, 0x52, 0xa6, 0xa1, 0xfd, 0x87, 0x28, 0x6e, 0x0c, 0xe3,
        0x46, 0x49, 0xf0, 0xf7, 0x26, 0xa0, 0x49, 0xdc, 0x12, 0x7d, 0x91, 0x1f, 0x64, 0x56, 0x44, 0xee,
        0x42, 0x94, 0x8e, 0x36, 0x86, 0x7f, 0x8d, 0x5e, 0x75, 0x70, 0xb6, 0xb8, 0x1a, 0x82, 0x68, 0x90,
        0xb1, 0xe9, 0x5e, 0x86, 0x66, 0xc7, 0xe2, 0x23, 0xaf, 0xf1, 0x01, 0xc9, 0x7b, 0x18, 0xb3, 0xa8,
        0x5a, 0x02, 0x81, 0x2c, 0xac, 0x81, 0x0d, 0x11, 0x3d, 0x46, 0x02, 0x50, 0xe8, 0x28, 0xb6, 0xf8,
        0x99, 0x80, 0x94, 0x59, 0xba, 0x04, 0x8e, 0x1e, 0xa1, 0xaf, 0x40, 0x22, 0x84, 0xa1, 0xcd, 0xce,
        0x19, 0x83, 0xd5, 0xa9, 0xdb, 0xd3, 0xf0, 0x76, 0x63, 0x73, 0x8c, 0x18, 0xe0, 0xc3, 0xd4, 0xd1,
        0x86, 0xd2, 0x8b, 0xc8, 0x44, 0x3c, 0x94, 0x26, 0xbb, 0x76, 0x5c, 0x2e, 0xd1, 0xb3, 0x4b, 0x9c,
        0xc0, 0x6f, 0x03, 0x68, 0x91, 0x72, 0xa6, 0xa3, 0xfb, 0x25, 0xff, 0x24, 0x26, 0xcb, 0x76, 0xb4,
        0x39, 0x99, 0x0f, 0x1a, 0x6b, 0x70, 0x8e, 0x52, 0xd0, 0xae, 0x28, 0x8a, 0xba, 0x57, 0xd4, 0x96,
        0x61, 0x7b, 0xfb, 0xcd, 0x94, 0x41, 0x54, 0xd5, 0x2d, 0x8c, 0x47, 0xb5, 0xd2, 0xb1, 0xb3, 0x99,
        0xb6, 0xee, 0xb9, 0xc5, 0xb7, 0xc1, 0xdf, 0x37, 0x89, 0x9e, 0xf2, 0xdd, 0x85, 0xa3, 0x1c, 0x67,
        0xda, 0xaa, 0x67, 0x3e, 0x40, 0x6f, 0x5e, 0x98, 0xa5, 0xd5, 0x13, 0x17, 0x54, 0x16, 0x65, 0xce,
        0xa9, 0x2c, 0x0f, 0xd9, 0x6b, 0xb5, 0xa2, 0x44, 0xca, 0xbb, 0x21, 0x82, 0xde, 0xe3, 0x52, 0x18,
        0xf7, 0xa3, 0x0a, 0x69, 0x18, 0x4f, 0x0d, 0x1b, 0xd0, 0x61, 0xaf, 0x3a, 0xa3, 0xd7, 0x2a, 0x8a,
        0xbf, 0x61, 0x43, 0xd9, 0x53, 0x57, 0x43, 0xeb, 0x30, 0xeb, 0xee, 0x5b, 0x85, 0x67, 0x1a, 0x7b,
        0x67, 0x52, 0x34, 0xb9, 0xa0, 0x5a, 0x34, 0x88, 0xb1, 0x61, 0xf1, 0x76, 0x13, 0xa7, 0x18, 0x95,
        0x48, 0x88, 0x12, 0x9c, 0xc7, 0xde, 0x26, 0x43, 0xe9, 0x27, 0xd8, 0x80, 0xf6, 0x5d, 0x5a, 0xd6,
        0x50, 0xa7, 0x02, 0xa3, 0x33, 0xc2, 0xc3, 0x5c, 0xb8, 0x47, 0x2f, 0x6b, 0xfd, 0x76, 0x54, 0x3c,
        0x9c, 0xe8, 0x2d, 0xc9, 0x9f, 0x23, 0x4a, 0xfa, 0x4c, 0xc8, 0xd5, 0xd9, 0x30, 0xb3, 0x9f, 0xb9,
        0x44, 0xba, 0x43, 0x64, 0x1f, 0x2d, 0xce, 0xdb, 0xbd, 0x1e, 0xf6, 0x11, 0xbd, 0xdd, 0xd1, 0xd1,
        0x34, 0xbf, 0xf7, 0x90, 0x0b, 0xde, 0xec, 0xe1, 0x66, 0x7b, 0x76, 0xf4, 0x0c, 0x3d, 0x27, 0xb4,
        0xac, 0x32, 0xec, 0x63, 0xaf, 0xee, 0x68, 0x46, 0x38, 0xc3, 0x56, 0xd3, 0x88, 0xe7, 0x0b, 0x30,
        0x31, 0x0b, 0x4c, 0x86, 0x81, 0x26, 0x3b, 0x7a, 0x70, 0x51, 0x3b, 0x45, 0x2f, 0x9b, 0x1d, 0x22,
        0x40, 0xbe, 0x84, 0xe4, 0x23, 0xa1, 0x7b, 0x0f, 0xa2, 0x11, 0x6b, 0x70, 0x4d, 0x2b, 0x3a, 0x8a,
        0xec, 0x01, 0x57, 0x8c, 0x24, 0x27, 0x9d, 0xcd, 0x81, 0x02, 0x31, 0x8b, 0x5f, 0x86, 0xfd, 0x8b,
        0x38, 0xf6, 0x6e, 0xbe, 0x09, 0xdb, 0x5d, 0x0b, 0x35, 0x51, 0x05, 0x88, 0x2c, 0x30, 0x2c, 0x1e,
        0xa9, 0xc7, 0xe2, 0x83, 0x98, 0xac, 0x2b, 0xaf, 0x89, 0xc6, 0x97, 0xf6, 0x8a, 0x33, 0xf4, 0xab,
        0xc2, 0xda, 0x12, 0x52, 0xf2, 0x1d, 0x41, 0x1d, 0xe4, 0x04, 0x3d, 0x11, 0xc6, 0x65, 0xc5, 0xa3,
        0x8d, 0xcd, 0xad, 0x8c, 0x7e, 0x1e, 0xa6, 0xa5, 0x2e, 0xfe, 0x8f, 0xd0, 0xa3, 0x99, 0xc3, 0x03,
        0x1a, 0xef, 0xd0, 0x9f, 0x1c, 0x6d, 0x6c, 0x6e, 0x5d, 0x6d, 0x2d, 0x7d, 0x3f, 0xfa, 0x65, 0x85,
        0xac, 0x2f, 0xf9, 0x43, 0xf4, 0x6a, 0xea, 0xf9, 0x63, 0x39, 0x57, 0xd6, 0x0c, 0xb6, 0x50, 0xf9,
        0x90, 0xd6, 0xf4, 0xed, 0x81, 0xe0, 0x4a, 0x45, 0x18, 0xeb, 0x1f, 0x1e, 0x05, 0x2d, 0x16, 0x62,
        0x03, 0xfc, 0x8e, 0xea, 0xba, 0x35, 0x7f, 0x93, 0x60, 0xbb, 0x21, 0xd3, 0x70, 0x9f, 0x3c, 0xe8,
        0x0f, 0x60, 0x54, 0x05, 0xb6, 0x7b, 0xac, 0xd2, 0xbe, 0x39, 0x6d, 0x31, 0xc7, 0x6e, 0x36, 0xd7,
        0xc0, 0xcc, 0x19, 0xbd, 0xbb, 0x90, 0xb3, 0x07, 0x6b, 0x1b, 0x6c, 0xdd, 0xf2, 0x92, 0x3e, 0xd0,
        0x3b, 0x4d, 0x96, 0x45, 0x78, 0x58, 0xb7, 0xcc, 0x36, 0x1b, 0x32, 0xa1, 0x3f, 0x79, 0xd0, 0x5b,
        0x8e, 0x31, 0xe4, 0xe4, 0x5e, 0x5a, 0x8b, 0xcf, 0x40, 0xdc, 0xa5, 0x11, 0x3d, 0x31, 0x68, 0xed,
        0x67, 0xc9, 0xea, 0x97, 0x16, 0x7e, 0x31, 0x5b, 0x0d, 0x10, 0xe5, 0x5a, 0x62, 0xc9, 0xa2, 0x27,
        0x4b, 0x49, 0x29, 0xe1, 0xb9, 0x4a, 0x9b, 0x14, 0xd1, 0xd1, 0x3f, 0x72, 0x27, 0xb3, 0x32, 0xa2,
        0xcb, 0x09, 0x78, 0xb8, 0x4d, 0x09, 0x9b, 0xe1, 0x2a, 0xfe, 0x56, 0x8f, 0xcc, 0x7a, 0x18, 0xa1,
        0x55, 0xb9, 0x82, 0xec, 0x94, 0x2d, 0xb7, 0x20, 0xed, 0x6f, 0xb5, 0x3a, 0x88, 0xb5, 0x72, 0xfb,
        0xd9, 0x14, 0x30, 0xa1, 0x00, 0xdd, 0xfa, 0x75, 0x92, 0xd0, 0xc9, 0x2f, 0x87, 0x2f, 0x2e, 0x3c,
        0x68, 0x80, 0x45, 0x86, 0x57, 0xae, 0x53, 0x0c, 0xc8, 0xd2, 0x9f, 0x0f, 0xdd, 0x8e, 0x14, 0x3d,
        0xeb, 0xd8, 0xe7, 0x30, 0xb1, 0x9d, 0x34, 0x47, 0x65, 0x99, 0x3f, 0xce, 0x94, 0xd0, 0x55, 0x1f,
        0xec, 0x45, 0x68, 0xe5, 0x21, 0x64, 0x73, 0xec, 0xdc, 0x42, 0x8f, 0xa1, 0x4a, 0xb2, 0x5f, 0xff,
        0xb2, 0xe4, 0xeb, 0xd8, 0x97, 0x29, 0x5e, 0x69, 0x9d, 0xec, 0xa0, 0xe7, 0x47, 0xc2, 0x27, 0x31,
        0x75, 0x22, 0x3b, 0x1b, 0xe8, 0xe1, 0x93, 0x0f, 0xd2, 0x6e, 0x44, 0xff, 0x54, 0xf2, 0x29, 0x7f,
        0x3e, 0xe0, 0xc5, 0x59, 0x6f, 0x1d, 0x85, 0x73, 0x9b, 0x8a, 0x39, 0x97, 0x6b, 0x0a, 0xc2, 0x3b,
        0x4b, 0xd1, 0x02, 0xc9, 0xb6, 0xe1, 0x34, 0xde, 0x7b, 0xf0, 0x3b, 0x7b, 0x90, 0x9a, 0x2b, 0x9b,
        0xd3, 0xa2, 0x72, 0x9e, 0xac, 0x7e, 0xa7, 0x70, 0xa0, 0xe9, 0x1b, 0x03, 0x67, 0xa9, 0x19, 0xf7,
        0x39, 0x85, 0xeb, 0x9c, 0x3e, 0xe0, 0xc1, 0x3a, 0xc5, 0x9f, 0x2d, 0xd4, 0x5e, 0x0f, 0x88, 0x20,
        0xca, 0x3a, 0xc0, 0x45, 0xd2, 0x76, 0x55, 0xe0, 0x07, 0x7e, 0xec, 0xf4, 0x72, 0xcb, 0xfd, 0xca,
        0x9b, 0xe8, 0x91, 0xbd, 0xfe, 0x3e, 0xf4, 0x77, 0xb0, 0x7f, 0x3a, 0xfa, 0x3f, 0x96, 0x73, 0x49,
        0x2e, 0x4c, 0xdd, 0x5e, 0xb0, 0x9c, 0x4d, 0x9e, 0x21, 0x9a, 0x7e, 0xa3, 0xe0, 0x6e, 0x66, 0x04,
        0xc9, 0xa9, 0xd1, 0x37, 0x70, 0x1c, 0x74, 0x66, 0xdb, 0x20, 0x17, 0xca, 0x56, 0x2e, 0xd1, 0x5f,
        0xf3, 0x0c, 0xd9, 0xed, 0x77, 0x2f, 0x37, 0x09, 0x90, 0x57, 0xdd, 0xa5, 0x3c, 0x22, 0x5b, 0xfc,
        0x16, 0xc7, 0x41, 0x67, 0xb6, 0x17, 0x58, 0x8e, 0x2b, 0x7b, 0xfb, 0xcf, 0x8c, 0x65, 0x5a, 0xbc,
        0xdc, 0x64, 0xc7, 0x4e, 0xdc, 0xa3, 0x7a, 0xdf, 0x3e, 0x21, 0x59, 0x66, 0xa8, 0x79, 0x8d, 0xe3,
        0xe0, 0x8e, 0x17, 0xc4, 0xe4, 0x3c, 0x4b, 0x9f, 0xea, 0x87, 0x1e, 0x2a, 0x86, 0x01, 0x49, 0xf8,
        0x51, 0x8f, 0xdd, 0xa2, 0x2a, 0x32, 0x96, 0xe7, 0x1c, 0x27, 0xa2, 0x0f, 0x21, 0x08, 0xdb, 0xa9,
        0x13, 0x13, 0x5e, 0x7c, 0x42, 0x6a, 0xf1, 0x89, 0x89, 0x84, 0xde, 0xe9, 0xd6, 0x43, 0x3d, 0x97,
        0x3d, 0x48, 0x66, 0x8f, 0xe3, 0x84, 0xc0, 0x92, 0x09, 0xfd, 0xab, 0x7a, 0x7e, 0xe4, 0x78, 0xf3,
        0xf7, 0x48, 0x52, 0x3a, 0xa3, 0xa7, 0x47, 0x8d, 0x07, 0xa5, 0x5d, 0xbf, 0x0f, 0xc3, 0x9f, 0x41,
        0xff, 0x82, 0x9e, 0xff, 0xf8, 0x99, 0xcf, 0xa1, 0xb7, 0x20, 0x9c, 0xf2, 0x01, 0xf4, 0x2f, 0x4b,
        0xfe, 0x0e, 0x21, 0x01, 0xe3, 0x6f, 0x44, 0x3f, 0x7d, 0xf2, 0x60, 0xb7, 0x28, 0x77, 0x60, 0xde,
        0xe4, 0x38, 0x5c, 0x13, 0xab, 0xd2, 0x0b, 0xe2, 0xaf, 0xb7, 0x08, 0xc2, 0xb9, 0x4d, 0x8d, 0x47,
        0xd4, 0x1d, 0x62, 0x12, 0x02, 0x66, 0xd0, 0xc1, 0xec, 0x3b, 0x38, 0x0e, 0x7b, 0xa4, 0x57, 0xb8,
        0x90, 0x7d, 0x37, 0xde, 0x64, 0x39, 0x7b, 0x1f, 0xc6, 0x6a, 0xdf, 0xac, 0xe7, 0xc6, 0x70, 0x3a,
        0x4a, 0x2b, 0xde, 0xe7, 0x58, 0xce, 0xcf, 0x44, 0x8f, 0x7b, 0x10, 0x9c, 0x6c, 0x3c, 0x9f, 0x64,
        0x39, 0x3f, 0x13, 0x3d, 0x7e, 0x3d, 0xa4, 0x24, 0x1b, 0xcf, 0xef, 0x66, 0x39, 0xff, 0x03, 0xca,
        0x1b, 0xaa, 0xdf, 0x6a, 0xd1, 0xba, 0xa4, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae,
        0x42, 0x60, 0x82};

    int mg_font_size = (int)sizeof(mg_font);

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_internal.h

#define _CRT_SECURE_NO_WARNINGS NOPE

    // Creates a new bitmap, with extra payload bytes.
    MgSurface *mgBitmap2(int w, int h, int extra);

    // Resizes an existing bitmap.
    void mgResize(MgSurface *surface, int w, int h);

    // Calculates the biggest scale that a bitmap can fit into an area at.
    int mgCalcScale(int bmpW, int bmpH, int areaW, int areaH);

    // Calculates a new scale, taking minimum-scale flags into account.
    int mgEnforceScale(int scale, int flags);

    // Calculates the correct position for a bitmap to fit into a window.
    void mgPosition(MgSurface *surface, int scale, int windowW, int windowH, int out[4]);

// ----------------------------------------------------------
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#endif

#if __linux__
#include <X11/X.h>
#include <X11/Xlib.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#endif

    typedef struct
    {

#ifdef _WIN32
        HGLRC hglrc;
        HDC dc;
#endif
        GLuint tex[2];
        GLuint vao;
        GLuint program;
        GLuint uniform_projection;
        GLuint uniform_model;
        GLuint uniform_parameters;
        int gl_legacy;
        int gl_user_opengl_rendering;
    } GLStuff;

    typedef struct
    {
        int shown, closed;
        GLStuff gl;

#ifdef _WIN32
        wchar_t *wtitle;
        DWORD dwStyle;
        RECT oldPos;
#endif // _WIN32

#ifdef __linux__
        Display *dpy;
        Window win;
        GLXContext glc;
        XIC ic;
#endif // __linux__

        MgSurface *widgets;
        int widgetsWanted;
        unsigned char widgetAlpha;
        float widgetsScale;

        float p1, p2, p3, p4;

        int flags;
        int scale;
        int pos[4];
        int lastChar;
        char keys[256], prev[256];

#if defined(__linux__)
        int mouseButtons;
        int mouseX;
        int mouseY;
#endif // __linux__

    } MgInternal;

    // ----------------------------------------------------------

    MgInternal *mgInternal(MgSurface *surface);

    void mgGAPICreate(MgSurface *surface);
    void mgGAPIDestroy(MgSurface *surface);
    int mgGAPIBegin(MgSurface *surface);
    int mgGAPIEnd(MgSurface *surface);
    void mgGAPIPresent(MgSurface *surface, int w, int h);

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_upscale_gl_fs.h

    // clang-format off
const char mg_upscale_gl_fs[] = {
    "#version 330 core\n"
    "in vec2 uv;"
    "out vec4 color;"
    "uniform sampler2D image;"
    "uniform vec4 parameters;"
    "void fxShader(out vec4 color, in vec2 coord);"
    "void main()"
    "{"
    "   fxShader(color, uv);"
    "}\n"
};
    // clang-format on

    const int mg_upscale_gl_fs_size = (int)sizeof(mg_upscale_gl_fs) - 1;

    // clang-format off
const char mg_default_fx_gl_fs[] = {
    "void fxShader(out vec4 color, in vec2 uv) {"
    "   vec2 tex_size = vec2(textureSize(image, 0));"
    "   vec2 uv_blur = mix(floor(uv * tex_size) + 0.5, uv * tex_size, parameters.xy) / tex_size;"
    "   vec4 c = texture(image, uv_blur);"
    "   c.rgb *= mix(0.5, 1.0 - fract(uv.y * tex_size.y), parameters.z) * 2.0; //scanline\n"
    "   c = mix(vec4(0.5), c, parameters.w); //contrast\n"
    "   color = c;"
    "}"
};
    // clang-format on

    const int mg_default_fx_gl_fs_size = (int)sizeof(mg_default_fx_gl_fs) - 1;

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_upscale_gl_vs.h

    // clang-format off
const char mg_upscale_gl_vs[] = {
    "#version 330 core\n"
    "layout (location = 0) in vec2 pos_in;"
    "layout (location = 1) in vec2 uv_in;"
    "out vec2 uv;"
    "uniform mat4 model;"
    "uniform mat4 projection;"
    "void main()"
    "{"
    "   uv = uv_in;"
    "   gl_Position = projection * model * vec4(pos_in, 0.0, 1.0);"
    "}"
};
    // clang-format on

    const int mg_upscale_gl_vs_size = (int)sizeof(mg_upscale_gl_vs) - 1;

// #######################################################################################################################################
// #######################################################################################################################################
// #######################################################################################################################################

// mg_bitmaps.c

// Expands 0-255 into 0-256
#define EXPAND(X) ((X) + ((X) > 0))

#define CLIP0(CX, X, X2, W) \
    if (X < CX)             \
    {                       \
        int D = CX - X;     \
        W -= D;             \
        X2 += D;            \
        X += D;             \
    }
#define CLIP1(X, DW, W) \
    if (X + W > DW)     \
        W = DW - X;
#define CLIP()                  \
    CLIP0(dst->cx, dx, sx, w);  \
    CLIP0(dst->cy, dy, sy, h);  \
    CLIP0(0, sx, dx, w);        \
    CLIP0(0, sy, dy, h);        \
    CLIP1(dx, dst->cx + cw, w); \
    CLIP1(dy, dst->cy + ch, h); \
    CLIP1(sx, src->w, w);       \
    CLIP1(sy, src->h, h);       \
    if (w <= 0 || h <= 0)       \
    return

    MgSurface *mgBitmap2(int w, int h, int extra)
    {
        MgSurface *surface = (MgSurface *)calloc(1, sizeof(MgSurface) + extra);
        surface->w = w;
        surface->h = h;
        surface->cw = -1;
        surface->ch = -1;
        surface->pixels = (MgPixel *)calloc(w * h, sizeof(MgPixel));
        surface->blitMode = MG_BLEND_ALPHA;
        return surface;
    }

    MgSurface *mgBitmap(int w, int h)
    {
        return mgBitmap2(w, h, 0);
    }

    void mgResize(MgSurface *surface, int w, int h)
    {
        if (surface->w == w && surface->h == h)
        {
            return;
        }

        int y, cw, ch;
        MgPixel *newpix = (MgPixel *)calloc(w * h, sizeof(MgPixel));
        cw = (w < surface->w) ? w : surface->w;
        ch = (h < surface->h) ? h : surface->h;

        // Copy any old data across.
        for (y = 0; y < ch; y++)
            memcpy(newpix + y * w, surface->pixels + y * surface->w, cw * sizeof(MgPixel));

        free(surface->pixels);
        surface->pixels = newpix;
        surface->w = w;
        surface->h = h;
    }

    int mgCalcScale(int bmpW, int bmpH, int areaW, int areaH)
    {
        // We want it as big as possible in the window, but still
        // maintaining the correct aspect ratio, and always
        // having an integer pixel size.
        int scale = 0;
        for (;;)
        {
            scale++;
            if (bmpW * scale > areaW || bmpH * scale > areaH)
            {
                scale--;
                break;
            }
        }
        return (scale > 1) ? scale : 1;
    }

    int mgEnforceScale(int scale, int flags)
    {
        if ((flags & MG_4X) && scale < 4)
            scale = 4;
        if ((flags & MG_3X) && scale < 3)
            scale = 3;
        if ((flags & MG_2X) && scale < 2)
            scale = 2;
        return scale;
    }

    void mgPosition(MgSurface *surface, int scale, int windowW, int windowH, int out[4])
    {
        // Center the image on screen at this scale.
        out[0] = (windowW - surface->w * scale) / 2;
        out[1] = (windowH - surface->h * scale) / 2;
        out[2] = out[0] + surface->w * scale;
        out[3] = out[1] + surface->h * scale;
    }

    void mgClear(MgSurface *surface, MgPixel color)
    {
        int count = surface->w * surface->h;
        int n;
        for (n = 0; n < count; n++)
            surface->pixels[n] = color;
    }

    void mgFill(MgSurface *surface, int x, int y, int w, int h, MgPixel color)
    {
        MgPixel *td;
        int dt, i;

        if (x < 0)
        {
            w += x;
            x = 0;
        }
        if (y < 0)
        {
            h += y;
            y = 0;
        }
        if (x + w > surface->w)
        {
            w = surface->w - x;
        }
        if (y + h > surface->h)
        {
            h = surface->h - y;
        }
        if (w <= 0 || h <= 0)
            return;

        td = &surface->pixels[y * surface->w + x];
        dt = surface->w;
        do
        {
            for (i = 0; i < w; i++)
                td[i] = color;
            td += dt;
        } while (--h);
    }

    void mgLine(MgSurface *surface, int x0, int y0, int x1, int y1, MgPixel color)
    {
        int sx, sy, dx, dy, err, e2;
        dx = abs(x1 - x0);
        dy = abs(y1 - y0);
        if (x0 < x1)
            sx = 1;
        else
            sx = -1;
        if (y0 < y1)
            sy = 1;
        else
            sy = -1;
        err = dx - dy;

        do
        {
            mgPlot(surface, x0, y0, color);
            e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y0 += sy;
            }
        } while (x0 != x1 || y0 != y1);
    }

    void mgFillRect(MgSurface *surface, int x, int y, int w, int h, MgPixel color)
    {
        x += 1;
        y += 1;
        w -= 2;
        h -= 2;

        int cx = surface->cx;
        int cy = surface->cy;
        int cw = surface->cw >= 0 ? surface->cw : surface->w;
        int ch = surface->ch >= 0 ? surface->ch : surface->h;

        if (x < cx)
        {
            w += (x - cx);
            x = cx;
        }
        if (y < cy)
        {
            h += (y - cy);
            y = cy;
        }
        if (x + w > cx + cw)
        {
            w -= (x + w) - (cx + cw);
        }
        if (y + h > cy + ch)
        {
            h -= (y + h) - (cy + ch);
        }
        if (w <= 0 || h <= 0)
            return;

        MgPixel *td = &surface->pixels[y * surface->w + x];
        int dt = surface->w;
        int xa = EXPAND(color.a);
        int a = xa * xa;

        do
        {
            for (int i = 0; i < w; i++)
            {
                td[i].r += (unsigned char)((color.r - td[i].r) * a >> 16);
                td[i].g += (unsigned char)((color.g - td[i].g) * a >> 16);
                td[i].b += (unsigned char)((color.b - td[i].b) * a >> 16);
                td[i].a += (surface->blitMode) * (unsigned char)((color.a - td[i].a) * a >> 16);
            }
            td += dt;
        } while (--h);
    }

    void mgRect(MgSurface *surface, int x, int y, int w, int h, MgPixel color)
    {
        int x1, y1;
        if (w <= 0 || h <= 0)
        {
            return;
        }

        if (w == 1)
        {
            mgLine(surface, x, y, x, y + h, color);
        }
        else if (h == 1)
        {
            mgLine(surface, x, y, x + w, y, color);
        }
        else
        {
            x1 = x + w - 1;
            y1 = y + h - 1;
            mgLine(surface, x, y, x1, y, color);
            mgLine(surface, x1, y, x1, y1, color);
            mgLine(surface, x1, y1, x, y1, color);
            mgLine(surface, x, y1, x, y, color);
        }
    }

    void mgFillCircle(MgSurface *surface, int x0, int y0, int r, MgPixel color)
    {
        if (r <= 0)
        {
            return;
        }

        int E = 1 - r;
        int dx = 0;
        int dy = -2 * r;
        int x = 0;
        int y = r;

        mgLine(surface, x0 - r + 1, y0, x0 + r, y0, color);

        while (x < y - 1)
        {
            x++;

            if (E >= 0)
            {
                y--;
                dy += 2;
                E += dy;
                mgLine(surface, x0 - x + 1, y0 + y, x0 + x, y0 + y, color);
                mgLine(surface, x0 - x + 1, y0 - y, x0 + x, y0 - y, color);
            }

            dx += 2;
            E += dx + 1;

            if (x != y)
            {
                mgLine(surface, x0 - y + 1, y0 + x, x0 + y, y0 + x, color);
                mgLine(surface, x0 - y + 1, y0 - x, x0 + y, y0 - x, color);
            }
        }
    }

    void mgCircle(MgSurface *surface, int x0, int y0, int r, MgPixel color)
    {
        int E = 1 - r;
        int dx = 0;
        int dy = -2 * r;
        int x = 0;
        int y = r;

        mgPlot(surface, x0, y0 + r, color);
        mgPlot(surface, x0, y0 - r, color);
        mgPlot(surface, x0 + r, y0, color);
        mgPlot(surface, x0 - r, y0, color);

        while (x < y - 1)
        {
            x++;

            if (E >= 0)
            {
                y--;
                dy += 2;
                E += dy;
            }

            dx += 2;
            E += dx + 1;

            mgPlot(surface, x0 + x, y0 + y, color);
            mgPlot(surface, x0 - x, y0 + y, color);
            mgPlot(surface, x0 + x, y0 - y, color);
            mgPlot(surface, x0 - x, y0 - y, color);

            if (x != y)
            {
                mgPlot(surface, x0 + y, y0 + x, color);
                mgPlot(surface, x0 - y, y0 + x, color);
                mgPlot(surface, x0 + y, y0 - x, color);
                mgPlot(surface, x0 - y, y0 - x, color);
            }
        }
    }

    MgPixel mgGet(MgSurface *surface, int x, int y)
    {
        MgPixel empty = {0, 0, 0, 0};
        if (x >= 0 && y >= 0 && x < surface->w && y < surface->h)
            return surface->pixels[y * surface->w + x];
        return empty;
    }

    void mgPlot(MgSurface *surface, int x, int y, MgPixel pixel)
    {
        int xa, i, a;

        int cx = surface->cx;
        int cy = surface->cy;
        int cw = surface->cw >= 0 ? surface->cw : surface->w;
        int ch = surface->ch >= 0 ? surface->ch : surface->h;

        if (x >= cx && y >= cy && x < cx + cw && y < cy + ch)
        {
            xa = EXPAND(pixel.a);
            a = xa * xa;
            i = y * surface->w + x;

            surface->pixels[i].r += (unsigned char)((pixel.r - surface->pixels[i].r) * a >> 16);
            surface->pixels[i].g += (unsigned char)((pixel.g - surface->pixels[i].g) * a >> 16);
            surface->pixels[i].b += (unsigned char)((pixel.b - surface->pixels[i].b) * a >> 16);
            surface->pixels[i].a += (surface->blitMode) * (unsigned char)((pixel.a - surface->pixels[i].a) * a >> 16);
        }
    }

    void mgClip(MgSurface *surface, int cx, int cy, int cw, int ch)
    {
        surface->cx = cx;
        surface->cy = cy;
        surface->cw = cw;
        surface->ch = ch;
    }

    void mgBlit(MgSurface *dst, MgSurface *src, int dx, int dy, int sx, int sy, int w, int h)
    {
        int cw = dst->cw >= 0 ? dst->cw : dst->w;
        int ch = dst->ch >= 0 ? dst->ch : dst->h;

        CLIP();

        MgPixel *ts = &src->pixels[sy * src->w + sx];
        MgPixel *td = &dst->pixels[dy * dst->w + dx];
        int st = src->w;
        int dt = dst->w;
        do
        {
            memcpy(td, ts, w * sizeof(MgPixel));
            ts += st;
            td += dt;
        } while (--h);
    }

    void mgBlitTint(MgSurface *dst, MgSurface *src, int dx, int dy, int sx, int sy, int w, int h, MgPixel tint)
    {
        int cw = dst->cw >= 0 ? dst->cw : dst->w;
        int ch = dst->ch >= 0 ? dst->ch : dst->h;

        CLIP();

        int xr = EXPAND(tint.r);
        int xg = EXPAND(tint.g);
        int xb = EXPAND(tint.b);
        int xa = EXPAND(tint.a);

        MgPixel *ts = &src->pixels[sy * src->w + sx];
        MgPixel *td = &dst->pixels[dy * dst->w + dx];
        int st = src->w;
        int dt = dst->w;
        do
        {
            for (int x = 0; x < w; x++)
            {
                unsigned r = (xr * ts[x].r) >> 8;
                unsigned g = (xg * ts[x].g) >> 8;
                unsigned b = (xb * ts[x].b) >> 8;
                unsigned a = xa * EXPAND(ts[x].a);
                td[x].r += (unsigned char)((r - td[x].r) * a >> 16);
                td[x].g += (unsigned char)((g - td[x].g) * a >> 16);
                td[x].b += (unsigned char)((b - td[x].b) * a >> 16);
                td[x].a += (dst->blitMode) * (unsigned char)((ts[x].a - td[x].a) * a >> 16);
            }
            ts += st;
            td += dt;
        } while (--h);
    }

    void mgBlitAlpha(MgSurface *dst, MgSurface *src, int dx, int dy, int sx, int sy, int w, int h, float alpha)
    {
        alpha = (alpha < 0) ? 0 : (alpha > 1 ? 1 : alpha);
        mgBlitTint(dst, src, dx, dy, sx, sy, w, h, mgRGBA(0xff, 0xff, 0xff, (unsigned char)(alpha * 255)));
    }

    void mgBlitMode(MgSurface *dst, int mode)
    {
        dst->blitMode = mode;
    }

#undef CLIP0
#undef CLIP1
#undef CLIP

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_loadpng.c

#include <errno.h>

    typedef struct
    {
        const unsigned char *p, *end;
    } PNG;

    static unsigned get32(const unsigned char *v)
    {
        return (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    }

    static const unsigned char *find(PNG *png, const char *chunk, unsigned minlen)
    {
        const unsigned char *start;
        while (png->p < png->end)
        {
            unsigned len = get32(png->p + 0);
            start = png->p;
            png->p += len + 12;
            if (memcmp(start + 4, chunk, 4) == 0 && len >= minlen && png->p <= png->end)
                return start + 8;
        }

        return NULL;
    }

    static unsigned char paeth(unsigned char a, unsigned char b, unsigned char c)
    {
        int p = a + b - c;
        int pa = abs(p - a), pb = abs(p - b), pc = abs(p - c);
        return (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b
                                                       : c;
    }

    static int rowBytes(int w, int bipp)
    {
        int rowBits = w * bipp;
        return rowBits / 8 + ((rowBits % 8) ? 1 : 0);
    }

    static int unfilter(int w, int h, int bipp, unsigned char *raw)
    {
        int len = rowBytes(w, bipp);
        int bpp = rowBytes(1, bipp);
        int x, y;
        unsigned char *first = (unsigned char *)malloc(len + 1);
        memset(first, 0, len + 1);
        unsigned char *prev = first;
        for (y = 0; y < h; y++, prev = raw, raw += len)
        {
#define LOOP(A, B)            \
    for (x = 0; x < bpp; x++) \
        raw[x] += A;          \
    for (; x < len; x++)      \
        raw[x] += B;          \
    break
            switch (*raw++)
            {
            case 0:
                break;
            case 1:
                LOOP(0, raw[x - bpp]);
            case 2:
                LOOP(prev[x], prev[x]);
            case 3:
                LOOP(prev[x] / 2, (raw[x - bpp] + prev[x]) / 2);
            case 4:
                LOOP(prev[x], paeth(raw[x - bpp], prev[x], prev[x - bpp]));
            default:
                return 0;
            }
#undef LOOP
        }
        free(first);
        return 1;
    }

    static void convert(int bypp, int w, int h, const unsigned char *src, MgPixel *dest, const unsigned char *trns)
    {
        int x, y;
        for (y = 0; y < h; y++)
        {
            src++; // skip filter byte
            for (x = 0; x < w; x++, src += bypp)
            {
                switch (bypp)
                {
                case 1:
                {
                    unsigned char c = src[0];
                    if (trns && c == *trns)
                    {
                        *dest++ = mgRGBA(c, c, c, 0);
                        break;
                    }
                    else
                    {
                        *dest++ = mgRGB(c, c, c);
                        break;
                    }
                }
                case 2:
                    *dest++ = mgRGBA(src[0], src[0], src[0], src[1]);
                    break;
                case 3:
                {
                    unsigned char r = src[0];
                    unsigned char g = src[1];
                    unsigned char b = src[2];
                    if (trns && trns[1] == r && trns[3] == g && trns[5] == b)
                    {
                        *dest++ = mgRGBA(r, g, b, 0);
                        break;
                    }
                    else
                    {
                        *dest++ = mgRGB(r, g, b);
                        break;
                    }
                }
                case 4:
                    *dest++ = mgRGBA(src[0], src[1], src[2], src[3]);
                    break;
                }
            }
        }
    }

    static void depalette(int w,
                          int h,
                          unsigned char *src,
                          MgPixel *dest,
                          int bipp,
                          const unsigned char *plte,
                          const unsigned char *trns,
                          int trnsSize)
    {
        int x, y, c;
        unsigned char alpha;
        int mask = 0, len = 0;

        switch (bipp)
        {
        case 4:
            mask = 15;
            len = 1;
            break;
        case 2:
            mask = 3;
            len = 3;
            break;
        case 1:
            mask = 1;
            len = 7;
        }

        for (y = 0; y < h; y++)
        {
            src++; // skip filter byte
            for (x = 0; x < w; x++)
            {
                if (bipp == 8)
                {
                    c = *src++;
                }
                else
                {
                    int pos = x & len;
                    c = (src[0] >> ((len - pos) * bipp)) & mask;
                    if (pos == len)
                    {
                        src++;
                    }
                }
                alpha = 255;
                if (c < trnsSize)
                {
                    alpha = trns[c];
                }
                *dest++ = mgRGBA(plte[c * 3 + 0], plte[c * 3 + 1], plte[c * 3 + 2], alpha);
            }
        }
    }

#define FAIL()          \
    {                   \
        errno = EINVAL; \
        goto err;       \
    }
#define CHECK(X) \
    if (!(X))    \
    FAIL()

    static int outsize(MgSurface *surface, int bipp)
    {
        return (rowBytes(surface->w, bipp) + 1) * surface->h;
    }

    static MgSurface *mgLoadPng(PNG *png)
    {
        const unsigned char *ihdr, *idat, *plte, *trns, *first;
        int trnsSize = 0;
        int depth, ctype, bipp;
        int datalen = 0;
        unsigned char *data = NULL, *out;
        MgSurface *surface = NULL;

        CHECK(memcmp(png->p, "\211PNG\r\n\032\n", 8) == 0); // PNG signature
        png->p += 8;
        first = png->p;

        // Read IHDR
        ihdr = find(png, "IHDR", 13);
        CHECK(ihdr);
        depth = ihdr[8];
        ctype = ihdr[9];
        switch (ctype)
        {
        case 0:
            bipp = depth;
            break; // greyscale
        case 2:
            bipp = 3 * depth;
            break; // RGB
        case 3:
            bipp = depth;
            break; // paletted
        case 4:
            bipp = 2 * depth;
            break; // grey+alpha
        case 6:
            bipp = 4 * depth;
            break; // RGBA
        default:
            FAIL();
        }

        // Allocate bitmap (+1 width to save room for stupid PNG filter bytes)
        surface = mgBitmap(get32(ihdr + 0) + 1, get32(ihdr + 4));
        CHECK(surface);
        surface->w--;

        // We support 8-bit color components and 1, 2, 4 and 8 bit palette formats.
        // No interlacing, or wacky filter types.
        CHECK((depth != 16) && ihdr[10] == 0 && ihdr[11] == 0 && ihdr[12] == 0);

        // Join IDAT chunks.
        for (idat = find(png, "IDAT", 0); idat; idat = find(png, "IDAT", 0))
        {
            unsigned len = get32(idat - 8);
            data = (unsigned char *)realloc(data, datalen + len);
            if (!data)
                break;

            memcpy(data + datalen, idat, len);
            datalen += len;
        }

        // Find palette.
        png->p = first;
        plte = find(png, "PLTE", 0);

        // Find transparency info.
        png->p = first;
        trns = find(png, "tRNS", 0);
        if (trns)
        {
            trnsSize = get32(trns - 8);
        }

        CHECK(data && datalen >= 6);
        CHECK((data[0] & 0x0f) == 0x08    // compression method (RFC 1950)
              && (data[0] & 0xf0) <= 0x70 // window size
              && (data[1] & 0x20) == 0);  // preset dictionary present

        out = (unsigned char *)surface->pixels + outsize(surface, 32) - outsize(surface, bipp);
        CHECK(mgInflate(out, outsize(surface, bipp), data + 2, datalen - 6));
        CHECK(unfilter(surface->w, surface->h, bipp, out));

        if (ctype == 3)
        {
            CHECK(plte);
            depalette(surface->w, surface->h, out, surface->pixels, bipp, plte, trns, trnsSize);
        }
        else
        {
            CHECK(bipp % 8 == 0);
            convert(bipp / 8, surface->w, surface->h, out, surface->pixels, trns);
        }

        free(data);
        return surface;

    err:
        if (data)
            free(data);
        if (surface)
            mgFree(surface);
        return NULL;
    }

#undef CHECK
#undef FAIL

    MgSurface *mgLoadImageMem(const void *data, int length)
    {
        PNG png;
        png.p = (unsigned char *)data;
        png.end = (unsigned char *)data + length;
        return mgLoadPng(&png);
    }

    MgSurface *mgLoadImage(const char *fileName)
    {
        int len;
        void *data;
        MgSurface *surface;

        data = mgReadFile(fileName, &len);
        if (!data)
            return NULL;

        surface = mgLoadImageMem(data, len);
        free(data);
        return surface;
    }

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_savepng.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

    typedef struct
    {
        unsigned crc, adler, bits, prev, runlen;
        FILE *out;
        unsigned crcTable[256];
    } Save;

    static const unsigned crctable[16] = {0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4,
                                          0x4db26158, 0x5005713c, 0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
                                          0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c};

    static void put(Save *s, unsigned v)
    {
        fputc(v, s->out);
        s->crc = (s->crc >> 4) ^ crctable[(s->crc & 15) ^ (v & 15)];
        s->crc = (s->crc >> 4) ^ crctable[(s->crc & 15) ^ (v >> 4)];
    }

    static void updateAdler(Save *s, unsigned v)
    {
        unsigned s1 = s->adler & 0xffff, s2 = (s->adler >> 16) & 0xffff;
        s1 = (s1 + v) % 65521;
        s2 = (s2 + s1) % 65521;
        s->adler = (s2 << 16) + s1;
    }

    static void put32(Save *s, unsigned v)
    {
        put(s, (v >> 24) & 0xff);
        put(s, (v >> 16) & 0xff);
        put(s, (v >> 8) & 0xff);
        put(s, v & 0xff);
    }

    void putbits(Save *s, unsigned data, unsigned bitcount)
    {
        while (bitcount--)
        {
            unsigned prev = s->bits;
            s->bits = (s->bits >> 1) | ((data & 1) << 7);
            data >>= 1;
            if (prev & 1)
            {
                put(s, s->bits);
                s->bits = 0x80;
            }
        }
    }

    void putbitsr(Save *s, unsigned data, unsigned bitcount)
    {
        while (bitcount--)
            putbits(s, data >> bitcount, 1);
    }

    static void begin(Save *s, const char *id, unsigned len)
    {
        put32(s, len);
        s->crc = 0xffffffff;
        put(s, id[0]);
        put(s, id[1]);
        put(s, id[2]);
        put(s, id[3]);
    }

    static void literal(Save *s, unsigned v)
    {
        // Encode a literal/length using the built-in tables.
        // Could do better with a custom table but whatever.
        if (v < 144)
            putbitsr(s, 0x030 + v - 0, 8);
        else if (v < 256)
            putbitsr(s, 0x190 + v - 144, 9);
        else if (v < 280)
            putbitsr(s, 0x000 + v - 256, 7);
        else
            putbitsr(s, 0x0c0 + v - 280, 8);
    }

    static void encodelen(Save *s, unsigned code, unsigned bits, unsigned len)
    {
        literal(s, code + (len >> bits));
        putbits(s, len, bits);
        putbits(s, 0, 5);
    }

    static void endrun(Save *s)
    {
        s->runlen--;
        literal(s, s->prev);

        if (s->runlen >= 67)
            encodelen(s, 277, 4, s->runlen - 67);
        else if (s->runlen >= 35)
            encodelen(s, 273, 3, s->runlen - 35);
        else if (s->runlen >= 19)
            encodelen(s, 269, 2, s->runlen - 19);
        else if (s->runlen >= 11)
            encodelen(s, 265, 1, s->runlen - 11);
        else if (s->runlen >= 3)
            encodelen(s, 257, 0, s->runlen - 3);
        else
            while (s->runlen--)
                literal(s, s->prev);
    }

    static void encodeByte(Save *s, unsigned char v)
    {
        updateAdler(s, v);

        // Simple RLE compression. We could do better by doing a search
        // to find matches, but this works pretty well TBH.
        if (s->prev == v && s->runlen < 115)
        {
            s->runlen++;
        }
        else
        {
            if (s->runlen)
                endrun(s);

            s->prev = v;
            s->runlen = 1;
        }
    }

    static void savePngHeader(Save *s, MgSurface *bmp)
    {
        fwrite("\211PNG\r\n\032\n", 8, 1, s->out);
        begin(s, "IHDR", 13);
        put32(s, bmp->w);
        put32(s, bmp->h);
        put(s, 8); // bit depth
        put(s, 6); // RGBA
        put(s, 0); // compression (deflate)
        put(s, 0); // filter (standard)
        put(s, 0); // interlace off
        put32(s, ~s->crc);
    }

    static long savePngData(Save *s, MgSurface *surface, long dataPos)
    {
        int x, y;
        long dataSize;
        begin(s, "IDAT", 0);
        put(s, 0x08);     // zlib compression method
        put(s, 0x1d);     // zlib compression flags
        putbits(s, 3, 3); // zlib last block + fixed dictionary
        for (y = 0; y < surface->h; y++)
        {
            MgPixel *row = &surface->pixels[y * surface->w];
            MgPixel prev = mgRGBA(0, 0, 0, 0);

            encodeByte(s, 1); // sub filter
            for (x = 0; x < surface->w; x++)
            {
                encodeByte(s, row[x].r - prev.r);
                encodeByte(s, row[x].g - prev.g);
                encodeByte(s, row[x].b - prev.b);
                encodeByte(s, row[x].a - prev.a);
                prev = row[x];
            }
        }
        endrun(s);
        literal(s, 256); // terminator
        while (s->bits != 0x80)
            putbits(s, 0, 1);
        put32(s, s->adler);
        dataSize = (ftell(s->out) - dataPos) - 8;
        put32(s, ~s->crc);
        return dataSize;
    }

    int mgSaveImage(const char *fileName, MgSurface *surface)
    {
        Save s;
        long dataPos, dataSize, err;

        // TODO - unicode?
        FILE *out = fopen(fileName, "wb");
        if (!out)
            return 1;

        s.out = out;
        s.adler = 1;
        s.bits = 0x80;
        s.prev = 0xffff;
        s.runlen = 0;

        savePngHeader(&s, surface);
        dataPos = ftell(s.out);
        dataSize = savePngData(&s, surface, dataPos);

        // End chunk.
        begin(&s, "IEND", 0);
        put32(&s, ~s.crc);

        // Write back payload size.
        fseek(out, dataPos, SEEK_SET);
        put32(&s, dataSize);

        err = ferror(out);
        fclose(out);
        return !err;
    }

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_inflate.c

#include <stdlib.h>
#include <setjmp.h>

    typedef struct
    {
        unsigned bits, count;
        const unsigned char *in, *inend;
        unsigned char *out, *outend;
        jmp_buf jmp;
        unsigned litcodes[288], distcodes[32], lencodes[19];
        int tlit, tdist, tlen;
    } State;

#define FAIL() longjmp(s->jmp, 1)
#define CHECK(X) \
    if (!(X))    \
    FAIL()

    // Built-in DEFLATE standard tables.
    static char order[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    static char lenBits[29 + 2] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
                                   3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 0, 0};
    static int lenBase[29 + 2] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
                                  35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
    static char distBits[30 + 2] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
                                    7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 0, 0};
    static int distBase[30 + 2] = {
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129,
        193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};

    // Table to bit-reverse a byte.
    static const unsigned char reverseTable[256] = {
#define R2(n) n, n + 128, n + 64, n + 192
#define R4(n) R2(n), R2(n + 32), R2(n + 16), R2(n + 48)
#define R6(n) R4(n), R4(n + 8), R4(n + 4), R4(n + 12)
        R6(0), R6(2), R6(1), R6(3)};

    static unsigned rev16(unsigned n)
    {
        return (reverseTable[n & 0xff] << 8) | reverseTable[(n >> 8) & 0xff];
    }

    static int bits(State *s, int n)
    {
        int v = s->bits & ((1 << n) - 1);
        s->bits >>= n;
        s->count -= n;
        while (s->count < 16)
        {
            CHECK(s->in != s->inend);
            s->bits |= (*s->in++) << s->count;
            s->count += 8;
        }
        return v;
    }

    static unsigned char *emit(State *s, int len)
    {
        s->out += len;
        CHECK(s->out <= s->outend);
        return s->out - len;
    }

    static void copy(State *s, const unsigned char *src, int len)
    {
        unsigned char *dest = emit(s, len);
        while (len--)
            *dest++ = *src++;
    }

    static int build(State *s, unsigned *tree, unsigned char *lens, unsigned int symcount)
    {
        unsigned int codes[16], first[16], counts[16] = {0};

        // Frequency count.
        for (unsigned int n = 0; n < symcount; n++)
            counts[lens[n]]++;

        // Distribute codes.
        counts[0] = codes[0] = first[0] = 0;
        for (unsigned int n = 1; n <= 15; n++)
        {
            codes[n] = (codes[n - 1] + counts[n - 1]) << 1;
            first[n] = first[n - 1] + counts[n - 1];
        }
        CHECK(first[15] + counts[15] <= symcount);

        // Insert keys into the tree for each symbol.
        for (unsigned int n = 0; n < symcount; n++)
        {
            int len = lens[n];
            if (len != 0)
            {
                unsigned code = codes[len]++, slot = first[len]++;
                tree[slot] = (code << (32 - len)) | (n << 4) | len;
            }
        }

        return first[15];
    }

    static int decode(State *s, unsigned tree[], int max)
    {
        // Find the next prefix code.
        unsigned lo = 0, hi = max, key;
        unsigned search = (rev16(s->bits) << 16) | 0xffff;
        while (lo < hi)
        {
            unsigned guess = (lo + hi) / 2;
            if (search < tree[guess])
                hi = guess;
            else
                lo = guess + 1;
        }

        // Pull out the key and check it.
        key = tree[lo - 1];
        CHECK(((search ^ key) >> (32 - (key & 0xf))) == 0);

        bits(s, key & 0xf);
        return (key >> 4) & 0xfff;
    }

    static void run(State *s, int sym)
    {
        int length = bits(s, lenBits[sym]) + lenBase[sym];
        int dsym = decode(s, s->distcodes, s->tdist);
        int offs = bits(s, distBits[dsym]) + distBase[dsym];
        copy(s, s->out - offs, length);
    }

    static void block(State *s)
    {
        for (;;)
        {
            int sym = decode(s, s->litcodes, s->tlit);
            if (sym < 256)
                *emit(s, 1) = (unsigned char)sym;
            else if (sym > 256)
                run(s, sym - 257);
            else
                break;
        }
    }

    static void stored(State *s)
    {
        // Uncompressed data block.
        int len;
        bits(s, s->count & 7);
        len = bits(s, 16);
        CHECK(((len ^ s->bits) & 0xffff) == 0xffff);
        CHECK(s->in + len <= s->inend);

        copy(s, s->in, len);
        s->in += len;
        bits(s, 16);
    }

    static void fixed(State *s)
    {
        // Fixed set of Huffman codes.
        int n;
        unsigned char lens[288 + 32];
        for (n = 0; n <= 143; n++)
            lens[n] = 8;
        for (n = 144; n <= 255; n++)
            lens[n] = 9;
        for (n = 256; n <= 279; n++)
            lens[n] = 7;
        for (n = 280; n <= 287; n++)
            lens[n] = 8;
        for (n = 0; n < 32; n++)
            lens[288 + n] = 5;

        // Build lit/dist trees.
        s->tlit = build(s, s->litcodes, lens, 288);
        s->tdist = build(s, s->distcodes, lens + 288, 32);
    }

    static void dynamic(State *s)
    {
        int n, i, nlit, ndist, nlen;
        unsigned char lenlens[19] = {0}, lens[288 + 32];
        nlit = 257 + bits(s, 5);
        ndist = 1 + bits(s, 5);
        nlen = 4 + bits(s, 4);
        for (n = 0; n < nlen; n++)
            lenlens[(int)order[n]] = (unsigned char)bits(s, 3);

        // Build the tree for decoding code lengths.
        s->tlen = build(s, s->lencodes, lenlens, 19);

        // Decode code lengths.
        for (n = 0; n < nlit + ndist;)
        {
            int sym = decode(s, s->lencodes, s->tlen);
            switch (sym)
            {
            case 16:
                for (i = 3 + bits(s, 2); i; i--, n++)
                    lens[n] = lens[n - 1];
                break;
            case 17:
                for (i = 3 + bits(s, 3); i; i--, n++)
                    lens[n] = 0;
                break;
            case 18:
                for (i = 11 + bits(s, 7); i; i--, n++)
                    lens[n] = 0;
                break;
            default:
                lens[n++] = (unsigned char)sym;
                break;
            }
        }

        // Build lit/dist trees.
        s->tlit = build(s, s->litcodes, lens, nlit);
        s->tdist = build(s, s->distcodes, lens + nlit, ndist);
    }

    int mgInflate(void *out, unsigned outlen, const void *in, unsigned inlen)
    {
        int last;
        State *s = (State *)calloc(1, sizeof(State));

        // We assume we can buffer 2 extra bytes from off the end of 'in'.
        s->in = (unsigned char *)in;
        s->inend = s->in + inlen + 2;
        s->out = (unsigned char *)out;
        s->outend = s->out + outlen;
        s->bits = 0;
        s->count = 0;
        bits(s, 0);

        if (setjmp(s->jmp) == 1)
        {
            free(s);
            return 0;
        }

        do
        {
            last = bits(s, 1);
            switch (bits(s, 2))
            {
            case 0:
                stored(s);
                break;
            case 1:
                fixed(s);
                block(s);
                break;
            case 2:
                dynamic(s);
                block(s);
                break;
            case 3:
                FAIL();
            }
        } while (!last);

        free(s);
        return 1;
    }

#undef CHECK
#undef FAIL

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_print.c

#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>

#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

    MgFont mgStockFont;
    MgFont *mgfont = &mgStockFont;

    // Converts 8-bit codepage entries into Unicode code points.
    static int cp1252[] = {
        0x20ac,
        0xfffd,
        0x201a,
        0x0192,
        0x201e,
        0x2026,
        0x2020,
        0x2021,
        0x02c6,
        0x2030,
        0x0160,
        0x2039,
        0x0152,
        0xfffd,
        0x017d,
        0xfffd,
        0xfffd,
        0x2018,
        0x2019,
        0x201c,
        0x201d,
        0x2022,
        0x2013,
        0x2014,
        0x02dc,
        0x2122,
        0x0161,
        0x203a,
        0x0153,
        0xfffd,
        0x017e,
        0x0178,
        0x00a0,
        0x00a1,
        0x00a2,
        0x00a3,
        0x00a4,
        0x00a5,
        0x00a6,
        0x00a7,
        0x00a8,
        0x00a9,
        0x00aa,
        0x00ab,
        0x00ac,
        0x00ad,
        0x00ae,
        0x00af,
        0x00b0,
        0x00b1,
        0x00b2,
        0x00b3,
        0x00b4,
        0x00b5,
        0x00b6,
        0x00b7,
        0x00b8,
        0x00b9,
        0x00ba,
        0x00bb,
        0x00bc,
        0x00bd,
        0x00be,
        0x00bf,
        0x00c0,
        0x00c1,
        0x00c2,
        0x00c3,
        0x00c4,
        0x00c5,
        0x00c6,
        0x00c7,
        0x00c8,
        0x00c9,
        0x00ca,
        0x00cb,
        0x00cc,
        0x00cd,
        0x00ce,
        0x00cf,
        0x00d0,
        0x00d1,
        0x00d2,
        0x00d3,
        0x00d4,
        0x00d5,
        0x00d6,
        0x00d7,
        0x00d8,
        0x00d9,
        0x00da,
        0x00db,
        0x00dc,
        0x00dd,
        0x00de,
        0x00df,
        0x00e0,
        0x00e1,
        0x00e2,
        0x00e3,
        0x00e4,
        0x00e5,
        0x00e6,
        0x00e7,
        0x00e8,
        0x00e9,
        0x00ea,
        0x00eb,
        0x00ec,
        0x00ed,
        0x00ee,
        0x00ef,
        0x00f0,
        0x00f1,
        0x00f2,
        0x00f3,
        0x00f4,
        0x00f5,
        0x00f6,
        0x00f7,
        0x00f8,
        0x00f9,
        0x00fa,
        0x00fb,
        0x00fc,
        0x00fd,
        0x00fe,
        0x00ff,
    };

    static int border(MgSurface *surface, int x, int y)
    {
        MgPixel top = mgGet(surface, 0, 0);
        MgPixel c = mgGet(surface, x, y);
        return (c.r == top.r && c.g == top.g && c.b == top.b) || x >= surface->w || y >= surface->h;
    }

    static void scan(MgSurface *surface, int *x, int *y, int *rowh)
    {
        while (*y < surface->h)
        {
            if (*x >= surface->w)
            {
                *x = 0;
                (*y) += *rowh;
                *rowh = 1;
            }
            if (!border(surface, *x, *y))
                return;
            (*x)++;
        }
    }

    /*
     * Watermarks are encoded vertically in the alpha channel using seven pixels
     * starting at x, y. The first and last alpha values contain the magic values
     * 0b10101010 and 0b01010101 respectively.
     */
    static int readWatermark(MgSurface *bmp, int x, int y, int *big, int *small)
    {
        const int magicHeader = 0xAA;
        const int magicFooter = 0x55;

        unsigned char watermark[7];

        for (int i = 0; i < 7; i++)
        {
            MgPixel c = mgGet(bmp, x, y + i);
            watermark[i] = c.a;
        }

        if (watermark[0] != magicHeader || watermark[6] != magicFooter)
        {
            return 0;
        }

        *big = watermark[1] | (watermark[2] << 8) | (watermark[3] << 16) | (watermark[4] << 24);
        *small = watermark[5];

        return 1;
    }

    int mgLoadGlyphs(MgFont *font, int codepage)
    {
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
        int rowh = 1;

        MgGlyph *g;
        switch (codepage)
        {
        case TCP_ASCII:
            font->numGlyphs = 128 - 32;
            break;
        case TCP_1252:
            font->numGlyphs = 256 - 32;
            break;
        case TCP_UTF32:
            if (!readWatermark(font->surface, 0, 0, &font->numGlyphs, &rowh))
            {
                return 0;
            }
            h = rowh;
            x = 1;
            break;
        default:
            errno = EINVAL;
            return 0;
        }

        font->glyphs = (MgGlyph *)calloc(font->numGlyphs, sizeof(MgGlyph));

        for (int index = 0; index < font->numGlyphs; index++)
        {
            // Look up the Unicode code point.
            g = &font->glyphs[index];

            if (codepage != TCP_UTF32)
            {
                // Find the next glyph.
                scan(font->surface, &x, &y, &rowh);

                if (y >= font->surface->h)
                {
                    errno = EINVAL;
                    return 0;
                }

                // Scan the width and height
                w = h = 0;
                while (!border(font->surface, x + w, y))
                {
                    w++;
                }

                while (!border(font->surface, x, y + h))
                {
                    h++;
                }
            }

            switch (codepage)
            {
            case TCP_ASCII:
                g->code = index + 32;
                break;
            case TCP_1252:
                if (index < 96)
                {
                    g->code = index + 32;
                }
                else
                {
                    g->code = cp1252[index - 96];
                }
                break;
            case TCP_UTF32:
                if (!readWatermark(font->surface, x, y, &g->code, &w))
                {
                    // Maybe we are at the end of a row?
                    x = 0;
                    y += rowh;
                    if (!readWatermark(font->surface, x, y, &g->code, &w))
                    {
                        return 0;
                    }
                }
                x++;
                break;
            default:
                return 0;
            }

            g->x = x;
            g->y = y;
            g->w = w;
            g->h = h;
            x += w;
            if (h != font->glyphs[0].h)
            {
                errno = EINVAL;
                return 0;
            }

            if (h > rowh)
            {
                rowh = h;
            }
        }

        // Sort by code point.
        for (int i = 1; i < font->numGlyphs; i++)
        {
            int j = i;
            MgGlyph g = font->glyphs[i];
            while (j > 0 && font->glyphs[j - 1].code > g.code)
            {
                font->glyphs[j] = font->glyphs[j - 1];
                j--;
            }
            font->glyphs[j] = g;
        }

        return 1;
    }

    MgFont *mgLoadFont(MgSurface *surface, int codepage)
    {
        MgFont *font = (MgFont *)calloc(1, sizeof(MgFont));
        font->surface = surface;
        if (!mgLoadGlyphs(font, codepage))
        {
            mgFreeFont(font);
            return NULL;
        }
        return font;
    }

    void mgFreeFont(MgFont *font)
    {
        mgFree(font->surface);
        free(font->glyphs);
        free(font);
    }

    static MgGlyph *get(MgFont *font, int code)
    {
        unsigned lo = 0, hi = font->numGlyphs;
        while (lo < hi)
        {
            unsigned guess = (lo + hi) / 2;
            if (code < font->glyphs[guess].code)
                hi = guess;
            else
                lo = guess + 1;
        }

        if (lo == 0 || font->glyphs[lo - 1].code != code)
            return &font->glyphs['?' - 32];
        else
            return &font->glyphs[lo - 1];
    }

    void mgSetupFont(MgFont *font)
    {
        // Load the stock font if needed.
        if (font == mgfont && !mgfont->surface)
        {
            mgfont->surface = mgLoadImageMem(mg_font, mg_font_size);
            mgLoadGlyphs(mgfont, 1252);
        }
    }

    void mgPrint(MgSurface *dest, MgFont *font, int x, int y, MgPixel color, const char *text, ...)
    {
        char tmp[1024];
        MgGlyph *g;
        va_list args;
        const char *p;
        int start = x, c;

        mgSetupFont(font);

        // Expand the formatting string.
        va_start(args, text);
        vsnprintf(tmp, sizeof(tmp), text, args);
        tmp[sizeof(tmp) - 1] = 0;
        va_end(args);

        // Print each glyph.
        p = tmp;
        while (*p)
        {
            p = mgDecodeUTF8(p, &c);
            if (c == '\r')
                continue;
            if (c == '\n')
            {
                x = start;
                y += mgTextHeight(font, "");
                continue;
            }
            g = get(font, c);
            mgBlitTint(dest, font->surface, x, y, g->x, g->y, g->w, g->h, color);
            x += g->w;
        }
    }

    int mgTextWidth(MgFont *font, const char *text)
    {
        int x = 0, w = 0, c;
        mgSetupFont(font);

        while (*text)
        {
            text = mgDecodeUTF8(text, &c);
            if (c == '\n' || c == '\r')
            {
                x = 0;
            }
            else
            {
                x += get(font, c)->w;
                w = (x > w) ? x : w;
            }
        }
        return w;
    }

    int mgTextHeight(MgFont *font, const char *text)
    {
        int rowh, h, c;
        mgSetupFont(font);

        h = rowh = get(font, 0)->h;
        while (*text)
        {
            text = mgDecodeUTF8(text, &c);
            if (c == '\n' && *text)
                h += rowh;
        }
        return h;
    }

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_win.c

#include <assert.h>

    // not really windows stuff
    MgInternal *mgInternal(MgSurface *surface)
    {
        assert(surface->handle);
        return (MgInternal *)(surface + 1);
    }

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#pragma comment(lib, "opengl32") // glViewport
#pragma comment(lib, "shell32")  // CommandLineToArgvW
#pragma comment(lib, "user32")   // SetWindowLong
#pragma comment(lib, "gdi32")    // ChoosePixelFormat
#pragma comment(lib, "advapi32") // RegSetValueEx

#define WIDGET_SCALE 3
#define WIDGET_FADE 16

    int main(int argc, char *argv[]);

#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
    HKEY mgRegKey;
#endif

#ifdef __TINYC__
#define CP_UTF8 65001
    int WINAPI MultiByteToWideChar();
    int WINAPI WideCharToMultiByte();
#endif

    static wchar_t *unicode(const char *str)
    {
        int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
        wchar_t *dest = (wchar_t *)malloc(sizeof(wchar_t) * len);
        MultiByteToWideChar(CP_UTF8, 0, str, -1, dest, len);
        return dest;
    }

    void mgError(MgSurface *surface, const char *message, ...)
    {
        char tmp[1024];

        va_list args;
        va_start(args, message);
        _vsnprintf(tmp, sizeof(tmp), message, args);
        tmp[sizeof(tmp) - 1] = 0;
        va_end(args);

        MessageBoxW(surface ? (HWND)surface->handle : NULL, unicode(tmp), surface ? mgInternal(surface)->wtitle : L"Error",
                    MB_OK | MB_ICONERROR);
        exit(1);
    }

    void mgEnterBorderlessWindowed(MgSurface *surface)
    {
        // Enter borderless windowed mode.
        MONITORINFO mi = {sizeof(mi)};
        MgInternal *win = mgInternal(surface);

        GetWindowRect((HWND)surface->handle, &win->oldPos);

        GetMonitorInfo(MonitorFromWindow((HWND)surface->handle, MONITOR_DEFAULTTONEAREST), &mi);
        win->dwStyle = WS_VISIBLE | WS_POPUP;
        SetWindowLong((HWND)surface->handle, GWL_STYLE, win->dwStyle);
        SetWindowPos((HWND)surface->handle, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                     mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, 0);
    }

    void mgLeaveBorderlessWindowed(MgSurface *surface)
    {
        MgInternal *win = mgInternal(surface);

        win->dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
        SetWindowLong((HWND)surface->handle, GWL_STYLE, win->dwStyle);

        SetWindowPos((HWND)surface->handle, NULL, win->oldPos.left, win->oldPos.top, win->oldPos.right - win->oldPos.left,
                     win->oldPos.bottom - win->oldPos.top, 0);
    }

    void mgWinUpdateWidgets(MgSurface *surface, int dw, int dh)
    {
        POINT pt;
        int i, x, clicked = 0;
        char str[8];
        MgPixel col;
        MgPixel off = mgRGB(255, 255, 255);
        MgPixel on = mgRGB(0, 200, 255);
        MgInternal *win = mgInternal(surface);
        (void)dh;

        mgClear(win->widgets, mgRGBA(0, 0, 0, 0));

        if (!(win->dwStyle & WS_POPUP))
        {
            win->widgetsWanted = 0;
            win->widgetAlpha = 0;
            return;
        }

        // See if we want to be showing widgets or not.
        GetCursorPos(&pt);
        ScreenToClient((HWND)surface->handle, &pt);
        if (pt.y == 0)
            win->widgetsWanted = 1;
        if (pt.y > win->widgets->h * WIDGET_SCALE)
            win->widgetsWanted = 0;

        // Track the alpha.
        if (win->widgetsWanted)
            win->widgetAlpha = (win->widgetAlpha <= 255 - WIDGET_FADE) ? win->widgetAlpha + WIDGET_FADE : 255;
        else
            win->widgetAlpha = (win->widgetAlpha >= WIDGET_FADE) ? win->widgetAlpha - WIDGET_FADE : 0;

        // Get relative coords.
        pt.x -= (dw - win->widgets->w * WIDGET_SCALE);
        pt.x /= WIDGET_SCALE;
        pt.y /= WIDGET_SCALE;

        mgClear(win->widgets, mgRGBA(0, 0, 0, win->widgetAlpha));

        // Render it.
        for (i = 0; i < 3; i++)
        {
            switch (i)
            {
            case 0:
                str[0] = '_';
                str[1] = 0;
                break; // "_" (minimize)
            case 1:
                str[0] = 0xEF;
                str[1] = 0xBF;
                str[2] = 0xBD;
                str[3] = 0;
                break; // "[]" (maximize)
            case 2:
                str[0] = 0xC3;
                str[1] = 0x97;
                str[2] = 0;
                break; // "x" (close)
            }
            x = win->widgets->w + (i - 3) * 12;
            if (i == 2)
                off = mgRGB(255, 0, 0);
            if (pt.x >= x && pt.x < x + 10 && pt.y < win->widgets->h)
            {
                col = on;
                if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
                    clicked |= 1 << i;
            }
            else
            {
                col = off;
            }
            col.a = win->widgetAlpha;
            mgPrint(win->widgets, mgfont, x, 2, col, str);
        }

        if (clicked & 1)
            ShowWindow((HWND)surface->handle, SW_MINIMIZE);
        if (clicked & 2)
            mgLeaveBorderlessWindowed(surface);
        if (clicked & 4)
            SendMessage((HWND)surface->handle, WM_CLOSE, 0, 0);
    }

    void mgUpdate(MgSurface *surface)
    {
        MSG msg;
        RECT rc;
        int dw, dh;
        MgInternal *win = mgInternal(surface);

        if (!win->shown)
        {
            win->shown = 1;
            UpdateWindow((HWND)surface->handle);
            ShowWindow((HWND)surface->handle, SW_SHOW);
        }

        // Get the window size.
        GetClientRect((HWND)surface->handle, &rc);
        dw = rc.right - rc.left;
        dh = rc.bottom - rc.top;

        // Update the widget overlay.
        mgWinUpdateWidgets(surface, dw, dh);

        if (!mgGAPIBegin(surface))
        {
            mgGAPIPresent(surface, dw, dh);
            SwapBuffers(win->gl.dc);
            mgGAPIEnd(surface);
        }

        memcpy(win->prev, win->keys, 256);

        // Run the message pump.
        while (PeekMessage(&msg, (HWND)surface->handle, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC_)(int);
    static PFNWGLSWAPINTERVALFARPROC_ wglSwapIntervalEXT_ = 0;

    int mgGAPIBegin(MgSurface *surface)
    {
        MgInternal *win = mgInternal(surface);

        return wglMakeCurrent(win->gl.dc, win->gl.hglrc) ? 0 : -1;
    }

    int mgGAPIEnd(MgSurface *surface)
    {
        (void)surface;
        return wglMakeCurrent(NULL, NULL) ? 0 : -1;
    }

    static BOOL UnadjustWindowRectEx(LPRECT prc, DWORD dwStyle, BOOL fMenu, DWORD dwExStyle)
    {
        BOOL fRc;
        RECT rc;
        SetRectEmpty(&rc);
        fRc = AdjustWindowRectEx(&rc, dwStyle, fMenu, dwExStyle);
        if (fRc)
        {
            prc->left -= rc.left;
            prc->top -= rc.top;
            prc->right -= rc.right;
            prc->bottom -= rc.bottom;
        }
        return fRc;
    }

    LRESULT CALLBACK mgWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        MgSurface *surface;
        MgInternal *win = NULL;
        RECT rc;
        int dw, dh;

        GetClientRect(hWnd, &rc);
        dw = rc.right - rc.left;
        dh = rc.bottom - rc.top;

        surface = (MgSurface *)GetPropW(hWnd, L"minigfx");
        if (surface)
            win = mgInternal(surface);

        switch (message)
        {
        case WM_PAINT:
            if (!mgGAPIBegin(surface))
            {
                mgGAPIPresent(surface, dw, dh);
                SwapBuffers(win->gl.dc);
                mgGAPIEnd(surface);
            }
            ValidateRect(hWnd, NULL);
            break;
        case WM_CLOSE:
            if (win)
                win->closed = 1;
            break;
        case WM_GETMINMAXINFO:
            if (surface)
            {
                MINMAXINFO *info = (MINMAXINFO *)lParam;
                RECT rc;
                rc.left = 0;
                rc.top = 0;
                if (win->flags & MG_AUTO)
                {
                    rc.right = 32;
                    rc.bottom = 32;
                }
                else
                {
                    int minscale = mgEnforceScale(1, win->flags);
                    rc.right = surface->w * minscale;
                    rc.bottom = surface->h * minscale;
                }
                AdjustWindowRectEx(&rc, win->dwStyle, FALSE, 0);
                info->ptMinTrackSize.x = rc.right - rc.left;
                info->ptMinTrackSize.y = rc.bottom - rc.top;
            }
            return 0;
        case WM_SIZING:
            if (win)
            {
                // Calculate scale-constrained sizes.
                RECT *rc = (RECT *)lParam;
                int dx, dy;
                UnadjustWindowRectEx(rc, win->dwStyle, FALSE, 0);
                dx = (rc->right - rc->left) % win->scale;
                dy = (rc->bottom - rc->top) % win->scale;
                switch (wParam)
                {
                case WMSZ_LEFT:
                    rc->left += dx;
                    break;
                case WMSZ_RIGHT:
                    rc->right -= dx;
                    break;
                case WMSZ_TOP:
                    rc->top += dy;
                    break;
                case WMSZ_TOPLEFT:
                    rc->left += dx;
                    rc->top += dy;
                    break;
                case WMSZ_TOPRIGHT:
                    rc->right -= dx;
                    rc->top += dy;
                    break;
                case WMSZ_BOTTOM:
                    rc->bottom -= dy;
                    break;
                case WMSZ_BOTTOMLEFT:
                    rc->left += dx;
                    rc->bottom -= dy;
                    break;
                case WMSZ_BOTTOMRIGHT:
                    rc->right -= dx;
                    rc->bottom -= dy;
                    break;
                }
                AdjustWindowRectEx(rc, win->dwStyle, FALSE, 0);
            }
            return TRUE;
        case WM_SIZE:
            if (win)
            {
                if (wParam != SIZE_MINIMIZED)
                {
                    // Detect window size changes and update our bitmap accordingly.
                    dw = LOWORD(lParam);
                    dh = HIWORD(lParam);
                    if (win->flags & MG_AUTO)
                    {
                        mgResize(surface, dw / win->scale, dh / win->scale);
                    }
                    else
                    {
                        win->scale = mgEnforceScale(mgCalcScale(surface->w, surface->h, dw, dh), win->flags);
                    }
                    mgPosition(surface, win->scale, dw, dh, win->pos);
                }

#ifndef MG_DO_NOT_MAXIMIZE_TO_FULLSCREEN
                // If someone tried to maximize us (e.g. via shortcut launch options),
                // prefer instead to be borderless.
                if (wParam == SIZE_MAXIMIZED)
                {
                    ShowWindow((HWND)surface->handle, SW_NORMAL);
                    mgEnterBorderlessWindowed(surface);
                }
#endif
            }
            return 0;
#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
        case WM_WINDOWPOSCHANGED:
        {
            // Save our position.
            WINDOWPLACEMENT wp = {sizeof(WINDOWPLACEMENT)};
            GetWindowPlacement(hWnd, &wp);
            if (win->dwStyle & WS_POPUP)
                wp.showCmd = SW_MAXIMIZE;
            RegSetValueExW(mgRegKey, win->wtitle, 0, REG_BINARY, (BYTE *)&wp, sizeof(wp));
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }
#endif
        case WM_ACTIVATE:
            if (win)
            {
                memset(win->keys, 0, 256);
                memset(win->prev, 0, 256);
                win->lastChar = 0;
            }
            return 0;
        case WM_CHAR:
            if (win)
            {
                if (wParam == '\r')
                {
                    wParam = '\n';
                }
                int repeating = (HIWORD(lParam) & KF_REPEAT) == KF_REPEAT;
                if (!repeating)
                {
                    win->lastChar = wParam;
                }
            }
            return DefWindowProcW(hWnd, message, wParam, lParam);
        case WM_MENUCHAR:
            // Disable beep on Alt+Enter
            if (LOWORD(wParam) == VK_RETURN)
                return MNC_CLOSE << 16;
            return DefWindowProcW(hWnd, message, wParam, lParam);
        case WM_SYSKEYDOWN:
            if (win)
            {
                if (wParam == VK_RETURN)
                {
                    // Alt+Enter
                    if (win->dwStyle & WS_POPUP)
                        mgLeaveBorderlessWindowed(surface);
                    else
                        mgEnterBorderlessWindowed(surface);
                    return 0;
                }
            }
            // fall-thru
        case WM_KEYDOWN:
            if (win)
                win->keys[wParam] = 1;
            return DefWindowProcW(hWnd, message, wParam, lParam);
        case WM_SYSKEYUP:
            // fall-thru
        case WM_KEYUP:
            if (win)
                win->keys[wParam] = 0;
            return DefWindowProcW(hWnd, message, wParam, lParam);
        default:
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    MgSurface *mgWindow(int w, int h, const char *title, int flags)
    {
        WNDCLASSEXW wcex = {0};
        int maxW, maxH, scale;
        HWND hWnd;
        DWORD dwStyle;
        RECT rc;
        DWORD err;
        MgSurface *surface;
        MgInternal *win;
#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
        WINDOWPLACEMENT wp;
        DWORD wpsize = sizeof(wp);
#endif

        wchar_t *wtitle = unicode(title);

// Find our registry key.
#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
        RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\minigfx", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
                        &mgRegKey, NULL);
#endif

        // Register a window class.
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcex.lpfnWndProc = mgWndProc;
        wcex.hInstance = GetModuleHandle(NULL);
        wcex.hIcon = NULL;
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.lpszClassName = L"minigfx";
        RegisterClassExW(&wcex);

        if (flags & MG_AUTO)
        {
            // Always use a 1:1 pixel size.
            scale = 1;
        }
        else
        {
            // See how big we can make it and still fit on-screen.
            maxW = GetSystemMetrics(SM_CXSCREEN) * 3 / 4;
            maxH = GetSystemMetrics(SM_CYSCREEN) * 3 / 4;
            scale = mgCalcScale(w, h, maxW, maxH);
        }

        scale = mgEnforceScale(scale, flags);

        // Get the final window size.
        dwStyle = WS_OVERLAPPEDWINDOW;
        rc.left = 0;
        rc.top = 0;
        rc.right = w * scale;
        rc.bottom = h * scale;
        AdjustWindowRect(&rc, dwStyle, FALSE);

        // Make a window.
        hWnd = CreateWindowW(L"minigfx", wtitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
                             NULL, NULL, wcex.hInstance, NULL);
        err = GetLastError();
        if (!hWnd)
            ExitProcess(1);

        if (flags & MG_NOCURSOR)
        {
            ShowCursor(FALSE);
        }

        // Wrap a bitmap around it.
        surface = mgBitmap2(w, h, sizeof(MgInternal));
        surface->handle = hWnd;

        // Set up the Windows parts.
        win = mgInternal(surface);
        win->dwStyle = dwStyle;
        win->wtitle = wtitle;
        win->shown = 0;
        win->closed = 0;
        win->scale = scale;
        win->lastChar = 0;
        win->flags = flags;

        win->p1 = win->p2 = win->p3 = 0;
        win->p4 = 1;

        win->widgetsWanted = 0;
        win->widgetAlpha = 0;
        win->widgetsScale = WIDGET_SCALE;
        win->widgets = mgBitmap(40, 14);

        SetPropW(hWnd, L"minigfx", surface);

        mgGAPICreate(surface);

        if (flags & MG_FULLSCREEN)
        {
            mgEnterBorderlessWindowed(surface);
        }
        else
        {
// Try and restore our window position.
#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
            if (RegQueryValueExW(mgRegKey, wtitle, NULL, NULL, (BYTE *)&wp, &wpsize) == ERROR_SUCCESS)
            {
                if (wp.showCmd == SW_MAXIMIZE)
                    mgEnterBorderlessWindowed(surface);
                else
                    SetWindowPlacement(hWnd, &wp);
            }
#endif
        }

        wglSwapIntervalEXT_ = (PFNWGLSWAPINTERVALFARPROC_)wglGetProcAddress("wglSwapIntervalEXT");
        if (wglSwapIntervalEXT_)
            wglSwapIntervalEXT_(1);

        return surface;
    }

    void mgFree(MgSurface *surface)
    {
        if (surface->handle)
        {
            MgInternal *win = mgInternal(surface);
            mgGAPIDestroy(surface);

            if (win->gl.hglrc && !wglDeleteContext(win->gl.hglrc))
            {
                mgError(surface, "Cannot delete OpenGL context.\n");
            }
            win->gl.hglrc = NULL;

            if (win->gl.dc && !ReleaseDC((HWND)surface->handle, win->gl.dc))
            {
                mgError(surface, "Cannot release OpenGL device context.\n");
            }
            win->gl.dc = NULL;

            DestroyWindow((HWND)surface->handle);
            free(win->wtitle);
            mgFree(win->widgets);
        }
        free(surface->pixels);
        free(surface);
    }

    int mgClosed(MgSurface *surface)
    {
        MgInternal *win = mgInternal(surface);
        int val = win->closed;
        win->closed = 0;
        return val;
    }

    float mgTime()
    {
        static int first = 1;
        static LARGE_INTEGER prev;

        LARGE_INTEGER cnt, freq;
        ULONGLONG diff;
        QueryPerformanceCounter(&cnt);
        QueryPerformanceFrequency(&freq);

        if (first)
        {
            first = 0;
            prev = cnt;
        }

        diff = cnt.QuadPart - prev.QuadPart;
        prev = cnt;
        return (float)(diff / (double)freq.QuadPart);
    }

    void mgMouse(MgSurface *surface, int *x, int *y, int *buttons)
    {
        POINT pt;
        MgInternal *win;

        win = mgInternal(surface);
        GetCursorPos(&pt);
        ScreenToClient((HWND)surface->handle, &pt);
        *x = (pt.x - win->pos[0]) / win->scale;
        *y = (pt.y - win->pos[1]) / win->scale;
        *buttons = 0;
        if (GetFocus() != surface->handle)
            return;
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
            *buttons |= 1;
        if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
            *buttons |= 2;
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
            *buttons |= 4;
    }

    int mgTouch(MgSurface *surface, MgTouchPoint *points, int maxPoints)
    {
        int buttons = 0;
        if (maxPoints > 0)
        {
            mgMouse(surface, &points[0].x, &points[1].y, &buttons);
        }
        return buttons ? 1 : 0;
    }

    static int mgWinVK(int key)
    {
        if (key >= 'A' && key <= 'Z')
            return key;
        if (key >= '0' && key <= '9')
            return key;
        switch (key)
        {
        case MK_BACKSPACE:
            return VK_BACK;
        case MK_TAB:
            return VK_TAB;
        case MK_RETURN:
            return VK_RETURN;
        case MK_SHIFT:
            return VK_SHIFT;
        case MK_CONTROL:
            return VK_CONTROL;
        case MK_ALT:
            return VK_MENU;
        case MK_PAUSE:
            return VK_PAUSE;
        case MK_CAPSLOCK:
            return VK_CAPITAL;
        case MK_ESCAPE:
            return VK_ESCAPE;
        case MK_SPACE:
            return VK_SPACE;
        case MK_PAGEUP:
            return VK_PRIOR;
        case MK_PAGEDN:
            return VK_NEXT;
        case MK_END:
            return VK_END;
        case MK_HOME:
            return VK_HOME;
        case MK_LEFT:
            return VK_LEFT;
        case MK_UP:
            return VK_UP;
        case MK_RIGHT:
            return VK_RIGHT;
        case MK_DOWN:
            return VK_DOWN;
        case MK_INSERT:
            return VK_INSERT;
        case MK_DELETE:
            return VK_DELETE;
        case MK_LWIN:
            return VK_LWIN;
        case MK_RWIN:
            return VK_RWIN;
        case MK_PAD0:
            return VK_NUMPAD0;
        case MK_PAD1:
            return VK_NUMPAD1;
        case MK_PAD2:
            return VK_NUMPAD2;
        case MK_PAD3:
            return VK_NUMPAD3;
        case MK_PAD4:
            return VK_NUMPAD4;
        case MK_PAD5:
            return VK_NUMPAD5;
        case MK_PAD6:
            return VK_NUMPAD6;
        case MK_PAD7:
            return VK_NUMPAD7;
        case MK_PAD8:
            return VK_NUMPAD8;
        case MK_PAD9:
            return VK_NUMPAD9;
        case MK_PADMUL:
            return VK_MULTIPLY;
        case MK_PADADD:
            return VK_ADD;
        case MK_PADENTER:
            return VK_SEPARATOR;
        case MK_PADSUB:
            return VK_SUBTRACT;
        case MK_PADDOT:
            return VK_DECIMAL;
        case MK_PADDIV:
            return VK_DIVIDE;
        case MK_F1:
            return VK_F1;
        case MK_F2:
            return VK_F2;
        case MK_F3:
            return VK_F3;
        case MK_F4:
            return VK_F4;
        case MK_F5:
            return VK_F5;
        case MK_F6:
            return VK_F6;
        case MK_F7:
            return VK_F7;
        case MK_F8:
            return VK_F8;
        case MK_F9:
            return VK_F9;
        case MK_F10:
            return VK_F10;
        case MK_F11:
            return VK_F11;
        case MK_F12:
            return VK_F12;
        case MK_NUMLOCK:
            return VK_NUMLOCK;
        case MK_SCROLL:
            return VK_SCROLL;
        case MK_LSHIFT:
            return VK_LSHIFT;
        case MK_RSHIFT:
            return VK_RSHIFT;
        case MK_LCONTROL:
            return VK_LCONTROL;
        case MK_RCONTROL:
            return VK_RCONTROL;
        case MK_LALT:
            return VK_LMENU;
        case MK_RALT:
            return VK_RMENU;
        case MK_SEMICOLON:
            return VK_OEM_1;
        case MK_EQUALS:
            return VK_OEM_PLUS;
        case MK_COMMA:
            return VK_OEM_COMMA;
        case MK_MINUS:
            return VK_OEM_MINUS;
        case MK_DOT:
            return VK_OEM_PERIOD;
        case MK_SLASH:
            return VK_OEM_2;
        case MK_BACKTICK:
            return VK_OEM_3;
        case MK_LSQUARE:
            return VK_OEM_4;
        case MK_BACKSLASH:
            return VK_OEM_5;
        case MK_RSQUARE:
            return VK_OEM_6;
        case MK_TICK:
            return VK_OEM_7;
        }
        return 0;
    }

    int mgKeyDown(MgSurface *surface, int key)
    {
        MgInternal *win;
        int k = mgWinVK(key);
        if (GetFocus() != surface->handle)
            return 0;
        win = mgInternal(surface);
        return win->keys[k] && !win->prev[k];
    }

    int mgKeyHeld(MgSurface *surface, int key)
    {
        MgInternal *win;
        int k = mgWinVK(key);
        if (GetFocus() != surface->handle)
            return 0;
        win = mgInternal(surface);
        return win->keys[k];
    }

    int mgReadChar(MgSurface *surface)
    {
        MgInternal *win = mgInternal(surface);
        int c = win->lastChar;
        win->lastChar = 0;
        return c;
    }

// We supply our own WinMain and just chain through to the user's
// real entry point.
#ifdef UNICODE
    int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
#else
    int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
    {
        int n, argc;
        LPWSTR *wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
        char **argv = (char **)calloc(argc + 1, sizeof(int));

        (void)hInstance;
        (void)hPrevInstance;
        (void)lpCmdLine;
        (void)nCmdShow;

        for (n = 0; n < argc; n++)
        {
            int len = WideCharToMultiByte(CP_UTF8, 0, wargv[n], -1, 0, 0, NULL, NULL);
            argv[n] = (char *)malloc(len);
            WideCharToMultiByte(CP_UTF8, 0, wargv[n], -1, argv[n], len, NULL, NULL);
        }
        return main(argc, argv);
    }
#endif // _WIN32

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_linux.c

#if __linux__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xlocale.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <GL/glx.h>

    static Display *dpy;
    static Window root;
    static XVisualInfo *vi;
    static Atom wmDeleteMessage;
    static XIM inputMethod;
    static GLXFBConfig fbConfig;

    static PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = 0;

    static void initX11Stuff()
    {
        static int done = 0;
        if (!done)
        {
            dpy = XOpenDisplay(NULL);
            if (dpy == NULL)
            {
                mgError(0, "Cannot connect to X server");
            }

            root = DefaultRootWindow(dpy);

            static int attribList[] = {GLX_RENDER_TYPE,
                                       GLX_RGBA_BIT,
                                       GLX_DRAWABLE_TYPE,
                                       GLX_WINDOW_BIT,
                                       GLX_DOUBLEBUFFER,
                                       1,
                                       GLX_RED_SIZE,
                                       1,
                                       GLX_GREEN_SIZE,
                                       1,
                                       GLX_BLUE_SIZE,
                                       1,
                                       None};

            int fbcCount = 0;
            GLXFBConfig *fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), attribList, &fbcCount);
            if (!fbc)
            {
                mgError(0, "Failed to choose FB config");
            }
            fbConfig = fbc[0];

            vi = glXGetVisualFromFBConfig(dpy, fbConfig);
            if (vi == NULL)
            {
                mgError(0, "No appropriate visual found");
            }

            GLXContext tmpCtx = glXCreateContext(dpy, vi, 0, GL_TRUE);
            glXCreateContextAttribsARB =
                (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");
            glXDestroyContext(dpy, tmpCtx);
            if (!glXCreateContextAttribsARB)
            {
                mgError(0, "Failed to get glXCreateContextAttribsARB");
            }

            inputMethod = XOpenIM(dpy, NULL, NULL, NULL);
            if (inputMethod == NULL)
            {
                mgError(0, "Failed to create input method");
            }

            wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);

            done = 1;
        }
    }

    static int hasGLXExtension(Display *display, const char *wanted)
    {
        const char *extensions = glXQueryExtensionsString(display, DefaultScreen(display));
        char *dup = strdup(extensions);
        char *found = 0;

        for (char *start = dup;; start = 0)
        {
            found = strtok(start, " ");
            if (found == 0 || strcmp(found, wanted) == 0)
            {
                break;
            }
        }

        free(dup);
        return found != 0;
    }

    static void setupVSync(Display *display, Window win)
    {
        if (hasGLXExtension(display, "GLX_EXT_swap_control"))
        {
            PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT =
                (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalEXT");
            if (glXSwapIntervalEXT)
            {
                glXSwapIntervalEXT(display, win, 1);
            }
        }
        else if (hasGLXExtension(display, "GLX_MESA_swap_control"))
        {
            PFNGLXSWAPINTERVALMESAPROC glXSwapIntervalMESA =
                (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalMESA");
            if (glXSwapIntervalMESA)
            {
                glXSwapIntervalMESA(1);
            }
        }
        else if (hasGLXExtension(display, "GLX_SGI_swap_control"))
        {
            PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI =
                (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalSGI");
            if (glXSwapIntervalSGI)
            {
                glXSwapIntervalSGI(1);
            }
        }
    }

    static void mgHideCursor(MgInternal *win)
    {
        Cursor invisibleCursor;
        Pixmap bitmapNoData;
        XColor black;
        static char noData[] = {0, 0, 0, 0, 0, 0, 0, 0};
        black.red = black.green = black.blue = 0;

        bitmapNoData = XCreateBitmapFromData(win->dpy, win->win, noData, 8, 8);
        invisibleCursor = XCreatePixmapCursor(win->dpy, bitmapNoData, bitmapNoData, &black, &black, 0, 0);
        XDefineCursor(win->dpy, win->win, invisibleCursor);
        XFreeCursor(win->dpy, invisibleCursor);
        XFreePixmap(win->dpy, bitmapNoData);
    }

    typedef struct
    {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long inputMode;
        unsigned long status;
    } WindowHints;

    MgSurface *mgWindow(int w, int h, const char *title, int flags)
    {
        MgSurface *surface = 0;
        Colormap cmap;
        XSetWindowAttributes swa;
        Window xwin;
        GLXContext glc;
        XIC ic;
        int scale;

        initX11Stuff();

        if (flags & MG_AUTO)
        {
            // Always use a 1:1 pixel size, unless downscaled by mgEnforceScale below.
            scale = 1;
        }
        else
        {
            // See how big we can make it and still fit on-screen.
            Screen *screen = DefaultScreenOfDisplay(dpy);
            int maxW = WidthOfScreen(screen);
            int maxH = HeightOfScreen(screen);
            scale = mgCalcScale(w, h, maxW, maxH);
        }

        scale = mgEnforceScale(scale, flags);

        cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
        swa.colormap = cmap;
        swa.event_mask = StructureNotifyMask;

        // Create window of wanted size
        xwin = XCreateWindow(dpy, root, 0, 0, w * scale, h * scale, 0, vi->depth, InputOutput, vi->visual,
                             CWColormap | CWEventMask, &swa);
        XMapWindow(dpy, xwin);

        if (flags & MG_FULLSCREEN)
        {
            // https://superuser.com/questions/1680077/does-x11-actually-have-a-native-fullscreen-mode
            Atom wm_state = XInternAtom(dpy, "_NET_WM_STATE", true);
            Atom wm_fullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", true);
            XChangeProperty(dpy, xwin, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wm_fullscreen, 1);
        }
        else
        {
            // Wait for window to get mapped
            for (;;)
            {
                XEvent e;
                XNextEvent(dpy, &e);
                if (e.type == MapNotify)
                {
                    break;
                }
            }

            // Reset size if we did not get the window size we wanted above.
            XWindowAttributes wa;
            XGetWindowAttributes(dpy, xwin, &wa);
            scale = mgCalcScale(w, h, wa.width, wa.height);
            scale = mgEnforceScale(scale, flags);
            XResizeWindow(dpy, xwin, w * scale, h * scale);
        }

        XTextProperty prop;
        int result = Xutf8TextListToTextProperty(dpy, (char **)&title, 1, XUTF8StringStyle, &prop);
        if (result == Success)
        {
            Atom wmName = XInternAtom(dpy, "_NET_WM_NAME", 0);
            XSetTextProperty(dpy, xwin, &prop, wmName);
            XFree(prop.value);
        }

        ic = XCreateIC(inputMethod, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, xwin, NULL);
        if (ic == NULL)
        {
            printf("Failed to create input context\n");
            exit(0);
        }
        XSetICFocus(ic);

        XSetWMProtocols(dpy, xwin, &wmDeleteMessage, 1);

        glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
        int contextAttributes[] = {GLX_CONTEXT_MAJOR_VERSION_ARB, 3, GLX_CONTEXT_MINOR_VERSION_ARB, 3, None};
        glc = glXCreateContextAttribsARB(dpy, fbConfig, NULL, GL_TRUE, contextAttributes);
        glXMakeCurrent(dpy, xwin, glc);

        setupVSync(dpy, xwin);

        surface = mgBitmap2(w, h, sizeof(MgInternal));
        surface->handle = (void *)xwin;

        MgInternal *win = mgInternal(surface);
        win->win = xwin;
        win->dpy = dpy;
        win->glc = glc;
        win->ic = ic;

        win->shown = 0;
        win->closed = 0;
        win->scale = scale;

        win->lastChar = 0;
        win->flags = flags;
        win->p1 = win->p2 = win->p3 = 0;
        win->p4 = 1;
        win->widgetsWanted = 0;
        win->widgetAlpha = 0;
        win->widgetsScale = 0;
        win->widgets = 0;
        win->gl.gl_legacy = 0;

        memset(win->keys, 0, 256);
        memset(win->prev, 0, 256);

        if (flags & MG_NOCURSOR)
        {
            mgHideCursor(win);
        }

        mgPosition(surface, win->scale, surface->w, surface->h, win->pos);
        mgGAPICreate(surface);
        mgGAPIBegin(surface);

        return surface;
    }

    int mgClosed(MgSurface *surface)
    {
        MgInternal *win = mgInternal(surface);
        return win->win == 0;
    }

    int mgGAPIBegin(MgSurface *surface)
    {
        MgSurfaceInternal *win = mgInternal(surface);
        return glXMakeCurrent(win->dpy, win->win, win->glc) ? 0 : -1;
    }

    int mgGAPIEnd(MgSurface *surface)
    {
        (void)surface;
        return glXMakeCurrent(NULL, 0, 0) ? 0 : -1;
    }

    int mgKeyDown(MgSurface *surface, int key)
    {
        MgSurfaceInternal *win;
        assert(key < 256);
        win = mgInternal(surface);
        return win->keys[key] && !win->prev[key];
    }

    int mgKeyHeld(MgSurface *surface, int key)
    {
        MgSurfaceInternal *win;
        assert(key < 256);
        win = mgInternal(surface);
        return win->keys[key];
    }

    int mgReadChar(MgSurface *surface)
    {
        MgSurfaceInternal *win = mgInternal(surface);
        int c = win->lastChar;
        win->lastChar = 0;
        return c;
    }

    uint8_t mgKeyFromX11(KeySym sym)
    {
        if (sym >= 'a' && sym <= 'z')
        {
            return (uint8_t)sym - ('a' - 'A');
        }

        if (sym >= '0' && sym <= '9')
        {
            return (uint8_t)sym;
        }

        switch (sym)
        {
        case XK_KP_0:
            return MK_PAD0;
        case XK_KP_1:
            return MK_PAD1;
        case XK_KP_2:
            return MK_PAD2;
        case XK_KP_3:
            return MK_PAD3;
        case XK_KP_4:
            return MK_PAD4;
        case XK_KP_5:
            return MK_PAD5;
        case XK_KP_6:
            return MK_PAD6;
        case XK_KP_7:
            return MK_PAD7;
        case XK_KP_8:
            return MK_PAD8;
        case XK_KP_9:
            return MK_PAD9;

        case XK_KP_Multiply:
            return MK_PADMUL;
        case XK_KP_Divide:
            return MK_PADDIV;
        case XK_KP_Add:
            return MK_PADADD;
        case XK_KP_Subtract:
            return MK_PADSUB;
        case XK_KP_Decimal:
            return MK_PADDOT;
        case XK_KP_Enter:
            return MK_PADENTER;

        case XK_F1:
            return MK_F1;
        case XK_F2:
            return MK_F2;
        case XK_F3:
            return MK_F3;
        case XK_F4:
            return MK_F4;
        case XK_F5:
            return MK_F5;
        case XK_F6:
            return MK_F6;
        case XK_F7:
            return MK_F7;
        case XK_F8:
            return MK_F8;
        case XK_F9:
            return MK_F9;
        case XK_F10:
            return MK_F10;
        case XK_F11:
            return MK_F11;
        case XK_F12:
            return MK_F12;

        case XK_BackSpace:
            return MK_BACKSPACE;
        case XK_Tab:
            return MK_TAB;
        case XK_Return:
            return MK_RETURN;
        case XK_Pause:
            return MK_PAUSE;
        case XK_Caps_Lock:
            return MK_CAPSLOCK;
        case XK_Escape:
            return MK_ESCAPE;
        case XK_space:
            return MK_SPACE;

        case XK_Page_Up:
            return MK_PAGEUP;
        case XK_Page_Down:
            return MK_PAGEDN;
        case XK_End:
            return MK_END;
        case XK_Home:
            return MK_HOME;
        case XK_Left:
            return MK_LEFT;
        case XK_Up:
            return MK_UP;
        case XK_Right:
            return MK_RIGHT;
        case XK_Down:
            return MK_DOWN;
        case XK_Insert:
            return MK_INSERT;
        case XK_Delete:
            return MK_DELETE;

        case XK_Meta_L:
            return MK_LWIN;
        case XK_Meta_R:
            return MK_RWIN;
        case XK_Num_Lock:
            return MK_NUMLOCK;
        case XK_Scroll_Lock:
            return MK_SCROLL;
        case XK_Shift_L:
            return MK_LSHIFT;
        case XK_Shift_R:
            return MK_RSHIFT;
        case XK_Control_L:
            return MK_LCONTROL;
        case XK_Control_R:
            return MK_RCONTROL;
        case XK_Alt_L:
            return MK_LALT;
        case XK_Alt_R:
            return MK_RALT;

        case XK_semicolon:
            return MK_SEMICOLON;
        case XK_equal:
            return MK_EQUALS;
        case XK_comma:
            return MK_COMMA;
        case XK_minus:
            return MK_MINUS;
        case XK_period:
            return MK_DOT;
        case XK_slash:
            return MK_SLASH;
        case XK_grave:
            return MK_BACKTICK;
        case XK_bracketleft:
            return MK_LSQUARE;
        case XK_backslash:
            return MK_BACKSLASH;
        case XK_bracketright:
            return MK_RSQUARE;
        case XK_apostrophe:
            return MK_TICK;
        }
        return 0;
    }

    static void mgUpdateModifiers(MgSurfaceInternal *win)
    {
        win->keys[MK_SHIFT] = win->keys[MK_LSHIFT] || win->keys[MK_RSHIFT];
        win->keys[MK_CONTROL] = win->keys[MK_LCONTROL] || win->keys[MK_RCONTROL];
        win->keys[MK_ALT] = win->keys[MK_LALT] || win->keys[MK_RALT];
    }

    static void mgInterpretChar(MgSurfaceInternal *win, Window root, unsigned int keycode, unsigned int mask)
    {
        XKeyEvent event;
        memset(&event, 0, sizeof(event));
        event.type = KeyPress;
        event.display = win->dpy;
        event.root = root;
        event.window = win->win;
        event.state = mask;
        event.keycode = keycode;
        char inputTextUTF8[10];
        Status status = 0;
        Xutf8LookupString(win->ic, &event, inputTextUTF8, sizeof(inputTextUTF8), NULL, &status);

        if (status == XLookupChars)
        {
            mgDecodeUTF8(inputTextUTF8, &win->lastChar);
        }
    }

    static void mgProcessInput(MgSurfaceInternal *win, int winWidth, int winHeight)
    {
        {
            Window focused;
            int revertTo;
            XGetInputFocus(win->dpy, &focused, &revertTo);

            if (win->win != focused)
            {
                return;
            }
        }

        Window root;
        Window child;
        int rootX;
        int rootY;
        int winX;
        int winY;
        unsigned int mask;

        if (XQueryPointer(win->dpy, win->win, &root, &child, &rootX, &rootY, &winX, &winY, &mask))
        {
            static unsigned int prevButtons;
            unsigned int buttons = mask & (Button1Mask | Button2Mask | Button3Mask);

            win->mouseX = (winX - win->pos[0]) / win->scale;
            win->mouseY = (winY - win->pos[1]) / win->scale;

            if (buttons != prevButtons && (winX > 0 && winX < winWidth) && (winY > 0 && winY < winHeight))
            {
                win->mouseButtons = (buttons & Button1Mask)       ? 1
                                    : 0 | (buttons & Button3Mask) ? 2
                                    : 0 | (buttons & Button2Mask) ? 4
                                                                  : 0;
            }
            prevButtons = buttons;
        }

        static char prevKeys[32];
        char keys[32];
        XQueryKeymap(win->dpy, keys);
        for (int i = 0; i < 32; i++)
        {
            char thisBlock = keys[i];
            char prevBlock = prevKeys[i];
            if (thisBlock != prevBlock)
            {
                for (int j = 0; j < 8; j++)
                {
                    int thisBit = thisBlock & 1;
                    int prevBit = prevBlock & 1;
                    thisBlock >>= 1;
                    prevBlock >>= 1;
                    if (thisBit != prevBit)
                    {
                        int keyCode = 8 * i + j;
                        KeySym keySym = XkbKeycodeToKeysym(win->dpy, keyCode, 0, 0);
                        if (keySym != NoSymbol)
                        {
                            int key = mgKeyFromX11(keySym);
                            win->keys[key] = thisBit;
                            mgUpdateModifiers(win);

                            if (thisBit)
                            {
                                mgInterpretChar(win, root, keyCode, mask);
                            }
                        }
                    }
                }
            }
        }
        memcpy(prevKeys, keys, 32);

        XEvent event;
        while (XCheckTypedWindowEvent(win->dpy, win->win, ClientMessage, &event))
        {
            if (event.xclient.data.l[0] == wmDeleteMessage)
            {
                glXMakeCurrent(win->dpy, None, NULL);
                glXDestroyContext(win->dpy, win->glc);
                XDestroyWindow(win->dpy, win->win);
                win->win = 0;
            }
        }
        XFlush(win->dpy);
    }

    void mgUpdate(MgSurface *surface)
    {
        XWindowAttributes gwa;

        MgSurfaceInternal *win = mgInternal(surface);

        memcpy(win->prev, win->keys, 256);

        XGetWindowAttributes(win->dpy, win->win, &gwa);

        if (win->flags & MG_AUTO)
            mgResize(surface, gwa.width / win->scale, gwa.height / win->scale);
        else
            win->scale = mgEnforceScale(mgCalcScale(surface->w, surface->h, gwa.width, gwa.height), win->flags);

        mgPosition(surface, win->scale, gwa.width, gwa.height, win->pos);
        glXMakeCurrent(win->dpy, win->win, win->glc);
        mgGAPIPresent(surface, gwa.width, gwa.height);
        glXSwapBuffers(win->dpy, win->win);

        mgProcessInput(win, gwa.width, gwa.height);
    }

    void mgFree(MgSurface *surface)
    {
        if (surface->handle)
        {
            MgSurfaceInternal *win = mgInternal(surface);
            if (win->win)
            {
                glXMakeCurrent(win->dpy, None, NULL);
                glXDestroyContext(win->dpy, win->glc);
                XDestroyWindow(win->dpy, win->win);
                win->win = 0;
            }
        }
        free(surface->pix);
        free(surface);
    }

    void mgError(MgSurface *surface, const char *message, ...)
    {
        char tmp[1024];

        va_list args;
        va_start(args, message);
        vsnprintf(tmp, sizeof(tmp), message, args);
        tmp[sizeof(tmp) - 1] = 0;
        va_end(args);

        printf("minigfx fatal error: %s\n", tmp);

        exit(1);
    }

    float mgTime()
    {
        static double lastTime = 0;

        struct timeval tv;
        gettimeofday(&tv, NULL);

        double now = (double)tv.tv_sec + (tv.tv_usec / 1000000.0);
        double elapsed = lastTime == 0 ? 0 : now - lastTime;
        lastTime = now;

        return (float)elapsed;
    }

    void mgMouse(MgSurface *surface, int *x, int *y, int *buttons)
    {
        MgSurfaceInternal *win = mgInternal(surface);
        if (x)
        {
            *x = win->mouseX;
        }
        if (y)
        {
            *y = win->mouseY;
        }
        if (buttons)
        {
            *buttons = win->mouseButtons;
        }
    }

    int mgTouch(MgSurface *surface, MgSurfaceTouchPoint *points, int maxPoints)
    {
        int buttons = 0;
        if (maxPoints > 0)
        {
            mgMouse(surface, &points->x, &points->y, &buttons);
        }
        return buttons ? 1 : 0;
    }

#endif // __linux__

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_gl.c

#if __linux__
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glext.h>
#endif

    extern const char mg_upscale_gl_vs[], mg_upscale_gl_fs[], mg_default_fx_gl_fs[];
    extern const int mg_upscale_gl_vs_size, mg_upscale_gl_fs_size, mg_default_fx_gl_fs_size;

#ifdef _WIN32

#ifdef MG_GAPI_GL_WIN_USE_GLEXT
#include <glext.h>
#include <wglext.h>
#else // short version of glext.h and wglext.h so we don't need to depend on them
#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
    typedef ptrdiff_t GLsizeiptr;
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_BGRA 0x80E1
#define GL_TEXTURE0 0x84C0
    typedef void(APIENTRYP PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
    typedef void(APIENTRYP PFNGLGENBUFFERSARBPROC)(GLsizei n, GLuint *buffers);
    typedef void(APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
    typedef void(APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
    typedef void(APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint array);
    typedef void(APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
    typedef void(APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index,
                                                         GLint size,
                                                         GLenum type,
                                                         GLboolean normalized,
                                                         GLsizei stride,
                                                         const void *pointer);
    typedef GLuint(APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
    typedef char GLchar;
    typedef void(APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader,
                                                  GLsizei count,
                                                  const GLchar *const *string,
                                                  const GLint *length);
    typedef void(APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
    typedef GLuint(APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
    typedef void(APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
    typedef void(APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
    typedef void(APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
    typedef void(APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);
    typedef void(APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
    typedef void(APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    typedef void(APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
    typedef void(APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    typedef void(APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint program);
    typedef GLint(APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
    typedef void(APIENTRYP PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    typedef void(APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint location,
                                                      GLsizei count,
                                                      GLboolean transpose,
                                                      const GLfloat *value);
    typedef void(APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum texture);
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
    typedef BOOL(WINAPI *PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc,
                                                         const int *piAttribIList,
                                                         const FLOAT *pfAttribFList,
                                                         UINT nMaxFormats,
                                                         int *piFormats,
                                                         UINT *nNumFormats);
    typedef HGLRC(WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int *attribList);
#endif

    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs;
    PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    PFNGLGENBUFFERSARBPROC glGenBuffers;
    PFNGLBINDBUFFERPROC glBindBuffer;
    PFNGLBUFFERDATAPROC glBufferData;
    PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    PFNGLCREATESHADERPROC glCreateShader;
    PFNGLSHADERSOURCEPROC glShaderSource;
    PFNGLCOMPILESHADERPROC glCompileShader;
    PFNGLCREATEPROGRAMPROC glCreateProgram;
    PFNGLATTACHSHADERPROC glAttachShader;
    PFNGLLINKPROGRAMPROC glLinkProgram;
    PFNGLDELETESHADERPROC glDeleteShader;
    PFNGLDELETEPROGRAMPROC glDeleteProgram;
    PFNGLGETSHADERIVPROC glGetShaderiv;
    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    PFNGLGETPROGRAMIVPROC glGetProgramiv;
    PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    PFNGLUSEPROGRAMPROC glUseProgram;
    PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    PFNGLUNIFORM4FPROC glUniform4f;
    PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
    PFNGLACTIVETEXTUREPROC glActiveTexture;

    int mgGL11Init(MgSurface *surface)
    {
        int pixel_format;
        MgInternal *win = mgInternal(surface);
        GLStuff *gl = &win->gl;
        PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR),
                                     1,
                                     PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE,
                                     PFD_TYPE_RGBA,
                                     32, // color bits
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     0,
                                     24, // depth
                                     8,  // stencil
                                     0,
                                     PFD_MAIN_PLANE, // is it ignored ?
                                     0,
                                     0,
                                     0,
                                     0};
        if (!(gl->dc = GetDC((HWND)surface->handle)))
        {
            mgError(surface, "Cannot create OpenGL device context.\n");
            return -1;
        }
        if (!(pixel_format = ChoosePixelFormat(gl->dc, &pfd)))
        {
            mgError(surface, "Cannot choose OpenGL pixel format.\n");
            return -1;
        }
        if (!SetPixelFormat(gl->dc, pixel_format, &pfd))
        {
            mgError(surface, "Cannot set OpenGL pixel format.\n");
            return -1;
        }
        if (!(gl->hglrc = wglCreateContext(gl->dc)))
        {
            mgError(surface, "Cannot create OpenGL context.\n");
            return -1;
        }
        if (!wglMakeCurrent(gl->dc, gl->hglrc))
        {
            mgError(surface, "Cannot activate OpenGL context.\n");
            return -1;
        }
        gl->gl_legacy = 1;
        return 0;
    }

    int mgGL33Init(MgSurface *surface)
    {
        int pixel_format;
        UINT num_formats;
        MgInternal *win = mgInternal(surface);
        GLStuff *gl = &win->gl;

        wglChoosePixelFormat = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
        wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
        glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffers");
        glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
        glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
        glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
        glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
        glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
        glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
        glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
        glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
        glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
        glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
        glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
        glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
        glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
        glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
        glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
        glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
        glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
        glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
        glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
        glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
        glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
        glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

        if (!wglChoosePixelFormat || !wglCreateContextAttribs)
        {
            mgError(surface, "Cannot create OpenGL context.\n");
            return -1;
        }
        const int attribList[] = {WGL_DRAW_TO_WINDOW_ARB,
                                  GL_TRUE,
                                  WGL_SUPPORT_OPENGL_ARB,
                                  GL_TRUE,
                                  WGL_DOUBLE_BUFFER_ARB,
                                  GL_TRUE,
                                  WGL_PIXEL_TYPE_ARB,
                                  WGL_TYPE_RGBA_ARB,
                                  WGL_COLOR_BITS_ARB,
                                  32,
                                  WGL_DEPTH_BITS_ARB,
                                  24,
                                  WGL_STENCIL_BITS_ARB,
                                  8,
                                  0};
        int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, 3, WGL_CONTEXT_MINOR_VERSION_ARB, 3, 0};
        if (!wglChoosePixelFormat(gl->dc, attribList, NULL, 1, &pixel_format, &num_formats))
        {
            mgError(surface, "Cannot choose OpenGL pixel format.\n");
            return -1;
        }
        if (!(gl->hglrc = wglCreateContextAttribs(gl->dc, gl->hglrc, attribs)))
        {
            mgError(surface, "Cannot create OpenGL context attribs.\n");
            return -1;
        }
        if (!wglMakeCurrent(gl->dc, gl->hglrc))
        {
            mgError(surface, "Cannot activate OpenGL context.\n");
            return -1;
        }
        gl->gl_legacy = 0;
        return 0;
    }
#endif

    void mgCheckGLError(const char *state)
    {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            mgError(NULL, "got GL error %x when doing %s\n", err, state);
        }
    }

    void mgCheckShaderErrors(GLuint object)
    {
        GLint success;
        GLchar info[2048];
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, sizeof(info), NULL, info);
            mgError(NULL, "shader compile error : %s\n", info);
        }
    }

    void mgCheckProgramErrors(GLuint object)
    {
        GLint success;
        GLchar info[2048];
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, sizeof(info), NULL, info);
            mgError(NULL, "shader link error : %s\n", info);
        }
    }

    void mgCreateShaderProgram(GLStuff *gl, const char *fxSource, int fxSize)
    {
        if (gl->program != 0)
        {
            glDeleteProgram(gl->program);
            gl->program = 0;
        }

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        const char *vs_source = (const char *)&mg_upscale_gl_vs;
        glShaderSource(vs, 1, &vs_source, &mg_upscale_gl_vs_size);
        glCompileShader(vs);
        mgCheckShaderErrors(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fs_sources[] = {
            (const char *)mg_upscale_gl_fs,
            fxSource,
        };
        const int fs_lengths[] = {
            mg_upscale_gl_fs_size,
            fxSize,
        };
        glShaderSource(fs, 2, fs_sources, fs_lengths);
        glCompileShader(fs);
        mgCheckShaderErrors(fs);

        gl->program = glCreateProgram();
        glAttachShader(gl->program, vs);
        glAttachShader(gl->program, fs);
        glLinkProgram(gl->program);
        mgCheckProgramErrors(gl->program);
        glDeleteShader(vs);
        glDeleteShader(fs);

        gl->uniform_projection = glGetUniformLocation(gl->program, "projection");
        gl->uniform_model = glGetUniformLocation(gl->program, "model");
        gl->uniform_parameters = glGetUniformLocation(gl->program, "parameters");
    }

    void mgGAPICreate(MgSurface *surface)
    {
        MgInternal *win = mgInternal(surface);
        GLStuff *gl = &win->gl;
        GLuint VBO;
        GLfloat vertices[] = {// pos      uv
                              0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

#ifdef _WIN32
        if (mgGL11Init(surface))
            return;
        mgGL33Init(surface);
#endif

        if (!gl->gl_legacy)
        {
            // create vao
            glGenVertexArrays(1, &gl->vao);
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glBindVertexArray(gl->vao);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);

            // create program
            mgCreateShaderProgram(gl, mg_default_fx_gl_fs, mg_default_fx_gl_fs_size);
        }

        // create textures
        if (gl->gl_legacy)
        {
            glEnable(GL_TEXTURE_2D);
        }
        glGenTextures(2, gl->tex);
        for (int i = 0; i < 2; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, gl->tex[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl->gl_legacy ? GL_NEAREST : GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl->gl_legacy ? GL_NEAREST : GL_LINEAR);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }

        mgCheckGLError("initialization");
    }

    void mgGAPIDestroy(MgSurface *surface)
    {
        MgInternal *win = mgInternal(surface);
        GLStuff *gl = &win->gl;

        if (mgGAPIBegin(surface) < 0)
        {
            mgError(surface, "Cannot activate OpenGL context.\n");
            return;
        }

        if (!gl->gl_legacy)
        {
            glDeleteTextures(2, gl->tex);
            glDeleteProgram(gl->program);
        }

        mgCheckGLError("destroy");

        if (mgGAPIEnd(surface) < 0)
        {
            mgError(surface, "Cannot deactivate OpenGL context.\n");
            return;
        }
    }

    void mgGAPIDraw(int legacy, GLuint uniform_model, GLuint tex, MgSurface *surface, int x1, int y1, int x2, int y2)
    {
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

        if (!legacy)
        {
            float sx = (float)(x2 - x1);
            float sy = (float)(y2 - y1);
            float tx = (float)x1;
            float ty = (float)y1;

            float model[16] = {sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, tx, ty, 0.0f, 1.0f};

            glUniformMatrix4fv(uniform_model, 1, GL_FALSE, model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        else
        {
            glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2i(x2, y1);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2i(x1, y1);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2i(x1, y2);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2i(x2, y2);
            glEnd();
        }
    }

    void mgGAPIPresent(MgSurface *surface, int w, int h)
    {
        MgInternal *win = mgInternal(surface);
        GLStuff *gl = &win->gl;

        glViewport(0, 0, w, h);
        if (!gl->gl_user_opengl_rendering)
        {
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        if (!gl->gl_legacy)
        {
            float projection[16] = {2.0f / w, 0.0f, 0.0f, 0.0f, 0.0f, -2.0f / h, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f};

            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(gl->vao);
            glUseProgram(gl->program);
            glUniformMatrix4fv(gl->uniform_projection, 1, GL_FALSE, projection);
            glUniform4f(gl->uniform_parameters, win->p1, win->p2, win->p3, win->p4);
        }
        else
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, w, h, 0, -1.0f, 1.0f);
            glEnable(GL_TEXTURE_2D);
        }

        if (gl->gl_user_opengl_rendering)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }
        mgGAPIDraw(gl->gl_legacy, gl->uniform_model, gl->tex[0], surface, win->pos[0], win->pos[1], win->pos[2], win->pos[3]);

        if (win->widgetsScale > 0)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            mgGAPIDraw(gl->gl_legacy, gl->uniform_model, gl->tex[1], win->widgets,
                       (int)(w - win->widgets->w * win->widgetsScale), 0, w, (int)(win->widgets->h * win->widgetsScale));
        }

        mgCheckGLError("present");

        gl->gl_user_opengl_rendering = 0;
    }

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_utils.c

#include <stdio.h>
#include <stdlib.h>

    void *mgReadFile(const char *fileName, int *length)
    {

        // TODO - unicode?
        FILE *file;
        char *data;
        size_t len;

        if (length)
            *length = 0;

        file = fopen(fileName, "rb");
        if (!file)
            return NULL;

        fseek(file, 0, SEEK_END);
        len = ftell(file);
        fseek(file, 0, SEEK_SET);

        data = (char *)malloc(len + 1);
        if (!data)
        {
            fclose(file);
            return NULL;
        }

        if (fread(data, 1, len, file) != len)
        {
            free(data);
            fclose(file);
            return NULL;
        }
        data[len] = '\0';
        fclose(file);

        if (length)
            *length = len;

        return data;
    }

    // Reads a single UTF8 codepoint.
    const char *mgDecodeUTF8(const char *text, int *cp)
    {
        unsigned char c = *text++;
        int extra = 0, min = 0;
        *cp = 0;
        if (c >= 0xf0)
        {
            *cp = c & 0x07;
            extra = 3;
            min = 0x10000;
        }
        else if (c >= 0xe0)
        {
            *cp = c & 0x0f;
            extra = 2;
            min = 0x800;
        }
        else if (c >= 0xc0)
        {
            *cp = c & 0x1f;
            extra = 1;
            min = 0x80;
        }
        else if (c >= 0x80)
        {
            *cp = 0xfffd;
        }
        else
        {
            *cp = c;
        }
        while (extra--)
        {
            c = *text++;
            if ((c & 0xc0) != 0x80)
            {
                *cp = 0xfffd;
                break;
            }
            (*cp) = ((*cp) << 6) | (c & 0x3f);
        }
        if (*cp < min)
        {
            *cp = 0xfffd;
        }
        return text;
    }

    char *mgEncodeUTF8(char *text, int cp)
    {
        if (cp < 0 || cp > 0x10ffff)
        {
            cp = 0xfffd;
        }

#define EMIT(X, Y, Z) *text++ = X | ((cp >> Y) & Z)
        if (cp < 0x80)
        {
            EMIT(0x00, 0, 0x7f);
        }
        else if (cp < 0x800)
        {
            EMIT(0xc0, 6, 0x1f);
            EMIT(0x80, 0, 0x3f);
        }
        else if (cp < 0x10000)
        {
            EMIT(0xe0, 12, 0xf);
            EMIT(0x80, 6, 0x3f);
            EMIT(0x80, 0, 0x3f);
        }
        else
        {
            EMIT(0xf0, 18, 0x7);
            EMIT(0x80, 12, 0x3f);
            EMIT(0x80, 6, 0x3f);
            EMIT(0x80, 0, 0x3f);
        }
        return text;
#undef EMIT
    }

    int mgBeginOpenGL(MgSurface *surface)
    {
#ifdef MG_GAPI_GL
        MgInternal *win = mgInternal(surface);
        win->gl.gl_user_opengl_rendering = 1;
        return mgGAPIBegin(surface) == 0;
#else
    return 0;
#endif
    }

    void mgSetPostShader(MgSurface *surface, const char *code, int size)
    {
#ifdef MG_GAPI_GL
        mgGAPIBegin(surface);
        MgInternal *win = mgInternal(surface);
        GLStuff *gl = &win->gl;
        mgCreateShaderProgram(gl, code, size);
        mgGAPIEnd(surface);
#endif
    }

    void mgSetPostFX(MgSurface *bmp, float p1, float p2, float p3, float p4)
    {
        MgInternal *win = mgInternal(bmp);
        win->p1 = p1;
        win->p2 = p2;
        win->p3 = p3;
        win->p4 = p4;
    }

    // #######################################################################################################################################
    // #######################################################################################################################################
    // #######################################################################################################################################

    // mg_extensions.c

    int mgFps(float dt)
    {
        static int fps = 0;
        static float accumulatedTime = 0.0f;
        static int frameCount = 0;

        accumulatedTime += dt;
        (frameCount)++;

        if (accumulatedTime >= 1.0f)
        {
            fps = frameCount;
            frameCount = 0;
            accumulatedTime = 0.0f;
        }

        return fps;
    }

#ifdef __cplusplus
}
#endif
#endif // MINIGFX_IMPLEMENTATION
