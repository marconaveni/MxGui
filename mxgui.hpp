#ifndef MXGUI_HPP
#define MXGUI_HPP

//--------------------------- Mini XPerience Gui-------------------------------
//                                                                            |
// To use this library, you can define it like this:                          |
//                                                                            |
//----------------------------custom backend-----------------------------------
//                                                                            |
// #define MX_GUI_IMPLEMENTATION                                              |
// #include "mxgui.hpp"                                                       |
//                                                                            |
//----------------------------with Sfml----------------------------------------
//                                                                            |
// #define MX_SFML_BACKEND_IMPLEMENTATION                                     |
// #include "mxgui.hpp"                                                       |
//                                                                            |
//----------------------------with raylib--------------------------------------
//                                                                            |
// #define MX_RAYLIB_BACKEND_IMPLEMENTATION                                   |
// #include "mxgui.hpp"                                                       |
//                                                                            |
//-----------------------------------------------------------------------------

//--------------------------------MXGUI----------------------------------------
//
//  (Panel)                     | Component | state     |
//  (Image)                     | Component | stateless |
//  (Button) -> (Label)         | Component | stateless |
//  (Label)                     | Component | stateless |
//  (ScrollPanel)               | Component | state     | Container(begin)(end)
//  (Slider)                    | Component | state     |
//  (SliderProgress)            | Component | stateless |
//  (Icon)                      | Component | stateless |
//  (ButtonIcon)                | Component | stateless |
//  (ToggleEx)                  | Component | stateless |
//  (CheckBox) -> (ToggleEx)    | Component | stateless |
//  (Toogle) -> (ToggleEx)      | Component | stateless |
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// This file is organized by sections.
// you can search for (SECTION)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// (SECTION) Library version
//-----------------------------------------------------------------------------

#define MXGUI_VERSION "0.0.1 ALPHA"
#define MXGUI_VERSION_NUM 0

//-----------------------------------------------------------------------------
// (SECTION) configs
//-----------------------------------------------------------------------------


#ifndef FORCE_DEBUG
#define FORCE_DEBUG 0 // Force debug
#endif

#ifndef MX_FONT_NOTO_ID
#define MX_FONT_NOTO_ID "notosans" // Font default ID
#endif

#ifndef MX_FONT_AWESOME_ID
#define MX_FONT_AWESOME_ID "fontawesome" // Font awesome ID
#endif

#ifndef MX_DRAG_OFFSET
#define MX_DRAG_OFFSET 4 // Extra area px rect drags
#endif

#ifndef MX_TEXT_LINE_SPACING
#define MX_TEXT_LINE_SPACING 0.0f // Config \n space in texts
#endif

#ifndef MX_BAR_SIZE
#define MX_BAR_SIZE 6 // Bar size scroll
#endif

#ifndef MX_FONT_AWESOME
#define MX_FONT_AWESOME 1 // Enable font_awesome (0 - disabled | 1 enabled)
#endif

#ifndef MX_SUPPRESS_WARNINGS
#define MX_SUPPRESS_WARNINGS 1 // Supress Warnings    (0 - disabled | 1 enabled)
#endif


// warnings headers
#if defined(__GNUC__) && (MX_SUPPRESS_WARNINGS == 1) // GCC and Clang
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

//-----------------------------------------------------------------------------
// (SECTION) Header and defines
//-----------------------------------------------------------------------------

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "mxgui_notosans.hpp"
#include "string.h"


#ifdef MX_RAYLIB_BACKEND_IMPLEMENTATION
#define MX_RAYLIB 1
#elifdef MX_SFML_BACKEND_IMPLEMENTATION
#define MX_SFML 1
#endif

#if (defined(MX_RAYLIB_BACKEND_IMPLEMENTATION) || defined(MX_SFML_BACKEND_IMPLEMENTATION)) && (!defined(MX_GUI_IMPLEMENTATION))
#define MX_GUI_IMPLEMENTATION
#endif

#if FORCE_DEBUG
#define DEBUG_MODE
#endif // FORCE_DEBUG

#ifdef DEBUG_MODE
#include <assert.h>

#ifdef _MSC_VER // MSVC
#define MX_ASSERT(condition, msg) \
    if (!(condition))             \
    __debugbreak()
#else // GCC/Clang
#define MX_ASSERT(condition, msg) \
    if (!(condition))             \
    __builtin_trap()
#endif // _MSC_VER

#else

#define MX_ASSERT(condition, msg)

#endif // _DEBUG


//-----------------------------------------------------------------------------
// (SECTION) Custom memory allocators
//-----------------------------------------------------------------------------

#ifndef MX_MALLOC
#define MX_MALLOC(sz) malloc(sz)
#endif
#ifndef MX_CALLOC
#define MX_CALLOC(n, sz) calloc(n, sz)
#endif
#ifndef MX_REALLOC
#define MX_REALLOC(ptr, sz) realloc(ptr, sz)
#endif
#ifndef MX_FREE
#define MX_FREE(ptr) free(ptr)
#endif


//-----------------------------------------------------------------------------
// (SECTION) nothings libs
//-----------------------------------------------------------------------------


// Third-party header: always suppress regardless of MX_SUPPRESS_WARNINGS,
// since these warnings come from stb_truetype.h, not from mxgui itself.
#if defined(__GNUC__) // GCC and Clang
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#define STBTT_malloc(x, u) ((void)(u), MX_MALLOC(x))
#define STBTT_free(x, u) ((void)(u), MX_FREE(x))

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#if defined(__GNUC__)      // GCC and Clang
#pragma GCC diagnostic pop // "-Wunused-parameter"  "-Wunused-function"
#endif

//-----------------------------------------------------------------------------
// (SECTION) GuiIcons FontAwesome
//-----------------------------------------------------------------------------

#if MX_FONT_AWESOME
#include "mxgui_font_awesome.h"
#include "mxgui_icons_font_awesome7.hpp"
#endif

//-----------------------------------------------------------------------------
// (SECTION) Structs Forward declarations
//-----------------------------------------------------------------------------

struct MxFontManager;
struct MxTextureNative;
struct MxGuiContext;
struct MxFont;
struct MxVec2;

//-----------------------------------------------------------------------------
// (SECTION) basic types
//-----------------------------------------------------------------------------

typedef std::string MxTag;
typedef signed char MxChar8;         // 8-bit signed integer
typedef unsigned char MxUChar8;      // 8-bit unsigned integer
typedef signed short MxShort16;      // 16-bit signed integer
typedef unsigned short MxUShort16;   // 16-bit unsigned integer
typedef signed int MxInt32;          // 32-bit signed integer == int
typedef unsigned int MxUInt32;       // 32-bit unsigned integer
typedef signed long long MxInt64;    // 64-bit signed integer
typedef unsigned long long MxUInt64; // 64-bit unsigned integer

//-----------------------------------------------------------------------------
// (SECTION) Structs types
//-----------------------------------------------------------------------------


struct MxVec2
{
    float x{0.0f};
    float y{0.0f};
};

struct MxRect
{
    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
};

inline constexpr MxVec2 toMxVec2(const MxRect& rec)
{
    return MxVec2{rec.x, rec.y};
}

inline constexpr MxRect toMxRect(const MxVec2& vec)
{
    return MxRect{vec.x, vec.y, 0, 0};
}

inline constexpr MxRect toMxRect(const MxVec2& vec, const MxRect& rec)
{
    return MxRect{vec.x, vec.y, rec.width, rec.height};
}

inline constexpr MxRect toMxRect(const MxVec2& vec, const MxVec2& vec2)
{
    return MxRect{vec.x, vec.y, vec2.x, vec2.y};
}

struct MxImage
{
    void* data{nullptr}; // Image raw data
    int width{0};        // Image base width
    int height{0};       // Image base height
    int mipmaps{1};      // Mipmap levels, 1 by default
    int format{1};       // Data format (PixelFormat type)
};

// MxGlyphInfo, font characters glyphs info
struct MxGlyphInfo
{
    int value{0};    // Character value (Unicode)
    int offsetX{0};  // Character offset X when drawing
    int offsetY{0};  // Character offset Y when drawing
    int advanceX{0}; // Character advance position X
    MxImage image{}; // Character image data
};

// Font, MxGlyphInfo, TextureIdAtlas and array data
struct MxFont
{
    int baseSize{0};              // Base size (default chars height)
    int glyphCount{0};            // Number of glyph characters
    int glyphPadding{0};          // Padding around the glyph characters
    std::string textureNameID{};  // Id Texture atlas containing the glyphs storaged TextureManager
    MxRect* recs{nullptr};        // Rectangles in texture for the glyphs
    MxGlyphInfo* glyphs{nullptr}; // Glyphs info data
};

struct MxColor
{
    MxUChar8 r{255}; // Color red value
    MxUChar8 g{255}; // Color green value
    MxUChar8 b{255}; // Color blue value
    MxUChar8 a{255}; // Color alpha value

    static MxColor WhiteGray;   // Gray (Almost White)
    static MxColor LightGray;   // Light Gray
    static MxColor Gray;        // Gray
    static MxColor DarkGray;    // Dark Gray
    static MxColor Yellow;      // Yellow
    static MxColor Gold;        // Gold
    static MxColor Orange;      // Orange
    static MxColor Pink;        // Pink
    static MxColor Red;         // Red
    static MxColor Maroon;      // Maroon
    static MxColor Green;       // Green
    static MxColor Lime;        // Lime
    static MxColor DarkGreen;   // Dark Green
    static MxColor SkyBlue;     // Sky Blue
    static MxColor Blue;        // Blue
    static MxColor DarkBlue;    // Dark Blue
    static MxColor Purple;      // Purple
    static MxColor Violet;      // Violet
    static MxColor DarkPurple;  // Dark Purple
    static MxColor Beige;       // Beige
    static MxColor Brown;       // Brown
    static MxColor DarkBrown;   // DarkBrown
    static MxColor White;       // White
    static MxColor Magenta;     // Magenta
    static MxColor Cyan;        // Cyan
    static MxColor Black;       // Black
    static MxColor Transparent; // Transparent (no color)
};

