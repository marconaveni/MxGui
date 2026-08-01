


#include <array>
#include <iostream>

#define MX_RAYLIB_BACKEND_IMPLEMENTATION
// #define MX_SFML_BACKEND_IMPLEMENTATION
// #define MX_GUI_IMPLEMENTATION
#include "mxgui.hpp"


void testScissor();


int main()
{


#if MX_RAYLIB
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "GUI");
    SetTextLineSpacing(0);
    //SetTargetFPS(60);

#elif MX_SFML
    sf::RenderWindow window(sf::VideoMode({800, 600}), "GUI");
    s_windowRef = &window;
#endif

    loadFont("teste", "/home/marco/Diversos/Inter,Noto_Sans/Inter/static/Inter_28pt-Regular.ttf", 20, NULL, 0);
    MxStyle style = MxStyle::Light;
    style.iconSize = 28;
    MxGuiContext* ctx = mxgui::createContext(style);

    mxgui::createImage("/home/marco/Imagens/icons/nfsu2.png", "nfsu2");

    const float width = 400;
    const float height = 25;
    MxVec2 anchor{10, 10};
    bool checked = false;
    bool toogle = false;

#if MX_RAYLIB
    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground((mxgui::getStyle(ctx).isDarkMode) ? BLACK : RAYWHITE);
#elif MX_SFML
    while (window.isOpen())
    {
        while (const std::optional event = windowPollEvent(&window))
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        window.clear(sf::Color::White);
#endif


        anchor = mxgui::guiPanel(ctx, "Canvas1", toMxRect(anchor, MxVec2{width, height}), MxVec2{}, true);
        mxgui::guiPanel(ctx, "Canvas2", MxRect{0, (height - 1), width, width}, anchor, false);
        if (mxgui::guiButton(ctx, "Click", MxRect{10, 35, 75, 35}, anchor, MX_OUTLINE, true))
        {
            std::cout << "clicked" << '\n';
            setSmoothTexture(MX_FONT_NOTO_ID, !isSmoothTexture(MX_FONT_NOTO_ID));
            setSmoothTexture(MX_FONT_AWESOME_ID, !isSmoothTexture(MX_FONT_AWESOME_ID));
        }
        mxgui::guiImage(ctx, "nfsu2", MxRect{220, 35, 180, 180}, anchor);
        mxgui::guiIcon(ctx, MxRect{10, 80, 0, 0}, anchor, ICON_FA_CIRCLE_PLAY);
        if (mxgui::guiIconButton(ctx, MxRect{10, 120, 20, 20}, anchor, ICON_FA_CIRCLE_PLAY, 28))
        {
            std::cout << "clicked Icon" << '\n';
        }

        mxgui::guiScrollPanelBegin(ctx, "ScrollPanel", MxRect{300, 200, 100, 200}, MxRect{300, 200, 100, 550}, MxVec2{100, 100}, true);
        mxgui::guiLabel(ctx, "hello world", MxVec2{100, 0}, anchor);
        mxgui::guiLabel(ctx, "hello world 2", MxVec2{0, 80});
        mxgui::guiScrollPanelEnd(ctx, "ScrollPanel");


        mxgui::guiCheckBox(ctx, MxRect{100, 100}, MxVec2{}, checked);
        mxgui::guiToogle(ctx, MxRect{150, 100}, MxVec2{}, toogle);


        const float progress = mxgui::guiSlider(ctx, "Slider", MxRect{50, 530, 700, 6}, MxVec2{}, true);
        mxgui::guiLabel(ctx, std::to_string(progress), MxVec2{10, 30});
        mxgui::guiSliderProgress(ctx, MxRect{50, 560, 700, 6}, MxVec2{}, 0.8f);


        const MxFont* font = getFont(MX_FONT_NOTO_ID);
        const MxFont* font2 = getFont("teste");
        MxColor color = {200, 41, 55, 255};

        drawTextEx(*font, "teste \nteste quebra linha", MxVec2{60, 60}, 20, 0, color);
        drawTextEx(*font2, "teste \nteste quebra linha", MxVec2{60, 160}, 20, 0, color);

#if MX_RAYLIB

        DrawFPS(10, 10);
        // DrawText(TextFormat("cor: %zu", sizeof(Transform)), 30, 30, 20, BLACK);
        EndDrawing();
#elif MX_SFML
        drawFPS(10, 10);
        windowDisplay(&window);
#endif
    }


    mxgui::destroyContext(ctx);

#if MX_RAYLIB
    CloseWindow();
#endif

    return 0;
}

#if MX_RAYLIB


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

#endif
