#include "mxgui.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

#include "mxgui_notosans.hpp"
#include "mxgui_render.hpp"

// mxGui

MxTransform updateTransformWorld(MxGuiContext* ctx, MxRect bounds, MxVec2 anchor);

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
        initManagers();
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

        drawTextPro(ctx->m_style.fontName, text, MxVec2{rect.x, rect.y}, MxVec2{}, 0, 20, 0, ctx->m_style.textColor);
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

        MxTransform transform = updateTransformWorld(ctx, bounds, anchor);
        MxTransform transformBar = transform;

        MxMouseEvents mouseEvents{};

        if (enable)
        {
            mouseEvents.isMouseHover = (checkCollisionPointRect(getMousePosition(), transform.worldBounds));
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

        // transform = updateTransformWorld(ctx, transform.bounds, anchor);
        ctx->updateCurrents(transform, mouseEvents);

        drawRectanglePro(transform.worldBounds, MxVec2{}, 0, fadeColor(color, 1.0f));
        drawRectanglePro(transformBar.worldBounds, MxVec2{}, 0, fadeColor(MxColor::Red, 1.0f));
        
        return slider.progress;
    }


} // namespace mxgui
