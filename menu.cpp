#include "menu.h"
#include <iostream>

Menu::Menu(SDL_Renderer* renderer) {
    font = TTF_OpenFont("fonts/Bitcount.ttf", 38);
    backgroundTex = IMG_LoadTexture(renderer, "assets/menuART.png");

    std::vector<std::string> labels = {"Start Game", "Quit"};
    float startY = 200;

    for (size_t i = 0; i < labels.size(); i++) {
        Button btn;
        btn.rect = {500, startY + i * 100, 280, 60};
        btn.label = labels[i];
        btn.isHovered = false;

        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surf = TTF_RenderText_Blended(font, btn.label.c_str(), 0, white);
        btn.textTexture = SDL_CreateTextureFromSurface(renderer, surf);
        btn.textRect = {
            btn.rect.x + (btn.rect.w - surf->w) * 0.5f,
            btn.rect.y + (btn.rect.h - surf->h) * 0.5f,
            (float)surf->w,
            (float)surf->h
        };
        SDL_DestroySurface(surf);
        buttons.push_back(btn);
    }
}

Menu::~Menu() {
    for (auto& btn : buttons) SDL_DestroyTexture(btn.textTexture);
    SDL_DestroyTexture(backgroundTex);
    TTF_CloseFont(font);
}

Scene Menu::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) return Scene::QUIT;

        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            for (auto& btn : buttons) {
                btn.isHovered = (event.motion.x >= btn.rect.x && event.motion.x <= btn.rect.x + btn.rect.w &&
                                 event.motion.y >= btn.rect.y && event.motion.y <= btn.rect.y + btn.rect.h);
            }
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            for (auto& btn : buttons) {
                if (btn.isHovered) {
                    if (btn.label == "Start Game") return Scene::GAME;
                    if (btn.label == "Quit") return Scene::QUIT;
                }
            }
        }
    }
    return Scene::MENU;
}

void Menu::render(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);
    SDL_FRect bgRect = {0, 0, 800, 600};
    SDL_RenderTexture(renderer, backgroundTex, NULL, &bgRect);

    for (auto& btn : buttons) {
        if (btn.isHovered) SDL_SetRenderDrawColor(renderer, 140, 140, 255, 255);
        else SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);

        SDL_RenderFillRect(renderer, &btn.rect);
        SDL_RenderTexture(renderer, btn.textTexture, NULL, &btn.textRect);
    }
    SDL_RenderPresent(renderer);
}
