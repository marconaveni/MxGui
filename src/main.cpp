


#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "raylib.h"
#include "mxgui.hpp"

// mxGui



int main(int argc, const char** argv)
{

    InitWindow(800, 600, "GUI");
    SetTextLineSpacing(0);


    MxGuiContext* ctx = mxgui::createContext();

    mxgui::createCanvas(ctx, "Canvas1");
    mxgui::createCanvas(ctx, "Canvas2");
    mxgui::createButton(ctx, "ButtonClick1");
    mxgui::createLabel(ctx, "Label1", "hello world");
    mxgui::createLabel(ctx, "Label2", "testando");
    mxgui::createScrollPanel(ctx, "ScrollPanel");


    MxTransform transform;
    transform.bounds.x = 10;
    transform.bounds.y = 10;
    transform.anchor.x = 100;
    transform.anchor.y = 100;


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mxgui::guiButton(ctx, "ButtonClick1", Vector2{250, 100}, Vector2{transform.bounds.x, transform.bounds.y});
        mxgui::guiScrollPanelBegin(ctx, "ScrollPanel", Vector2{300, 200}, Vector2{100, 100});

        mxgui::guiCanvas(ctx, "Canvas1", Vector2{transform.bounds.x, transform.bounds.y}, transform.anchor, true);
        transform = mxgui::getCurrentTransform(ctx);

        mxgui::guiCanvas(ctx, "Canvas2", Vector2{0, 0}, Vector2{transform.bounds.x, transform.bounds.y}, false);
        mxgui::guiLabel(ctx, "Label1", Vector2{100, 0}, Vector2{transform.bounds.x, transform.bounds.y});
        mxgui::guiLabel(ctx, "Label2", Vector2{0, 0});
        mxgui::guiScrollPanelEnd(ctx, "ScrollPanel");

        DrawFPS(10, 10);

        //DrawText(TextFormat("cor: %zu", sizeof(MxColor)), 30, 30, 20, BLACK);
        EndDrawing();
    }

    mxgui::destroyContext(ctx);
    CloseWindow();

    return 0;
}
