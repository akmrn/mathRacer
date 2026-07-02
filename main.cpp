#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <string>

#include "Player.h"
#include "MathBox.h"
#include "Background.h"
#include "constants.h"
#include "menu.h"
#include "AudioManager.h"

void renderText(SDL_Renderer* renderer,
                TTF_Font* font,
                const std::string& text,
                float x,
                float y,
                SDL_Color color)
{
    if (!renderer || !font || text.empty()) {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(
        font,
        text.c_str(),
        text.length(),
        color
    );

    if (!surface) {
        std::cout << "Text render error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) {
        std::cout << "Texture create error: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect dst = {
        x,
        y,
        static_cast<float>(surface->w),
        static_cast<float>(surface->h)
    };

    SDL_RenderTexture(renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void resetGame(Player& player1,
               Player& player2,
               MathBox& mathBox,
               bool& gameOver,
               bool& gameWon,
               bool& policeActivated,
               Uint64& lastTick)
{
    player1.reset(car_x, car_y);
    player2.reset(plice_x, plice_y);
    mathBox.reset();

    gameOver = false;
    gameWon = false;
    policeActivated = false;

    lastTick = SDL_GetTicks();
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() < 0) {
        std::cout << "TTF init error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Math Driver",
        screen_w,
        screen_h,
        0
    );

    if (!window) {
        std::cout << "Window create error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer) {
        std::cout << "Renderer create error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    AudioManager audio;

    if (!audio.loadMusic("assets/background.wav")) {
        std::cout << "Warning: background music could not be loaded." << std::endl;
    }

    TTF_Font* gameFont = TTF_OpenFont(
        "fonts/BlackOpsOne-Regular.ttf",
        48
    );

    if (!gameFont) {
        std::cout << "Game font load error: " << SDL_GetError() << std::endl;
    }

    TTF_Font* smallFont = TTF_OpenFont(
        "fonts/BlackOpsOne-Regular.ttf",
        20
    );

    if (!smallFont) {
        std::cout << "Small font load error: " << SDL_GetError() << std::endl;
    }

    Background background(
        renderer,
        "assets/BG.png",
        0.0f,
        0.0f,
        static_cast<float>(screen_w),
        static_cast<float>(screen_h)
    );

    Player player1(
        renderer,
        "assets/chevy.png",
        car_x,
        car_y
    );

    Player player2(
        renderer,
        "assets/police.png",
        plice_x,
        plice_y
    );

    MathBox mathBox(
        renderer,
        "assets/Box.png",
        "fonts/BlackOpsOne-Regular.ttf",
        220.0f,
        400.0f,
        360.0f,
        190.0f
    );

    Menu gameMenu(renderer);
    Scene currentScene = Scene::MENU;

    bool running = true;
    bool gameOver = false;
    bool gameWon = false;
    bool policeActivated = false;

    Uint64 last = SDL_GetTicks();

    while (running) {
        if (currentScene == Scene::MENU) {
            Scene nextScene = gameMenu.handleEvents();

            if (nextScene == Scene::GAME) {
                resetGame(
                    player1,
                    player2,
                    mathBox,
                    gameOver,
                    gameWon,
                    policeActivated,
                    last
                );

                audio.playMusic();
            }

            currentScene = nextScene;

            if (currentScene == Scene::QUIT) {
                running = false;
                continue;
            }

            gameMenu.render(renderer);
            last = SDL_GetTicks();
        }
        else if (currentScene == Scene::GAME) {
            Uint64 now = SDL_GetTicks();
            float deltaTime = static_cast<float>(now - last) / 1000.0f;
            last = now;

            SDL_Event event;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = false;
                }

                if (event.type == SDL_EVENT_KEY_DOWN) {
                    if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                        audio.stopMusic();
                        currentScene = Scene::MENU;
                        continue;
                    }

                    if (event.key.scancode == SDL_SCANCODE_R && (gameOver || gameWon)) {
                        resetGame(
                            player1,
                            player2,
                            mathBox,
                            gameOver,
                            gameWon,
                            policeActivated,
                            last
                        );

                        audio.playMusic();
                        continue;
                    }
                }

                if (!gameOver && !gameWon) {
                    int result = mathBox.handleEvent(event);

                    if (result == 1) {
                        player1.moveForward(45.0f);

                        if (!policeActivated) {
                            policeActivated = true;
                        }
                    }
                }
            }

            if (!gameOver && !gameWon) {
                player1.update(deltaTime);
                player2.update(deltaTime);
                mathBox.update(deltaTime);

                if (policeActivated) {
                    player2.moveToward(player1, deltaTime);
                }

                if (player1.checkCollision(player2)) {
                    gameOver = true;
                }

                if (player1.reachedGoal(finishLineX)) {
                    gameWon = true;
                }
            }

            SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
            SDL_RenderClear(renderer);

            background.render(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

            SDL_FRect finishLine = {
                finishLineX,
                finishLineY,
                finishLineW,
                finishLineH
            };

            SDL_RenderFillRect(renderer, &finishLine);

            player1.render(renderer);
            player2.render(renderer);

            if (!gameOver && !gameWon) {
                mathBox.render(renderer);
            }

            if (gameOver) {
                renderText(
                    renderer,
                    gameFont,
                    "You Lost!",
                    260.0f,
                    220.0f,
                    {255, 40, 40, 255}
                );

                renderText(
                    renderer,
                    smallFont,
                    "Press R to Restart",
                    280.0f,
                    290.0f,
                    {255, 255, 255, 255}
                );
            }

            if (gameWon) {
                renderText(
                    renderer,
                    gameFont,
                    "You Win!",
                    270.0f,
                    220.0f,
                    {40, 255, 120, 255}
                );

                renderText(
                    renderer,
                    smallFont,
                    "Press R to Play Again",
                    270.0f,
                    290.0f,
                    {255, 255, 255, 255}
                );
            }

            if (!gameOver && !gameWon) {
                renderText(
                    renderer,
                    smallFont,
                    "Reach the yellow finish line before the police catches you",
                    80.0f,
                    370.0f,
                    {255, 255, 255, 255}
                );
            }

            SDL_RenderPresent(renderer);
        }
    }

    if (gameFont) {
        TTF_CloseFont(gameFont);
        gameFont = nullptr;
    }

    if (smallFont) {
        TTF_CloseFont(smallFont);
        smallFont = nullptr;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}
