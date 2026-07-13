#include "mxgui_render.hpp"

#include <string>
#include <vector>

#include "mxgui_notosans.hpp"
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


    void init()
    {
        if (m_fonts.size() > 0)
        {
            return;
        }

        // Default font
        m_fonts[MX_DEFAULT_FONT] = MxFontSpecsInternal{
            .font = LoadFontFromMemory(".ttf", notosans::data, notosans::size, 20, NULL, 255),
            .size = 20,
            .spacing = 0,
        };
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

void initManagers()
{
    s_fontManager.init();
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

void drawTextEx(const std::string& fontName, const std::string& text, MxVec2 position, float fontSize, float spacing, MxColor tint)
{

    MxFontSpecsInternal font = s_fontManager.getFont(fontName);

    if (!IsFontValid(font.font))
    {
        font.font = GetFontDefault();
    }

    DrawTextEx(font.font, text.c_str(), toVector(position), fontSize, spacing, toColor(tint));
}
