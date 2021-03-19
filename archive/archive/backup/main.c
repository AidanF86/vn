#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "fileIO.h"
#include "clock.h"
#include "display.h"
#include "actions.h"

#include "fileIO.c"
#include "clock.c"
#include "display.c"
#include "actions.c"

void doRender(SDL_Renderer *renderer)
{
    //Set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    //Clear the screen (to blue)
    SDL_RenderClear(renderer);

    //Set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect bgRect = {0, 0, WINDOWWIDTH, WINDOWHEIGHT};
    SDL_RenderCopy(renderer, background, NULL, &bgRect);

    //Render the text box
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, BOXALPHA);
    SDL_Rect textBox = {BOXPADDINGX, BOXPADDINGY, WINDOWWIDTH - BOXPADDINGX * 2, WINDOWHEIGHT - BOXPADDINGY * 2};
    SDL_RenderFillRect(renderer, &textBox);
    for(int i = 1; i <= OUTLINECOUNT; i++)
    {
        SDL_Rect textBoxOutline = {BOXPADDINGX - i * OUTLINEPADDING, BOXPADDINGY - i * OUTLINEPADDING, WINDOWWIDTH - BOXPADDINGX * 2 + i * 2 * OUTLINEPADDING, WINDOWHEIGHT - BOXPADDINGY * 2 + i * 2* OUTLINEPADDING};
        SDL_RenderDrawRect(renderer, &textBoxOutline);
    }

    switch(mode)
    {
        case textFinished:
        {
            renderNextLine(renderer);
            nextLineRequested = false;
        }
        break;
        case textRendering:
        {
            finishLineRendering(renderer);
        }
    }
    
    SDL_RenderPresent(renderer);
}

int processEvents(SDL_Window *window)
{
    SDL_Event event;
    short done = 0;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(window)
                {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            }
            break;
            case SDL_KEYDOWN:
            {
                if(mode == textFinished)
                    processKeyDownTextFinished(event);
                else if(mode == textRendering)
                    processKeyDownTextRendering(event);
            }
            break;
            case SDL_QUIT:
                done = 1;
            break;
        }
    }
    
    return done;
}

int main()
{

    /* ------ Set variables ------ */

    lastUpdateError = 0;
    currentTime = 0;
    nextLineRequested = true;
    backgroundRendered = false;
    pages = 1;
    currentChapter = 0;
    chapterFiles[currentChapter] = "input.txt";

    //load(0);

    /* ------ Variables set ------ */
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Game Window",
            SDL_WINDOWPOS_UNDEFINED, //Initial x
            SDL_WINDOWPOS_UNDEFINED, //Initial y
            WINDOWWIDTH,             //width
            WINDOWHEIGHT,            //height
            0
            );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Surfaces
    printf("Loading surfaces...\n");
    SDL_Surface *backgroundSurface = NULL;
    backgroundSurface = IMG_Load("background.jpg");
    if(backgroundSurface == NULL)
    {
        printf("Cannot find background.png!\n\n");
        SDL_Quit();
        return 1;
    }
    background = SDL_CreateTextureFromSurface(renderer, backgroundSurface);


    // Load font
    printf("Loading fonts...\n");
    TTF_Init();
    mainFont = TTF_OpenFont("fonts/FreeMono.ttf", 100);
    if(!mainFont) 
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }

    // Glyphs
    SDL_Color color = {230, 230, 230};
    SDL_Surface *glyphCache[128-20];
    Uint16 ch;
    for(ch = 20; ch < 128; ++ch)
    {
        glyphCache[ch - 20] = TTF_RenderGlyph_Solid(mainFont, ch, color);
        if(glyphCache[ch - 20] == NULL)
            printf("Glyph not found\n");
        glyphs[ch - 20] = SDL_CreateTextureFromSurface(renderer, glyphCache[ch - 20]);
        SDL_FreeSurface(glyphCache[ch - 20]);
        //printf("%c: %d", (char)ch, ch);
    }

    SDL_FreeSurface(backgroundSurface);

    printf("Starting...\n");
    int done = 0;

    // Load text file (chapter)
    loadChapterFile(0);

    startTime = clock();
    lastUpdateTime = startTime;
    timeSinceLastCharRender = 0;
    lastUpdateError = 0;
    double renderTimeToUpdateError = 0;

    // Event Loop
    while(!done)
    {
        currentTime = clock();

        // Check for updates
        done = processEvents(window);

        long double timeSinceLastUpdate = getSecondsSinceLastUpdate();
        if(timeSinceLastUpdate + lastUpdateError >= 1.0 / FPS_CAP)
        {
            //printf("%Lf\n", timeSinceLastUpdate);
            lastUpdateError = getSecondsSinceLastUpdate() + lastUpdateError - (1.0 / FPS_CAP);

            // Render display
            if(nextLineRequested)
            {
                doRender(renderer);
            }

            if(finishLine)
            {
                finishLineRendering(renderer);
            }

            if(mode == textRendering)
            {
                int timesToUpdate = (double)lineRenderSpeed * timeSinceLastUpdate + renderTimeToUpdateError;
                renderTimeToUpdateError += (double)lineRenderSpeed * timeSinceLastUpdate - timesToUpdate;
                timeSinceLastCharRender += getSecondsSinceLastUpdate();
                for(int i = 0; i < timesToUpdate; i++)
                {
                    advanceLineRendering(renderer, true);
                    timeSinceLastCharRender = 0;
                }
            }

            lastUpdateTime = clock();
        }
    }
    
    SDL_DestroyTexture(background);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();

    return 0;
}