inline MxColor MxColor::WhiteGray{245, 245, 245, 255}; // Gray (Almost White)
inline MxColor MxColor::LightGray{200, 200, 200, 255}; // Light Gray
inline MxColor MxColor::Gray{130, 130, 130, 255};      // Gray
inline MxColor MxColor::DarkGray{80, 80, 80, 255};     // Dark Gray
inline MxColor MxColor::Yellow{253, 249, 0, 255};      // Yellow
inline MxColor MxColor::Gold{255, 203, 0, 255};        // Gold
inline MxColor MxColor::Orange{255, 161, 0, 255};      // Orange
inline MxColor MxColor::Pink{255, 109, 194, 255};      // Pink
inline MxColor MxColor::Red{230, 41, 55, 255};         // Red
inline MxColor MxColor::Maroon{190, 33, 55, 255};      // Maroon
inline MxColor MxColor::Green{0, 228, 48, 255};        // Green
inline MxColor MxColor::Lime{0, 158, 47, 255};         // Lime
inline MxColor MxColor::DarkGreen{0, 117, 44, 255};    // Dark Green
inline MxColor MxColor::SkyBlue{102, 191, 255, 255};   // Sky Blue
inline MxColor MxColor::Blue{0, 121, 241, 255};        // Blue
inline MxColor MxColor::DarkBlue{0, 82, 172, 255};     // Dark Blue
inline MxColor MxColor::Purple{200, 122, 255, 255};    // Purple
inline MxColor MxColor::Violet{135, 60, 190, 255};     // Violet
inline MxColor MxColor::DarkPurple{112, 31, 126, 255}; // Dark Purple
inline MxColor MxColor::Beige{211, 176, 131, 255};     // Beige
inline MxColor MxColor::Brown{127, 106, 79, 255};      // Brown
inline MxColor MxColor::DarkBrown{76, 63, 47, 255};    // DarkBrown
inline MxColor MxColor::White{255, 255, 255, 255};     // White
inline MxColor MxColor::Magenta{255, 0, 255, 255};     // Magenta
inline MxColor MxColor::Cyan{0, 255, 255, 255};        // Cyan
inline MxColor MxColor::Black{0, 0, 0, 255};           // Black
inline MxColor MxColor::Transparent{0, 0, 0, 255};     // Transparent (no color)

struct MxMouseEvents
{
    bool isMouseHover{false};
    bool isMousePressed{false};
    bool isMouseRelease{false};
    bool isMouseDown{false};
};

struct MxTransform
{
    MxRect bounds{};
    MxRect worldBounds{};
    MxVec2 anchor{};
};


struct MxStyle
{
    MxColor primaryColor{MxColor::Gray};
    MxInt32 borderWidth{1};
    MxColor borderColor{MxColor::DarkGray};
    MxColor backgroundColor{MxColor::LightGray};
    MxColor textColor{MxColor::DarkGray};
    MxInt32 textSize{20};
    MxInt32 textSpacing{0};
    MxInt32 iconSize{20};
    std::string fontName{MX_FONT_NOTO_ID};
    bool isDarkMode{false};

    static MxStyle Light; // ThemeLight;
    static MxStyle Dark;  // ThemeDark;
};

inline MxStyle MxStyle::Light{}; // Note: that the default parameters are light theme values.
inline MxStyle MxStyle::Dark{.primaryColor{MxColor::WhiteGray},
                             .borderWidth{1},
                             .borderColor{MxColor::White},
                             .backgroundColor{MxColor::DarkGray}, // Dark
                             .textColor{MxColor::White},
                             .textSize{20},
                             .textSpacing{0},
                             .iconSize{20},
                             .fontName{MX_FONT_NOTO_ID},
                             .isDarkMode{true}};


//-----------------------------------------------------------------------------
// (SECTION) Structs components
//-----------------------------------------------------------------------------

struct PanelComponent
{
    MxVec2 Offset{};
    bool isDrag{false};
};

struct ScrollPanelComponent
{
    bool isDrag{false};
    MxTransform transformCanvas{};
    float scrollTop{0.0f};
    MxRect scrollBarThumb{};
    float pointDrag{0.0f};
};

struct SliderComponent
{
    bool isDrag{false};
    float progress{0.5f};
};

//-----------------------------------------------------------------------------
// (SECTION) enums
//-----------------------------------------------------------------------------

typedef enum
{
    MX_NONE = 0,
    MX_CONTAINED = 1,
    MX_OUTLINE = 2,
} MxButtonStyle;

typedef enum
{
    MX_MOUSE_BUTTON_LEFT = 0,   // Mouse button left
    MX_MOUSE_BUTTON_RIGHT = 1,  // Mouse button right
    MX_MOUSE_BUTTON_MIDDLE = 2, // Mouse button middle (pressed wheel)
} MxMouseButton;


//-----------------------------------------------------------------------------
// (SECTION) public API functions
//-----------------------------------------------------------------------------

namespace mxgui
{

    MxGuiContext* createContext(MxStyle style = MxStyle{});
    void destroyContext(MxGuiContext* ctx);
    MxGuiContext* getCurrentContext();
    MxStyle getStyle(MxGuiContext* ctx);

    MxTransform getCurrentTransform(MxGuiContext* ctx);
    MxMouseEvents getCurrentMouseEvents(MxGuiContext* ctx);

    void createImage(const std::filesystem::path& path, const std::string& imageName, bool smooth = true);

    MxVec2 guiPanel(MxGuiContext* ctx, MxTag tag, MxRect bounds, MxVec2 anchor = MxVec2{0}, bool enableDrag = false);
    void guiImage(MxGuiContext* ctx, const std::string& imageName, MxRect bounds, MxVec2 anchor = MxVec2{0}, MxColor color = MxColor::White);
    bool guiButton(MxGuiContext* ctx, const std::string& text, MxRect bounds, MxVec2 anchor = MxVec2{0}, int buttonStyle = MX_CONTAINED, bool enable = true);
    void guiLabel(MxGuiContext* ctx, const std::string& text, MxVec2 bounds, MxVec2 anchor = MxVec2{0});
    void guiScrollPanelBegin(MxGuiContext* ctx, MxTag tag, MxRect bounds, MxRect scrollBounds, MxVec2 anchor = MxVec2{0}, bool enable = true);
    void guiScrollPanelEnd(MxGuiContext* ctx, MxTag tag);
    float guiSlider(MxGuiContext* ctx, MxTag tag, MxRect bounds, MxVec2 anchor, bool enable);
    void guiSliderProgress(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, float progress);
    void guiIcon(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, int codepoint, int size = -1);
    bool guiIconButton(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, int codepoint, int size = -1, bool enable = true);
    bool guiCheckBox(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, bool& checked);
    bool guiToogle(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, bool& checked);

} // namespace mxgui


//-----------------------------------------------------------------------------
// (SECTION) back-end functions
// Note: These functions need to be implemented if CUSTOM_BACKEND is used.
//-----------------------------------------------------------------------------

void nativeInit();
MxTextureNative nativeLoadTexture(const std::filesystem::path& path);
MxTextureNative nativeLoadTextureFromImageData(void* data, int width, int height, int mipmaps, int format);
void nativeSetTextureSmooth(MxTextureNative* texture, bool enable);
MxVec2 nativeTextureSize(const MxTextureNative* texture);
bool nativeTextureIsSmooth(const MxTextureNative* texture);
bool nativeIsValidTexture(const MxTextureNative* texture);
void nativeUnloadTexture(const MxTextureNative* texture);

// window
bool isCursorOnScreen();
MxVec2 windowSize();

// clip
void beginScissorMode(int x, int y, int width, int height);
void endScissorMode();

// mouse
MxVec2 getMousePosition();
MxVec2 getMouseDelta();
float getMouseWheelMove();
bool isMouseButtonPressed(int button);
bool isMouseButtonDown(int button);
bool isMouseButtonReleased(int button);

// draw
void drawRectangleLinesEx(MxRect rec, float lineThick, MxColor color);
void drawRectanglePro(MxRect rec, MxVec2 origin, float rotation, MxColor color);
void drawTexturePro(const std::string& textureNameID, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint);
void drawCircle(MxVec2 center, float radius, MxColor color);

//-----------------------------------------------------------------------------
// (SECTION) Internal forward declarations
//-----------------------------------------------------------------------------

// internal functions
bool isValidFont(const MxFont& font);
bool isValidImage(const MxImage& image);
MxVec2 measureTextInternal(MxFont font, const std::string& text, float fontSize, float spacing);
MxFont loadFontFromMemoryInternal(const std::string& textureNameID, const unsigned char* fileData, int fontSize, const int* codepoints, int codepointCount);

// mxgui functions
void pushScissor(int x, int y, int width, int height);
void popScissor();
MxTransform updateTransformWorld(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor);

// managers functions handle
void initManagers(MxStyle style);
void closeManagers();

// texture managers functions
void loadTexture(const std::filesystem::path& path, const std::string& textureNameID);
void loadTextureFromMemory(void* data, int width, int height, int format, int mipmaps, const std::string& textureNameID);
void unloadTexture(const std::string& textureNameID);
MxVec2 getSizeTexture(const std::string& textureNameID);
void setSmoothTexture(const std::string& textureNameID, bool enable);
bool isSmoothTexture(const std::string& textureNameID);
const MxTextureNative* getTexture(const std::string& textureNameID);

// font managers functions
const MxFont* getFont(const std::string& fontNameID);
MxVec2 measureText(const std::string& fontNameID, const std::string& text, int fontSize, int spacing);
void drawText(const std::string& fontNameID, const std::string& text, MxVec2 position, float fontSize, float spacing, MxColor tint);
void drawIconEx(int codepoint, MxVec2 position, MxColor color, int size);
void loadFont(const std::string& textureNameID, const std::filesystem::path& path, int fontSize, const int* codepoints, int codepointCount);


//-----------------------------------------------------------------------------
// macros getters and setters to MxGuiContext
//-----------------------------------------------------------------------------

#define INSERT_COMPONENT(componentsList, type)                                            \
    inline type* insert##type(MxTag tag, type component)                                  \
    {                                                                                     \
        const MxTag hash = "##" + tag;                                                    \
        auto [insertedIt, isInserted] = componentsList.insert_or_assign(hash, component); \
        return &insertedIt->second;                                                       \
    }

