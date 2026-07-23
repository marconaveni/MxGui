

#include <array>
#define MX_GUI_IMPLEMENTATION
#include "mxgui.hpp"
#include "raylib.h"






void testScissor();


int main()
{


    //     InitWindow(800, 450, "Font Awesome + raylib");




    // int tamanhoOriginal = 414704; 
    // unsigned char* dadosFinaisOTF = (unsigned char*)malloc(tamanhoOriginal);
    
    // // 2. Descompacta DIRETO do array gerado (Sem precisar decodificar nada antes!)
    // stb_decompress(dadosFinaisOTF, Font_compressed_data, Font_compressed_size);
    

    // // f015  
    // int count = sizeof(codepoints) / sizeof(codepoints[0]);

    // //Font faFont = LoadFontEx("/home/marco/Downloads/fontawesome-free-7.3.1-desktop/otfs/Font Awesome 7 Free-Solid-900.otf", 64, codepoints, count);
    // Font faFont = LoadFontFromMemory(  ".otf", dadosFinaisOTF,  tamanhoOriginal, 64, codepoints, count);
    // SetTextureFilter(faFont.texture, TEXTURE_FILTER_BILINEAR);

    // while (!WindowShouldClose()) {
    //     BeginDrawing();
    //     ClearBackground(RAYWHITE);

    //     // Converte o codepoint pra UTF-8 antes de desenhar
    //     int byteCount = 0;
    //     const char *icon = CodepointToUTF8(0xf007, &byteCount);

    //     DrawTextEx(faFont, icon, Vector2{ 100, 180 }, 64, 0, BLACK);

    //     EndDrawing();
    // }

    // UnloadFont(faFont);
    // CloseWindow();
    // return 0;

    // InitWindow(800, 600, "image");
    // SetTargetFPS(60);

    //MxImage image = genIcon(guiIcons, 0);
    // Image image1 = genIcon(guiIcons, 1);
    // Image image2 = genIcon(guiIcons, 2);
    // Image image3 = genIcon(guiIcons, 3);
    // Texture texture = LoadTextureFromImage(image);
    // Texture texture1 = LoadTextureFromImage(image1);
    // Texture texture2 = LoadTextureFromImage(image2);
    // Texture texture3 = LoadTextureFromImage(image3);

    // //SetTextureFilter(texture1, TEXTURE_FILTER_BILINEAR);


    // while (!WindowShouldClose())
    // {
    //     BeginDrawing();
    //     ClearBackground(RAYWHITE);
    //     // DrawRectangle(10,10,16,16, GRAY);
    //     DrawTexture(texture, 10, 10, DARKGRAY);
    //     DrawTexture(texture1, 10 + 16, 10, DARKGRAY);
    //     DrawTexture(texture2, 10, 10 + 16, DARKGRAY);
    //     DrawTexture(texture3, 10 + 16, 10 + 16, DARKGRAY);
    //     EndDrawing();
    // }
    // CloseWindow();


    // /////////////////////////////////////


    // //return 0;

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
        mxgui::guiIcon(ctx, MxRect{10, 80, 0, 0}, anchor, MxIconFontAwesomeIndex::MX_PLUS);

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
