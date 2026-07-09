#include "mxgui.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "mxgui_notosans.hpp"
#include "raylib.h"

// mxGui

void beginTransformWorld(MxGuiContext* ctx, MxTransform& transform);



struct FontSpecs
{
    Font font{};
    int size{20};
};


class FontManager
{
public:


    void init()
    {
        auto it = m_fonts.find(MX_DEFAULT_FONT);
        if (it != m_fonts.end())
        {
            return;
        }

        m_fonts[MX_DEFAULT_FONT] = FontSpecs{// Default font
                                             .font = LoadFontFromMemory(".ttf", notosans::data, notosans::size, 20, NULL, 255),
                                             .size = 20};
    }

    void unload()
    {
        for (auto& [id, fontSpec] : m_fonts)
        {
            UnloadFont(fontSpec.font);
        }
        m_fonts.clear();
    }

    FontSpecs getFont(std::string name)
    {
        auto it = m_fonts.find(name);
        if (it != m_fonts.end())
        {
            return it->second;
        }

        return FontSpecs{};
    }


private:

    std::unordered_map<std::string, FontSpecs> m_fonts{};
};


///////////////////////

#define INSERT_COMPONENT(componentsList, type)                                            \
    inline type* insert##type(MxWidgetTag tag, type component)                            \
    {                                                                                     \
        const MxWidgetTag hash = "##" + tag;                                              \
        auto [insertedIt, isInserted] = componentsList.insert_or_assign(hash, component); \
        return &insertedIt->second;                                                       \
    }

#define GET_COMPONENT(componentsList, type)                                    \
    inline type* get##type(MxWidgetTag tag)                                    \
    {                                                                          \
        const MxWidgetTag hash = "##" + tag;                                   \
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

    void init() { m_fontManager.init(); }
    void close() { m_fontManager.unload(); }

    FontManager& getFontManager() { return m_fontManager; }

    COMPONENT(m_canvas, CanvasComponent);
    COMPONENT(m_labels, LabelComponent);
    COMPONENT(m_buttons, ButtonComponent);
    COMPONENT(m_scrollPanels, ScrollPanelComponent);


    MxTransform m_currentTransform{};
    MxMouseEvents m_currentMouseEvents{};

    std::unordered_map<MxWidgetTag, CanvasComponent> m_canvas;
    std::unordered_map<MxWidgetTag, LabelComponent> m_labels;
    std::unordered_map<MxWidgetTag, ButtonComponent> m_buttons;
    std::unordered_map<MxWidgetTag, ScrollPanelComponent> m_scrollPanels;

    FontManager m_fontManager;
    Vector2 m_anchor{};
    float m_scrollTop{};
};

static std::unique_ptr<MxGuiContext> g_context{nullptr};


void beginTransformWorld(MxGuiContext* ctx, MxTransform& transform)
{
    transform.worldBounds = {
        .x = transform.bounds.x + transform.anchor.x + ctx->m_anchor.x,
        .y = transform.bounds.y + transform.anchor.y + ctx->m_anchor.y - ctx->m_scrollTop,
        .width = transform.bounds.width,
        .height = transform.bounds.height,
    };
}

namespace mxgui
{

    MxGuiContext* createContext()
    {
        g_context = std::make_unique<MxGuiContext>();
        g_context->init();
        return g_context.get();
    }

    void destroyContext(MxGuiContext* ctx)
    {
        g_context->close();
        g_context.reset();
    }

    void setTextValue(MxGuiContext* ctx, MxWidgetTag tagName, const std::string& newText)
    {
        LabelComponent& label = *ctx->getLabelComponent(tagName);
        Font font = ctx->getFontManager().getFont(label.fontName).font;
        label.text.value = newText;
        label.text.size = MeasureTextEx(font, newText.c_str(), 20, 0);
    }

    MxTransform getCurrentTransform(MxGuiContext* ctx)
    {
        return ctx->m_currentTransform;
    }

    MxMouseEvents getCurrentMouseEvents(MxGuiContext* ctx)
    {
        return ctx->m_currentMouseEvents;
    }

    void createCanvas(MxGuiContext* ctx, MxWidgetTag tagName)
    {
        CanvasComponent canvas;
        ctx->insertCanvasComponent(tagName, canvas);
    }

#define TEXT_LABEL "button"

    void createButton(MxGuiContext* ctx, MxWidgetTag tagName)
    {
        ButtonComponent button;
        button.transform.bounds = Rectangle{0, 0, 80, 40};
        button.style = ButtonStyle::MxOutLine;
        button.mouseEvents.enable = true;
        createLabel(ctx, TEXT_LABEL + tagName, "button");
        ctx->insertButtonComponent(tagName, button);
    }

    void createLabel(MxGuiContext* ctx, MxWidgetTag tagName, const std::string& newText)
    {
        LabelComponent label;
        ctx->insertLabelComponent(tagName, label);
        setTextValue(ctx, tagName, newText);
    }

