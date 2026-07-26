#ifndef MXGUI_HPP
#define MXGUI_HPP

// Mini XPerience Gui

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
// (SECTION) configs
//-----------------------------------------------------------------------------

#define FORCE_DEBUG 0
#define MX_DEFAULT_FONT_ID "notosans20"
#define MX_FONT_AWESOME_ID "fontawesome"
#define MX_DRAG_OFFSET 4
#define MX_BAR_SIZE 6

#define FONT_AWESOME 1
#define RAYLIB_BACKEND 1
#define SFML_BACKEND 1
// #define CUSTOM_BACKEND 1

//-----------------------------------------------------------------------------
// (SECTION) Header and defines
//-----------------------------------------------------------------------------

#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <type_traits>


#ifdef CUSTOM_BACKEND
#undef RAYLIB_BACKEND
#undef SFML_BACKEND
#else
#ifdef SFML_BACKEND
#undef RAYLIB_BACKEND
#undef CUSTOM_BACKEND
#else
#define RAYLIB_BACKEND 1
#endif // SFML_BACKEND
#endif // CUSTOM_BACKEND

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
// (SECTION) GuiIcons FontAwesome
// Note:
//-----------------------------------------------------------------------------

#if FONT_AWESOME
#include "mxgui_font_awesome.h"
#include "mxgui_icons_font_awesome7.hpp"
#endif

//-----------------------------------------------------------------------------
// (SECTION) Structs Forward declarations
//-----------------------------------------------------------------------------

struct MxGuiContext;

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

struct MxRect;

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

struct MxText
{
    std::string text{};
    MxVec2 measuredText{};
    int size{20};
};

struct MxStyle
{
    MxColor primaryColor{MxColor::Gray};
    MxInt32 borderWidth{1};
    MxColor borderColor{MxColor::DarkGray};
    MxColor backgroundColor{MxColor::LightGray};
    MxColor textColor{MxColor::DarkGray};
    MxInt32 textSize{20};
    MxInt32 iconSize{20};
    std::string fontName{MX_DEFAULT_FONT_ID};
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
                             .iconSize{20},
                             .fontName{MX_DEFAULT_FONT_ID},
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
    MxStyle getStyle(MxGuiContext* ctx);

    MxTransform getCurrentTransform(MxGuiContext* ctx);
    MxMouseEvents getCurrentMouseEvents(MxGuiContext* ctx);

    void createImage(const std::filesystem::path& path, const std::string& imageName);

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


//-----------------------------------------------------------------------------
// (SECTION) back-end functions
// Note: These functions need to be implemented if CUSTOM_BACKEND is used.
//-----------------------------------------------------------------------------

// clip
void pushScissor(int x, int y, int width, int height);
void popScissor();

// managers
void initManagers(MxStyle style);
void closeManagers();

// misc
void loadTexture(const std::filesystem::path& path, const std::string& name);
MxVec2 getTextureSize(const std::string& textureName);
MxVec2 measureText(const std::string& name, const std::string& text);

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
void drawTexturePro(const std::string& textureName, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint);
void drawTextPro(const std::string& fontName, const std::string& text, MxVec2 position, MxVec2 origin, float rotation, float fontSize, float spacing, MxColor tint);
void drawCircle(MxVec2 center, float radius, MxColor color);
void drawIconEx(int codepoint, MxVec2 position, MxColor color, int size = -1);

//-----------------------------------------------------------------------------
// (SECTION) internal functions
//-----------------------------------------------------------------------------

MxTransform updateTransformWorld(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor);


/////////////////////////////////////////////////////
//  implementations
/////////////////////////////////////////////////////

#ifdef MX_GUI_IMPLEMENTATION


#include <memory>
#include <unordered_map>
#include <vector>

#include "mxgui_notosans.hpp"


struct MxGuiContext;

static std::unique_ptr<MxGuiContext> s_context{nullptr};

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


//-----------------------------------------------------------------------------
// Internal functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// (SECTION) Decompression code
//  an algorithm with the sole objective of compressing the matrices to save font size
//  Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/deprecated/stb.h#L10437
//-----------------------------------------------------------------------------


#include "string.h"

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

// clang-format on


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

