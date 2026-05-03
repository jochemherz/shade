#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "textbox.h"
#include "rlgl.h"



int main()
{
    int screenWidth = 640;
    int screenHeight = 640;
    InitWindow(screenWidth, screenHeight, "hotloader");
    SetTargetFPS(60);

    SetTextLineSpacing(0);

// shader vector stuff

    const int SHADER_COUNT = 3;
    int sh = 0;         // current shader selected, controls array index of vectors
    std::vector<std::string> locations = {"shader.glsl", "shader2.glsl", "shader3.glsl"};
    std::vector<std::string> shader_texts;              // strings with the text of all the shaders
    std::vector<Shader> shaders;                        // actual shader struct which raylib uses

    for(int i=0; i<SHADER_COUNT; i++){                                                // read shaders text from file and load it in the array
        std::ifstream file(locations[i]); // open file
        if(!file.is_open()){
            continue;       // error code here
        }

        std::string text;
        std::string line;
        while(getline(file, line)) {
            text += line + '\n'; // copy file to string line by line i guess
        }
        shader_texts.push_back(text);
        shaders.push_back(LoadShaderFromMemory(0, shader_texts[i].c_str()));
    }

// text editor initialization

    Font font = LoadFont("../../assets/fonts/DejaVuSansMono/DejaVuSansMono.ttf");
    Texture tex = LoadTexture("../../assets/textures/flowerpaint.png");
    //Texture tex = LoadTexture("../../assets/textures/square_red.png");
    Image texDefaultImage = GenImageColor(1, 1, BLACK);
    Texture texDefault = LoadTextureFromImage(texDefaultImage);
    bool showingText = true;
    // tex = {0};
    bool texvalid = IsTextureValid(tex);
    int fontsize = 24;
    float scale = (float)fontsize / font.baseSize;

    int index = GetGlyphIndex(font, 'M');
    GlyphInfo g = font.glyphs[index];
    int advanceX = g.advanceX;
    if(advanceX == 0){                                  // if no advanceX available, fall back to the glyph rec width
        advanceX = font.recs[index].width;
    }

    int cellwidth = advanceX * scale;
    int cellheight = fontsize;

    textbox_mono tb = textbox_mono(shader_texts[sh] /*"hello\nworld"*/, 10, 10, screenWidth, screenHeight, cellwidth, cellheight); // initialize to first shader


//---------------------------------------------------------------------------------------------------------------------------------------

// main loop

    while(!WindowShouldClose()){

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) tb.SetCursorToPos(GetMouseX(), GetMouseY());

            //move shader array
            if(IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)){ tb.MoveCursorLeft(); }
            if(IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)){ tb.MoveCursorRight();}
            if(IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)){ tb.MoveCursorUp();}
            if(IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)){ tb.MoveCursorDown();}

            char ch = GetCharPressed();
            while(ch>0){
                tb.HandleInput({InputType::Character, ch});
                ch = GetCharPressed();
            }
            if(IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) tb.HandleInput({InputType::Backspace});
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressedRepeat(KEY_ENTER)) tb.HandleInput({InputType::Enter});
            if(IsKeyPressed(KEY_TAB) || IsKeyPressedRepeat(KEY_TAB)) tb.HandleInput({InputType::Tab});

            tb.Update();     // increase frame counter

            //DEBUG
            if(IsKeyPressed(KEY_F1)) {std::string buf = tb.GetLineBuffer(); std::cout << '\n' << buf; }
            if(IsKeyPressed(KEY_F2)){
                Shader shd = LoadShaderFromMemory(0, tb.GetText().c_str());

                if(IsShaderValid(shd)){
                    UnloadShader(shaders[sh]);
                    shaders[sh] = shd;
                }

            }
            if(IsKeyPressed(KEY_F3)){ showingText = !showingText; }
            if(IsKeyPressed(KEY_F4)){ tb.HandleInput({InputType::DEBUG_println}); }

//---------------------------------------------------------------------------------------------------------------------------------------

    BeginDrawing();

            ClearBackground(RAYWHITE);

            // We only draw a white full-screen rectangle, frame is generated in shader
            BeginShaderMode(shaders[sh]);
                    int loc = GetShaderLocation(shaders[sh], "tex");
                    if(loc != -1){
                            if(texvalid)
                                SetShaderValueTexture(shaders[sh], loc, tex);
                            else
                                SetShaderValueTexture(shaders[sh], loc, texDefault);
                    }


                DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
                //DrawTexture(tex, 0, 0, WHITE);
            EndShaderMode();

            // draw text
            if(showingText){
                    if((tb.framecounter/35)%2 == 0)
                    DrawLine(tb.GetCursorX(), tb.GetCursorY(), tb.GetCursorX(), tb.GetCursorY() + fontsize, WHITE);
                    //DrawText(shader_texts[sh].c_str(), 0, 0, 16, RAYWHITE);
                    DrawTextEx(font, tb.GetText().c_str(), {tb.x, tb.y}, fontsize, 0, WHITE);
                    DrawText(TextFormat("%i", tb.GetCursor()), screenWidth - 60, screenHeight - 40, 20, GOLD);
            }

    EndDrawing();

//---------------------------------------------------------------------------------------------------------------------------------------

    }

    CloseWindow();
}


