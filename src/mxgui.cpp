#include "mxgui.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "mxgui_notosans.hpp"
#include "mxgui_render.hpp"
#include "raylib.h"

// mxGui

void updateTransformWorld(MxGuiContext* ctx, MxTransform& transform, MxRect bounds, MxVec2 anchor);


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

inline MxRect toMxRect(Rectangle rec)
{
    return MxRect{rec.x, rec.y, rec.width, rec.height};
}

inline MxColor toMxColor(Color color)
{
    return MxColor{color.r, color.g, color.b, color.a};
}


// struct FontSpecs
// {
//     Font font{};
//     int size{20};
// };


// class FontManager
// {
// public:


//     void init()
//     {
//         if (m_fonts.size() > 0)
//         {
//             return;
//         }

//         m_fonts[MX_DEFAULT_FONT] = FontSpecs{// Default font
//                                              .font = LoadFontFromMemory(".ttf", notosans::data, notosans::size, 20, NULL, 255),
//                                              .size = 20};
//     }

//     void unload()
//     {
//         for (auto& [id, fontSpec] : m_fonts)
//         {
//             UnloadFont(fontSpec.font);
//         }
//         m_fonts.clear();
//     }

//     // FontSpecs getFont(std::string name)
//     // {
//     //     auto it = m_fonts.find(name);
//     //     if (it != m_fonts.end())
//     //     {
//     //         return it->second;
//     //     }

//     //     return FontSpecs{};
//     // }


// private:

//     std::unordered_map<std::string, FontSpecs> m_fonts{};
// };


class MxTextureManager
{
public:


    void init()
    {
        if (m_textures.size() > 0)
        {
            return;
        }
        // Loading defaults icons
        // todo: implement
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

    void unload()
    {
        for (auto& [id, texture] : m_textures)
        {
            UnloadTexture(texture);
        }
        m_textures.clear();
    }

    Texture getTexture(std::string name)
    {
        auto it = m_textures.find(name);
        if (it != m_textures.end())
        {
            return it->second;
        }

        return Texture{};
    }


private:

    std::unordered_map<std::string, Texture> m_textures{};
};


//-----------------------------------------------------------------------------
// macros getters and setters to MxGuiContext
//-----------------------------------------------------------------------------

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

    void init(MxStyle style = MxStyle{})
    {
        // m_fontManager.init();
        m_textureManager.init();
        m_style = style;
        initManagers();
    }
    void close()
    {
        // m_fontManager.unload();
        m_textureManager.unload();
        closeManagers();
    }

    // FontManager& getFontManager() { return m_fontManager; }
    MxTextureManager& getTextureManager() { return m_textureManager; }

    COMPONENT(m_canvas, CanvasComponent);
    COMPONENT(m_labels, LabelComponent);
    COMPONENT(m_buttons, ButtonComponent);
    COMPONENT(m_scrollPanels, ScrollPanelComponent);
    COMPONENT(m_imageComponents, ImageComponent);

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

    std::unordered_map<MxWidgetTag, CanvasComponent> m_canvas;
    std::unordered_map<MxWidgetTag, LabelComponent> m_labels;
    std::unordered_map<MxWidgetTag, ButtonComponent> m_buttons;
    std::unordered_map<MxWidgetTag, ScrollPanelComponent> m_scrollPanels;
    std::unordered_map<MxWidgetTag, ImageComponent> m_imageComponents;

    //-----------------------------------------------------------------------------
    // Managers
    //-----------------------------------------------------------------------------

