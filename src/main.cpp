


#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "raylib.h"
#include "raymath.h"


// mxGui


namespace Globals
{
    static Vector2 anchor;
    static float scrollTop;
} // namespace Globals


class FontManager
{
public:

    void init() { m_fonts["inter20"] = LoadFontEx("/home/marco/Downloads/Inter,Noto_Sans/Noto_Sans/static/NotoSans-Regular.ttf", 20, NULL, 255); }
    void unload()
    {
        for (auto& [id, font] : m_fonts)
        {
            UnloadFont(font);
        }
        m_fonts.clear();
    }

    Font* getFont(std::string name)
    {
        auto it = m_fonts.find(name);
        if (it != m_fonts.end())
        {
            return &it->second;
        }

        return nullptr;
    }


private:

    std::unordered_map<std::string, Font> m_fonts;
};


struct MouseEvents
{
    Vector2 mouseOffset{};
    bool isMouseHover{false};
    bool isMousePressed{false};
    bool isMouseRelease{false};
    bool isMouseDown{false};
    bool isDrag{false};
    bool enable{false};
};

struct Transforms
{
    Rectangle bounds{};
    Rectangle worldBounds{};
    Vector2 anchor{};
};

struct Text
{
    std::string value{};
    Vector2 size{};
};

struct CanvasComponent
{
    Transforms transform{.bounds = Rectangle{0, 0, 100, 100}};
    Color color{RED};
    MouseEvents mouseEvents{};
};

struct LabelComponent
{
    Transforms transform{};
    Color color{BLACK};
    std::string fontName{"inter20"};
    Text text{};
};

enum class ButtonStyle
{
    Contained,
    OutLine,
    None,
};

struct ButtonComponent
{
    ButtonStyle style{ButtonStyle::Contained};
    Transforms transform{};
    Color color{RED};
    MouseEvents mouseEvents{};
};

struct ScrollPanelComponent
{
    Transforms transform{};
    Color color{RED};
    MouseEvents mouseEvents{};
    Transforms transformCanvas{};
    float scrollTop{0.0f};
    Rectangle scrollBarThumb{};
};

using WidgetTag = std::string;
struct Context;

void beginTransformWorld(Transforms& transform);
void setTextValue(Context& ctx, WidgetTag tagName, const std::string& newText = "label");
Transforms getCurrentTransform(Context& ctx);
MouseEvents getCurrentMouseEvents(Context& ctx);

void createCanvas(Context& ctx, WidgetTag tagName);
void createButton(Context& ctx, WidgetTag tagName);
void createLabel(Context& ctx, WidgetTag tagName, const std::string& newText);
void createScrollPanel(Context& ctx, WidgetTag tagName);

void guiCanvas(Context& ctx, WidgetTag tag, Vector2 bounds = Vector2{0}, Vector2 anchor = Vector2{0}, bool enableDrag = false);
void guiButton(Context& ctx, WidgetTag tag, Vector2 bounds = Vector2{0}, Vector2 anchor = Vector2{0});
void guiLabel(Context& ctx, WidgetTag tag, Vector2 bounds = Vector2{0}, Vector2 anchor = Vector2{0});
void guiScrollPanelBegin(Context& ctx, WidgetTag tag, Vector2 bounds = Vector2{0}, Vector2 anchor = Vector2{0});
void guiScrollPanelEnd(Context& ctx, WidgetTag tag);


///////////////////////

#define INSERT_COMPONENT(componentsList, type)                                            \
    inline type* insert##type(WidgetTag tag, type component)                              \
    {                                                                                     \
        const WidgetTag hash = "##" + tag;                                                \
        auto [insertedIt, isInserted] = componentsList.insert_or_assign(hash, component); \
        return &insertedIt->second;                                                       \
    }