#define GET_COMPONENT(componentsList, type)                                    \
    inline type* get##type(MxTag tag)                                          \
    {                                                                          \
        const MxTag hash = "##" + tag;                                         \
                                                                               \
        auto it = componentsList.find(hash);                                   \
        if (it != componentsList.end())                                        \
        {                                                                      \
            return &it->second;                                                \
        }                                                                      \
                                                                               \
        auto [insertedIt, isInserted] = componentsList.insert({hash, type{}}); \
        return &insertedIt->second;                                            \
    }

#define COMPONENT(componentsList, type)    \
    INSERT_COMPONENT(componentsList, type) \
    GET_COMPONENT(componentsList, type)


struct MxGuiContext
{

    void init(MxStyle style)
    {
        m_style = style;
        initManagers(m_style);
    }
    void close() { closeManagers(); }


    COMPONENT(m_panels, PanelComponent);
    COMPONENT(m_scrollPanels, ScrollPanelComponent);
    COMPONENT(m_sliderComponents, SliderComponent);

    //-----------------------------------------------------------------------------
    // Store the values ​​of the common types from the last invoked component
    //-----------------------------------------------------------------------------
    void updateCurrents(MxTransform currentTransform, MxMouseEvents currentMouseEvents)
    {
        m_currentTransform = currentTransform;
        m_currentMouseEvents = currentMouseEvents;
    }

    MxTransform m_currentTransform{};
    MxMouseEvents m_currentMouseEvents{};

    //-----------------------------------------------------------------------------
    // Components pools memory
    //-----------------------------------------------------------------------------

    std::unordered_map<MxTag, PanelComponent> m_panels;
    std::unordered_map<MxTag, ScrollPanelComponent> m_scrollPanels;
    std::unordered_map<MxTag, SliderComponent> m_sliderComponents;

    //-----------------------------------------------------------------------------
    // Shareds positions
    //-----------------------------------------------------------------------------

    MxVec2 m_anchor{0, 0};
    float m_scrollTop{0.0f};
    MxInt32 m_layerMouseEvents{0};

    //-----------------------------------------------------------------------------
    // Themes
    //-----------------------------------------------------------------------------

    MxStyle m_style{};
};


// warnings headers
#if defined(__GNUC__) && (MX_SUPPRESS_WARNINGS == 1) // GCC and Clang
#pragma GCC diagnostic pop                           // "-Wunused-parameter"  "-Wunused-function"
#endif

#endif // MXGUI_HPP


/////////////////////////////////////////////////////
//  implementations
/////////////////////////////////////////////////////

#ifdef MX_GUI_IMPLEMENTATION

// warnings implementations
#if defined(__GNUC__) && (MX_SUPPRESS_WARNINGS == 1) // GCC and Clang
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif


//-----------------------------------------------------------------------------
// Internal functions
//-----------------------------------------------------------------------------

std::vector<char> loadFileData(const std::filesystem::path& path)
{

    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << path << std::endl;
        return std::vector<char>{};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);

    if (file.read(buffer.data(), size))
    {
        std::cout << "File '" << path << "' loaded sucessfull. Bytes: " << size << '\n';
    }
    return buffer;
}


//-----------------------------------------------------------------------------
// (SECTION) Draw Text Funtions
// Note: The code is initially the same code as text.c "raylib", to validate it and have something functional.
// Todo: adapt as necessary to make it work and optimize it for mxgui.
//-----------------------------------------------------------------------------

const char* codepointToUTF8(int codepoint, int* utf8Size)
{
    static char utf8[6] = {0};
    memset(utf8, 0, 6); // Clear static array
    int size = 0;       // Byte size of codepoint

    if (codepoint <= 0x7f)
    {
        utf8[0] = (char)codepoint;
        size = 1;
    }
    else if (codepoint <= 0x7ff)
    {
        utf8[0] = (char)(((codepoint >> 6) & 0x1f) | 0xc0);
        utf8[1] = (char)((codepoint & 0x3f) | 0x80);
        size = 2;
    }
    else if (codepoint <= 0xffff)
    {
        utf8[0] = (char)(((codepoint >> 12) & 0x0f) | 0xe0);
        utf8[1] = (char)(((codepoint >> 6) & 0x3f) | 0x80);
        utf8[2] = (char)((codepoint & 0x3f) | 0x80);
        size = 3;
    }
    else if (codepoint <= 0x10ffff)
    {
        utf8[0] = (char)(((codepoint >> 18) & 0x07) | 0xf0);
        utf8[1] = (char)(((codepoint >> 12) & 0x3f) | 0x80);
        utf8[2] = (char)(((codepoint >> 6) & 0x3f) | 0x80);
        utf8[3] = (char)((codepoint & 0x3f) | 0x80);
        size = 4;
    }

    *utf8Size = size;

    return utf8;
}


int getCodepointNext(const char* text, int* codepointSize)
{
    const char* ptr = text;
    int codepoint = 0x3f; // Codepoint (defaults to '?')
    *codepointSize = 1;
    if (text == NULL)
    {
        return codepoint;
    }

    // Get current codepoint and bytes processed
    if (0xf0 == (0xf8 & ptr[0]))
    {
        // 4 byte UTF-8 codepoint
        if (((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80) || ((ptr[3] & 0xC0) ^ 0x80))
        {
            return codepoint;
        } // 10xxxxxx checks
        codepoint = ((0x07 & ptr[0]) << 18) | ((0x3f & ptr[1]) << 12) | ((0x3f & ptr[2]) << 6) | (0x3f & ptr[3]);
        *codepointSize = 4;
    }
    else if (0xe0 == (0xf0 & ptr[0]))
    {
        // 3 byte UTF-8 codepoint */
        if (((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80))
        {
            return codepoint;
        } // 10xxxxxx checks
        codepoint = ((0x0f & ptr[0]) << 12) | ((0x3f & ptr[1]) << 6) | (0x3f & ptr[2]);
        *codepointSize = 3;
    }
    else if (0xc0 == (0xe0 & ptr[0]))
    {
        // 2 byte UTF-8 codepoint
        if ((ptr[1] & 0xC0) ^ 0x80)
        {
            return codepoint;
        } // 10xxxxxx checks
        codepoint = ((0x1f & ptr[0]) << 6) | (0x3f & ptr[1]);
        *codepointSize = 2;
    }
    else if (0x00 == (0x80 & ptr[0]))
    {
        // 1 byte UTF-8 codepoint
        codepoint = ptr[0];
        *codepointSize = 1;
    }

    return codepoint;
}

int getGlyphIndex(MxFont font, int codepoint)
{
    int index = 0;
    if (!isValidFont(font))
    {
        return index;
    }

    int fallbackIndex = 0; // Get index of fallback glyph '?'

    // Look for character index in the unordered charset
    for (int i = 0; i < font.glyphCount; i++)
    {
        if (font.glyphs[i].value == 63)
        {
            fallbackIndex = i;
        }

        if (font.glyphs[i].value == codepoint)
        {
            index = i;
            break;
        }
    }

    if ((index == 0) && (font.glyphs[0].value != codepoint))
    {
        index = fallbackIndex;
    }


    return index;
}

void drawTextCodepoint(MxFont font, int codepoint, MxVec2 position, float fontSize, MxColor tint)
{
    // Character index position in sprite font
    // NOTE: In case a codepoint is not available in the font, index returned points to '?'
    int index = getGlyphIndex(font, codepoint);
    float scaleFactor = fontSize / font.baseSize; // Character quad scaling factor

    // Character destination rectangle on screen
    // NOTE: Considering glyph padding on drawing
    MxRect dstRec = {position.x + font.glyphs[index].offsetX * scaleFactor - (float)font.glyphPadding * scaleFactor,
                     position.y + font.glyphs[index].offsetY * scaleFactor - (float)font.glyphPadding * scaleFactor,
                     (font.recs[index].width + 2.0f * font.glyphPadding) * scaleFactor,
                     (font.recs[index].height + 2.0f * font.glyphPadding) * scaleFactor};

    // Character source rectangle from font texture atlas
    // NOTE: Considering glyphs padding when drawing, it could be required for outline/glow shader effects
    MxRect srcRec = {font.recs[index].x - (float)font.glyphPadding, font.recs[index].y - (float)font.glyphPadding, font.recs[index].width + 2.0f * font.glyphPadding, font.recs[index].height + 2.0f * font.glyphPadding};

    // Draw the character texture on the screen
    drawTexturePro(font.textureNameID, srcRec, dstRec, MxVec2{0, 0}, 0.0f, tint);
}

void drawTextEx(MxFont font, const std::string& text, MxVec2 position, float fontSize, float spacing, MxColor tint)
{

    if (font.textureNameID.empty()) // Security check in case of not valid atlas fontID
    {
        return;
    }

    int size = text.length(); // Total size in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0.0f; // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f; // Offset X to next character to draw

    float scaleFactor = fontSize / font.baseSize; // Character quad scaling factor

    for (int i = 0; i < size;)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = getCodepointNext(&text[i], &codepointByteCount);
        int index = getGlyphIndex(font, codepoint);

        if (codepoint == '\n')
        {

            // NOTE: Line spacing is a global variable, use setTextLineSpacing() to setup
            textOffsetY += (fontSize + MX_TEXT_LINE_SPACING);
            textOffsetX = 0.0f;
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                drawTextCodepoint(font, codepoint, MxVec2{position.x + textOffsetX, position.y + textOffsetY}, fontSize, tint);
            }

            if (font.glyphs[index].advanceX == 0)
            {
                textOffsetX += ((float)font.recs[index].width * scaleFactor + spacing);
            }
            else
            {
                textOffsetX += ((float)font.glyphs[index].advanceX * scaleFactor + spacing);
            }
        }

        i += codepointByteCount; // Move text bytes counter to next codepoint
    }
}


