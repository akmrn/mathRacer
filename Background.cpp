#include "Background.h"

#include <SDL3_image/SDL_image.h>
#include <iostream>

Background::Background(SDL_Renderer* renderer,
                       const char* texturePath,
                       float x,
                       float y,
                       float w,
                       float h)
{
    texture = IMG_LoadTexture(renderer, texturePath);

    if (!texture)
    {
        std::cout << "Failed to load background texture from: " << texturePath << std::endl;
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        std::cout << "Background loaded successfully: " << texturePath << std::endl;
    }

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

Background::~Background()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Background::render(SDL_Renderer* renderer)
{
    if (texture)
    {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
}
