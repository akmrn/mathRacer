#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL3/SDL.h>

class Background
{
public:
    Background(SDL_Renderer* renderer,
               const char* texturePath,
               float x,
               float y,
               float w,
               float h);

    ~Background();

    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* texture;
    SDL_FRect rect;
};

#endif
