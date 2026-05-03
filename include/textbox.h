#include <string>
#include <vector>



// tokens to process input
enum class InputType{
    Character,
    Enter,
    Backspace,
    Tab,
    DEBUG_println
};

struct TextInput{
    InputType type;
    char c;
};

// vec2
struct Vec2{
    int x;
    int y;
};


// textbox for monospace font
class textbox_mono{
public:
    textbox_mono(std::string text, int x, int y, int width, int height, int cellwidth, int cellheight);

    void HandleInput(const TextInput& input);

    // layout
    int x;
    int y;
    int GetWidth() const;
    int GetHeight() const;
    int GetCellWidth() const;
    int GetCellHeight() const;
    void SetSize(int w, int h);
    unsigned int framecounter {0};
    void Update();

    // cursor
    int GetCursor() const;
    int GetCursorAtPos(int mouseX, int mouseY) const;
    void MoveCursorLeft();
    void MoveCursorRight();
    void MoveCursorUp();
    void MoveCursorDown();
    int GetCursorX();
    int GetCursorY();
    Vec2 GetCursorPixelPos();
    void SetCursorToPos(int _x, int _y);

    // string manipulation
    std::string GetText();
    void Write(char c);
    void Backspace();

    // DEBUG
    std::string GetLineBuffer();

private:
    int newlines;
    std::string text;
    int cursor;
    bool active;
    int cellwidth;      //font width + spacing      : to calculate x and y of individual characters
    int cellheight;     //font height + spacing

    int width;
    int height;

    std::vector<int> linestarts;
    std::string linebuffer;
    std::string GetLine(int c);
    std::string GetLine(){ return GetLine(cursor); }
    int GetLineStart(int c);
    int GetLineStart(){ return GetLineStart(cursor); }
    int GetLineEnd(int c);
    int GetLineEnd(){ return GetLineEnd(cursor); }
};



