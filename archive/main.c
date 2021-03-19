#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool done;

#include "fileIO.h"
#include "clock.h"
#include "display.h"
#include "actions.h"
#include "menu.h"

#include "fileIO.c"
#include "clock.c"
#include "display.c"
#include "actions.c"
#include "menu.c"

void executeCommand(char* command)
{
    printf("Executing command: '%s'\n", command);
    int partCount = 1;
    int commandLength = 0;
    for(int i = 0; command[i] != '\0'; i++)
    {
        if(command[i] == ' ' && command[i + 1] != '\0' && i != 0)
        {
            partCount++;
        }
        commandLength++;
    }
    printf("Command length: %d\n", commandLength);
    int partIndexes[partCount];
    int partSizes[partCount];
    partIndexes[0] = 0;
    int currentCommandPart = 0;
    int currentCommandPartLength = 0;

    // Get the amount of parts (separated by spaces)
    for(int i = 0; i < commandLength + 1; i++)
    {
        if(command[i] == ' ' && command[i + 1] != '\0' & i != 0)
        {
            partIndexes[currentCommandPart + 1] = i + 1;
            partSizes[currentCommandPart] = currentCommandPartLength;
            currentCommandPart++;
            currentCommandPartLength = 0;
        }
        else if(command[i] == '\0')
        {
            partSizes[currentCommandPart] = currentCommandPartLength;
        }
        else
        {
            currentCommandPartLength++;
        }
    }

    char parts[partCount][50];
    for(int i = 0; i < partCount; i++)
    {
        //printf("Part %d size: %d\n", i, partSizes[i]);
        strncpy(parts[i], command + partIndexes[i], partSizes[i]);
        parts[i][partSizes[i]] = '\0';
        //printf("Part %d: '%s'\n", i, parts[i]);
    }

    switch(command[partIndexes[0]])
    {
        case 's':
            {
                printf("Command type is 's'\n");

                // Get sprite index
                int index = 0;
                char fullSpriteName[partSizes[1] - 1 + 1 + partSizes[2] - 1 + 1]; 
                strcpy(fullSpriteName, "");
                strcat(fullSpriteName, parts[1]);
                strcat(fullSpriteName, "-");
                strcat(fullSpriteName, parts[2]);

                printf("Full sprite name: '%s'\n", fullSpriteName);

                for(int i = 0; i < sizeof(spriteNames) / sizeof(char*); i++)
                {
                    if(strcmp(spriteNames[i], fullSpriteName) == 0)
                    {
                        index = i;
                        break;
                    }
                }
                //free(fullSpriteName);
            }
            break;
        default:
            printf("Unrecognized command type '%c'\n", command[partIndexes[0]]);
            break;
    }
    /*
    for(int i = 0; i < partCount; i++)
    {
        char part[partSizes[i] + 1];
        printf("Index #%d: %d\n", i, partIndexes[i]);
        for(int a = 0; a < partSizes[i]; a++)
        {
            part[a] = command[partIndexes[i] + a];
        }
        part[partSizes[i]] = '\0';
        printf("Command part %d: '%s'\n", i, part);
    }
    */
}

void renderBackground(SDL_Renderer *renderer)
{
    SDL_Rect bgRect = {getPixelX(-XEDGE), getPixelY(YEDGE), getLengthX(XUNITS), getLengthY(YUNITS)};
    SDL_RenderCopy(renderer, background, NULL, &bgRect);
}