#define GET_COMPONENT(componentsList, type)                                    \
    inline type* get##type(WidgetTag tag)                                      \
    {                                                                          \
        const WidgetTag hash = "##" + tag;                                     \
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

struct Context
{

    void init() { m_fontManager.init(); }
    void close() { m_fontManager.unload(); }

    FontManager& getFontManager() { return m_fontManager; }

    COMPONENT(m_canvas, CanvasComponent);
    COMPONENT(m_labels, LabelComponent);
    COMPONENT(m_buttons, ButtonComponent);
    COMPONENT(m_scrollPanels, ScrollPanelComponent);

    // friend Transforms getCurrentTransform(Context& ctx);
    // friend MouseEvents getCurrentMouseEvents(Context& ctx);
    // friend void guiCanvas(Context& ctx, WidgetTag tag, Vector2 bounds, Vector2 anchor, bool enableDrag);

    Transforms m_currentTransform{};
    MouseEvents m_currentMouseEvents{};

    std::unordered_map<WidgetTag, CanvasComponent> m_canvas;
    std::unordered_map<WidgetTag, LabelComponent> m_labels;
    std::unordered_map<WidgetTag, ButtonComponent> m_buttons;
    std::unordered_map<WidgetTag, ScrollPanelComponent> m_scrollPanels;

    FontManager m_fontManager;
};


void beginTransformWorld(Transforms& transform)
{
    transform.worldBounds = {
        .x = transform.bounds.x + transform.anchor.x + Globals::anchor.x,
        .y = transform.bounds.y + transform.anchor.y + Globals::anchor.y - Globals::scrollTop,
        .width = transform.bounds.width,
        .height = transform.bounds.height,
    };
}

void setTextValue(Context& ctx, WidgetTag tagName, const std::string& newText)
{
    LabelComponent& label = *ctx.getLabelComponent(tagName);
    Font font = *ctx.getFontManager().getFont(label.fontName);
    label.text.value = newText;
    label.text.size = MeasureTextEx(font, newText.c_str(), 20, 0);
}

Transforms getCurrentTransform(Context& ctx)
{
    return ctx.m_currentTransform;
}

MouseEvents getCurrentMouseEvents(Context& ctx)
{
    return ctx.m_currentMouseEvents;
}

void createCanvas(Context& ctx, WidgetTag tagName)
{
    CanvasComponent canvas;
    ctx.insertCanvasComponent(tagName, canvas);
}

#define TEXT_LABEL "button"

void createButton(Context& ctx, WidgetTag tagName)
{
    ButtonComponent button;
    button.transform.bounds = Rectangle{0, 0, 80, 40};
    button.style = ButtonStyle::OutLine;
    button.mouseEvents.enable = true;
    createLabel(ctx, TEXT_LABEL + tagName, "button");
    ctx.insertButtonComponent(tagName, button);
}

void createLabel(Context& ctx, WidgetTag tagName, const std::string& newText)
{
    LabelComponent label;
    ctx.insertLabelComponent(tagName, label);
    setTextValue(ctx, tagName, newText);
}

void createScrollPanel(Context& ctx, WidgetTag tagName)
{
    ScrollPanelComponent panel;
    panel.mouseEvents.enable = true;
    panel.transform.bounds = Rectangle{0, 0, 100, 200};
    panel.transformCanvas.bounds = Rectangle{0, 0, panel.transform.bounds.width, panel.transform.bounds.height + 50};
    ctx.insertScrollPanelComponent(tagName, panel);
}


void guiCanvas(Context& ctx, WidgetTag tag, Vector2 bounds, Vector2 anchor, bool enableDrag)
{

    CanvasComponent& canvas = *ctx.getCanvasComponent(tag);
    canvas.transform.bounds.x = bounds.x;
    canvas.transform.bounds.y = bounds.y;
    canvas.transform.anchor = anchor;


    beginTransformWorld(canvas.transform);
    Rectangle rect = canvas.transform.worldBounds;

    canvas.mouseEvents.enable = enableDrag;

    if (canvas.mouseEvents.enable)
    {
        canvas.mouseEvents.isMouseHover = (CheckCollisionPointRec(GetMousePosition(), rect));
        canvas.mouseEvents.isMouseRelease = canvas.mouseEvents.isMouseHover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
        canvas.mouseEvents.isMouseDown = canvas.mouseEvents.isMouseHover && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        canvas.mouseEvents.isMousePressed = canvas.mouseEvents.isMouseHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (canvas.mouseEvents.isMousePressed)
        {
            Vector2 mousePosition = GetMousePosition();
            canvas.mouseEvents.mouseOffset.x = canvas.transform.anchor.x + mousePosition.x - rect.x;
            canvas.mouseEvents.mouseOffset.y = canvas.transform.anchor.y + mousePosition.y - rect.y;
            canvas.mouseEvents.isDrag = true;
        }
        if (canvas.mouseEvents.isDrag && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePosition = GetMousePosition();
            canvas.transform.bounds.x = mousePosition.x - canvas.mouseEvents.mouseOffset.x - Globals::anchor.x;
            canvas.transform.bounds.y = mousePosition.y - canvas.mouseEvents.mouseOffset.y - Globals::anchor.y + Globals::scrollTop;
        }
        else
        {
            canvas.mouseEvents.isDrag = false;
        }
    }

    DrawRectanglePro(rect, Vector2{}, 0, Fade(canvas.color, 0.5f));

    ctx.m_currentTransform = canvas.transform;
    ctx.m_currentMouseEvents = canvas.mouseEvents;
}

void guiLabel(Context& ctx, WidgetTag tag, Vector2 bounds, Vector2 anchor)
{

    LabelComponent& label = *ctx.getLabelComponent(tag);
    label.transform.bounds.x = bounds.x;
    label.transform.bounds.y = bounds.y;
    label.transform.anchor = anchor;

    beginTransformWorld(label.transform);
    Rectangle rect = label.transform.worldBounds;


    Font font = *ctx.getFontManager().getFont("inter20");
    if (!IsFontValid(font))
    {
        font = GetFontDefault();
    }

    DrawTextEx(font, label.text.value.c_str(), Vector2{rect.x, rect.y}, 20, 0, label.color);
}


void guiButton(Context& ctx, WidgetTag tag, Vector2 bounds, Vector2 anchor)
{
    ButtonComponent& button = *ctx.getButtonComponent(tag);
    button.transform.bounds.x = bounds.x;
    button.transform.bounds.y = bounds.y;
    button.transform.anchor = anchor;

    beginTransformWorld(button.transform);
    Rectangle rect = button.transform.worldBounds;


    int paint = 0;

    if (button.mouseEvents.enable)
    {
        button.mouseEvents.isMouseHover = (CheckCollisionPointRec(GetMousePosition(), rect));
        button.mouseEvents.isMouseRelease = button.mouseEvents.isMouseHover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
        button.mouseEvents.isMouseDown = button.mouseEvents.isMouseHover && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        button.mouseEvents.isMousePressed = button.mouseEvents.isMouseHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (button.mouseEvents.isMouseHover)
        {
            paint = 50;
        }
        if (button.mouseEvents.isMouseDown)
        {
            paint = 90;
        }
    }

    const int r = std::clamp(button.color.r - paint, 0, 255);
    const int g = std::clamp(button.color.g - paint, 0, 255);
    const int b = std::clamp(button.color.b - paint, 0, 255);

    Color finalColor = Color{
        .r = (unsigned char)r,
        .g = (unsigned char)g,
        .b = (unsigned char)b,
        .a = button.color.a,
    };

    if (button.style == ButtonStyle::Contained)
    {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, finalColor);
    }
    else if (button.style == ButtonStyle::OutLine)
    {
        DrawRectangleLinesEx(Rectangle{rect.x, rect.y, rect.width, rect.height}, 1, finalColor);
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(finalColor, 0.1f));
    }

    LabelComponent& label = *ctx.getLabelComponent(TEXT_LABEL + tag);
    Vector2 textSize = label.text.size;
    Vector2 textPosition = Vector2{rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2};
    textPosition.x -= Globals::anchor.x;
    textPosition.y -= Globals::anchor.y;

    guiLabel(ctx, TEXT_LABEL + tag, Vector2{0}, textPosition);
}