    // FontManager m_fontManager;
    MxTextureManager m_textureManager;

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

static std::unique_ptr<MxGuiContext> g_context{nullptr};


void updateTransformWorld(MxGuiContext* ctx, MxTransform& transform, MxRect bounds, MxVec2 anchor)
{
    transform.bounds = bounds;
    transform.anchor = anchor;

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
        ctx->close();
        g_context.reset();
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

    void createImage(MxGuiContext* ctx, MxWidgetTag tagName, const std::filesystem::path& path, const std::string& imageName)
    {
        ImageComponent imageComponent;
        loadTexture(path, imageName);
        ctx->insertImageComponent(tagName, imageComponent);
    }

#define TEXT_LABEL "button"

    void createButton(MxGuiContext* ctx, MxWidgetTag tagName)
    {
        ButtonComponent button;
        button.transform.bounds = MxRect{0, 0, 80, 40};
        createLabel(ctx, TEXT_LABEL + tagName);
        ctx->insertButtonComponent(tagName, button);
    }

    void createLabel(MxGuiContext* ctx, MxWidgetTag tagName)
    {
        LabelComponent label;
        ctx->insertLabelComponent(tagName, label);
        //setTextValue(ctx, tagName, newText);
    }

    void createScrollPanel(MxGuiContext* ctx, MxWidgetTag tagName)
    {
        ScrollPanelComponent panel;
        panel.transform.bounds = MxRect{0, 0, 100, 200};
        panel.transformCanvas.bounds = MxRect{0, 0, panel.transform.bounds.width, panel.transform.bounds.height + 50};
        ctx->insertScrollPanelComponent(tagName, panel);
    }


    void guiCanvas(MxGuiContext* ctx, MxWidgetTag tag, MxRect bounds, MxVec2 anchor, bool enableDrag)
    {

        CanvasComponent& canvas = *ctx->getCanvasComponent(tag);

        updateTransformWorld(ctx, canvas.transform, bounds, anchor);
        MxRect rect = canvas.transform.worldBounds;

        MxMouseEvents mouseEvents{};

        if (enableDrag)
        {
            mouseEvents.isMouseHover = (CheckCollisionPointRec(GetMousePosition(), toRectangle(rect)));
            mouseEvents.isMouseRelease = mouseEvents.isMouseHover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
            mouseEvents.isMouseDown = mouseEvents.isMouseHover && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
            mouseEvents.isMousePressed = mouseEvents.isMouseHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if (mouseEvents.isMousePressed)
            {
                Vector2 mousePosition = GetMousePosition();
                canvas.Offset.x = canvas.transform.anchor.x + mousePosition.x - rect.x;
                canvas.Offset.y = canvas.transform.anchor.y + mousePosition.y - rect.y;
                canvas.isDrag = true;
            }
            if (canvas.isDrag && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                Vector2 mousePosition = GetMousePosition();
                canvas.transform.bounds.x = mousePosition.x - canvas.Offset.x - ctx->m_anchor.x;
                canvas.transform.bounds.y = mousePosition.y - canvas.Offset.y - ctx->m_anchor.y + ctx->m_scrollTop;
            }
            else
            {
                canvas.isDrag = false;
            }
        }

        canvas.color = ctx->m_style.backgroundColor;
        DrawRectanglePro(toRectangle(rect), Vector2{}, 0, Fade(toColor(canvas.color), 1.0f));
        DrawRectangleLinesEx(toRectangle(rect), ctx->m_style.borderWidth, toColor(ctx->m_style.borderColor));
        ctx->updateCurrents(canvas.transform, mouseEvents);
    }

    void guiImage(MxGuiContext* ctx, MxWidgetTag tag, const std::string& imageName, MxRect bounds, MxVec2 anchor)
    {
        ImageComponent& imageComponent = *ctx->getImageComponent(tag);
        updateTransformWorld(ctx, imageComponent.transform, bounds, anchor);
        MxRect rect = imageComponent.transform.worldBounds;

        const Texture texture = ctx->getTextureManager().getTexture(imageName);
        if (IsTextureValid(texture))
        {
            const MxRect source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
            const MxRect dest = {rect.x, rect.y, (float)texture.width, (float)texture.height};
            drawTexturePro(imageName, source, dest, MxVec2{0, 0}, 0, imageComponent.color);
        }
        // const Texture texture = ctx->getTextureManager().getTexture(imageName);
        // if (IsTextureValid(texture))
        // {
        //     Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        //     Rectangle dest = {rect.x, rect.y, (float)texture.width, (float)texture.height};
        //     DrawTexturePro(texture, source, dest, Vector2{0, 0}, 0, toColor(imageComponent.color));
        // }

        ctx->updateCurrents(imageComponent.transform, MxMouseEvents{});
    }

    void guiLabel(MxGuiContext* ctx, MxWidgetTag tag, const std::string& text, MxVec2 bounds, MxVec2 anchor)
    {

        LabelComponent& label = *ctx->getLabelComponent(tag);
        updateTransformWorld(ctx, label.transform, MxVec2ToMxRect(bounds), anchor);
        MxRect rect = label.transform.worldBounds;


        // FontSpecs font = ctx->getFontManager().getFont(label.fontName);
        // if (!IsFontValid(font.font))
        // {
        //     font.font = GetFontDefault();
        // }

        // DrawTextEx(font.font, label.text.value.c_str(), Vector2{rect.x, rect.y}, 20, 0, toColor(label.color));
        drawTextEx(label.fontName, text, MxVec2{rect.x, rect.y}, 20, 0, label.color);
        ctx->updateCurrents(label.transform, MxMouseEvents{});
    }


    bool guiButton(MxGuiContext* ctx, MxWidgetTag tag, const std::string& text, MxRect bounds, MxVec2 anchor, ButtonStyle buttonStyle, bool isEnable)
    {
        ButtonComponent& button = *ctx->getButtonComponent(tag);
        updateTransformWorld(ctx, button.transform, bounds, anchor);

        MxRect rect = button.transform.worldBounds;
        MxMouseEvents mouseEvents{};

        int paint = 0;

        if (isEnable)
        {
            mouseEvents.isMouseHover = (CheckCollisionPointRec(GetMousePosition(), toRectangle(rect)));
            mouseEvents.isMouseRelease = mouseEvents.isMouseHover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
            mouseEvents.isMouseDown = mouseEvents.isMouseHover && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
            mouseEvents.isMousePressed = mouseEvents.isMouseHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

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
        color.r = std::clamp(color.r - paint, 0, 255);
        color.g = std::clamp(color.g - paint, 0, 255);
        color.b = std::clamp(color.b - paint, 0, 255);

        if (buttonStyle == ButtonStyle::MxContained)
        {
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, toColor(color));
        }
        else if (buttonStyle == ButtonStyle::MxOutLine)
        {
            DrawRectangleLinesEx(Rectangle{rect.x, rect.y, rect.width, rect.height}, 1, toColor(color));
            DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(toColor(color), 0.3f));
        }

        LabelComponent& label = *ctx->getLabelComponent(TEXT_LABEL + tag);
        MxVec2 textSize = measureText(label.fontName, text);
        MxVec2 textPosition = MxVec2{rect.x + (rect.width - textSize.x) / 2, rect.y + (rect.height - textSize.y) / 2};
        textPosition.x -= ctx->m_anchor.x;
        textPosition.y -= ctx->m_anchor.y;

        guiLabel(ctx, TEXT_LABEL + tag, text, MxVec2{0}, textPosition);

        ctx->updateCurrents(button.transform, mouseEvents);

        return mouseEvents.isMousePressed;
    }

