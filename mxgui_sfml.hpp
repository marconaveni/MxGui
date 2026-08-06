#ifndef MXGUI_SFML_HPP
#define MXGUI_SFML_HPP

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

struct MxTextureNative
{
    sf::Texture handle{};
    bool isValid{false};
};

static std::unique_ptr<sf::Text> s_text;
static std::unique_ptr<sf::Sprite> s_sprite;


struct MxMousePolling
{
    bool pressed{false};
    bool down{false};
    bool release{false};
};

static bool s_cursorOnScreen{false};

static sf::RectangleShape s_rectShape;
static sf::CircleShape s_circleShape;
static sf::RenderWindow* s_windowRef = nullptr;
static sf::Clock s_fpsClock{};

static MxVec2 s_mousePosition{};
static MxVec2 s_mouseDelta{};
static float s_mouseWheelScrolled{0.0f};
static MxMousePolling s_mousePolling[5]{
    MxMousePolling{},
    MxMousePolling{},
    MxMousePolling{},
};


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

inline MxVec2 toMxVec2(sf::Vector2u vec)
{
    return MxVec2{(float)vec.x, (float)vec.y};
}

void nativeInit()
{
    const MxTextureNative texture;
    s_sprite = std::make_unique<sf::Sprite>(texture.handle);
}

MxTextureNative nativeLoadTexture(const std::filesystem::path& path)
{
    MxTextureNative texture{};
    if (texture.handle.loadFromFile(path))
    {
        texture.isValid = true;
    }
    return texture;
}

MxTextureNative nativeLoadTextureFromImageData(void* data, int width, int height, int /*mipmaps*/, int format)
{

    MxTextureNative texture{};
    std::vector<unsigned char> rgba(width * height * 4);

    const unsigned char* src = (const unsigned char*)data;

    switch (format)
    {
        case 2:
            for (int i = 0; i < width * height; i++)
            {
                unsigned char gray = src[i * 2 + 0];
                unsigned char alpha = src[i * 2 + 1];
                rgba[i * 4 + 0] = gray;
                rgba[i * 4 + 1] = gray;
                rgba[i * 4 + 2] = gray;
                rgba[i * 4 + 3] = alpha;
            }
            break;

        case 1:
            for (int i = 0; i < width * height; i++)
            {
                unsigned char gray = src[i];
                rgba[i * 4 + 0] = gray;
                rgba[i * 4 + 1] = gray;
                rgba[i * 4 + 2] = gray;
                rgba[i * 4 + 3] = 255;
            }
            break;

        case 7: memcpy(rgba.data(), src, rgba.size()); break;
        default: MX_ASSERT(false, "loadTextureFromImageData: unsupported pixel format for SFML backend"); return texture;
    }

    sf::Image image({(unsigned int)width, (unsigned int)height}, rgba.data());

    if (texture.handle.loadFromImage(image))
    {
        texture.isValid = true;
    }

    return texture;
}

void nativeSetTextureSmooth(MxTextureNative* texture, bool enable)
{
    texture->handle.setSmooth(enable);
}

MxVec2 nativeTextureSize(const MxTextureNative* texture)
{
    return MxVec2{(float)texture->handle.getSize().x, (float)texture->handle.getSize().y};
}

bool nativeTextureIsSmooth(const MxTextureNative* texture)
{
    return texture->handle.isSmooth();
}

bool nativeIsValidTexture(const MxTextureNative* texture)
{
    return texture->isValid;
}

void nativeUnloadTexture(const MxTextureNative* /*texture*/)
{
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

    if (event->is<sf::Event::MouseEntered>())
    {
        s_cursorOnScreen = true;
    }

    if (event->is<sf::Event::MouseLeft>())
    {
        s_cursorOnScreen = false;
    }

    return event;
}

bool isCursorOnScreen()
{
    return s_cursorOnScreen;
}

MxVec2 windowSize()
{
    return toMxVec2(s_windowRef->getSize());
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

void drawTexturePro(const std::string& textureNameID, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint)
{

    if (!s_sprite)
    {
        MX_ASSERT(s_sprite, "s_sprite is not valid");
        return;
    }


    const MxTextureNative* texture = getTexture(textureNameID);

    if (!texture->isValid)
    {
        MX_ASSERT(texture->isValid, "texture is not valid");
        return;
    }


    const sf::IntRect rect({(int)source.x, (int)source.y}, {(int)source.width, (int)source.height});
    s_sprite->setTexture(texture->handle);
    s_sprite->setTextureRect(rect);

    // The scale factor must be derived manually from dest size / source size
    const float scaleX = (source.width != 0.0f) ? (dest.width / source.width) : 1.0f;
    const float scaleY = (source.height != 0.0f) ? (dest.height / source.height) : 1.0f;
    s_sprite->setScale({scaleX, scaleY});

    s_sprite->setPosition({dest.x, dest.y});
    s_sprite->setOrigin({origin.x, origin.y});
    s_sprite->setRotation(sf::degrees(rotation));
    s_sprite->setColor(toColor(tint));


    MX_ASSERT(s_windowRef, "window not reference");
    s_windowRef->draw(*s_sprite);
}


void drawCircle(MxVec2 center, float radius, MxColor color)
{
    s_circleShape.setRadius(radius);
    s_circleShape.setPosition(sf::Vector2f{center.x - radius, center.y - radius});
    s_circleShape.setFillColor(toColor(color));


    MX_ASSERT(s_windowRef, "window not reference");
    s_windowRef->draw(s_circleShape);
}


void drawFPS(float x, float y)
{
    static float fps = 0.0f;

    float deltaTime = s_fpsClock.restart().asSeconds();
    if (deltaTime > 0.0f)
    {
        fps = 1.0f / deltaTime;
    }

    std::string text = std::to_string((int)fps) + " FPS";
    MxColor color = fps > 30 ? MxColor::Green : MxColor::Yellow;

    const MxFont* font = getFont(MX_FONT_NOTO_ID, 20);
    drawTextEx(*font, text, MxVec2{x, y}, 20, 0, color);
}



#endif // MXGUI_SFML_HPP
