#ifndef MXGUI_RAYLIB_HPP
#define MXGUI_RAYLIB_HPP

#include <cstdio>

#include "raylib.h"

struct MxTextureNative
{
    Texture handle{};
    bool isSmooth{false};
};

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

void nativeInit()
{
}

MxTextureNative nativeLoadTexture(const std::filesystem::path& path)
{
    return MxTextureNative{.handle = LoadTexture(path.string().c_str())};
}

MxTextureNative nativeLoadTextureFromImageData(void* data, int width, int height, int mipmaps, int format)
{
    Image image{
        .data = data,
        .width = width,
        .height = height,
        .mipmaps = mipmaps,
        .format = format,
    };
    return MxTextureNative{.handle = LoadTextureFromImage(image)};
}

void nativeSetTextureSmooth(MxTextureNative* texture, bool enable)
{
    const int filter = enable ? TEXTURE_FILTER_BILINEAR : TEXTURE_FILTER_POINT;
    texture->isSmooth = enable;
    SetTextureFilter(texture->handle, filter);
}

MxVec2 nativeTextureSize(const MxTextureNative* texture)
{
    const float width = (float)texture->handle.width;
    const float height = (float)texture->handle.height;
    return MxVec2{width, height};
}

bool nativeTextureIsSmooth(const MxTextureNative* texture)
{
    return texture->isSmooth;
}

bool nativeIsValidTexture(const MxTextureNative* texture)
{
    return IsTextureValid(texture->handle);
}

void nativeUnloadTexture(const MxTextureNative* texture)
{
    UnloadTexture(texture->handle);
}

bool isCursorOnScreen()
{
    const bool isValid = (GetMousePosition().x > 0 && GetMousePosition().y > 0 && GetMousePosition().x < GetScreenWidth() && // check cursor is outscreen
                          GetMousePosition().y < GetScreenHeight()) &&
                         IsCursorOnScreen();
    return isValid;
}

MxVec2 windowSize()
{
    return MxVec2{(float)GetScreenWidth(), (float)GetScreenHeight()};
}

void setClipboardText(const std::string& text)
{
    SetClipboardText(text.c_str());
}

std::string getClipboardText()
{
    return GetClipboardText();
}

void beginScissorMode(int x, int y, int width, int height)
{
    BeginScissorMode(x, y, width, height);
}

void endScissorMode()
{
    EndScissorMode();
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

bool isKeyPressed(int key)
{
    return IsKeyPressed(key);
}

bool isKeyDown(int key)
{
    return IsKeyDown(key);
}

bool isKeyReleased(int key)
{
    return IsKeyReleased(key);
}

int getCharPressed()
{
    return GetCharPressed();
}

void drawRectangleLinesEx(MxRect rec, float lineThick, MxColor color)
{
    DrawRectangleLinesEx(toRectangle(rec), lineThick, toColor(color));
}

void drawRectanglePro(MxRect rec, MxVec2 origin, float rotation, MxColor color)
{
    DrawRectanglePro(toRectangle(rec), toVector(origin), rotation, toColor(color));
}

void drawTexturePro(const MxNameID& textureNameID, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint)
{
    const MxTextureNative* texture = getTexture(textureNameID);
    if (!IsTextureValid(texture->handle))
    {
        DrawText(textureNameID.c_str(), (int)dest.x, (int)dest.y, 10, BLACK);
        return;
    }
    DrawTexturePro(texture->handle, toRectangle(source), toRectangle(dest), toVector(origin), rotation, toColor(tint));
}

void drawCircle(MxVec2 center, float radius, MxColor color)
{
    DrawCircleV(toVector(center), radius, toColor(color));
}


#endif // MXGUI_RAYLIB_HPP