inline bool isValidFont(const MxFont& font)
{
    const bool validFont = ((font.baseSize > 0) &&           // Validate font size
                            (font.glyphCount > 0) &&         // Validate font contains some glyph
                            (!font.textureNameID.empty()) && // Validate font contains Texture Atlas ID
                            (font.recs != NULL) &&           // Validate font recs defining glyphs on texture atlas
                            (font.glyphs != NULL));          // Validate glyph data is loaded
    return validFont;
}

inline bool isValidImage(const MxImage& image)
{
    const bool validImage = ((image.data != NULL) && // Validate pixel data available
                             (image.width > 0) &&    // Validate image width
                             (image.height > 0));    // Validate image height
    return validImage;
}

MxVec2 measureTextInternal(MxFont font, const std::string& text, float fontSize, float spacing)
{
    MxVec2 textSize{};

    if (!isValidFont(font))
    {
        return textSize; // Security check
    }

    int size = text.length(); // Get size in bytes of text
    int tempByteCounter = 0;  // Used to count longer text line num chars
    int byteCounter = 0;

    float textWidth = 0.0f;
    float tempTextWidth = 0.0f; // Used to count longer text line width

    float textHeight = fontSize;
    float scaleFactor = fontSize / (float)font.baseSize;

    int letter = 0; // Current character
    int index = 0;  // Index position in sprite font

    for (int i = 0; i < size;)
    {
        byteCounter++;

        int codepointByteCount = 0;
        letter = getCodepointNext(&text[i], &codepointByteCount);
        index = getGlyphIndex(font, letter);

        i += codepointByteCount;

        if (letter != '\n')
        {
            if (font.glyphs[index].advanceX > 0)
            {
                textWidth += font.glyphs[index].advanceX;
            }
            else
            {
                textWidth += (font.recs[index].width + font.glyphs[index].offsetX);
            }
        }
        else
        {
            if (tempTextWidth < textWidth)
            {
                tempTextWidth = textWidth;
            }
            byteCounter = 0;
            textWidth = 0;

            textHeight += (fontSize + MX_TEXT_LINE_SPACING);
        }

        if (tempByteCounter < byteCounter)
        {
            tempByteCounter = byteCounter;
        }
    }

    if (tempTextWidth < textWidth)
    {
        tempTextWidth = textWidth;
    }

    textSize.x = tempTextWidth * scaleFactor + (float)((tempByteCounter - 1) * spacing);
    textSize.y = textHeight;

    return textSize;
}


//-----------------------------------------------------------------------------
// (SECTION) LoadFontData
// Note: The code is initially the same as text.c, to validate it and have something functional.
// Todo: adapt as necessary to make it work and optimize it for mxgui.
//-----------------------------------------------------------------------------


MxGlyphInfo* loadFontData(const unsigned char* fileData, int fontSize, const int* codepoints, int codepointCount, int* glyphCount)
{


    MxGlyphInfo* glyphs = NULL;
    int glyphCounter = 0;

    // Load font data (including pixel data) from TTF memory file
    // NOTE: Loaded information should be enough to generate font image atlas, using any packaging method
    if (fileData != NULL)
    {
        bool genFontChars = false;
        stbtt_fontinfo fontInfo{};
        // TODO: Should a shallow copy be created to avoid "dealing" with a const user array?
        int* requiredCodepoints = (int*)codepoints;

        if (stbtt_InitFont(&fontInfo, (unsigned char*)fileData, 0)) // Initialize font for data reading
        {
            // Calculate font scale factor
            float scaleFactor = stbtt_ScaleForPixelHeight(&fontInfo, (float)fontSize);

            // Calculate font basic metrics
            // NOTE: ascent is equivalent to font baseline
            int ascent = 0;
            int descent = 0;
            int lineGap = 0;
            stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);

            // In case no chars count provided, default to 95
            codepointCount = (codepointCount > 0) ? codepointCount : 95;

            // Fill fontChars in case not provided externally
            // NOTE: By default filling glyphCount consecutively, starting at 32 (Space)
            if (requiredCodepoints == NULL)
            {
                requiredCodepoints = (int*)MX_MALLOC(codepointCount * sizeof(int));
                for (int i = 0; i < codepointCount; i++)
                {
                    requiredCodepoints[i] = i + 32;
                }
                genFontChars = true;
            }

            // Check available glyphs on provided font before loading them
            for (int i = 0, index; i < codepointCount; i++)
            {
                index = stbtt_FindGlyphIndex(&fontInfo, requiredCodepoints[i]);
                if (index > 0)
                {
                    glyphCounter++;
                }
            }

            // WARNING: Allocating space for maximum number of codepoints
            glyphs = (MxGlyphInfo*)MX_CALLOC(glyphCounter, sizeof(MxGlyphInfo));
            glyphCounter = 0; // Reset to reuse

            int k = 0;
            for (int i = 0; i < codepointCount; i++)
            {
                int cpWidth = 0, cpHeight = 0;  // Codepoint width and height (on generation)
                int cp = requiredCodepoints[i]; // Codepoint value to get info for

                //  Render a unicode codepoint to a bitmap
                //      stbtt_GetCodepointBitmap()           -- allocates and returns a bitmap
                //      stbtt_GetCodepointBitmapBox()        -- how big the bitmap must be
                //      stbtt_MakeCodepointBitmap()          -- renders into a provided bitmap

                // Check if a glyph is available in the font
                // WARNING: if (index == 0), glyph not found, it could fallback to default .notdef glyph (if defined in font)
                int index = stbtt_FindGlyphIndex(&fontInfo, cp);

                if (index > 0)
                {
                    // NOTE: Only storing glyphs for codepoints found in the font
                    glyphs[k].value = cp;
                    glyphs[k].image.data = stbtt_GetCodepointBitmap(&fontInfo, scaleFactor, scaleFactor, cp, &cpWidth, &cpHeight, &glyphs[k].offsetX, &glyphs[k].offsetY);


                    if (glyphs[k].image.data != NULL) // Glyph data has been found in the font
                    {
                        stbtt_GetCodepointHMetrics(&fontInfo, cp, &glyphs[k].advanceX, NULL);
                        glyphs[k].advanceX = (int)((float)glyphs[k].advanceX * scaleFactor);


                        // Load glyph image
                        glyphs[k].image.width = cpWidth;
                        glyphs[k].image.height = cpHeight;
                        glyphs[k].image.mipmaps = 1;
                        glyphs[k].image.format = 1;

                        glyphs[k].offsetY += (int)((float)ascent * scaleFactor);
                    }
                    // else TRACELOG(LOG_WARNING, "FONT: Glyph [0x%08x] has no image data available", cp); // Only reported for 0x20 and 0x3000

                    // Create an empty image for Space character (0x20), useful for sprite font generation
                    // NOTE: Another space to consider: 0x3000 (CJK - Ideographic Space)
                    if ((cp == 0x20) || (cp == 0x3000))
                    {
                        stbtt_GetCodepointHMetrics(&fontInfo, cp, &glyphs[k].advanceX, NULL);
                        glyphs[k].advanceX = (int)((float)glyphs[k].advanceX * scaleFactor);

                        MxImage imSpace = {.data = NULL, .width = glyphs[k].advanceX, .height = fontSize, .mipmaps = 1, .format = 1};

                        // Only allocate space image if required
                        if (glyphs[k].advanceX > 0)
                        {
                            imSpace.data = MX_CALLOC(glyphs[k].advanceX * fontSize, 1);
                        }
                        else
                        {
                            glyphs[k].advanceX = 0;
                        }

                        glyphs[k].image = imSpace;
                    }

                    k++;
                    glyphCounter++;
                }
                else
                {
                    // WARNING: Glyph not found on font, optionally use a fallback glyph
                }
            }

            if (glyphCounter < codepointCount)
            {
                // TRACELOG(LOG_WARNING, "FONT: Requested codepoints glyphs found: [%i/%i]", k, codepointCount);
            }
        }
        else
        {
            // TRACELOG(LOG_WARNING, "FONT: Failed to process TTF font data");
        }

        if (genFontChars)
        {
            MX_FREE(requiredCodepoints);
        }
    }


    *glyphCount = glyphCounter;
    return glyphs;
}


