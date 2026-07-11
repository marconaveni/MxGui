#include "mxgui_render.hpp"

#include <vector>

#include "mxgui.hpp"
#include "raylib.h"


std::vector<Rectangle> g_stackScissors{};

Rectangle intersectionArea(const Rectangle& rect2)
{
    if (!g_stackScissors.empty())
    {
        return GetCollisionRec(g_stackScissors.back(), rect2);
    }

    return rect2;
}

void pushScissor(int x, int y, int width, int height)
{
    if (!g_stackScissors.empty())
    {
        EndScissorMode();
    }

    Rectangle rect{(float)x, (float)y, (float)width, (float)height};
    rect = intersectionArea(rect);

    g_stackScissors.push_back(rect);
    BeginScissorMode((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height);
}

void popScissor()
{
    EndScissorMode();

    if (!g_stackScissors.empty())
    {
        g_stackScissors.pop_back();
    }

    if (!g_stackScissors.empty())
    {
        Rectangle rect = g_stackScissors.back();
        BeginScissorMode((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height);
    }

}

