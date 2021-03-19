#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 100000
#define NEW_PAGE_CHARACTER '@'
#define COMMAND_CHARACTER '#'
#define MAX_LINE_LENGTH = 300;

char* text;
int* lineBeginnings;
int textLength;
int lines;
int* pageBeginnings;
int pages;
char* chapterFiles[1];
char** commands;
int* commandLocations;
int commandCount;
int nextCommandIndex;

int getLineLength(int);
int getPageLength(int);
int loadChapterFile(int);
int save(int);
int load(int);
