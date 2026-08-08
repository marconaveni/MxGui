#ifndef MXGUI_CUSTOM_RENDER_HPP
#define MXGUI_CUSTOM_RENDER_HPP


void nativeInit()
{
}

MxTextureNative nativeLoadTexture(const std::filesystem::path& path)
{
    return {};
}

MxTextureNative nativeLoadTextureFromImageData(void* data, int width, int height, int /*mipmaps*/, int format)
{
    return {};
}

void nativeSetTextureSmooth(MxTextureNative* texture, bool enable)
{
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

bool isCursorOnScreen()
{
    return false;
}

MxVec2 windowSize()
{
    return MxVec2{};
}

void setClipboardText(const std::string& text)
{
}

std::string getClipboardText()
{
    return std::string();
}

void beginScissorMode(int x, int y, int width, int height)
{
}

void endScissorMode()
{
}

MxVec2 getMousePosition()
{
    return {};
}

MxVec2 getMouseDelta()
{
    return {};
}

float getMouseWheelMove()
{
    return 0;
}

bool isMouseButtonPressed(int button)
{
    return false;
}

bool isMouseButtonDown(int button)
{
    return false;
}

bool isMouseButtonReleased(int button)
{
    return false;
}

bool isKeyPressed(int key)
{
    return false;
}

bool isKeyDown(int key)
{
    return false;
}

bool isKeyReleased(int key)
{
    return false;
}

int getCharPressed()
{
    return 0;
}

void drawRectangleLinesEx(MxRect rec, float lineThick, MxColor color)
{
}

void drawRectanglePro(MxRect rec, MxVec2 origin, float rotation, MxColor color)
{
}

void drawTexturePro(const std::string& textureNameID, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint)
{
}


void drawCircle(MxVec2 center, float radius, MxColor color)
{
}


void drawFPS(float x, float y)
{
}

#endif // MXGUI_CUSTOM_RENDER_HPP