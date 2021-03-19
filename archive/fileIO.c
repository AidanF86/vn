int getLineLength(int line)
{
    if(line < lines)
    {
        if(line == lines - 1)
        {
            return textLength - lineBeginnings[line];
        }
        return lineBeginnings[line + 1] - lineBeginnings[line];
    }
    printf("ERROR: Line %d is out of bounds\n", line);
    return -1;
}

int getPageLength(int page)
{
    if(page < pages)
    {
        if(page == pages - 1)
        {
            return lines - pageBeginnings[page];
        }
        return pageBeginnings[page + 1] - pageBeginnings[page];
    }
    printf("ERROR: Page %d is out of bounds\n", page);
    return -1;
}

int loadChapterFile(int chapterToLoad)
{
    // Open file for reading
    char* filename = chapterFiles[chapterToLoad];
    printf("Opening file %s...\n", filename);
    FILE* fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("Could not open file %s. Are you sure that it exists in this directory?\n", filename);
        return 1;
    }
    printf("File successfully opened, loading text...\n");
    
    lines = 1;
    commandCount = 0;
    // Get text length and line count
    char ch;
    char prevChar = ' ';
    bool inComment = false;
    while((ch = fgetc(fp)) != EOF)
    {
        if(ch == '\n')
        {
            if(prevChar != NEW_PAGE_CHARACTER)
            {
                lines++;
            }
        }
        else if(ch == COMMAND_CHARACTER)
        {
            commandCount++;
            while((ch = fgetc(fp)) != EOF && ch != '\n')
            {
            }
        }
        else if(ch == NEW_PAGE_CHARACTER)
        {
            pages++;
        }
        else
        {
            textLength++;
        }
    }

    lineBeginnings = (int*)malloc(lines * sizeof(int));
    pageBeginnings = (int*)malloc(pages * sizeof(int));
    commands = malloc(commandCount * sizeof(char*));
    commandLocations = malloc(commandCount * sizeof(int*));

    char textLoading[textLength];

    rewind(fp);
    int index = 0;
    lines = 1;
    commandCount = 0;
    pages = 1;
    lineBeginnings[0] = 0;
    pageBeginnings[0] = 0;

    prevChar = ' ';
    // Get text and line sizes and beginning points
    while((ch = fgetc(fp)) != EOF)
    {
        if(ch == '\n')
        {
            if(prevChar != NEW_PAGE_CHARACTER)
            {
                lineBeginnings[lines] = index;
                lines++;
            }
        }
        else if(ch == COMMAND_CHARACTER)
        {
            commandLocations[commandCount] = lines - 1;
            int commandSize = 1;
            while((ch = fgetc(fp)) != EOF && ch != '\n')
            {
                commandSize++;
            }
            fseek(fp, -commandSize, SEEK_CUR);
            commands[commandCount] = malloc(commandSize + 1);
            commandSize = 0;
            while((ch = fgetc(fp)) != EOF && ch != '\n')
            {
                commands[commandCount][commandSize] = ch;
                commandSize++;
            }
            commands[commandCount][commandSize] = '\0';
            commandCount++;
        }
        else if(ch == NEW_PAGE_CHARACTER)
        {
            pageBeginnings[pages] = lines - 1;
            pages++;
        }
        else
        {
            textLoading[index] = ch;
            index++;
        }
        prevChar = ch;
    }

    // Copy text buffer to text
    text = (char*)malloc((textLength + 1) * sizeof(char));
    strcpy(text, textLoading);

    // Debug
    printf("Lines: %d, Pages: %d\n", lines, pages);

    return 0;
}

int save(int saveNumber)
{
    printf("Attempting data save to slot %d...\n", saveNumber);
    char filename[80];
    sprintf(filename, "save%d", saveNumber);
    FILE* fp = fopen(filename, "w");
    if(fp == NULL)
    {
        printf("File %s could not be opened for writing!\n", filename);
        return 1;
    }
    printf("Saving data...\n");
    int data[3] = {currentChapter, currentPage, lineInPage};
    fwrite(data, sizeof(int), sizeof(data), fp);
    printf("Data saved to slot %d!\n", saveNumber);
    return 0;
}

int load(int loadNumber)
{
    printf("Attempting data load from slot %d...\n", loadNumber);
    char filename[80];
    sprintf(filename, "save%d", loadNumber);
    FILE* fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("File %s could not be opened for reading! Are you sure that it exists?\n", filename);
        return 1;
    }
    printf("Loading data...\n");
    int data[3];
    fread(data, sizeof(int), sizeof(data), fp);
    currentChapter = data[0];
    currentPage =  data[1];
    lineInPage = data[2];
    printf("Data from slot %d loaded!\n", loadNumber);
    return 0;
}
