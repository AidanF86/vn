#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 100000
#define CHARACTERS_PER_LINE 50
#define NEW_PAGE_CHARACTER '@'

static int MAX_LINE_LENGTH = 300;

FILE *fp;
char* filename = "input.txt";
char* text;
int* lineBeginnings;
int* lineLengths;
int textSize = 0;
int lines;
int* pageBeginnings;
int pages = 1;

int loadFile()
{
    // Open file for reading
    printf("Opening file %s...\n", filename);
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("Could not open file %s. Are you sure that it exists in this directory?\n", filename);
        return 1;
    }
    printf("File successfully opened, loading text...\n");
    
    lines = 1;
    // Get text length and line count
    char ch;
    char prevChar = ' ';
    while((ch = fgetc(fp)) != EOF)
    {
        if(ch == 'n')
        {
            if(prevChar != NEW_PAGE_CHARACTER)
            {
                lines++;
            }
        }
        else if(ch == NEW_PAGE_CHARACTER)
        {
            pages++;
        }
        else
        {
            textSize++;
        }
    }

    lineBeginnings = (int*)malloc(lines * sizeof(int));
    pageBeginnings = (int*)malloc(pages * sizeof(int));
    lineLengths = (int*)malloc(lines * sizeof(int));

    char textLoading[textSize];

    rewind(fp);
    int index = 0;
    lines = 1;
    pages = 1;
    lineBeginnings[0] = 0;
    pageBeginnings[0] = 0;
    pageLocationLines[0] = 0;
    int lineLength = 0;

    // Get text and line sizes and beginning points
    while((ch = fgetc(fp)) != EOF)
    {
        textLoading[index] = ch;
        if(ch == '\n')
        {
            if(index + 1 < textSize)
            {
                lineLengths[lines - 1] = lineLength / CHARACTERS_PER_LINE + 1;
                lineLength = -1;
                lineBeginnings[lines] = index + 1;
                lines++;
            }
        }
        else if(ch == '$')
        {
            pageBeginnings[pages] = index;
            pageLocationLines[pages] = lines;
            if((ch = fgetc(fp)) != '\n')
            {
                fpos_t* temp;
                fseek(fp, -1, fgetpos(fp, temp));
            }
            index--;
            pages++;
        }
        lineLength++;
        index++;
    }
    lineLengths[lines - 1] = lineLength / CHARACTERS_PER_LINE + 1;

    // Copy text buffer to text
    text = (char*)malloc((textSize + 1) * sizeof(char));
    strcpy(text, textLoading);

    return 0;
}
