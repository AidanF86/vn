#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "fileIO.h"

// Coordinates go from -XUNITS to XUNITS, same for YUNITS (- is left, down).
// Basically a XUNITSxYUNITS coordinate grid centered at 0,0.
#define XUNITS 20.0
#define YUNITS 20.0
#define XEDGE (XUNITS / 2.0)
#define YEDGE (YUNITS / 2.0)

#define BOXALPHA 120
#define BOXPADDINGX 1.0
#define BOXPADDINGY 1.0
#define OUTLINEPADDING 0.05
#define OUTLINECOUNT 2

#define TEXTHEIGHT 0.7
#define TEXTWIDTH .3
#define TEXTPADDINGX 1.0
#define TEXTPADDINGY 1.0
#define CHARACTERSPACING 0.0
#define LINESPACING 1.4

//#define CHARACTERS_PER_LINE (WINDOWWIDTH-2*BOXPADDINGX-2*TEXTPADDINGX)/(TEXTWIDTH+CHARACTERSPACING)
#define CHARACTERS_PER_LINE 55
#define LINES_PER_PAGE 16
#define DEFAULT_SPEED 300 // in an unknown unit

#define FPS_CAP 60

SDL_Texture *glyphs[128-20];
SDL_Texture *background;
TTF_Font *mainFont;

#define SPRITE_COUNT 2
#define MAX_SPRITE_VARIANTS 5

bool spritesTiled; // TODO: Works like a tiling window manager; sprites are tiled horizontally

SDL_Texture *sprites[SPRITE_COUNT][MAX_SPRITE_VARIANTS];
char *spriteNames[SPRITE_COUNT];
char *spriteVariantNames[SPRITE_COUNT][MAX_SPRITE_VARIANTS];

double spritePositions[SPRITE_COUNT][2];
double spriteScales[SPRITE_COUNT];
int spriteDimensions[SPRITE_COUNT][2];
bool spriteEnabled[SPRITE_COUNT];
int spriteVariant[SPRITE_COUNT];

int windowWidth;
int windowHeight;
int actualWindowWidth;
int actualWindowHeight;
double desiredWScale;
double desiredHScale;

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
int getPixelX();
int getPixelY();
int getLengthX();
int getLengthY();
