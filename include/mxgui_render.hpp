#ifndef MXGUI_RENDER_HPP
#define MXGUI_RENDER_HPP

#include <memory>
#include <unordered_map>

#include "mxgui.hpp"

void pushScissor(int x, int y, int width, int height);
void popScissor();


void initManagers();
void closeManagers();
void loadTexture(const std::filesystem::path& path, const std::string& name);
MxVec2 measureText(const std::string& name, const std::string& text);


void drawRectangleLinesEx(MxRect rec, float lineThick, MxColor color);
void drawRectanglePro(MxRect rec, MxVec2 origin, float rotation, MxColor color);
void drawTexturePro(const std::string& textureName, MxRect source, MxRect dest, MxVec2 origin, float rotation, MxColor tint);
void drawTextEx(const std::string& fontName, const std::string& text, MxVec2 position, float fontSize, float spacing, MxColor tint);




#endif // MXGUI_RENDER_HPP