    void createImage(const std::filesystem::path& path, const std::string& imageName)
    {
        loadTexture(path, imageName);
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
            if (canvas.isDrag && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT))
            {
                const MxVec2 mousePosition = getMousePosition();
                transform.bounds.x = mousePosition.x - canvas.Offset.x - ctx->m_anchor.x;
                transform.bounds.y = mousePosition.y - canvas.Offset.y - ctx->m_anchor.y + ctx->m_scrollTop;
            }
            else
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

        const MxVec2 texture = getTextureSize(imageName);
        const MxRect source = {0.0f, 0.0f, texture.x, texture.y};
        const MxRect dest = {rect.x, rect.y, texture.x, texture.y};

        drawTexturePro(imageName, source, dest, MxVec2{0, 0}, 0, color);
        ctx->updateCurrents(transform, MxMouseEvents{});
    }

    void guiLabel(MxGuiContext* ctx, const std::string& text, MxVec2 bounds, MxVec2 anchor)
    {
        MxTransform transform = updateTransformWorld(ctx, toMxRect(bounds), anchor);
        MxRect rect = transform.worldBounds;

        drawTextPro(ctx->m_style.fontName, text, MxVec2{rect.x, rect.y}, MxVec2{}, 0, ctx->m_style.textSize, 0, ctx->m_style.textColor);
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

        MxVec2 textSize = measureText(ctx->m_style.fontName, text);
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
            const float previousScrollTop = scrollPanel.scrollTop;

            if (checkCollisionPointRect(getMousePosition(), scrollPanel.scrollBarThumb) && isMouseButtonPressed(MX_MOUSE_BUTTON_LEFT))
            {
                scrollPanel.isDrag = true;
            }

            if (scrollPanel.isDrag && isMouseButtonDown(MX_MOUSE_BUTTON_LEFT))
            {
                scrollPanel.scrollTop += getMouseDelta().y;
            }
            else if (checkCollisionPointRect(getMousePosition(), rect))
            {
                scrollPanel.isDrag = false;
                scrollPanel.scrollTop -= getMouseWheelMove() * 10;
            }
            else
            {
                scrollPanel.isDrag = false;
            }


            scrollPanel.scrollTop = (scrollPanel.scrollTop < 0) ? 0 : scrollPanel.scrollTop;
            ctx->m_scrollTop = scrollPanel.scrollTop;


            scrollPanel.transformCanvas.bounds.x = 0;
            scrollPanel.transformCanvas.bounds.y = 0;
            scrollPanel.transformCanvas.anchor = MxVec2{};
            scrollPanel.transformCanvas = updateTransformWorld(ctx, scrollPanel.transformCanvas.bounds, MxVec2{0, 0});
            rectCanvas = scrollPanel.transformCanvas.worldBounds;

            scrollPanel.scrollTop = (rectCanvas.y + rectCanvas.height < rect.y + rect.height) ? previousScrollTop : scrollPanel.scrollTop;
            ctx->m_scrollTop = scrollPanel.scrollTop;

            float visibleProportion = rect.height / rectCanvas.height;
            float progress = scrollPanel.scrollTop / (rectCanvas.height - rect.height);

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

        drawIconEx(codepoint, toMxVec2(transform.worldBounds), color, size);
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


        drawIconEx(codepoint, toMxVec2(transform.worldBounds), color, size);

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
        return guiToogleEx(ctx, bounds, anchor, checked, ICON_FA_SQUARE_CHECK, ICON_FA_SQUARE);
    }

    bool guiToogle(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor, bool& checked)
    {
        return guiToogleEx(ctx, bounds, anchor, checked, ICON_FA_TOGGLE_ON, ICON_FA_TOGGLE_OFF);
    }


} // namespace mxgui


