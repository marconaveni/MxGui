
#define MX_LOG(...)                                             \
    printf("FILE: [%s] LINE: [%d] (", __FILE_NAME__, __LINE__); \
    printf(__VA_ARGS__);                                        \
    printf(")\n");


#define MX_SUPPRESS_WARNINGS 0


// #define MX_CUSTOM_BACKEND_HEADER "mxgui_custom_render.hpp"
#define MX_RAYLIB_BACKEND_IMPLEMENTATION
// #define MX_SFML_BACKEND_IMPLEMENTATION
#define MX_GUI_IMPLEMENTATION
#include "mxgui.hpp"


// #define MX_SFML 1


// includes temp for fast tests

// #include "temp/include_test.h"
// #include "temp/scissor_test.h"
// #include "temp/text_block_prototype.h"


int main()
{

//init_text_block();

#if MX_RAYLIB

    // init_test_scissor();

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "GUI");
    SetTextLineSpacing(0);
     // SetTargetFPS(60);

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
            MX_LOG("Clicked");
            setSmoothTexture(MX_FONT_NOTO_ID, !isSmoothTexture(MX_FONT_NOTO_ID));
            setSmoothTexture(MX_FONT_AWESOME_ID, !isSmoothTexture(MX_FONT_AWESOME_ID));
        }
        mxgui::guiImage(ctx, "nfsu2", MxRect{220, 35, 180, 180}, anchor);
#if MX_FONT_AWESOME
        mxgui::guiIcon(ctx, MxRect{10, 80, 0, 0}, anchor, ICON_FA_CIRCLE_PLAY);
        if (mxgui::guiIconButton(ctx, MxRect{10, 120, 20, 20}, anchor, ICON_FA_CIRCLE_PLAY, 28))
        {
            MX_LOG("Clicked Icon");
        }
        mxgui::pushIconSize(ctx, 50);
        mxgui::guiIcon(ctx, MxRect{10, 160, 0, 0}, anchor, ICON_FA_COPY);
        mxgui::pushIconSize(ctx, 28);
        #endif // MX_FONT_AWESOME
        
        mxgui::guiScrollPanelBegin(ctx, "ScrollPanel", MxRect{300, 200, 100, 200}, MxRect{300, 200, 100, 550}, MxVec2{100, 100}, true);
        mxgui::pushTextSize(ctx, 25);
        mxgui::guiLabel(ctx, "hello world", MxVec2{100, 0}, anchor);
        mxgui::guiLabel(ctx, "hello world 2", MxVec2{0, 80});
        mxgui::guiLabel(ctx, "hello world 3", MxVec2{0, 100});
        mxgui::guiLabel(ctx, "hello world 4", MxVec2{0, 120});
        mxgui::pushTextSize(ctx, 20);
        mxgui::guiScrollPanelEnd(ctx, "ScrollPanel");
        
        
        mxgui::guiCheckBox(ctx, MxRect{100, 100}, MxVec2{}, checked);
        mxgui::guiToogle(ctx, MxRect{150, 100}, MxVec2{}, toogle);

        mxgui::guiTextBox(ctx, "textbox", MxRect{200,300,150,23});
        
        
        const float progress = mxgui::guiSlider(ctx, "Slider", MxRect{50, 530, 700, 6}, MxVec2{}, true);
        mxgui::guiLabel(ctx, std::to_string(progress), MxVec2{10, 30});
        mxgui::guiSliderProgress(ctx, MxRect{50, 560, 700, 6}, MxVec2{}, 0.8f);
        
        //MX_LOG("getframetime intern %.6f raylib %.6f", getFrameTime(), GetFrameTime());

        // MX_LOG("teste %.2f", progress);
        if (isCursorOnScreen())
        {
            const MxFont* font = getFont(MX_FONT_NOTO_ID, 20);
            const MxFont* font2 = getFont("teste", 30);
            MxColor color = {200, 41, 55, 255};

            drawTextEx(*font, "teste \nteste quebra linha", MxVec2{60, 60}, 20, 0, color);
            drawTextEx(*font2, "teste \nteste quebra linha", MxVec2{60, 160}, 30, 0, color);
        }


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
