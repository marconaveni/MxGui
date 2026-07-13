#include "mxgui_render.hpp"

#include <vector>

#include "mxgui_notosans.hpp"
#include "raylib.h"

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
        if (m_fonts.size() > 0)
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


class TextureManager
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


static FontManager s_fontManager;
static TextureManager s_textureManager;
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
}

void closeManagers()
{
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

void drawTextEx(const std::string& fontName, const std::string& text, MxVec2 position, float fontSize, float spacing, MxColor tint)
{
}