#ifdef RAYLIB_BACKEND

#include <cstdio>

#include "raylib.h"

// MxType to Raylib type helper
inline Vector2 toVector(MxVec2 vec)
{
    return Vector2{vec.x, vec.y};
}

inline Rectangle toRectangle(MxRect rec)
{
    return Rectangle{rec.x, rec.y, rec.width, rec.height};
}

inline Color toColor(MxColor color)
{
    return Color{color.r, color.g, color.b, color.a};
}

inline MxVec2 toMxVec2(Vector2 vec)
{
    return MxVec2{vec.x, vec.y};
}

// Raylib to MxType type helper
inline MxRect toMxRect(Rectangle rec)
{
    return MxRect{rec.x, rec.y, rec.width, rec.height};
}

inline MxColor toMxColor(Color color)
{
    return MxColor{color.r, color.g, color.b, color.a};
}


struct MxFontSpecsInternal
{
    Font font{};
    int size{20};
    int spacing{0};
};


class MxFontManager
{
public:

    void setupDefaultFont(int textSize)
    {
        int codepoints[95];
        for (int i = 0; i < 95; i++)
        {
            codepoints[i] = 32 + i; // ASCII: espaço (32) até ~ (126)
        }
        Font font = LoadFontFromMemory(".ttf", notosans::data, notosans::size, textSize, codepoints, 95);

        // Default font
        m_fonts[MX_DEFAULT_FONT_ID] = MxFontSpecsInternal{
            .font = font,
            .size = textSize,
            .spacing = 0,
        };
        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    }

    void setupFontAwesome(int iconSize)
    {
#if FONT_AWESOME
        int arrayOriginalSize = 414704;
        unsigned char* fontAwesomeData = (unsigned char*)malloc(arrayOriginalSize);
        stb_decompress(fontAwesomeData, fa_compressed_data, fa_compressed_size);

        int count = sizeof(codepointsFontAwesome) / sizeof(codepointsFontAwesome[0]);
        Font faFont = LoadFontFromMemory(".otf", fontAwesomeData, arrayOriginalSize, iconSize, codepointsFontAwesome, count);

        m_fonts[MX_FONT_AWESOME_ID] = MxFontSpecsInternal{
            .font = faFont,
            .size = iconSize,
            .spacing = 0,
        };
        SetTextureFilter(faFont.texture, TEXTURE_FILTER_BILINEAR);
#endif
    }

    void init(MxStyle style)
    {
        if (m_fonts.size() > 0)
        {
            return;
        }

        setupDefaultFont(style.textSize);
        setupFontAwesome(style.iconSize);
    }

    void unload()
    {
        for (auto& [id, fontSpec] : m_fonts)
        {
            UnloadFont(fontSpec.font);
        }
        m_fonts.clear();
    }

    MxVec2 measureText(const std::string& name, const std::string& text)
    {
        MxFontSpecsInternal font = getFont(name);
        if (!IsFontValid(font.font))
        {
            font.font = GetFontDefault();
        }
        return toMxVec2(MeasureTextEx(font.font, text.c_str(), font.size, font.spacing));
    }

    MxFontSpecsInternal getFont(const std::string& name)
    {
        auto it = m_fonts.find(name);
        if (it != m_fonts.end())
        {
            return it->second;
        }

        return MxFontSpecsInternal{};
    }


private:

    std::unordered_map<std::string, MxFontSpecsInternal> m_fonts{};
};


class MxTextureManager
{
public:


    void init()
    {
        if (m_textures.size() > 0)
        {
            return;
        }
    }

    void loadTexture(const std::filesystem::path& path, const std::string& name)
    {
        auto it = m_textures.find(name);
        if (it != m_textures.end())
        {
            UnloadTexture(it->second);
        }
        Texture texture = LoadTexture(path.string().c_str());
        m_textures.insert_or_assign(name, texture);
    }