MxImage genImageFontAtlas(const MxGlyphInfo* glyphs, MxRect** glyphRecs, int glyphCount, int fontSize, int padding)
{
    MxImage atlas{};

    if (glyphs == NULL)
    {
        // TRACELOG(LOG_WARNING, "FONT: Provided glyphs info not valid, returning empty image atlas");
        return atlas;
    }

    *glyphRecs = NULL;

    // In case no chars count provided, suppose default of 95
    glyphCount = (glyphCount > 0) ? glyphCount : 95;

    // NOTE: MxRects memory is loaded here!
    MxRect* recs = (MxRect*)MX_MALLOC(glyphCount * sizeof(MxRect));

    // Calculate image size based on total glyph width and glyph row count
    int totalWidth = 0;
    int maxGlyphWidth = 0;

    for (int i = 0; i < glyphCount; i++)
    {
        if (glyphs[i].image.width > maxGlyphWidth)
        {
            maxGlyphWidth = glyphs[i].image.width;
        }
        totalWidth += glyphs[i].image.width + 2 * padding;
    }

    int paddedFontSize = fontSize + 2 * padding;

    // Estimate image atlas size from available data
    // NOTE: Multiplying total expected area by 1.2f scale factor but in case
    // some glyphs do not fit, the atlas height is scaled x2 to fit them
    float totalArea = totalWidth * paddedFontSize * 1.2f;
    float imageMinSize = sqrtf(totalArea);
    int imageSize = (int)powf(2, ceilf(logf(imageMinSize) / logf(2)));

    if (totalArea < ((imageSize * imageSize) / 2))
    {
        atlas.width = imageSize;      // Atlas bitmap width
        atlas.height = imageSize / 2; // Atlas bitmap height
    }
    else
    {
        atlas.width = imageSize;  // Atlas bitmap width
        atlas.height = imageSize; // Atlas bitmap height
    }

    int atlasDataSize = atlas.width * atlas.height;           // Save total size for bounds checking
    atlas.data = (unsigned char*)MX_CALLOC(atlasDataSize, 1); // Create a bitmap to store characters (8 bpp)
    atlas.format = 1;
    atlas.mipmaps = 1;

    // DEBUG: View padding in the generated image setting a gray background...
    // for (int i = 0; i < atlas.width*atlas.height; i++) ((unsigned char *)atlas.data)[i] = 100;

    // Use basic packing algorithm

    int offsetX = padding;
    int offsetY = padding;

    // NOTE: Using simple packaging, one char after another
    for (int i = 0; i < glyphCount; i++)
    {
        // Check remaining space for glyph
        if (offsetX >= (atlas.width - glyphs[i].image.width - 2 * padding))
        {
            offsetX = padding;

            // NOTE: Be careful on offsetY for SDF fonts, by default SDF
            // use an internal padding of 4 pixels, it means char rectangle
            // height is bigger than fontSize, it could be up to (fontSize + 8)
            offsetY += (fontSize + 2 * padding);

            if (offsetY > (atlas.height - fontSize - padding))
            {
                // TRACELOG(LOG_WARNING, "FONT: Updating atlas size to fit all characters");

                // Update atlas size to fit all characters
                int updatedAtlasHeight = atlas.height * 2;
                int updatedAtlasDataSize = atlas.width * updatedAtlasHeight;
                unsigned char* updatedAtlasData = (unsigned char*)MX_CALLOC(updatedAtlasDataSize, 1);

                memcpy(updatedAtlasData, atlas.data, atlasDataSize);
                MX_FREE(atlas.data);
                atlas.data = updatedAtlasData;
                atlas.height = updatedAtlasHeight;
                atlasDataSize = updatedAtlasDataSize;
            }
        }

        // Copy pixel data from glyph image to atlas
        for (int y = 0; y < glyphs[i].image.height; y++)
        {
            for (int x = 0; x < glyphs[i].image.width; x++)
            {
                int destX = offsetX + x;
                int destY = offsetY + y;

                // Security: check both lower and upper bounds
                if ((destX >= 0) && (destX < atlas.width) && (destY >= 0) && (destY < atlas.height))
                {
                    ((unsigned char*)atlas.data)[destY * atlas.width + destX] = ((unsigned char*)glyphs[i].image.data)[y * glyphs[i].image.width + x];
                }
            }
        }

        // Fill chars rectangles in atlas info
        recs[i].x = (float)offsetX;
        recs[i].y = (float)offsetY;
        recs[i].width = (float)glyphs[i].image.width;
        recs[i].height = (float)glyphs[i].image.height;

        // Move atlas position X for next character drawing
        offsetX += (glyphs[i].image.width + 2 * padding);
    }

    // Convert image data from GRAYSCALE to GRAY_ALPHA
    unsigned char* dataGrayAlpha = (unsigned char*)MX_MALLOC(atlas.width * atlas.height * sizeof(unsigned char) * 2); // Two channels

    for (int i = 0, k = 0; i < atlas.width * atlas.height; i++, k += 2)
    {
        dataGrayAlpha[k] = 255;
        dataGrayAlpha[k + 1] = ((unsigned char*)atlas.data)[i];
    }

    MX_FREE(atlas.data);
    atlas.data = dataGrayAlpha;
    atlas.format = 2; // PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA;

    *glyphRecs = recs;

    return atlas;
}


MxFont loadFontFromMemoryInternal(const std::string& textureNameID, const unsigned char* fileData, int fontSize, const int* codepoints, int codepointCount)
{
    MxFont font{};

    font.baseSize = fontSize;
    font.glyphs = loadFontData(fileData, font.baseSize, codepoints, (codepointCount > 0) ? codepointCount : 95, &font.glyphCount);


    if (font.glyphs != NULL)
    {
        font.glyphPadding = 4; // TTF font generation default glyphs padding

        MxImage atlas = genImageFontAtlas(font.glyphs, &font.recs, font.glyphCount, font.baseSize, font.glyphPadding);

        if ((atlas.width != 0) && (atlas.height != 0))
        {
            // Note: The texture atlas is stored in the TextureManager; we only keep the reference ID.
            loadTextureFromMemory(atlas.data, atlas.width, atlas.height, atlas.format, atlas.mipmaps, textureNameID);
            font.textureNameID = textureNameID;
        }

        // It is not necessary to retain the glyph image data.
        for (int i = 0; i < font.glyphCount; i++)
        {
            MX_FREE(font.glyphs[i].image.data);
            font.glyphs[i].image = MxImage{};
        }

        MX_FREE(atlas.data);

        // TRACELOG(LOG_INFO, "FONT: Data loaded successfully (%i pixel size | %i glyphs)", font.baseSize, font.glyphCount);
    }
    else
    {
        font = MxFont{};
    }

    return font;
}


//-----------------------------------------------------------------------------
// (SECTION) Decompression code
//  an algorithm with the sole objective of compressing the matrices to save font size
//  Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/deprecated/stb.h#L10437
//-----------------------------------------------------------------------------


#define stb__in2(x) ((i[x] << 8) + i[(x) + 1])
#define stb__in3(x) ((i[x] << 16) + stb__in2((x) + 1))
#define stb__in4(x) ((i[x] << 24) + stb__in3((x) + 1))

static unsigned char *stb__barrier_out_e, *stb__barrier_out_b;
static const unsigned char* stb__barrier_in_b;
static unsigned char* stb__dout;

