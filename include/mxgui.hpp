#ifndef MXGUI_HPP
#define MXGUI_HPP

//-----------------------------------------------------------------------------
// (SECTION) Header defines
//-----------------------------------------------------------------------------

#include <filesystem>
#include <string>
#include <type_traits>


#define MX_DEFAULT_FONT "notosans20"
#define MX_DRAG_OFFSET 3

#ifdef _DEBUG
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
// (SECTION) Structs Forward declarations
//-----------------------------------------------------------------------------

struct MxGuiContext;

//-----------------------------------------------------------------------------
// (SECTION) basic types
//-----------------------------------------------------------------------------
typedef std::string MxWidgetTag;
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
    std::string value{};
    MxVec2 size{};
};


//-----------------------------------------------------------------------------
// (SECTION) Structs components
//-----------------------------------------------------------------------------

struct CanvasComponent
{
    MxTransform transform{.bounds = MxRect{0, 0, 100, 100}};
    MxColor color{MxColor::Red};
    MxVec2 Offset{};
    bool isDrag{false};
};

struct ImageComponent
{
    MxTransform transform{};
    MxColor color{MxColor::White};
};

struct LabelComponent
{
    MxTransform transform{};
    MxColor color{MxColor::Black};
    std::string fontName{MX_DEFAULT_FONT};
    MxText text{};
};

enum class ButtonStyle
{
    MxContained,
    MxOutLine,
    MxNone,
};

struct ButtonComponent
{
    ButtonStyle style{ButtonStyle::MxContained};
    MxTransform transform{};
    MxColor color{MxColor::Red};
};

struct ScrollPanelComponent
{
    MxTransform transform{};
    MxColor color{MxColor::Red};
    bool isDrag{false};
    MxTransform transformCanvas{};
    float scrollTop{0.0f};
    MxRect scrollBarThumb{};
};

//-----------------------------------------------------------------------------
// (SECTION) public API functions
//-----------------------------------------------------------------------------

namespace mxgui
{


    MxGuiContext* createContext();
    void destroyContext(MxGuiContext* ctx = nullptr);

    void setTextValue(MxGuiContext* ctx, MxWidgetTag tagName, const std::string& newText = "label");
    MxTransform getCurrentTransform(MxGuiContext* ctx);
    MxMouseEvents getCurrentMouseEvents(MxGuiContext* ctx);

    void createCanvas(MxGuiContext* ctx, MxWidgetTag tagName);
    void createImage(MxGuiContext* ctx, MxWidgetTag tagName, const std::filesystem::path& path, const std::string& imageName);
    void createButton(MxGuiContext* ctx, MxWidgetTag tagName);
    void createLabel(MxGuiContext* ctx, MxWidgetTag tagName, const std::string& newText);
    void createScrollPanel(MxGuiContext* ctx, MxWidgetTag tagName);

    void guiCanvas(MxGuiContext* ctx, MxWidgetTag tag, MxRect bounds = MxRect{0}, MxVec2 anchor = MxVec2{0}, bool enableDrag = false);
    void guiImage(MxGuiContext* ctx, MxWidgetTag tag, const std::string& imageName, MxRect bounds = MxRect{0}, MxVec2 anchor = MxVec2{0});
    bool guiButton(MxGuiContext* ctx, MxWidgetTag tag, MxRect bounds = MxRect{0}, MxVec2 anchor = MxVec2{0}, bool isEnable = true);
    void guiLabel(MxGuiContext* ctx, MxWidgetTag tag, MxVec2 bounds = MxVec2{0}, MxVec2 anchor = MxVec2{0});
    void guiScrollPanelBegin(MxGuiContext* ctx, MxWidgetTag tag, MxRect bounds = MxRect{0}, MxRect scrollBounds = MxRect{0}, MxVec2 anchor = MxVec2{0}, bool isEnable = true);
    void guiScrollPanelEnd(MxGuiContext* ctx, MxWidgetTag tag);

} // namespace mxgui

inline void fromMxRect(const MxRect& from, MxVec2& to)
{
    to.x = from.x;
    to.y = from.y;
}

inline void fromMxVec2(const MxVec2& from, MxRect& to)
{
    to.x = from.x;
    to.y = from.y;
}

inline MxVec2 MxRectToMxVec2(const MxRect& rec)
{
    return MxVec2{rec.x, rec.y};
}

inline MxRect MxVec2ToMxRect(const MxVec2& vec)
{
    return MxRect{vec.x, vec.y, 0, 0};
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
    MX_ASSERT(std::is_signed_v<T> == std::is_signed_v<U> && std::is_signed_v<T> == std::is_signed_v<V>,
              "Clamp arguments must all be of the same signedness to avoid errors.");

    return (value < min) ? min : (value > max) ? max : value;
}


#endif // MXGUI_HPP
