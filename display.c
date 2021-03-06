void renderCharacter(char ch, int row, int charInRow, SDL_Renderer *renderer)
{
    double xOffset = -XEDGE + BOXPADDINGX + TEXTPADDINGX + (double)charInRow * (TEXTWIDTH + CHARACTERSPACING);
    double yOffset = YEDGE - (BOXPADDINGY + TEXTPADDINGY + (double)row * (TEXTHEIGHT * LINESPACING));
    SDL_Rect character = {getPixelX(xOffset), getPixelY(yOffset), getLengthX(TEXTWIDTH), getLengthY(TEXTHEIGHT)};
    if((int)ch - 20 > 0 && (int)ch - 20 < 128 - 20)
    {
        SDL_RenderCopy(renderer, glyphs[(int)ch - 20], NULL, &character);
    }
    else
    {
        SDL_RenderCopy(renderer, glyphs[0], NULL, &character);
    }
}

void advanceLineRendering(SDL_Renderer *renderer, bool renderChange)
{
    if(charInLine < lineLength)
    {
        if(ch == ' ')
        {
            for(nextSpace = 1; nextSpace + charInLine < lineLength && text[nextSpace + textIndex] != ' '; nextSpace++) {}
            if(nextSpace > CHARACTERS_PER_LINE - charInRow)
            {
                charInRow = 0;
                currentRow++;
                if(charInRow < lineLength - 1)
                {
                    textIndex++;
                    charInLine++;
                    ch = text[textIndex];
                }
            }
        }
        if(charInRow >= CHARACTERS_PER_LINE)
        {
            charInRow = 0;
            currentRow++;
        }
        renderCharacter(ch, currentRow, charInRow, renderer);
        textIndex++;
        charInRow++;
        ch = text[textIndex];
        charInLine++;
        if(renderChange)
            SDL_RenderPresent(renderer);
    }
    else
    {
        nextLineRequested = false;
        mode = textFinished;
    }
}

void finishLineRendering(SDL_Renderer *renderer)
{
    while(charInLine < lineLength)
    {
        advanceLineRendering(renderer, false);
    }
    //SDL_RenderPresent(renderer);
    nextLineRequested = false;
    mode = textFinished;
    finishLine = false;
}

void renderLine(SDL_Renderer *renderer, int line, int speed)
{
    if(speed == -1)
    {
        lineBeginning = lineBeginnings[line];
        charInRow = 0;
        textIndex = lineBeginning;
        ch = text[textIndex];
        lineLength = getLineLength(line);
        nextSpace = -1;
        for(charInLine = 0; charInLine < lineLength; charInLine++)
        {
            if(ch == ' ')
            {
                for(nextSpace = 1; nextSpace + charInLine < lineLength && text[nextSpace + textIndex] != ' '; nextSpace++) {}
                if(nextSpace > CHARACTERS_PER_LINE - charInRow)
                {
                    charInRow = 0;
                    currentRow++;
                    if(charInRow < lineLength - 1)
                    {
                        textIndex++;
                        charInLine++;
                        ch = text[textIndex];
                    }
                }
            }
            if(charInRow >= CHARACTERS_PER_LINE)
            {
                charInRow = 0;
                currentRow++;
            }
            renderCharacter(ch, currentRow, charInRow, renderer);
            textIndex++;
            charInRow++;
            ch = text[textIndex];
        }
    }
    else
    {
        lineBeginning = lineBeginnings[line];
        charInRow = 0;
        charInLine = 0;
        textIndex = lineBeginning;
        ch = text[textIndex];
        lineLength = getLineLength(line);
        nextSpace = -1;
        lineRenderSpeed = speed;
        mode = textRendering;
    }
}

void renderPagePart(SDL_Renderer *renderer, int page, int numLines)
{
    currentRow = 0;
    int startingPoint = pageBeginnings[page];
    int endingPoint = startingPoint + numLines;
    for(int i = startingPoint; i < endingPoint; i++)
    {
        renderLine(renderer, i, -1);
        currentRow++;
    }
}

void renderPage(SDL_Renderer *renderer, int page)
{
    //printf("Rendering page %d\n", page);
    renderPagePart(renderer, page, getPageLength(page));
    currentRow = 0;
    //printf("Page %d rendered\n", page);
}

void renderNextLine(SDL_Renderer *renderer)
{
    int currentPageLength = getPageLength(currentPage);
    if(lineInPage >= currentPageLength)
    {
        currentPage++;
        lineInPage = 0;
        currentPageLength = getPageLength(currentPage);
        lineIndex++;
    }
    if(currentPage < pages)
    {
        //printf("Current page: %d, length: %d, line in page: %d\n", currentPage, currentPageLength, lineInPage);
        renderPagePart(renderer, currentPage, lineInPage);
        renderLine(renderer, pageBeginnings[currentPage] + lineInPage, DEFAULT_SPEED);
        lineIndex++;
        lineInPage++;
    }
    else
    {
        printf("No more pages!\n");
    }
}

int getPixelX(double coordinateX)
{
    return (double)(actualWindowWidth - windowWidth) / 2 + (double)(coordinateX / XEDGE + 1) / 2.0 * (double)windowWidth;
}

int getPixelY(double coordinateY)
{
    return (double)(actualWindowHeight - windowHeight) / 2 + (double)(-1.0 * coordinateY / YEDGE + 1) / 2.0 * (double)windowHeight;
}

int getLengthX(double lengthX)
{
    return (double)(lengthX / XUNITS) * (double)windowWidth;
}

int getLengthY(double lengthY)
{
    return (double)(lengthY / YUNITS) * (double)windowHeight;
}
