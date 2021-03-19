#include "clock.h"
#include "display.h"

enum modes{textFinished, textRendering};
enum modes mode;

void processKeyDownTextFinished(SDL_Event);
void processKeyDownTextRendering(SDL_Event);
