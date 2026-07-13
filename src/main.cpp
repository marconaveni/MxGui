

#include "mxgui.hpp"
#include "mxgui_render.hpp"
#include "raylib.h"

void testScissor();

int main()
{


    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "GUI");
    SetTextLineSpacing(0);
    // SetTargetFPS(60);

    MxGuiContext* ctx = mxgui::createContext();

    mxgui::createCanvas(ctx, "Canvas1");
    mxgui::createCanvas(ctx, "Canvas2");
    mxgui::createButton(ctx, "ButtonClick1");
    mxgui::createLabel(ctx, "Label1");
    mxgui::createLabel(ctx, "Label2");
    mxgui::createScrollPanel(ctx, "ScrollPanel");
    mxgui::createImage(ctx, "Image", "/home/marco/Imagens/icons/nfsu2.png", "nfsu2");


    MxTransform transform;
    transform.bounds.x = 10;
    transform.bounds.y = 10;
    transform.bounds.width = 400;
    transform.bounds.height = 25;
    transform.anchor.x = 0;
    transform.anchor.y = 0;


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        
        mxgui::guiImage(ctx, "Image", "nfsu2", MxRect{50, 50, 100, 100});
        
        mxgui::guiCanvas(ctx, "Canvas1", transform.bounds, transform.anchor, true);
        transform = mxgui::getCurrentTransform(ctx);
        mxgui::guiCanvas(ctx, "Canvas2", MxRect{0, (transform.bounds.height - 1), 400, 400}, MxRectToMxVec2(transform.bounds), false);
        if (mxgui::guiButton(ctx, "ButtonClick1", "Click", MxRect{10, 35, 75, 35}, MxRectToMxVec2(transform.bounds), MxOutLine, true ))
        {
            TraceLog(LOG_INFO, "clicked");
        }
        
        mxgui::guiScrollPanelBegin(ctx, "ScrollPanel", MxRect{300, 200, 100, 200}, MxRect{300, 200, 100, 250}, MxVec2{100, 100}, true);
        mxgui::guiLabel(ctx, "Label1", "hello world", MxVec2{100, 0}, MxRectToMxVec2(transform.bounds));
        mxgui::guiLabel(ctx, "Label2", "hello world 2", MxVec2{0, 0});
        mxgui::guiScrollPanelEnd(ctx, "ScrollPanel");

        DrawFPS(10, 10);

        // DrawText(TextFormat("cor: %zu", sizeof(MxColor)), 30, 30, 20, BLACK);
        EndDrawing();
    }

    mxgui::destroyContext(ctx);
    CloseWindow();

    return 0;
}


void testScissor()
{
    InitWindow(800, 450, "Raylib - Scissor Stack");
    Rectangle rect = {10, 10, 400, 300};
    Rectangle root = {};
    Rectangle child = {250, 100, 250, 200};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 mousepos = GetMousePosition();
        root.x = mousepos.x - 300 / 2;
        root.y = mousepos.y - 300 / 2;
        root.width = 300;
        root.height = 300;

        // root scissor
        pushScissor(root.x, root.y, root.width, root.height);
        DrawRectangleRec(rect, RED);

        // child scissor
        pushScissor(250, 100, 250, 200);
        DrawRectangleRec(child, GRAY);
        DrawText("Text in scissor child!", 240, 150, 20, WHITE);

        // end child scissor
        popScissor();

        DrawText("Text in scissor root!", 240, 170, 20, WHITE); // draw in root scissor

        // end root scissor
        popScissor();

        DrawRectangleLines(root.x, root.y, root.width, root.height, BLACK);
        EndDrawing();
    }
    CloseWindow();
}
