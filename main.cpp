
#define MX_LOG(...)                                                 \
    do                                                              \
    {                                                               \
        printf("FILE: [%s] LINE: [%d] (", __FILE_NAME__, __LINE__); \
        printf(__VA_ARGS__);                                        \
        printf(")\n");                                              \
    } while (0)


#define MX_SUPPRESS_WARNINGS 0


// #define MX_CUSTOM_BACKEND_HEADER "mxgui_custom_render.hpp"
#define MX_RAYLIB_BACKEND_IMPLEMENTATION
// #define MX_SFML_BACKEND_IMPLEMENTATION
#define MX_GUI_IMPLEMENTATION
#include "mxgui.hpp"


#if MX_SFML
static sf::RenderWindow* s_window = nullptr;
#endif


// #define MX_SFML 1


// includes temp for fast tests

// #include "temp/include_test.h"
// #include "temp/scissor_test.h"
// #include "temp/text_block_prototype.h"

void Init();
void Begin(MxGuiContext* ctx);
bool ShouldClose();
void End();

int main()
{


    Init();

    MxStyle styleMxGui = MxStyle::MxGui;
    MxStyle styleLight = MxStyle::Light;
    MxStyle styleDark = MxStyle::Dark;
    styleMxGui.iconSize = 28;
    styleLight.iconSize = 28;
    styleDark.iconSize = 28;
    MxGuiContext* ctx = mxgui::createContext(styleLight);
    
    mxgui::createFont("inter", "assets/inter.ttf", 20, NULL, 255, false);
    mxgui::createTexture("assets/test.png", "testpng");

    const float width = 400;
    const float height = 25;
    MxVec2 anchor{10, 10};
    bool checked = false;
    bool toggle = false;
    std::string list = "Charmander,Bulbasaur,Squirtle,Pikachu,Eevee,Pidgey";
    float progress = 0.5f;

    int themeIndex = 1;


    while (!ShouldClose())
    {

        Begin(ctx);

        mxgui::beginMx();

        anchor = mxgui::guiPanel(ctx, "Canvas1", toMxRect(anchor, MxVec2{width, height}), MxVec2{}, true);
        mxgui::guiPanel(ctx, "Canvas2", MxRect{0, (height - 1), width, width}, anchor, false);
        if (mxgui::guiButton(ctx, "Click", MxRect{10, 35, 75, 35}, anchor, MX_BUTTON_OUTLINE, true))
        {
            themeIndex++;
            if (themeIndex > 3)
            {
                themeIndex = 1;
            }
            
            switch (themeIndex)
            {
                case 1: mxgui::setStyle(ctx, styleLight); break;
                case 2: mxgui::setStyle(ctx, styleDark); break;
                case 3: mxgui::setStyle(ctx, styleMxGui); break;
                default: break;
            }
            

            MX_LOG("Clicked");
            setSmoothTexture(MX_FONT_NOTO_ID, !isSmoothTexture(MX_FONT_NOTO_ID));
            setSmoothTexture(MX_FONT_AWESOME_ID, !isSmoothTexture(MX_FONT_AWESOME_ID));
        }
        mxgui::guiButton(ctx, "Click", MxRect{95, 35, 75, 35}, anchor, MX_BUTTON_OUTLINE_FILL, true);
        if(mxgui::guiButton(ctx, "Click", MxRect{10, 85, 75, 35}, anchor, MX_BUTTON_CONTAINED, true))
        {
            progress = 0.5f;
        }

        mxgui::guiImage(ctx, "testpng", MxRect{210, 35, 180, 180}, anchor);

        if (mxgui::guiIconButton(ctx, MxRect{10, 140, 20, 20}, anchor, ICON_FA_CIRCLE_PLAY, 28))
        {
            MX_LOG("Clicked Icon");
            if (list == "Charmander,Bulbasaur,Squirtle,Pikachu,Eevee,Pidgey")
            {
                //list = "Chikorita,Cyndaquil,Totodile,Pichu,Marill,Hoothoot";
                list = "Chikorita,Cyndaquil,Totodile,Pichu";
            }
            else
            {
                list = "Charmander,Bulbasaur,Squirtle,Pikachu,Eevee,Pidgey";
            }
        }
        mxgui::guiIcon(ctx, MxRect{10, 180, 0, 0}, anchor, ICON_FA_CIRCLE_PLAY);

        mxgui::iconSize(ctx, 50);
        mxgui::guiIcon(ctx, MxRect{10, 260, 0, 0}, anchor, ICON_FA_COPY);
        mxgui::iconSize(ctx, 28);


        mxgui::guiScrollPanelBegin(ctx, "ScrollPanel", MxRect{300, 200, 100, 200}, MxRect{300, 200, 100, 550}, MxVec2{100, 100}, true);
        //mxgui::textSize(ctx, 20);
        mxgui::guiLabel(ctx, "hello world 1", MxVec2{100, 0}, anchor);
        mxgui::guiLabel(ctx, "hello world 2", MxVec2{0, 80});
        mxgui::guiLabel(ctx, "hello world 3", MxVec2{0, 100});
        mxgui::guiLabel(ctx, "hello world 4", MxVec2{0, 120});
        mxgui::guiToolTip(ctx, "tooltip" , "test tooltip msg" , MxRect{0, 120, 100, 100});
        mxgui::guiListView(ctx, "guiListViewScroll", MxRect{0, 0, 100, 150}, anchor, list);
        mxgui::guiScrollPanelEnd(ctx, "ScrollPanel");
        
        
        mxgui::guiCheckBox(ctx, MxRect{100, 100}, MxVec2{}, checked);
        mxgui::guiToggle(ctx, MxRect{150, 100}, MxVec2{}, toggle);
        
        
        //mxgui::textSize(ctx, 20);
        mxgui::guiTextBox(ctx, "textbox", MxRect{200, 300, 150, 0});
        mxgui::useFont(ctx, "inter", 20);
        mxgui::guiTextBox(ctx, "textbox2", MxRect{200, 350, 150, 0});

        mxgui::useFont(ctx, MX_FONT_NOTO_ID, 20);

        mxgui::guiSlider(ctx, "Slider", MxRect{50, 530, mxgui::getWindowSize().x - 100, 6}, MxVec2{}, true, progress);
        mxgui::guiLabel(ctx, std::to_string(progress), MxVec2{10, 30});
        mxgui::guiSliderProgress(ctx, MxRect{50, 560, mxgui::getWindowSize().x - 100, 6}, MxVec2{}, 0.8f);
        mxgui::guiListView(ctx, "guiListView", MxRect{20, 400, 100, 150}, anchor, list);




        mxgui::endMx();

        End();
    }


    mxgui::destroyContext(ctx);

#if MX_RAYLIB
    CloseWindow();
#endif

    return 0;
}

