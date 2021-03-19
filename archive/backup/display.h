#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "fileIO.h"

#define WINDOWWIDTH  1280
#define WINDOWHEIGHT 900

#define BOXALPHA 120
#define BOXPADDINGX 70
#define BOXPADDINGY 70
#define OUTLINEPADDING 3
#define OUTLINECOUNT 2

#define TEXTHEIGHT 35
#define TEXTWIDTH 20
#define TEXTPADDINGX 30
#define TEXTPADDINGY 15
#define CHARACTERSPACING 0
#define LINESPACING 1.2

#define CHARACTERS_PER_LINE 50
#define LINES_PER_PAGE 16

#define DEFAULT_SPEED 80 // About in chars/sec

#define FPS_CAP 60

SDL_Texture *glyphs[128-20];

SDL_Texture *background;

TTF_Font *mainFont;

int currentRow;
int currentPage;
int lineInPage;
int currentChapter;
bool backgroundRendered;
bool nextLineRequested;
bool finishLine;

// For line rendering
int lineBeginning;
int charInRow;
int charInLine;
int nextSpace;
int textIndex;
int lineIndex;
char ch;
int lineLength;
int nextSpace;
double lineRenderSpeed;

void renderCharacter(char, int, int, SDL_Renderer*);
void advanceLineRendering(SDL_Renderer*, bool);
void finishLineRendering(SDL_Renderer*);
void renderLine(SDL_Renderer*, int, int);
void renderPagePart(SDL_Renderer*, int, int);
void renderPage(SDL_Renderer*, int);
void renderNextLine(SDL_Renderer*);