    void createScrollPanel(MxGuiContext* ctx, MxWidgetTag tagName)
    {
        ScrollPanelComponent panel;
        panel.mouseEvents.enable = true;
        panel.transform.bounds = Rectangle{0, 0, 100, 200};
        panel.transformCanvas.bounds = Rectangle{0, 0, panel.transform.bounds.width, panel.transform.bounds.height + 50};
        ctx->insertScrollPanelComponent(tagName, panel);
    }


    void guiCanvas(MxGuiContext* ctx, MxWidgetTag tag, Vector2 bounds, Vector2 anchor, bool enableDrag)
    {

        CanvasComponent& canvas = *ctx->getCanvasComponent(tag);
        canvas.transform.bounds.x = bounds.x;
        canvas.transform.bounds.y = bounds.y;
        canvas.transform.anchor = anchor;


        beginTransformWorld(ctx, canvas.transform);
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
                canvas.transform.bounds.x = mousePosition.x - canvas.mouseEvents.mouseOffset.x - ctx->m_anchor.x;
                canvas.transform.bounds.y = mousePosition.y - canvas.mouseEvents.mouseOffset.y - ctx->m_anchor.y + ctx->m_scrollTop;
            }
            else
            {
                canvas.mouseEvents.isDrag = false;
            }
        }

        DrawRectanglePro(rect, Vector2{}, 0, Fade(canvas.color, 0.5f));

        ctx->m_currentTransform = canvas.transform;
        ctx->m_currentMouseEvents = canvas.mouseEvents;
    }

    void guiLabel(MxGuiContext* ctx, MxWidgetTag tag, Vector2 bounds, Vector2 anchor)
    {

        LabelComponent& label = *ctx->getLabelComponent(tag);
        label.transform.bounds.x = bounds.x;
        label.transform.bounds.y = bounds.y;
        label.transform.anchor = anchor;

        beginTransformWorld(ctx, label.transform);
        Rectangle rect = label.transform.worldBounds;


        Font font = ctx->getFontManager().getFont(label.fontName).font;
        if (!IsFontValid(font))
        {
            font = GetFontDefault();
        }

        DrawTextEx(font, label.text.value.c_str(), Vector2{rect.x, rect.y}, 20, 0, label.color);
    }


    void guiButton(MxGuiContext* ctx, MxWidgetTag tag, Vector2 bounds, Vector2 anchor)
    {
        ButtonComponent& button = *ctx->getButtonComponent(tag);
        button.transform.bounds.x = bounds.x;
        button.transform.bounds.y = bounds.y;
        button.transform.anchor = anchor;

        beginTransformWorld(ctx, button.transform);
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

        if (button.style == ButtonStyle::MxContained)
        {
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, finalColor);
        }
        else if (button.style == ButtonStyle::MxOutLine)
        {
            DrawRectangleLinesEx(Rectangle{rect.x, rect.y, rect.width, rect.height}, 1, finalColor);
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(finalColor, 0.1f));
        }

        LabelComponent& label = *ctx->getLabelComponent(TEXT_LABEL + tag);
        Vector2 textSize = label.text.size;
        Vector2 textPosition = Vector2{rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2};
        textPosition.x -= ctx->m_anchor.x;
        textPosition.y -= ctx->m_anchor.y;

        guiLabel(ctx, TEXT_LABEL + tag, Vector2{0}, textPosition);
    }

    void guiScrollPanelBegin(MxGuiContext* ctx, MxWidgetTag tag, Vector2 bounds, Vector2 anchor)
    {
        ScrollPanelComponent& scrollPanel = *ctx->getScrollPanelComponent(tag);
        scrollPanel.transform.bounds.x = bounds.x;
        scrollPanel.transform.bounds.y = bounds.y;
        scrollPanel.transform.anchor = anchor;


        beginTransformWorld(ctx, scrollPanel.transform);
        Rectangle rect = scrollPanel.transform.worldBounds;
        Rectangle rectCanvas = rect;

        DrawRectangleLinesEx(rect, 1, Fade(scrollPanel.color, 0.5f));
        ctx->m_anchor = Vector2{rect.x, rect.y};


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
            ctx->m_scrollTop = scrollPanel.scrollTop;

            beginTransformWorld(ctx, scrollPanel.transformCanvas);
            rectCanvas = scrollPanel.transformCanvas.worldBounds;

            scrollPanel.scrollTop = (rectCanvas.y + rectCanvas.height < rect.y + rect.height) ? previousScrollTop : scrollPanel.scrollTop;
            ctx->m_scrollTop = scrollPanel.scrollTop;

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

    void guiScrollPanelEnd(MxGuiContext* ctx, MxWidgetTag tag)
    {
        ScrollPanelComponent& scrollPanel = *ctx->getScrollPanelComponent(tag);
        ctx->m_anchor = Vector2{};
        ctx->m_scrollTop = 0.0f;
        DrawRectangleRec(scrollPanel.scrollBarThumb, RED);
        EndScissorMode();
    }




} // namespace mxgui