void guiScrollPanelBegin(Context& ctx, WidgetTag tag, Vector2 bounds, Vector2 anchor)
{
    ScrollPanelComponent& scrollPanel = *ctx.getScrollPanelComponent(tag);
    scrollPanel.transform.bounds.x = bounds.x;
    scrollPanel.transform.bounds.y = bounds.y;
    scrollPanel.transform.anchor = anchor;


    beginTransformWorld(scrollPanel.transform);
    Rectangle rect = scrollPanel.transform.worldBounds;
    Rectangle rectCanvas = rect;

    DrawRectangleLinesEx(rect, 1, Fade(scrollPanel.color, 0.5f));
    Globals::anchor = Vector2{rect.x, rect.y};


    if (scrollPanel.transformCanvas.bounds.height > rect.height)
    {
        const float previousScrollTop = scrollPanel.scrollTop;

        if (CheckCollisionPointRec(GetMousePosition(), scrollPanel.scrollBarThumb) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            scrollPanel.mouseEvents.isDrag = true;
        }

        if (scrollPanel.mouseEvents.isDrag && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            scrollPanel.scrollTop += GetMouseDelta().y;
        }
        else
        {
            scrollPanel.mouseEvents.isDrag = false;
            scrollPanel.scrollTop -= GetMouseWheelMove() * 10;
        }

        scrollPanel.scrollTop = (scrollPanel.scrollTop < 0) ? previousScrollTop : scrollPanel.scrollTop;
        Globals::scrollTop = scrollPanel.scrollTop;

        beginTransformWorld(scrollPanel.transformCanvas);
        rectCanvas = scrollPanel.transformCanvas.worldBounds;

        scrollPanel.scrollTop = (rectCanvas.y + rectCanvas.height < rect.y + rect.height) ? previousScrollTop : scrollPanel.scrollTop;
        Globals::scrollTop = scrollPanel.scrollTop;

        float visibleProportion = rect.height / rectCanvas.height;
        float progress = scrollPanel.scrollTop / (rectCanvas.height - rect.height);

        scrollPanel.scrollBarThumb = Rectangle{
            .x = rect.x + rect.width - 5,
            .y = rect.y + (rect.height - rect.height * visibleProportion) * progress,
            .width = 5,
            .height = rect.height * visibleProportion,
        };
    }

    // BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
    DrawRectanglePro(rectCanvas, Vector2{}, 0, Fade(GRAY, 0.5f));
}

