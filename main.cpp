

#include <array>
#define MX_GUI_IMPLEMENTATION
#include "mxgui.hpp"
#include "raylib.h"






void testScissor();


int main()
{

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

    InitWindow(800, 600, "GUI");
    SetTextLineSpacing(0);
    // SetTargetFPS(60);

    MxGuiContext* ctx = mxgui::createContext(MxStyle::Light);

    mxgui::createImage("/home/marco/Downloads/icones/playsolid.png", "nfsu2");

    const float width = 400;
    const float height = 25;
    MxVec2 anchor{10, 10};


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground((mxgui::getStyle(ctx).isDarkMode) ? BLACK : RAYWHITE);


        anchor = mxgui::guiPanel(ctx, "Canvas1", toMxRect(anchor, MxVec2{width, height}), MxVec2{}, true);
        mxgui::guiPanel(ctx, "Canvas2", MxRect{0, (height - 1), width, width}, anchor, false);
        if (mxgui::guiButton(ctx, "Click", MxRect{10, 35, 75, 35}, anchor, MX_OUTLINE, true))
        {
            TraceLog(LOG_INFO, "clicked");
        }
        mxgui::guiImage(ctx, "nfsu2", MxRect{12, 35, 100, 100}, anchor);
        mxgui::guiIcon(ctx, MxRect{10, 80, 0, 0}, anchor, ICON_FA_CIRCLE_PLAY);

        mxgui::guiScrollPanelBegin(ctx, "ScrollPanel", MxRect{300, 200, 100, 200}, MxRect{300, 200, 100, 250}, MxVec2{100, 100}, true);
        mxgui::guiLabel(ctx, "hello world", MxVec2{100, 0}, anchor);
        mxgui::guiLabel(ctx, "hello world 2", MxVec2{0, 80});
        mxgui::guiScrollPanelEnd(ctx, "ScrollPanel");


        const float progress = mxgui::guiSlider(ctx, "Slider", MxRect{50, 530, 700, 6}, MxVec2{}, true);
        mxgui::guiLabel(ctx, std::to_string(progress), MxVec2{10, 30});
        mxgui::guiSliderProgress(ctx, MxRect{50, 560, 700, 6}, MxVec2{}, 0.8f);
        
        
        DrawFPS(10, 10);
        

        // DrawText(TextFormat("cor: %zu", sizeof(Transform)), 30, 30, 20, BLACK);
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
