#ifndef MXGUI_RENDER_HPP
#define MXGUI_RENDER_HPP


#include <filesystem>

#include "mxgui.hpp"

void pushScissor(int x, int y, int width, int height);
void popScissor();


void initManagers();
void closeManagers();
void loadTexture(const std::filesystem::path& path, const std::string& name);
MxVec2 getTextureSize(const std::string& textureName);
MxVec2 measureText(const std::string& name, const std::string& text);

MxVec2 getMousePosition();
MxVec2 getMouseDelta();
float getMouseWheelMove();
bool isMouseButtonPressed(int button);
bool isMouseButtonDown(int button);
bool isMouseButtonReleased(int button);


void drawRectangleLinesEx(MxRect rec, float lineThick, MxColor color);
void drawRectanglePro(MxRect rec, MxVec2 origin, float rotation, MxColor color);
void drawTexturePro(const std::string& textureName, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint);
void drawTextPro(const std::string& fontName, const std::string& text, MxVec2 position, MxVec2 origin, float rotation, float fontSize, float spacing, MxColor tint);
void drawCircle(MxVec2 center, float radius, MxColor color);   


#endif // MXGUI_RENDER_HPP