    void loadTextureFromImageData(const std::string& name, void* data, int width, int height, int mipmaps, int format)
    {
        Image image{
            .data = data,
            .width = width,
            .height = height,
            .mipmaps = mipmaps,
            .format = format,
        };

        Texture texture = LoadTextureFromImage(image);
        m_textures.insert_or_assign(name, texture);
    }

    MxVec2 getSize(const std::string& textureName)
    {
        Texture texture = getTexture(textureName);
        return MxVec2{(float)texture.width, (float)texture.height};
    }

    void unload()
    {
        for (auto& [id, texture] : m_textures)
        {
            UnloadTexture(texture);
        }
        m_textures.clear();
    }

    Texture getTexture(const std::string& textureName)
    {
        auto it = m_textures.find(textureName);
        if (it != m_textures.end())
        {
            return it->second;
        }

        return Texture{};
    }


private:

    std::unordered_map<std::string, Texture> m_textures{};
};


static MxFontManager s_fontManager;
static MxTextureManager s_textureManager;
static std::vector<Rectangle> s_stackScissors{};

Rectangle intersectionArea(const Rectangle& rect2)
{
    if (!s_stackScissors.empty())
    {
        return GetCollisionRec(s_stackScissors.back(), rect2);
    }

    return rect2;
}

void pushScissor(int x, int y, int width, int height)
{
    if (!s_stackScissors.empty())
    {
        EndScissorMode();
    }

    Rectangle rect{(float)x, (float)y, (float)width, (float)height};
    rect = intersectionArea(rect);

    s_stackScissors.push_back(rect);
    BeginScissorMode((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height);
}

void popScissor()
{
    EndScissorMode();

    if (!s_stackScissors.empty())
    {
        s_stackScissors.pop_back();
    }

    if (!s_stackScissors.empty())
    {
        Rectangle rect = s_stackScissors.back();
        BeginScissorMode((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height);
    }
}

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

void loadTexture(const std::filesystem::path& path, const std::string& name)
{
    s_textureManager.loadTexture(path, name);
}

MxVec2 getTextureSize(const std::string& textureName)
{
    return s_textureManager.getSize(textureName);
}

MxVec2 measureText(const std::string& name, const std::string& text)
{
    return s_fontManager.measureText(name, text);
}

MxVec2 getMousePosition()
{
    return toMxVec2(GetMousePosition());
}

MxVec2 getMouseDelta()
{
    return toMxVec2(GetMouseDelta());
}

float getMouseWheelMove()
{
    return GetMouseWheelMove();
}

bool isMouseButtonPressed(int button)
{
    return IsMouseButtonPressed(button);
}

bool isMouseButtonDown(int button)
{
    return IsMouseButtonDown(button);
}

bool isMouseButtonReleased(int button)
{
    return IsMouseButtonReleased(button);
}

void drawRectangleLinesEx(MxRect rec, float lineThick, MxColor color)
{
    DrawRectangleLinesEx(toRectangle(rec), lineThick, toColor(color));
}

void drawRectanglePro(MxRect rec, MxVec2 origin, float rotation, MxColor color)
{
    DrawRectanglePro(toRectangle(rec), toVector(origin), rotation, toColor(color));
}

void drawTexturePro(const std::string& textureName, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint)
{
    const Texture texture = s_textureManager.getTexture(textureName);
    if (!IsTextureValid(texture))
    {
        DrawText(textureName.c_str(), (int)dest.x, (int)dest.y, 10, BLACK);
        return;
    }
    DrawTexturePro(texture, toRectangle(source), toRectangle(dest), toVector(origin), rotation, toColor(tint));
}

void drawTextPro(const std::string& fontName, const std::string& text, MxVec2 position, MxVec2 origin, float rotation, float fontSize, float spacing, MxColor tint)
{

    MxFontSpecsInternal font = s_fontManager.getFont(fontName);

    if (!IsFontValid(font.font))
    {
        font.font = GetFontDefault();
    }

    DrawTextPro(font.font, text.c_str(), toVector(position), toVector(origin), rotation, fontSize, spacing, toColor(tint));
}

void drawCircle(MxVec2 center, float radius, MxColor color)
{
    DrawCircleV(toVector(center), radius, toColor(color));
}

void drawIconEx(int codepoint, MxVec2 position, MxColor color, int size)
{
    // Converte o codepoint pra UTF-8 antes de desenhar
    int byteCount = 0;
    const char* icon = CodepointToUTF8(codepoint, &byteCount);

    MxFontSpecsInternal font = s_fontManager.getFont(MX_FONT_AWESOME_ID);

    if (!IsFontValid(font.font))
    {
        return;
    }

    const int fontSize = (size < 0) ? font.size : size;
    DrawTextEx(font.font, icon, toVector(position), fontSize, 0, toColor(color));
}

#endif // RAYLIB_BACKEND

#ifdef SFML_BACKEND

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>


// void windowPollEvent(const std::optional<sf::Event> event);

static float computeSfmlSizeScale(const sf::Font& font)
{
    constexpr unsigned int reference = 1000;
    const float lineSpacing = font.getLineSpacing(reference);
    return (float)reference / lineSpacing;
}


struct MxFontSpecsInternal
{
    sf::Font font{};
    float sizeScale{1.0f};
    float size{20};
};

static std::unique_ptr<sf::Text> s_text;
static std::unique_ptr<sf::Sprite> s_sprite;

class MxFontManager
{
public:

    void setupDefaultFont(int textSize)
    {
        sf::Font font;
        if (!font.openFromMemory(notosans::data, notosans::size))
        {
            return;
        }


        font.setSmooth(true);

        // Default font
        m_fonts[MX_DEFAULT_FONT_ID] = MxFontSpecsInternal{
            .font = font,
            .sizeScale = computeSfmlSizeScale(font),
            .size = (float)textSize,
        };

        s_text = std::make_unique<sf::Text>(font, "Texto da GUI", 50);
    }

    void setupFontAwesome(int iconSize)
    {
#if FONT_AWESOME

        int arrayOriginalSize = 414704;
        unsigned char* fontAwesomeData = (unsigned char*)malloc(arrayOriginalSize);
        stb_decompress(fontAwesomeData, fa_compressed_data, fa_compressed_size);

        // int count = sizeof(codepointsFontAwesome) / sizeof(codepointsFontAwesome[0]);

        sf::Font faFont;

        if (!faFont.openFromMemory(fontAwesomeData, arrayOriginalSize))
        {
            return;
        }

        faFont.setSmooth(true);

        m_fonts[MX_FONT_AWESOME_ID] = MxFontSpecsInternal{
            .font = faFont,
            .sizeScale = computeSfmlSizeScale(faFont),
            .size = (float)iconSize,
        };

#endif
    }

    void init(MxStyle style)
    {
        if (m_fonts.size() > 0)
        {
            return;
        }

        setupDefaultFont(style.textSize);
        setupFontAwesome(style.iconSize);
    }

    void unload() { m_fonts.clear(); }

    MxVec2 measureText(const std::string& name, const std::string& text)
    {
        MxFontSpecsInternal font = getFont(name);
        if (!s_text)
        {
            MX_ASSERT(s_text, "s_text is not valid");
            return MxVec2{};
        }

        s_text->setFont(font.font);
        s_text->setString(text);
        s_text->setCharacterSize((MxUInt32)(font.size * font.sizeScale));

        sf::FloatRect bounds = s_text->getLocalBounds();

        return MxVec2{.x = bounds.size.x, .y = bounds.size.y};
    }

    MxFontSpecsInternal getFont(const std::string& name)
    {
        auto it = m_fonts.find(name);
        if (it != m_fonts.end())
        {
            return it->second;
        }

        return MxFontSpecsInternal{};
    }


private:

    std::unordered_map<std::string, MxFontSpecsInternal> m_fonts{};
};

class MxTextureManager
{
public:


    void init()
    {
        if (m_textures.size() > 0)
        {
            return;
        }
        sf::Texture texture;
        s_sprite = std::make_unique<sf::Sprite>(texture);
    }

    void loadTexture(const std::filesystem::path& path, const std::string& name)
    {
        sf::Texture texture;
        if (texture.loadFromFile(path))
        {
            m_textures.insert_or_assign(name, texture);
        }
    }

    void loadTextureFromImageData(const std::string& name, void* data, int width, int height, int mipmaps, int format)
    {
        sf::Texture texture;
        if (texture.loadFromMemory(data, width * height))
        {
            if (mipmaps > 1)
            {
                if (!texture.generateMipmap()) {}
            }
            m_textures.insert_or_assign(name, texture);
        }
    }

    MxVec2 getSize(const std::string& textureName)
    {
        sf::Texture texture = getTexture(textureName);
        return MxVec2{(float)texture.getSize().x, (float)texture.getSize().y};
    }

    void unload() { m_textures.clear(); }

    sf::Texture getTexture(const std::string& textureName)
    {
        auto it = m_textures.find(textureName);
        if (it != m_textures.end())
        {
            return it->second;
        }

        return sf::Texture{};
    }


private:

    std::unordered_map<std::string, sf::Texture> m_textures{};
};


struct MxMousePolling
{
    bool pressed{false};
    bool down{false};
    bool release{false};
};


static sf::RectangleShape s_rectShape;
static sf::CircleShape s_circleShape;
static sf::RenderWindow* s_windowRef = nullptr;
static MxVec2 s_mousePosition{};
static MxVec2 s_mouseDelta{};
static float s_mouseWheelScrolled{0.0f};
static MxMousePolling s_mousePolling[5]{
    MxMousePolling{},
    MxMousePolling{},
    MxMousePolling{},
};
static MxFontManager s_fontManager;
static MxTextureManager s_textureManager;
static std::vector<MxRect> s_stackScissors{};


inline sf::Vector2f toVectorF(MxVec2 vec)
{
    return sf::Vector2f{vec.x, vec.y};
}

inline sf::Vector2i toVectorI(MxVec2 vec)
{
    return sf::Vector2i{(int)vec.x, (int)vec.y};
}

inline sf::Color toColor(MxColor color)
{
    return sf::Color{color.r, color.g, color.b, color.a};
}

inline MxVec2 toMxVec2(sf::Vector2f vec)
{
    return MxVec2{vec.x, vec.y};
}
inline MxVec2 toMxVec2(sf::Vector2i vec)
{
    return MxVec2{(float)vec.x, (float)vec.y};
}

void windowDisplay(sf::RenderWindow* window)
{
    window->display();
    for (auto& mouse : s_mousePolling)
    {
        mouse.pressed = false;
        mouse.release = false;
    }
    s_mouseDelta = MxVec2{};
    s_mouseWheelScrolled = 0.0f;
}

std::optional<sf::Event> windowPollEvent(sf::RenderWindow* window)
{

    const std::optional event = window->pollEvent();

    if (!s_windowRef)
    {
        s_windowRef = window;
    }

    if (!event.has_value())
    {
        return event;
    }

    if (const auto* resized = event->getIf<sf::Event::Resized>())
    {
        sf::Vector2f newSize((float)resized->size.x, (float)resized->size.y);
        window->setView(sf::View(newSize / 2.0f, newSize));
    }

    if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>())
    {
        s_mouseDelta.x = mouseMove->position.x - s_mousePosition.x;
        s_mouseDelta.y = mouseMove->position.y - s_mousePosition.y;
        s_mousePosition = toMxVec2(mouseMove->position);
    }

    if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
    {
        s_mouseWheelScrolled = mouseWheelScrolled->delta;
    }

    if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        int button = (int)mousePressed->button;
        s_mousePolling[button].pressed = true;
        s_mousePolling[button].down = true;
    }

    if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {

        int button = (int)mouseReleased->button;
        s_mousePolling[button].pressed = false;
        s_mousePolling[button].down = false;
        s_mousePolling[button].release = true;
    }

    return event;
}