void renderTextBox(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, BOXALPHA);
    SDL_Rect textBox = {getPixelX(-XEDGE + BOXPADDINGX), getPixelY(YEDGE - BOXPADDINGY), getLengthX(XUNITS - BOXPADDINGX * 2.0), getLengthY(YUNITS - BOXPADDINGY * 2.0)};
    SDL_RenderFillRect(renderer, &textBox);

    // Render surrounding boxes
    for(int i = 1; i <= OUTLINECOUNT; i++)
    {
        SDL_Rect textBoxOutline = {getPixelX(-XEDGE + BOXPADDINGX - i * OUTLINEPADDING), getPixelY(YEDGE - BOXPADDINGY + i * OUTLINEPADDING), getLengthX(XUNITS - BOXPADDINGX * 2 + i * 2 * OUTLINEPADDING), getLengthY(YUNITS - BOXPADDINGY * 2 + i * 2 * OUTLINEPADDING)};
        SDL_RenderDrawRect(renderer, &textBoxOutline);
    }
}

void renderSprites(SDL_Renderer *renderer)
{
    for(int i = 0; i < sizeof(spriteEnabled) / sizeof(bool); i++)
    {
        if(spriteEnabled[i])
        {
            //double w = getLengthX(spriteDimensions[i][0] * spriteScales[i]);
            //double h = getLengthX(spriteDimensions[i][1] * spriteScales[i]);
            //int x = getPixelX(spritePositions[i][0]) - w / 2.0;
            //int y = getPixelY(spritePositions[i][1]) - h / 2.0;
            //SDL_Rect spriteRect = {x, y, w, h};
            //SDL_RenderCopy(renderer, sprites[i], NULL, &spriteRect);
        }
    }
}

void renderTextBackground(SDL_Renderer *renderer)
{
    SDL_Rect bgRect = {0, 0, windowWidth, windowHeight};
    SDL_RenderCopy(renderer, textBackground, NULL, &bgRect);
}

void doRender(SDL_Renderer *renderer)
{
    //Set the drawing color to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //Clear the screen (to black)
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if(currentMenu == noMenu)
    {
        textBackground = SDL_GetRenderTarget(renderer);
    }

    switch(mode)
    {
        case textFinished:
        {
            renderBackground(renderer);
            if(nextLineRequested)
            {
                if(nextCommandIndex < commandCount)
                {
                    //printf("Line: %d\n", lineIndex);
                    if(commandLocations[nextCommandIndex] == lineIndex)
                    {
                        executeCommand(commands[nextCommandIndex]);
                        nextCommandIndex++;
                    }
                }
                renderSprites(renderer);
                renderTextBox(renderer);
                renderNextLine(renderer);
                nextLineRequested = false;
            }
            else
            {
                renderSprites(renderer);
                renderTextBox(renderer);
                renderPagePart(renderer, currentPage, lineInPage);
            }
        }
        break;
        case textRendering:
        {
            renderBackground(renderer);
            renderSprites(renderer);
            renderTextBox(renderer);
            finishLineRendering(renderer);
        }
        break;
        case inMenu:
        {
            renderTextBackground(renderer);
        }
    }
    
    SDL_RenderPresent(renderer);
}

int processEvents(SDL_Window *window)
{
    SDL_Event event;

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
                    done = true;
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
                done = true;
            break;
        }
    }
    
    return done;
}

SDL_Surface *loadImage(char fileName[])
{
    SDL_Surface *surface = IMG_Load(fileName);
    if(surface == NULL)
    {
        printf("Cannot find %s", fileName);
        SDL_Quit();
        return NULL;
    }
    return surface;
}

