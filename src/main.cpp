

#include "mxgui.hpp"
#include "raylib.h"

// mxGui


int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "GUI");
    SetTextLineSpacing(0);
    //SetTargetFPS(60);

    MxGuiContext* ctx = mxgui::createContext();

    mxgui::createCanvas(ctx, "Canvas1");
    mxgui::createCanvas(ctx, "Canvas2");
    mxgui::createButton(ctx, "ButtonClick1");
    mxgui::createLabel(ctx, "Label1", "hello world");
    mxgui::createLabel(ctx, "Label2", "testando");
    mxgui::createScrollPanel(ctx, "ScrollPanel");
    mxgui::createImage(ctx, "Image", "/home/marco/Imagens/icons/nfsu2.png", "nfsu2");


    MxTransform transform;
    transform.bounds.x = 10;
    transform.bounds.y = 10;
    transform.anchor.x = 100;
    transform.anchor.y = 100;


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mxgui::guiImage(ctx, "Image", "nfsu2", MxVec2{50, 50});
        if (mxgui::guiButton(ctx, "ButtonClick1", MxVec2{250, 100}, MxRectToMxVec2(transform.bounds)))
        {
            TraceLog(LOG_INFO, "clicked");
        }

        mxgui::guiScrollPanelBegin(ctx, "ScrollPanel", MxVec2{300, 200}, MxVec2{100, 100}, true);

        mxgui::guiCanvas(ctx, "Canvas1", MxRectToMxVec2(transform.bounds), transform.anchor, true);
        transform = mxgui::getCurrentTransform(ctx);

        mxgui::guiCanvas(ctx, "Canvas2", MxVec2{0, 0}, MxRectToMxVec2(transform.bounds), false);
        mxgui::guiLabel(ctx, "Label1", MxVec2{100, 0}, MxRectToMxVec2(transform.bounds));
        mxgui::guiLabel(ctx, "Label2", MxVec2{0, 0});
        mxgui::guiScrollPanelEnd(ctx, "ScrollPanel");

        DrawFPS(10, 10);

        // DrawText(TextFormat("cor: %zu", sizeof(MxColor)), 30, 30, 20, BLACK);
        EndDrawing();
    }

    mxgui::destroyContext(ctx);
    CloseWindow();

    return 0;
}