static unsigned int stb_decompress_length(const unsigned char* input)
{
    return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static void stb__match(const unsigned char* data, unsigned int length)
{
    // INVERSE of memmove... write each byte before copying the next...
    MX_ASSERT(stb__dout + length <= stb__barrier_out_e, "");
    if (stb__dout + length > stb__barrier_out_e)
    {
        stb__dout += length;
        return;
    }
    if (data < stb__barrier_out_b)
    {
        stb__dout = stb__barrier_out_e + 1;
        return;
    }
    while (length--)
    {
        *stb__dout++ = *data++;
    }
}

static void stb__lit(const unsigned char* data, unsigned int length)
{
    MX_ASSERT(stb__dout + length <= stb__barrier_out_e, "");
    if (stb__dout + length > stb__barrier_out_e)
    {
        stb__dout += length;
        return;
    }
    if (data < stb__barrier_in_b)
    {
        stb__dout = stb__barrier_out_e + 1;
        return;
    }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

static const unsigned char* stb_decompress_token(const unsigned char* i)
{
    if (*i >= 0x20)
    { // use fewer if's for cases that expand small
        if (*i >= 0x80)
        {
            stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
        }
        else if (*i >= 0x40)
        {
            stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
        }
        else /* *i >= 0x20 */
        {
            stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
        }
    }
    else
    { // more ifs for cases that expand large, since overhead is amortized
        if (*i >= 0x18)
        {
            stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
        }
        else if (*i >= 0x10)
        {
            stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
        }
        else if (*i >= 0x08)
        {
            stb__lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
        }
        else if (*i == 0x07)
        {
            stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
        }
        else if (*i == 0x06)
        {
            stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
        }
        else if (*i == 0x04)
        {
            stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
        }
    }
    return i;
}

static unsigned int stb_adler32(unsigned int adler32, unsigned char* buffer, unsigned int buflen)
{
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen = buflen % 5552;

    unsigned long i;
    while (buflen)
    {
        for (i = 0; i + 7 < blocklen; i += 8)
        {
            s1 += buffer[0], s2 += s1;
            s1 += buffer[1], s2 += s1;
            s1 += buffer[2], s2 += s1;
            s1 += buffer[3], s2 += s1;
            s1 += buffer[4], s2 += s1;
            s1 += buffer[5], s2 += s1;
            s1 += buffer[6], s2 += s1;
            s1 += buffer[7], s2 += s1;

            buffer += 8;
        }

        for (; i < blocklen; ++i)
        {
            s1 += *buffer++, s2 += s1;
        }

        s1 %= ADLER_MOD, s2 %= ADLER_MOD;
        buflen -= blocklen;
        blocklen = 5552;
    }
    return (unsigned int)(s2 << 16) + (unsigned int)s1;
}


static unsigned int stb_decompress(unsigned char* output, const unsigned char* i, unsigned int /*length*/)
{
    if (stb__in4(0) != 0x57bC0000)
    {
        return 0;
    }
    if (stb__in4(4) != 0)
    {
        return 0; // error! stream is > 4GB
    }
    const unsigned int olen = stb_decompress_length(i);
    stb__barrier_in_b = i;
    stb__barrier_out_e = output + olen;
    stb__barrier_out_b = output;
    i += 16;

    stb__dout = output;
    while (true)
    {
        const unsigned char* old_i = i;
        i = stb_decompress_token(i);
        if (i == old_i)
        {
            if (*i == 0x05 && i[1] == 0xfa)
            {
                MX_ASSERT(stb__dout == output + olen, "");
                if (stb__dout != output + olen)
                {
                    return 0;
                }
                if (stb_adler32(1, output, olen) != (unsigned int)stb__in4(2))
                {
                    return 0;
                }
                return olen;
            }
            else
            {
                MX_ASSERT(0, "NOTREACHED"); /* NOTREACHED */
                return 0;
            }
        }
        MX_ASSERT(stb__dout <= output + olen, "");
        if (stb__dout > output + olen)
        {
            return 0;
        }
    }
}

template <typename T>
inline constexpr T mxMax(T min, T max)
{
    return (max < min) ? min : max;
}

template <typename T>
inline constexpr T mxMin(T min, T max)
{
    return (min < max) ? min : max;
}

template <typename T, typename U, typename V>
inline constexpr T mxClamp(T value, U min, V max)
{
    MX_ASSERT(std::is_signed_v<T> == std::is_signed_v<U> && std::is_signed_v<T> == std::is_signed_v<V>, "Clamp arguments must all be of the same signedness to avoid errors.");

    return (value < min) ? min : (value > max) ? max : value;
}

inline bool checkCollisionPointRect(MxVec2 point, MxRect rec)
{
    const bool collision = ((point.x >= rec.x) && (point.x < (rec.x + rec.width)) && (point.y >= rec.y) && (point.y < (rec.y + rec.height)));
    return collision;
}

inline bool checkCollisionPointRectX(MxVec2 point, MxRect rec)
{
    const bool collision = ((point.x >= rec.x) && (point.x < (rec.x + rec.width)));
    return collision;
}

inline bool checkCollisionPointRectY(MxVec2 point, MxRect rec)
{
    const bool collision = ((point.y >= rec.y) && (point.y < (rec.y + rec.height)));
    return collision;
}

inline MxRect getCollisionRec(MxRect rect1, MxRect rect2)
{
    MxRect overlap{};

    float left = (rect1.x > rect2.x) ? rect1.x : rect2.x;
    float right1 = rect1.x + rect1.width;
    float right2 = rect2.x + rect2.width;
    float right = (right1 < right2) ? right1 : right2;
    float top = (rect1.y > rect2.y) ? rect1.y : rect2.y;
    float bottom1 = rect1.y + rect1.height;
    float bottom2 = rect2.y + rect2.height;
    float bottom = (bottom1 < bottom2) ? bottom1 : bottom2;

    if ((left < right) && (top < bottom))
    {
        overlap.x = left;
        overlap.y = top;
        overlap.width = right - left;
        overlap.height = bottom - top;
    }

    return overlap;
}

inline MxColor fadeColor(MxColor color, float alpha)
{
    MxColor result = color;

    if (alpha < 0.0f)
    {
        alpha = 0.0f;
    }
    else if (alpha > 1.0f)
    {
        alpha = 1.0f;
    }

    result.a = (MxUChar8)(255.0f * alpha);

    return result;
}

static std::vector<MxRect> s_stackScissors{};

MxRect intersectionArea(const MxRect& rect2)
{
    if (!s_stackScissors.empty())
    {
        return getCollisionRec(s_stackScissors.back(), rect2);
    }

    return rect2;
}

void pushScissor(int x, int y, int width, int height)
{
    if (!s_stackScissors.empty())
    {
        endScissorMode();
    }

    MxRect rect{(float)x, (float)y, (float)width, (float)height};
    rect = intersectionArea(rect);

    s_stackScissors.push_back(rect);
    beginScissorMode((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height);
}

void popScissor()
{
    endScissorMode();

    if (!s_stackScissors.empty())
    {
        s_stackScissors.pop_back();
    }

    if (!s_stackScissors.empty())
    {
        MxRect rect = s_stackScissors.back();
        beginScissorMode((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height);
    }
}

MxTransform updateTransformWorld(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor)
{
    MxTransform transform{};
    transform.bounds = bounds;
    transform.anchor = anchor;
    transform.worldBounds = {
        .x = transform.bounds.x + transform.anchor.x + ctx->m_anchor.x,
        .y = transform.bounds.y + transform.anchor.y + ctx->m_anchor.y - ctx->m_scrollTop,
        .width = transform.bounds.width,
        .height = transform.bounds.height,
    };

    return transform;
}

//-----------------------------------------------------------------------------
// APIS functions
//-----------------------------------------------------------------------------

namespace mxgui
{

    static std::unique_ptr<MxGuiContext> s_context{nullptr};

    MxGuiContext* createContext(MxStyle style)
    {
        s_context = std::make_unique<MxGuiContext>();
        s_context->init(style);
        return s_context.get();
    }

    void destroyContext(MxGuiContext* ctx)
    {
        ctx->close();
        s_context.reset();
    }

    MxGuiContext* getCurrentContext()
    {
        return s_context.get();
    }

    MxStyle getStyle(MxGuiContext* ctx)
    {
        return ctx->m_style;
    }

    MxTransform getCurrentTransform(MxGuiContext* ctx)
    {
        return ctx->m_currentTransform;
    }

    MxMouseEvents getCurrentMouseEvents(MxGuiContext* ctx)
    {
        return ctx->m_currentMouseEvents;
    }

    void createImage(const std::filesystem::path& path, const std::string& imageName, bool smooth)
    {
        loadTexture(path, imageName);
        setSmoothTexture(imageName, smooth);
    }

    MxVec2 guiPanel(MxGuiContext* ctx, MxTag tag, MxRect bounds, MxVec2 anchor, bool enableDrag)
    {

        PanelComponent& canvas = *ctx->getPanelComponent(tag);

        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxRect rect = transform.worldBounds;

        MxMouseEvents mouseEvents{};

        if (enableDrag)
        {
            mouseEvents.isMouseHover = (checkCollisionPointRect(getMousePosition(), rect));
            mouseEvents.isMouseRelease = mouseEvents.isMouseHover && isMouseButtonReleased(MX_MOUSE_BUTTON_LEFT);
            mouseEvents.isMouseDown = mouseEvents.isMouseHover && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT);
            mouseEvents.isMousePressed = mouseEvents.isMouseHover && isMouseButtonPressed(MX_MOUSE_BUTTON_LEFT);

            if (mouseEvents.isMousePressed)
            {
                const MxVec2 mousePosition = getMousePosition();
                canvas.Offset.x = transform.anchor.x + mousePosition.x - rect.x;
                canvas.Offset.y = transform.anchor.y + mousePosition.y - rect.y;
                canvas.isDrag = true;
            }
            if (canvas.isDrag && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT) && isCursorOnScreen())
            {
                const MxVec2 mousePosition = getMousePosition();
                transform.bounds.x = mousePosition.x - canvas.Offset.x - ctx->m_anchor.x;
                transform.bounds.y = mousePosition.y - canvas.Offset.y - ctx->m_anchor.y + ctx->m_scrollTop;
            }
            else /*if (mouseEvents.isMouseRelease)*/
            {
                canvas.isDrag = false;
            }
        }

        MxColor color = ctx->m_style.backgroundColor;

        transform = updateTransformWorld(ctx, transform.bounds, anchor);

        drawRectanglePro(transform.worldBounds, MxVec2{}, 0, fadeColor(color, 1.0f));
        drawRectangleLinesEx(transform.worldBounds, ctx->m_style.borderWidth, fadeColor(ctx->m_style.borderColor, 0.5f));
        ctx->updateCurrents(transform, mouseEvents);
        return MxVec2{transform.bounds.x, transform.bounds.y};
    }

    void guiImage(MxGuiContext* ctx, const std::string& imageName, MxRect bounds, MxVec2 anchor, MxColor color)
    {
        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxRect rect = transform.worldBounds;
        const MxVec2 texture = getSizeTexture(imageName);
        const MxVec2 scale = (rect.width > 0 && rect.height > 0) ? MxVec2{rect.width, rect.height} : texture;


        const MxRect source = {0.0f, 0.0f, texture.x, texture.y};
        const MxRect dest = {rect.x, rect.y, scale.x, scale.y};

        drawTexturePro(imageName, source, dest, MxVec2{0, 0}, 0, color);
        ctx->updateCurrents(transform, MxMouseEvents{});
    }

    void guiLabel(MxGuiContext* ctx, const std::string& text, MxVec2 bounds, MxVec2 anchor)
    {
        MxTransform transform = updateTransformWorld(ctx, toMxRect(bounds), anchor);
        MxRect rect = transform.worldBounds;
        const MxStyle style = ctx->m_style;

        drawText(style.fontName, text, MxVec2{rect.x, rect.y}, style.textSize, style.textSpacing, style.textColor);
        ctx->updateCurrents(transform, MxMouseEvents{});
    }


    bool guiButton(MxGuiContext* ctx, const std::string& text, MxRect bounds, MxVec2 anchor, int buttonStyle, bool enable)
    {
        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxRect rect = transform.worldBounds;

        MxMouseEvents mouseEvents{};

        int paint = 0;

        if (enable)
        {
            mouseEvents.isMouseHover = (checkCollisionPointRect(getMousePosition(), rect));
            mouseEvents.isMouseRelease = mouseEvents.isMouseHover && isMouseButtonReleased(MX_MOUSE_BUTTON_LEFT);
            mouseEvents.isMouseDown = mouseEvents.isMouseHover && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT);
            mouseEvents.isMousePressed = mouseEvents.isMouseHover && isMouseButtonPressed(MX_MOUSE_BUTTON_LEFT);

            if (mouseEvents.isMouseHover)
            {
                paint = 50;
            }
            if (mouseEvents.isMouseDown)
            {
                paint = 80;
            }
        }

        MxColor color = ctx->m_style.primaryColor;
        color.r = mxClamp(color.r - paint, 0, 255);
        color.g = mxClamp(color.g - paint, 0, 255);
        color.b = mxClamp(color.b - paint, 0, 255);

        MxColor borderColor = ctx->m_style.borderColor;
        borderColor.r = mxClamp(color.r - paint, 0, 255);
        borderColor.g = mxClamp(color.g - paint, 0, 255);
        borderColor.b = mxClamp(color.b - paint, 0, 255);

        if (buttonStyle == MX_CONTAINED)
        {
            drawRectanglePro(rect, MxVec2{}, 0, fadeColor(MxColor::Gray, 0.5f));
            drawRectanglePro(rect, MxVec2{}, 0, color);
        }
        else if (buttonStyle == MX_OUTLINE)
        {
            drawRectangleLinesEx(rect, ctx->m_style.borderWidth, borderColor);
            drawRectanglePro(rect, MxVec2{}, 0, fadeColor(color, 0.3f));
        }

        const MxStyle style = ctx->m_style;

        MxVec2 textSize = measureText(style.fontName, text, style.textSize, style.textSpacing);
        MxVec2 textPosition = MxVec2{rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2};
        textPosition.x -= ctx->m_anchor.x;
        textPosition.y -= ctx->m_anchor.y;

        guiLabel(ctx, text, MxVec2{0}, textPosition);

        ctx->updateCurrents(transform, mouseEvents);

        return mouseEvents.isMousePressed;
    }

    void guiScrollPanelBegin(MxGuiContext* ctx, MxTag tag, MxRect bounds, MxRect scrollBounds, MxVec2 anchor, bool enable)
    {
        ScrollPanelComponent& scrollPanel = *ctx->getScrollPanelComponent(tag);
        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxRect rect = transform.worldBounds;
        MxRect rectCanvas = rect;

        ctx->m_anchor = MxVec2{rect.x, rect.y};


        scrollPanel.transformCanvas.bounds = scrollBounds;

        if (scrollPanel.transformCanvas.bounds.height > rect.height && enable)
        {
            float scrollTop = scrollPanel.scrollTop;

            if (checkCollisionPointRect(getMousePosition(), scrollPanel.scrollBarThumb) && isMouseButtonPressed(MX_MOUSE_BUTTON_LEFT))
            {
                scrollPanel.isDrag = true;
                scrollPanel.pointDrag = getMousePosition().y - scrollPanel.scrollBarThumb.y;
            }
            else if (scrollPanel.isDrag && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT))
            {
                constexpr float offsetTrack = 3.0f;
                const float currentPointDrag = getMousePosition().y - scrollPanel.scrollBarThumb.y;
                const float mouseDelta = currentPointDrag - scrollPanel.pointDrag;
                if (currentPointDrag > scrollPanel.pointDrag + offsetTrack)
                {
                    scrollTop += 1.0f + mouseDelta;
                }
                else if (currentPointDrag < scrollPanel.pointDrag - offsetTrack)
                {
                    scrollTop -= 1.0f - mouseDelta;
                }
            }
            else if (scrollPanel.isDrag && isMouseButtonReleased(MX_MOUSE_BUTTON_LEFT))
            {
                scrollPanel.isDrag = false;
            }

            if (getMouseWheelMove() != 0 && checkCollisionPointRect(getMousePosition(), rect))
            {
                scrollPanel.isDrag = false;
                scrollTop -= getMouseWheelMove() * 40;
            }


            scrollPanel.transformCanvas.bounds.x = 0;
            scrollPanel.transformCanvas.bounds.y = 0;
            scrollPanel.transformCanvas.anchor = MxVec2{};
            scrollPanel.transformCanvas = updateTransformWorld(ctx, scrollPanel.transformCanvas.bounds, MxVec2{0, 0});
            rectCanvas = scrollPanel.transformCanvas.worldBounds;


            float visibleProportion = rect.height / rectCanvas.height;
            float progress = scrollTop / (rectCanvas.height - rect.height);


            scrollPanel.scrollTop = scrollTop;
            if (rectCanvas.y + rectCanvas.height - scrollTop <= rect.y + rect.height)
            {
                scrollPanel.scrollTop = (rectCanvas.y + rectCanvas.height) - (rect.y + rect.height);
            }
            else if (scrollTop < 0)
            {
                scrollPanel.scrollTop = 0;
            }


            // std::cout << (rectCanvas.y + rectCanvas.height) - (rect.y + rect.height) << '\n';
            // std::cout << scrollTop << " | " << rectCanvas.y + rectCanvas.height - scrollPanel.scrollTop << " | " << rect.y + rect.height << '\n';

            progress = mxClamp(progress, 0.0f, 1.0f);
            ctx->m_scrollTop = scrollPanel.scrollTop;

            scrollPanel.scrollBarThumb = MxRect{
                .x = rect.x + rect.width - 6 - MX_DRAG_OFFSET,
                .y = rect.y + (rect.height - rect.height * visibleProportion) * progress,
                .width = 6 + MX_DRAG_OFFSET * 2,
                .height = rect.height * visibleProportion,
            };
        }

        drawRectangleLinesEx(rect, ctx->m_style.borderWidth, ctx->m_style.borderColor);
        pushScissor(rect.x, rect.y, rect.width, rect.height); // call internal BeginScissorMode();

        drawRectanglePro(rectCanvas, MxVec2{}, 0, fadeColor(MxColor::LightGray, 0.5f)); // debug visual feedback

        ctx->updateCurrents(transform, MxMouseEvents{});
    }

    void guiScrollPanelEnd(MxGuiContext* ctx, MxTag tag)
    {

        popScissor(); // call internal EndScissorMode();

        ScrollPanelComponent& scrollPanel = *ctx->getScrollPanelComponent(tag);
        ctx->m_anchor = MxVec2{};
        ctx->m_scrollTop = 0.0f;
        MxRect rect = scrollPanel.scrollBarThumb;
        rect.x += MX_DRAG_OFFSET;
        rect.width -= MX_DRAG_OFFSET * 2;

        // drawRectanglePro(scrollPanel.scrollBarThumb, MxVec2{}, 0, MxColor::Blue); // debug offset
        drawRectanglePro(rect, MxVec2{}, 0, ctx->m_style.primaryColor);
    }

    float guiSlider(MxGuiContext* ctx, MxTag tag, MxRect bounds, MxVec2 anchor, bool enable)
    {
        SliderComponent& slider = *ctx->getSliderComponent(tag);

        const float radius = 8.0f;
        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxTransform transformBar = transform;
        MxTransform transformBarCollision = transform;
        transformBarCollision.worldBounds.x -= (MX_DRAG_OFFSET + radius) / 2;
        transformBarCollision.worldBounds.y -= (MX_DRAG_OFFSET + radius) / 2;
        transformBarCollision.worldBounds.width += (MX_DRAG_OFFSET + radius);
        transformBarCollision.worldBounds.height += (MX_DRAG_OFFSET + radius);

        MxMouseEvents mouseEvents{};

        if (enable)
        {
            mouseEvents.isMouseHover = (checkCollisionPointRect(getMousePosition(), transformBarCollision.worldBounds));
            mouseEvents.isMouseRelease = mouseEvents.isMouseHover && isMouseButtonReleased(MX_MOUSE_BUTTON_LEFT);
            mouseEvents.isMouseDown = mouseEvents.isMouseHover && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT);
            mouseEvents.isMousePressed = mouseEvents.isMouseHover && isMouseButtonPressed(MX_MOUSE_BUTTON_LEFT);

            if (mouseEvents.isMousePressed)
            {
                slider.isDrag = true;
            }
            if (slider.isDrag && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT))
            {
                const MxVec2 mousePosition = getMousePosition();
                slider.progress = (mousePosition.x - transform.worldBounds.x) / transform.worldBounds.width;
                slider.progress = mxClamp(slider.progress, 0.0f, 1.0f);
            }
            else
            {
                slider.isDrag = false;
            }
        }

        MxColor color = ctx->m_style.primaryColor;
        transformBar.worldBounds.width = slider.progress * transformBar.worldBounds.width;
        ctx->updateCurrents(transform, mouseEvents);

        // drawRectanglePro(transformBarCollision.worldBounds, MxVec2{}, 0, fadeColor(MxColor::Blue, 1.0f)); // debug offset
        drawRectanglePro(transform.worldBounds, MxVec2{}, 0, fadeColor(color, 1.0f));
        drawRectanglePro(transformBar.worldBounds, MxVec2{}, 0, fadeColor(MxColor::Red, 1.0f));

        MxVec2 point{.x = transformBar.worldBounds.x + transformBar.worldBounds.width, .y = transformBar.worldBounds.y + transformBar.worldBounds.height / 2};
        drawCircle(point, radius, fadeColor(MxColor::Red, 1.0f));

        return slider.progress;
    }

    void guiSliderProgress(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, float progress)
    {
        bounds.height = 12;

        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxTransform transformBar = transform;

        MxColor color = ctx->m_style.primaryColor;


        transformBar.worldBounds.width = progress * transformBar.worldBounds.width;

        ctx->updateCurrents(transform, MxMouseEvents{});

        drawRectanglePro(transform.worldBounds, MxVec2{}, 0, fadeColor(color, 1.0f));
        drawRectanglePro(transformBar.worldBounds, MxVec2{}, 0, fadeColor(MxColor::Red, 1.0f));
    }

    void guiIcon(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, int codepoint, int size)
    {
        const int iconSize = (size < 0) ? ctx->m_style.iconSize : size;
        bounds.width = iconSize;
        bounds.height = iconSize;
        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxColor color = ctx->m_style.primaryColor;

        drawIconEx(codepoint, toMxVec2(transform.worldBounds), color, iconSize);
    }

    bool guiIconButton(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, int codepoint, int size, bool enable)
    {
        const int iconSize = (size < 0) ? ctx->m_style.iconSize : size;
        bounds.width = iconSize;
        bounds.height = iconSize;
        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxRect rect = transform.worldBounds;

        MxMouseEvents mouseEvents{};

        int paint = 0;

        if (enable)
        {
            mouseEvents.isMouseHover = (checkCollisionPointRect(getMousePosition(), rect));
            mouseEvents.isMouseRelease = mouseEvents.isMouseHover && isMouseButtonReleased(MX_MOUSE_BUTTON_LEFT);
            mouseEvents.isMouseDown = mouseEvents.isMouseHover && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT);
            mouseEvents.isMousePressed = mouseEvents.isMouseHover && isMouseButtonPressed(MX_MOUSE_BUTTON_LEFT);

            if (mouseEvents.isMouseHover)
            {
                paint = 50;
            }
            if (mouseEvents.isMouseDown)
            {
                paint = 80;
            }
        }

        MxColor color = ctx->m_style.primaryColor;
        color.r = mxClamp(color.r - paint, 0, 255);
        color.g = mxClamp(color.g - paint, 0, 255);
        color.b = mxClamp(color.b - paint, 0, 255);


        drawIconEx(codepoint, toMxVec2(transform.worldBounds), color, iconSize);

        ctx->updateCurrents(transform, mouseEvents);

        return mouseEvents.isMousePressed;
    }


    bool guiToogleEx(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, bool& checked, int codeEnable, int codeDisable)
    {
        bounds.width = ctx->m_style.iconSize;
        bounds.height = ctx->m_style.iconSize;
        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxRect rect = transform.worldBounds;

        MxMouseEvents mouseEvents{};

        mouseEvents.isMouseHover = (checkCollisionPointRect(getMousePosition(), rect));
        mouseEvents.isMouseRelease = mouseEvents.isMouseHover && isMouseButtonReleased(MX_MOUSE_BUTTON_LEFT);
        mouseEvents.isMouseDown = mouseEvents.isMouseHover && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT);
        mouseEvents.isMousePressed = mouseEvents.isMouseHover && isMouseButtonPressed(MX_MOUSE_BUTTON_LEFT);


        if (mouseEvents.isMouseRelease)
        {
            checked = !checked;
        }

        int codepoint = checked ? codeEnable : codeDisable;

        guiIcon(ctx, bounds, anchor, codepoint);

        ctx->updateCurrents(transform, mouseEvents);
        return mouseEvents.isMouseRelease;
    }

    bool guiCheckBox(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, bool& checked)
    {
#if MX_FONT_AWESOME
        return guiToogleEx(ctx, bounds, anchor, checked, ICON_FA_SQUARE_CHECK, ICON_FA_SQUARE);
#else
        return false;
#endif // MX_FONT_AWESOME
    }

    bool guiToogle(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, bool& checked)
    {
#if MX_FONT_AWESOME
        return guiToogleEx(ctx, bounds, anchor, checked, ICON_FA_TOGGLE_ON, ICON_FA_TOGGLE_OFF);
#else
        return false;
#endif // MX_FONT_AWESOME
    }

} // namespace mxgui

