#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAXLINECOUNT 8
#define MAXLINELENGTH 55

int location;  // Location in the text[]
int textSize = 0;
char text[];

void addText(char *text, int textLength)
{
    printf("Adding text of length %d: '%s'\n", textLength, text);
    int lineCount = textLength / MAXLINELENGTH + 1;
    if(lineCount == 0)
       lineCount = 1;
    //printf("%d\n", lineCount);

    charInInput = 0;
    for(int a = 0; a < lineCount; a++)
    {
        if(currentLine < MAXLINECOUNT + 1)
        {
            int length;
            for(int i = 0; i < MAXLINELENGTH && charInInput < textLength; i++)
            {
                //printf("Adding character '%c'\n", &text[charInInput]);
                textBuffer[currentLine][i] = &text[charInInput];
                charInInput++;
                currentChar++;
                length = i;
                //printf("Character successfully added\n");
            }
            lineEndings[currentLine] = length;
            //printf("Outta here at %d\n", charInInput);
        }
        currentLine++;
    }
    printf("Text successfully added\n");
}

void addLine(char *text, int textLength)
{
    charInInput = 0;
    printf("Adding line of length %d: '%s'\n", textLength, text);
    for(int i = 0; i < textLength; i++)
    {
        textBuffer[currentLine][i] = &text[charInInput];
        charInInput++;
    }
    lineEndings[currentLine] = textLength;
    currentLine++;
    printf("Line successfully added\n");
}

void clear()
{
    currentLine = 0;
    currentChar = 0;
}