void guiScrollPanelEnd(Context& ctx, WidgetTag tag)
{
    ScrollPanelComponent& scrollPanel = *ctx.getScrollPanelComponent(tag);
    Globals::anchor = Vector2{};
    Globals::scrollTop = 0.0f;
    DrawRectangleRec(scrollPanel.scrollBarThumb, RED);
    EndScissorMode();
}


int main(int argc, const char** argv)
{

    InitWindow(800, 600, "GUI");
    SetTextLineSpacing(0);


    Context ctx;
    ctx.init();

    createCanvas(ctx, "Canvas1");
    createCanvas(ctx, "Canvas2");
    createButton(ctx, "ButtonClick1");
    createLabel(ctx, "Label1", "hello world");
    createLabel(ctx, "Label2", "testando");
    createScrollPanel(ctx, "ScrollPanel");


    Transforms transform;
    transform.bounds.x = 10;
    transform.bounds.y = 10;
    transform.anchor.x = 100;
    transform.anchor.y = 100;


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        guiButton(ctx, "ButtonClick1", Vector2{250, 100}, Vector2{transform.bounds.x, transform.bounds.y});
        guiScrollPanelBegin(ctx, "ScrollPanel", Vector2{300, 200}, Vector2{100, 100});

        guiCanvas(ctx, "Canvas1", Vector2{transform.bounds.x, transform.bounds.y}, transform.anchor, true);
        transform = getCurrentTransform(ctx);

        guiCanvas(ctx, "Canvas2", Vector2{0, 0}, Vector2{transform.bounds.x, transform.bounds.y}, false);
        guiLabel(ctx, "Label1", Vector2{100, 0}, Vector2{transform.bounds.x, transform.bounds.y});
        guiLabel(ctx, "Label2", Vector2{0, 0});
        guiScrollPanelEnd(ctx, "ScrollPanel");

        DrawFPS(10, 10);

        EndDrawing();
    }
    ctx.close();
    CloseWindow();

    return 0;
}
