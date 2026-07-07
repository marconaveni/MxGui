


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

    Font* getFont(std::string name) { return &m_fonts[name]; } // do jeito que esta pode gerar uma chave uma Font vazia, fazer uma guarda

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
    Vector2 anchor{};
    Rectangle worldBounds{};
};

void beginTransform(Transforms& transform)
{
    transform.worldBounds = {
        .x = transform.bounds.x + transform.anchor.x + Globals::anchor.x,
        .y = transform.bounds.y + transform.anchor.y + Globals::anchor.y - Globals::scrollTop,
        .width = transform.bounds.width,
        .height = transform.bounds.height,
    };
}


struct Canvas
{
    Transforms transform{};
    Color color{RED};
    MouseEvents mouseEvents{};
};

struct Text
{
    std::string value{};
    Vector2 size{};
};

struct Label
{
    Transforms transform{};
    Color color{BLACK};
    std::string fontName{"inter20"};
    Text text{};
    FontManager* fontManager{nullptr};
};

void setLabelText(Label& label, const std::string& newText)
{
    label.text.value = newText;
    label.text.size = MeasureTextEx(*label.fontManager->getFont(label.fontName), newText.c_str(), 20, 0);
}


struct  Button
{
    enum class Style
    {
        Contained,
        OutLine,
        None,
    };
    Style style{Style::Contained};
    Transforms transform{};
    Color color{RED};
    Label label{};
    MouseEvents mouseEvents{};
};

struct ScrollPanel
{
    Transforms transform{};
    Color color{RED};
    MouseEvents mouseEvents{};
    Canvas canvas{};
    float scrollTop{0.0f};
    Rectangle scrollBarThumb{};
};


void guiCanvas(Canvas& canvas)
{

    beginTransform(canvas.transform);
    Rectangle rect = canvas.transform.worldBounds;


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
            canvas.transform.bounds.x = mousePosition.x - canvas.mouseEvents.mouseOffset.x;
            canvas.transform.bounds.y = mousePosition.y - canvas.mouseEvents.mouseOffset.y;
        }
        else
        {
            canvas.mouseEvents.isDrag = false;
        }
    }

    DrawRectanglePro(rect, Vector2{}, 0, Fade(canvas.color, 0.5f));
}

void guiScrollPanelBegin(ScrollPanel& scrollPanel)
{
    beginTransform(scrollPanel.transform);
    Rectangle rect = scrollPanel.transform.worldBounds;

    DrawRectangleLinesEx(rect, 1, Fade(scrollPanel.color, 0.5f));
    Globals::anchor = Vector2{rect.x, rect.y};


    if (scrollPanel.canvas.transform.bounds.height > rect.height)
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

        beginTransform(scrollPanel.canvas.transform);
        Rectangle rectCanvas = scrollPanel.canvas.transform.worldBounds;


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


    guiCanvas(scrollPanel.canvas);
}

void guiScrollPanelEnd(ScrollPanel& scrollPanel)
{
    Globals::anchor = Vector2{};
    Globals::scrollTop = 0.0f;
    DrawRectangleRec(scrollPanel.scrollBarThumb, RED);
    EndScissorMode();
}

void guiLabel(Label& label)
{
    beginTransform(label.transform);
    Rectangle rect = label.transform.worldBounds;

    Font font = GetFontDefault();
    if (label.fontManager)
    {
        font = *label.fontManager->getFont("inter20");
    }

    DrawTextEx(font, label.text.value.c_str(), Vector2{rect.x, rect.y}, 20, 0, label.color);
}

void guiButton(Button& button)
{
    beginTransform(button.transform);
    Rectangle rect = button.transform.worldBounds;

    Vector2 vec = Vector2{rect.x + (rect.width - button.label.text.size.x) / 2, rect.y + (rect.height - button.label.text.size.y) / 2};
    button.label.transform.anchor = Vector2{vec.x, vec.y};

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

    if (button.style == Button::Style::Contained)
    {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, finalColor);
    }
    else if (button.style == Button::Style::OutLine)
    {
        DrawRectangleLinesEx(Rectangle{rect.x, rect.y, rect.width, rect.height}, 1, finalColor);
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(finalColor, 0.1f));
    }

    guiLabel(button.label);
}


using WidgetTag = std::string;

#define INSERT_COMPONENT(componentsList, type)                                        \
    inline type* insert##type(WidgetTag tag, type value)                              \
    {                                                                                 \
        const WidgetTag hash = "##" + tag;                                            \
        auto [insertedIt, isInserted] = componentsList.insert_or_assign(hash, value); \
        return &insertedIt->second;                                                   \
    }

#define GET_COMPONENT(componentsList, type)                                     \
    inline type* get##type(WidgetTag tag)                                       \
    {                                                                           \
        const WidgetTag hash = "##" + tag;                                      \
                                                                                \
        auto it = componentsList.find(hash);                                    \
        if (it != componentsList.end())                                         \
        {                                                                       \
            return &it->second;                                                 \
        }                                                                       \
                                                                                \
        auto [insertedIt, isInserted] = componentsList.insert({hash, type{}}); \
        return &insertedIt->second;                                             \
    }


class Context
{
public:

    INSERT_COMPONENT(m_labels, Label);
    INSERT_COMPONENT(m_buttons, Button);
    GET_COMPONENT(m_labels, Label);
    GET_COMPONENT(m_buttons, Button);


private:

    std::unordered_map<WidgetTag, Label> m_labels;
    std::unordered_map<WidgetTag, Button> m_buttons;
};


int main(int argc, const char** argv)
{

    Context ctx;
    ctx.insertButton("Btn", Button{});

    InitWindow(800, 600, "GUI");
    SetTextLineSpacing(0);

    FontManager fontManager;
    fontManager.init();


    Canvas canvas;
    canvas.transform.bounds = Rectangle{100, 100, 100, 100};
    canvas.transform.anchor = Vector2{100, 100};
    canvas.mouseEvents.enable = true;

    Canvas canvas2;
    canvas2.transform.bounds = Rectangle{0, 0, 50, 50};

    Label label;
    label.fontManager = &fontManager;
    label.transform.bounds = Rectangle{100, 0, 20, 20};
    setLabelText(label, "hello world");

    Label labelTest;
    labelTest.fontManager = &fontManager;
    labelTest.transform.bounds = Rectangle{0, 0, 20, 20};
    setLabelText(labelTest, "testando");

    Button button;
    button.label.fontManager = &fontManager;
    button.transform.bounds = Rectangle{250, 100, 80, 40};
    button.style = Button::Style::OutLine;
    button.mouseEvents.enable = true;
    setLabelText(button.label, "click");

    ScrollPanel panel;
    panel.transform.bounds = Rectangle{300, 200, 100, 200};
    panel.transform.anchor = Vector2{100, 100};
    panel.mouseEvents.enable = true;
    panel.canvas.transform.bounds = Rectangle{0, 0, panel.transform.bounds.width, panel.transform.bounds.height + 50};
    panel.canvas.color = GRAY;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        guiCanvas(canvas);

        canvas2.transform.anchor = Vector2{canvas.transform.bounds.x, canvas.transform.bounds.y};
        button.transform.anchor = Vector2{canvas.transform.bounds.x, canvas.transform.bounds.y};

        guiCanvas(canvas2);
        guiLabel(label);
        guiButton(button);
        guiScrollPanelBegin(panel);
        guiLabel(labelTest);
        guiScrollPanelEnd(panel);

        DrawFPS(10, 10);

        EndDrawing();
    }
    fontManager.unload();
    CloseWindow();

    return 0;
}
