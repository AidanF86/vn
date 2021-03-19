void processKeyDownTextFinished(SDL_Event event)
{
    switch(event.key.keysym.sym)
    {
        case SDLK_RETURN:
            nextLineRequested = true;
            break;
        case SDLK_s:
            save(0);
        case SDLK_l:
            load(0);
    }
}

void processKeyDownTextRendering(SDL_Event event)
{
    switch(event.key.keysym.sym)
    {
        case SDLK_RETURN:
            finishLine = true;
            break;
    }
}
