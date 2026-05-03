#include "textbox.h"
#include <string>
#include <algorithm>
#include <iostream>

textbox_mono::textbox_mono(std::string text, int x, int y, int width, int height, int cellwidth, int cellheight){
    this->text = text;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->cellwidth = cellwidth;
    this->cellheight = cellheight;
    active = true;
    newlines = 0;
    cursor = 0;
}

void textbox_mono::HandleInput(const TextInput& input){
    if(!active) return;

    switch(input.type){
case InputType::Character:
    Write(input.c);
    break;
case InputType::Backspace:
    Backspace();
    break;
case InputType::Enter:
    Write('\n');
    newlines++;
    break;
case InputType::Tab:
    {
    int n = 2;
    while(n--)Write(' ');
    break;
    }
case InputType::DEBUG_println:
    std::cout<< '\n' << "Line: " << GetLine(GetLineStart()-1) << '\n' << "Starting Index: " << GetLineStart(GetLineStart()-1) << " Ending Index: " << GetLineEnd(GetLineStart()-1);
    }
}

void textbox_mono::Update(){
    framecounter++;
}

int textbox_mono::GetWidth() const{
    return width;
}

int textbox_mono::GetHeight() const{
    return height;
}

int textbox_mono::GetCellWidth() const{
    return cellwidth;
}

int textbox_mono::GetCellHeight() const{
    return cellheight;
}

void textbox_mono::SetSize(int w, int h){
    width = w;
    height = h;
    //also recalculate word string layout?
}

int textbox_mono::GetCursor() const{
    return cursor;
}

int textbox_mono::GetCursorAtPos(int mouseX, int mouseY) const{
    return 0;
}

void textbox_mono::SetCursorToPos(int _x, int _y){
    // if not in bounds/nothing to select, return;

    framecounter = 0;       // always redraw cursor line

    linestarts = {0};
    for(int i=0; i<text.size(); i++){       // get newlines into vec
        if(text[i] == '\n'){
            linestarts.push_back(i + 1);
        }
    }

    // get row of mouseY (TODO: maybe add offset for qol)
    int row = (_y - y) / cellheight;
    if(row > linestarts.size()-1) { cursor = text.size(); return; }

    // get col of mouseX
    int col = (_x - x + cellwidth/2) / cellwidth;
    std::cout << row << GetLine(linestarts[row]);

    int lineEnd = GetLineEnd(linestarts[row]);
    int maxCol = lineEnd - linestarts[row];

    if(col > maxCol) { cursor = lineEnd; return; }
    cursor = linestarts[row] + col;
    std::cout << '\n' << "maxcol : " << maxCol << " col : " << col;

}

void textbox_mono::MoveCursorLeft(){
    cursor--;
    if(cursor < 0){
        cursor = 0;
        return;
    }
    framecounter = 0;

}

void textbox_mono::MoveCursorRight(){
    cursor++;
    int ts = text.size();
    if(cursor > ts){
        cursor = ts;
    }
    framecounter = 0;
}

void textbox_mono::MoveCursorUp(){

    int lineStart = GetLineStart();
    if(lineStart <= 0) return;      // can't go up further

    // TODO: make getcol and getrow functions and rewrite everything

    std::string buffer = GetLine(lineStart - 1);    // get previous line
    int col = cursor - lineStart;   // index of this line

    // move cursor
    if(buffer.size() < col){ cursor = lineStart - 1; return; }  // snap to the end of the next line
    cursor -= buffer.size() + 1;                                // move vertically

}

void textbox_mono::MoveCursorDown(){

    int lineStart = GetLineStart();
    int lineEnd = GetLineEnd();
    if(lineEnd >= text.size()) return;   // can't go down further
    int col = cursor - lineStart;        // index of this line

    int nextLineStart = lineEnd + 1;
    int nextLineEnd = GetLineEnd(nextLineStart);
    int bufferSize = nextLineEnd - nextLineStart; // size of next line under

    if(col > bufferSize){                       // snap to the end of the next line
            cursor = nextLineStart + bufferSize;
            return;
            }
    cursor = nextLineStart + col;

}

int textbox_mono::GetCursorX(){
    int _x = x;     // get x of textbox
    int c = std::min(cursor, (int)text.size());
    for(int i=c; i>0; i--){
        if(text[i-1] == '\n') break;
        _x += cellwidth;
    }
    return _x;
}

int textbox_mono::GetCursorY(){
    int _y = y;     // get y of textbox
    for(int i=0; i<cursor; i++){
        if(text[i] == '\n')
            _y += cellheight;
    }
    return _y;
}

Vec2 textbox_mono::GetCursorPixelPos(){ // convert index to x and y vec2
    int _x = x;
    int _y = y;

    for(int i=0; i<cursor; i++){
        if(text[i] == '\n'){
            _y += cellheight;
            _x = x;
        } else
        _x += cellwidth;
    }

    return {_x, _y};
}

std::string textbox_mono::GetText(){
    return text;
}

std::string textbox_mono::GetLine(int c){       // return the string of the line of text of cursor 'c'

    int lineStart = GetLineStart(c);
    int lineEnd = GetLineEnd(c);
    std::string line = "";
    for(int i=lineStart; i<lineEnd; i++){       // keep adding characters until finding a newline
        line += text[i];
    }
    return line;
}

int textbox_mono::GetLineStart(int c){          // find the beginning of the line

    int linestart{0};
    for(int i=c; i>=0; i--){
        linestart = i;
        if(i > 0 && text[i-1] == '\n'){
            break;
        }
    }
    return linestart;
}

int textbox_mono::GetLineEnd(int c){
    int lineEnd{c};
    for(; lineEnd < text.size(); lineEnd++){
        if(text[lineEnd] == '\n')
            break;
    }
    return lineEnd;             // index of '\n'
}

std::string textbox_mono::GetLineBuffer(){
    linebuffer = GetLine();
    return linebuffer;
}

void textbox_mono::Write(char c){
    text.insert(cursor, 1, c);
    MoveCursorRight();
}

void textbox_mono::Backspace(){
    if(cursor != 0){
        text.erase(cursor-1, 1);
        MoveCursorLeft();
    }
}