SDL_Texture *createTexture(char fileName[], SDL_Renderer *renderer)
{
    SDL_Surface *surface = loadImage(fileName);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int createSpriteTexture(char spriteName[], int spriteIndex, SDL_Renderer *renderer)
{
    spriteNames[spriteIndex] = malloc(strlen(spriteName) * sizeof(char) + 1);
    strcpy(spriteNames[spriteIndex], spriteName);
    char fileName[strlen(spriteName) + 4];
    sprintf(fileName, "%s.png", spriteName);
    printf("File name: %s\n", fileName);
    SDL_Surface *surface = loadImage(fileName);
    spriteDimensions[spriteIndex][0] = surface->w; spriteDimensions[spriteIndex][1] = surface->h;
    printf("w: %d, h: %d\n", surface->w, surface->h);
    spriteScales[spriteIndex] = .005;
    //spritePositions[spriteIndex][0] = 0; spritePositions[spriteIndex][1] = -5;
    sprites[spriteIndex] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return 1;
}

int main()
{
    /* ------ Set variables ------ */
    desiredWScale = 4.0;
    desiredHScale= 3.0;
    windowWidth  = 1920;
    windowHeight = 1080;
    lastUpdateError = 0;
    currentTime = 0;
    nextLineRequested = true;
    backgroundRendered = false;
    pages = 1;
    currentChapter = 0;
    lineIndex = 0;
    nextCommandIndex = 0;
    chapterFiles[currentChapter] = "chapter1.txt";
    //load(0);
    /* ------ Variables set ------ */

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Game Window",
            SDL_WINDOWPOS_CENTERED, //Initial x
            SDL_WINDOWPOS_CENTERED, //Initial y
            windowWidth,             //width
            windowHeight,            //height
            //SDL_WINDOW_RESIZABLE
            SDL_WINDOW_RESIZABLE
            );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Surfaces
    background = createTexture("background.jpg", renderer);
    int spriteIndex = 0;
    createSpriteTexture("kotomine_kirei-front", spriteIndex, renderer);
    spriteEnabled[0] = true;

    // Load font
    printf("Loading fonts...\n");
    TTF_Init();
    mainFont = TTF_OpenFont("fonts/JetBrainsMono-Light.ttf", 100);
    //mainFont = TTF_OpenFont("fonts/Cirno.ttf", 100);
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
    }

    printf("Starting...\n");
    printf("------------------------------\n");

    // Load text file (chapter)
    loadChapterFile(0);

    startTime = clock();
    lastUpdateTime = startTime;
    timeSinceLastCharRender = 0;
    lastUpdateError = 0;
    double renderTimeToUpdateError = 0;

    // Initialize menus
    initializeMenus();

    done = false;

    // Event Loop
    while(!done)
    {
        int tempWidth = actualWindowWidth; int tempHeight = actualWindowHeight;
        SDL_GetWindowSize(window, &actualWindowWidth, &actualWindowHeight);

        // Update window size
        if(tempWidth != actualWindowWidth || tempHeight != actualWindowHeight || (double)windowWidth / (double)windowHeight - desiredWScale / desiredHScale > 0.01)
        {
            if((double)actualWindowWidth / desiredWScale > (double)actualWindowHeight / desiredHScale)
            {
                windowHeight = actualWindowHeight;
                windowWidth = actualWindowHeight * desiredWScale / desiredHScale;
            }
            else if((double)actualWindowHeight / desiredHScale > (double)actualWindowWidth / desiredWScale)
            {
                windowWidth = actualWindowWidth;
                windowHeight = actualWindowWidth * desiredHScale / desiredWScale;
            }
            else
            {
                windowWidth = actualWindowWidth;
                windowHeight = actualWindowHeight;
            }
            doRender(renderer);
        }

        currentTime = clock();

        // Check for updates
        done = processEvents(window);

        long double timeSinceLastUpdate = getSecondsSinceLastUpdate();
        if(timeSinceLastUpdate + lastUpdateError >= 1.0 / FPS_CAP)
        {
            lastUpdateError = getSecondsSinceLastUpdate() + lastUpdateError - (1.0 / FPS_CAP);

            if(mode == textFinished || mode == textRendering)
            {
                // Render display
                if(nextLineRequested)
                {
                    doRender(renderer);
                }

                if(finishLine)
                {
                    finishLineRendering(renderer);
                    doRender(renderer);
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
            }
            else if(mode == inMenu)
            {
                if(menuChanged)
                {
                    doRender(renderer);
                    menuChanged = false;
                }
            }

            lastUpdateTime = clock();
        }

    }
    
    SDL_DestroyTexture(background);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_VideoQuit();
    SDL_Quit();

    printf("Exiting program...\n");

    return 0;
}