void Init()
{
#if MX_RAYLIB
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "GUI");
    SetTextLineSpacing(0);
    // SetTargetFPS(60);
#elif MX_SFML
    static sf::RenderWindow window(sf::VideoMode({800, 600}), "GUI");
    s_window = &window;
#endif
}

bool ShouldClose()
{
#if MX_RAYLIB
    return WindowShouldClose();
#elif MX_SFML
    return !s_window->isOpen();
#endif
}


void Begin(MxGuiContext* ctx)
{
    MxColor color = mxgui::getStyle(ctx).backgroundColor;
#if MX_RAYLIB
    BeginDrawing();
    Color backgroundColor = Color{color.r, color.g, color.b, color.a};
    ClearBackground(backgroundColor);
#elif MX_SFML
    while (const std::optional event = windowPollEvent(s_window))
    {
        if (event->is<sf::Event::Closed>())
        {
            s_window->close();
        }
    }
    sf::Color backgroundColor = sf::Color{color.r, color.g, color.b, color.a};
    s_window->clear(backgroundColor);

#endif
}

void End()
{
#if MX_RAYLIB

    DrawFPS(10, 10);
    // DrawText(TextFormat("cor: %zu", sizeof(Transform)), 30, 30, 20, BLACK);
    EndDrawing();
#elif MX_SFML
    drawFPS(10, 10);
    windowDisplay(s_window);
#endif
}