MxRect intersectionArea(const MxRect& rect2)
{
    if (!s_stackScissors.empty())
    {
        return getCollisionRec(s_stackScissors.back(), rect2);
    }

    return rect2;
}

void beginScissorMode(int x, int y, int width, int height)
{
    MX_ASSERT(s_windowRef, "window not reference");

    const sf::Vector2u winSize = s_windowRef->getSize();

    glEnable(GL_SCISSOR_TEST);
    glScissor(x, (GLint)((float)winSize.y - y - height), width, height);
}

void endScissorMode()
{
    glDisable(GL_SCISSOR_TEST);
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

void loadTexture(const std::filesystem::path& path, const std::string& name)
{
    s_textureManager.loadTexture(path, name);
}

MxVec2 getTextureSize(const std::string& textureName)
{
    return s_textureManager.getSize(textureName);
}

MxVec2 measureText(const std::string& name, const std::string& text)
{
    return s_fontManager.measureText(name, text);
}

MxVec2 getMousePosition()
{
    return s_mousePosition;
}

MxVec2 getMouseDelta()
{
    return s_mouseDelta;
}

float getMouseWheelMove()
{
    return s_mouseWheelScrolled;
}

bool isMouseButtonPressed(int button)
{
    return s_mousePolling[button].pressed;
}

bool isMouseButtonDown(int button)
{
    return s_mousePolling[button].down;
}

bool isMouseButtonReleased(int button)
{
    return s_mousePolling[button].release;
}

void drawRectangleLinesEx(MxRect rec, float lineThick, MxColor color)
{
    s_rectShape.setOutlineThickness(lineThick);
    s_rectShape.setPosition({rec.x + lineThick, rec.y + lineThick});
    s_rectShape.setSize({rec.width - lineThick * 2, rec.height - lineThick * 2});
    s_rectShape.setFillColor(sf::Color::Transparent);
    s_rectShape.setOutlineColor(toColor(color));

    MX_ASSERT(s_windowRef, "window not reference");
    s_windowRef->draw(s_rectShape);
}

void drawRectanglePro(MxRect rec, MxVec2 origin, float rotation, MxColor color)
{
    s_rectShape.setOutlineThickness(0);
    s_rectShape.setPosition({rec.x, rec.y});
    s_rectShape.setOrigin({origin.x, origin.y});
    s_rectShape.setRotation(sf::degrees(rotation));
    s_rectShape.setSize({rec.width, rec.height});
    s_rectShape.setFillColor(toColor(color));

    MX_ASSERT(s_windowRef, "window not reference");
    s_windowRef->draw(s_rectShape);
}

void drawTexturePro(const std::string& textureName, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint)
{

    if (!s_sprite)
    {
        MX_ASSERT(s_sprite, "s_sprite is not valid");
        return;
    }

    const sf::Texture texture = s_textureManager.getTexture(textureName);
    const sf::IntRect rect({(int)source.x, (int)source.y}, {(int)source.width, (int)source.height});
    s_sprite->setTexture(texture);
    s_sprite->setTextureRect(rect);
    s_sprite->setPosition({dest.x, dest.y});
    s_sprite->setOrigin({origin.x, origin.y});
    s_sprite->setRotation(sf::degrees(rotation));
    s_sprite->setColor(toColor(tint));

    MX_ASSERT(s_windowRef, "window not reference");
    s_windowRef->draw(*s_sprite);
}

void drawTextPro(const std::string& fontName, const std::string& text, MxVec2 position, MxVec2 origin, float rotation, float fontSize, float spacing, MxColor tint)
{
    if (!s_text)
    {
        MX_ASSERT(s_text, "s_text is not valid");
        return;
    }
    // sf::Text text(font, "Hello SFML", 20);
    MxFontSpecsInternal font = s_fontManager.getFont(fontName);
    s_text->setFont(font.font);
    s_text->setString(text);
    s_text->setPosition(toVectorF(position));
    s_text->setOrigin({origin.x, origin.y});
    s_text->setRotation(sf::degrees(rotation));
    s_text->setCharacterSize((MxUInt32)(fontSize * font.sizeScale));
    s_text->setLetterSpacing((spacing + 0.5f));
    s_text->setFillColor(toColor(tint));

    MX_ASSERT(s_windowRef, "window not reference");
    s_windowRef->draw(*s_text);
}

void drawCircle(MxVec2 center, float radius, MxColor color)
{
    s_circleShape.setRadius(radius);
    s_circleShape.setPosition(sf::Vector2f{center.x - radius, center.y - radius});
    s_circleShape.setFillColor(toColor(color));


    MX_ASSERT(s_windowRef, "window not reference");
    s_windowRef->draw(s_circleShape);
}

void drawIconEx(int codepoint, MxVec2 position, MxColor color, int size)
{
    if (!s_text)
    {
        MX_ASSERT(s_text, "s_text is not valid");
        return;
    }

    MxFontSpecsInternal font = s_fontManager.getFont(MX_FONT_AWESOME_ID);

    const float fontSize = (size < 0) ? font.size : (float)size;

    s_text->setFont(font.font);
    s_text->setString(sf::String(static_cast<char32_t>(codepoint)));
    s_text->setPosition(toVectorF(position));
    s_text->setOrigin({0.f, 0.f});
    s_text->setRotation(sf::degrees(0.f));
    s_text->setCharacterSize((MxUInt32)(fontSize * font.sizeScale));
    s_text->setLetterSpacing(0.5f);
    s_text->setFillColor(toColor(color));

    MX_ASSERT(s_windowRef, "window not reference");
    s_windowRef->draw(*s_text);
}


#endif // SFML_BACKEND
#ifdef CUSTOM_BACKEND

void pushScissor(int x, int y, int width, int height)
{
}

void popScissor()
{
}

void initManagers(MxStyle style)
{
}

void closeManagers()
{
}

void loadTexture(const std::filesystem::path& path, const std::string& name)
{
}

MxVec2 getTextureSize(const std::string& textureName)
{
    return MxVec2{};
}

MxVec2 measureText(const std::string& name, const std::string& text)
{
    return MxVec2{};
}

MxVec2 getMousePosition()
{
    return MxVec2{};
}

MxVec2 getMouseDelta()
{
    return MxVec2{};
}

float getMouseWheelMove()
{
    return 0.0f;
}

bool isMouseButtonPressed(int button)
{
    return false;
}

bool isMouseButtonDown(int button)
{
    return false;
}

bool isMouseButtonReleased(int button)
{
    return false;
}

void drawRectangleLinesEx(MxRect rec, float lineThick, MxColor color)
{
}

void drawRectanglePro(MxRect rec, MxVec2 origin, float rotation, MxColor color)
{
}

void drawTexturePro(const std::string& textureName, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint)
{
}

void drawTextPro(const std::string& fontName, const std::string& text, MxVec2 position, MxVec2 origin, float rotation, float fontSize, float spacing, MxColor tint)
{
}

void drawCircle(MxVec2 center, float radius, MxColor color)
{
}

void drawIconEx(int codepoint, MxVec2 position, MxColor color, int size)
{
}


#endif // CUSTOM_BACKEND

#endif // MX_GUI_IMPLEMENTATION
#endif // MXGUI_HPP