/////////////////////////////////////////////////////
// Backend Implementations
/////////////////////////////////////////////////////

#ifdef MX_RAYLIB_BACKEND_IMPLEMENTATION

//-----------------------------------------------------------------------------
// (SECTION) raylib backend
//-----------------------------------------------------------------------------


#include "mxgui_raylib.hpp"


#elifdef MX_SFML_BACKEND_IMPLEMENTATION

//-----------------------------------------------------------------------------
// (SECTION) SFML backend
//-----------------------------------------------------------------------------

#include "mxgui_sfml.hpp"

#else

//-----------------------------------------------------------------------------
// (SECTION) CUSTOM_BACKEND
//-----------------------------------------------------------------------------

// Note:  Here you can implement a custom renderer.
#ifdef MX_CUSTOM_BACKEND_HEADER

#include MX_CUSTOM_BACKEND_HEADER

#endif // MX_CUSTOM_BACKEND_HEADER

#endif // MX_XXX_BACKEND_IMPLEMENTATION


//-----------------------------------------------------------------------------
// (SECTION) Managers
//-----------------------------------------------------------------------------


struct MxFontManager
{

    void init(MxStyle style)
    {
        setupDefaultFont(style.textSize);
        setupFontAwesome(style.iconSize);
    }

    void loadFromMemory(const std::string& textureNameID, const unsigned char* fileData, int fontSize, const int* codepoints, int codepointCount, bool smooth)
    {
        MxFont font = loadFontFromMemoryInternal(textureNameID, fileData, fontSize, codepoints, codepointCount);
        setSmoothTexture(font.textureNameID, smooth);
        m_fonts.insert_or_assign(textureNameID, font);
    }