    void guiScrollPanelBegin(MxGuiContext* ctx, MxWidgetTag tag, MxRect bounds, MxRect scrollBounds, MxVec2 anchor, bool isEnable)
    {
        ScrollPanelComponent& scrollPanel = *ctx->getScrollPanelComponent(tag);
        updateTransformWorld(ctx, scrollPanel.transform, bounds, anchor);
        MxRect rect = scrollPanel.transform.worldBounds;
        MxRect rectCanvas = rect;

        ctx->m_anchor = MxVec2{rect.x, rect.y};

        scrollPanel.transformCanvas.bounds = scrollBounds;

        if (scrollPanel.transformCanvas.bounds.height > rect.height && isEnable)
        {
            const float previousScrollTop = scrollPanel.scrollTop;

            if (CheckCollisionPointRec(GetMousePosition(), toRectangle(scrollPanel.scrollBarThumb)) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                scrollPanel.isDrag = true;
            }

            if (scrollPanel.isDrag && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                scrollPanel.scrollTop += GetMouseDelta().y;
            }
            else if (CheckCollisionPointRec(GetMousePosition(), toRectangle(rect)))
            {
                scrollPanel.isDrag = false;
                scrollPanel.scrollTop -= GetMouseWheelMove() * 10;
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
            updateTransformWorld(ctx, scrollPanel.transformCanvas, scrollPanel.transformCanvas.bounds, MxVec2{0, 0});
            rectCanvas = scrollPanel.transformCanvas.worldBounds;

            scrollPanel.scrollTop = (rectCanvas.y + rectCanvas.height < rect.y + rect.height) ? previousScrollTop : scrollPanel.scrollTop;
            ctx->m_scrollTop = scrollPanel.scrollTop;

            float visibleProportion = rect.height / rectCanvas.height;
            float progress = scrollPanel.scrollTop / (rectCanvas.height - rect.height);

            scrollPanel.scrollBarThumb = MxRect{
                .x = rect.x + rect.width - 5 - MX_DRAG_OFFSET,
                .y = rect.y + (rect.height - rect.height * visibleProportion) * progress,
                .width = 5 + MX_DRAG_OFFSET * 2,
                .height = rect.height * visibleProportion,
            };
        }

        DrawRectangleLinesEx(toRectangle(rect), ctx->m_style.borderWidth, toColor(ctx->m_style.borderColor));

        pushScissor(rect.x, rect.y, rect.width, rect.height); // call internal BeginScissorMode();

        DrawRectanglePro(toRectangle(rectCanvas), Vector2{}, 0, Fade(GRAY, 0.5f)); // debug visual feedback
        ctx->updateCurrents(scrollPanel.transform, MxMouseEvents{});
    }

    void guiScrollPanelEnd(MxGuiContext* ctx, MxWidgetTag tag)
    {

        popScissor(); // call internal EndScissorMode();

        ScrollPanelComponent& scrollPanel = *ctx->getScrollPanelComponent(tag);
        ctx->m_anchor = MxVec2{};
        ctx->m_scrollTop = 0.0f;
        MxRect rect = scrollPanel.scrollBarThumb;
        rect.x += MX_DRAG_OFFSET;
        rect.width -= MX_DRAG_OFFSET * 2;
        // DrawRectangleRec(toRectangle(scrollPanel.scrollBarThumb), BLUE); // debug offset
        DrawRectangleRec(toRectangle(rect), toColor(ctx->m_style.borderColor));
    }


} // namespace mxgui