    void setupDefaultFont(int textSize)
    {
        int codepoints[95];
        for (int i = 0; i < 95; i++)
        {
            codepoints[i] = 32 + i; // ASCII: from (32) to ~ (126)
        }

        loadFromMemory(MX_FONT_NOTO_ID, notosans::data, textSize, codepoints, 95, false);
    }

    void setupFontAwesome(int iconSize)
    {
#if MX_FONT_AWESOME
        int arrayOriginalSize = 414704;
        unsigned char* fontAwesomeData = (unsigned char*)MX_MALLOC(arrayOriginalSize);
        stb_decompress(fontAwesomeData, fa_compressed_data, fa_compressed_size);
        int count = sizeof(codepointsFontAwesome) / sizeof(codepointsFontAwesome[0]);

        loadFromMemory(MX_FONT_AWESOME_ID, fontAwesomeData, iconSize, codepointsFontAwesome, count, true);
        MX_FREE(fontAwesomeData);
#endif
    }

    void unload()
    {
        for (auto& [id, font] : m_fonts)
        {
            unloadFont(font);
        }
        m_fonts.clear();
    }

    void unloadFont(MxFont font)
    {
        if (font.glyphs != NULL)
        {
            for (int i = 0; i < font.glyphCount; i++)
            {
                if (isValidImage(font.glyphs[i].image))
                {
                    MX_FREE(font.glyphs[i].image.data);
                }
            }
            MX_FREE(font.glyphs);
        }

        unloadTexture(font.textureNameID);
        MX_FREE(font.recs);

        // TraceLog(LOG_INFO, "MXFONT: Unloaded font data from RAM and VRAM");
    }

    MxVec2 measureText(const std::string& fontNameID, const std::string& text, int fontSize, int spacing)
    {
        const MxFont* font = getFont(fontNameID);
        return measureTextInternal(*font, text.c_str(), fontSize, spacing);
    }

    const MxFont* getFont(const std::string& fontNameID)
    {
        static MxFont empty{};
        auto it = m_fonts.find(fontNameID);
        if (it != m_fonts.end())
        {
            return &it->second;
        }

        return &empty;
    }

    std::unordered_map<std::string, MxFont> m_fonts{};
};


struct MxTextureManager
{
    void init() { nativeInit(); }

    void loadTexture(const std::filesystem::path& path, const std::string& textureNameID)
    {
        auto it = m_textures.find(textureNameID);
        if (it != m_textures.end())
        {
            unloadTexture(textureNameID);
        }
        MxTextureNative texture = nativeLoadTexture(path);
        m_textures.insert_or_assign(textureNameID, texture);
    }

    void loadTextureFromImageData(const std::string& textureNameID, void* data, int width, int height, int mipmaps, int format)
    {
        MxTextureNative texture = nativeLoadTextureFromImageData(data, width, height, mipmaps, format);
        m_textures.insert_or_assign(textureNameID, texture);
    }

    MxVec2 getSize(const std::string& textureNameID)
    {
        const MxTextureNative* texture = getTexture(textureNameID);
        return nativeTextureSize(texture);
    }

    bool isSmooth(const std::string& textureNameID)
    {
        const MxTextureNative* texture = getTexture(textureNameID);
        return nativeTextureIsSmooth(texture);
    }

    void setSmooth(const std::string& textureNameID, bool enable)
    {
        auto it = m_textures.find(textureNameID);
        if (it != m_textures.end())
        {
            nativeSetTextureSmooth(&it->second, enable);
        }
    }

    void unload()
    {
        for (auto& [textureNameID, texture] : m_textures)
        {
            const MxTextureNative* textureNative = getTexture(textureNameID);
            nativeUnloadTexture(textureNative);
        }
        m_textures.clear();
    }
    void unloadTexture(const std::string& textureNameID)
    {
        const MxTextureNative* texture = getTexture(textureNameID);
        nativeUnloadTexture(texture);
        m_textures.erase(textureNameID);
    }

    const MxTextureNative* getTexture(const std::string& textureNameID)
    {
        static MxTextureNative empty{};
        auto it = m_textures.find(textureNameID);
        if (it != m_textures.end())
        {
            return &it->second;
        }

        return &empty;
    }


    std::unordered_map<std::string, MxTextureNative> m_textures{};
};


static MxFontManager s_fontManager;
static MxTextureManager s_textureManager;


void initManagers(MxStyle style)
{
    s_fontManager.init(style);
    s_textureManager.init();
}

void closeManagers()
{
    s_fontManager.unload();
    s_textureManager.unload();
}

//-----------------------------------------------------------------------------
// Font manager functions
//-----------------------------------------------------------------------------

const MxFont* getFont(const std::string& fontName)
{
    const MxFont* font = s_fontManager.getFont(fontName);
    return font;
}

void loadFont(const std::string& textureNameID, const std::filesystem::path& path, int fontSize, const int* codepoints, int codepointCount)
{
    std::vector<char> fileData = loadFileData(path);
    if (!fileData.empty())
    {
        // Loading font from memory data
        s_fontManager.loadFromMemory(textureNameID, (unsigned char*)fileData.data(), fontSize, codepoints, codepointCount, false);
    }
}

MxVec2 measureText(const std::string& fontNameID, const std::string& text, int fontSize, int spacing)
{
    return s_fontManager.measureText(fontNameID, text, fontSize, spacing);
}

void drawText(const std::string& fontNameID, const std::string& text, MxVec2 position, float fontSize, float spacing, MxColor tint)
{
    const MxFont* font = s_fontManager.getFont(fontNameID);
    drawTextEx(*font, text, position, fontSize, spacing, tint);
}

void drawIconEx(int codepoint, MxVec2 position, MxColor color, int size)
{
    // Convert codepoint to UTF-8 before to draw
    int byteCount = 0;
    const char* icon = codepointToUTF8(codepoint, &byteCount);
    const MxFont* font = s_fontManager.getFont(MX_FONT_AWESOME_ID);

    drawTextEx(*font, icon, position, size, 0, color);
}


//-----------------------------------------------------------------------------
// Texture manager functions
//-----------------------------------------------------------------------------

void loadTexture(const std::filesystem::path& path, const std::string& textureNameID)
{
    s_textureManager.loadTexture(path, textureNameID);
}

void unloadTexture(const std::string& textureNameID)
{
    s_textureManager.unloadTexture(textureNameID);
}

void loadTextureFromMemory(void* data, int width, int height, int format, int mipmaps, const std::string& textureNameID)
{
    s_textureManager.loadTextureFromImageData(textureNameID, data, width, height, mipmaps, format);
}

MxVec2 getSizeTexture(const std::string& textureNameID)
{
    return s_textureManager.getSize(textureNameID);
}

void setSmoothTexture(const std::string& textureNameID, bool enable)
{
    s_textureManager.setSmooth(textureNameID, enable);
}

bool isSmoothTexture(const std::string& textureNameID)
{
    return s_textureManager.isSmooth(textureNameID);
}

const MxTextureNative* getTexture(const std::string& textureNameID)
{
    const MxTextureNative* texture = s_textureManager.getTexture(textureNameID);
    return texture;
}

#if defined(__GNUC__) && (MX_SUPPRESS_WARNINGS == 1) // GCC and Clang
#pragma GCC diagnostic pop                           // "-Wunused-parameter"  "-Wunused-function"
#endif

//-----------------------------------------------------------------------------
// (SECTION) CUSTOM_USER_FUNCTIONS
//-----------------------------------------------------------------------------

// Note:  Here you can implement a custom functions.
#ifdef MX_CUSTOM_USER_HEADER

#include MX_CUSTOM_USER_HEADER

#endif // MX_CUSTOM_IMPL_HEADER

#endif // MX_GUI_IMPLEMENTATION
