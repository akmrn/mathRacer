#include "MathBox.h"

#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <cmath>

MathBox::MathBox(SDL_Renderer* renderer,
                 const char* boxTexturePath,
                 const char* fontPath,
                 float x,
                 float y,
                 float w,
                 float h)
    : boxTexture(nullptr),
      font(nullptr),
      smallFont(nullptr),
      correctAnswer(0),
      selectedAnswer(-1)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    boxTexture = IMG_LoadTexture(renderer, boxTexturePath);
    if (!boxTexture)
    {
        std::cout << "Failed to load math box texture from: " << boxTexturePath << std::endl;
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }

    font = TTF_OpenFont(fontPath, 24);
    if (!font)
    {
        std::cout << "Failed to load main font: " << fontPath << std::endl;
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }

    smallFont = TTF_OpenFont(fontPath, 20);
    if (!smallFont)
    {
        std::cout << "Failed to load small font: " << fontPath << std::endl;
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }

    static bool seeded = false;
    if (!seeded)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    generateQuestion();
}

MathBox::~MathBox()
{
    if (boxTexture)
    {
        SDL_DestroyTexture(boxTexture);
        boxTexture = nullptr;
    }

    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (smallFont)
    {
        TTF_CloseFont(smallFont);
        smallFont = nullptr;
    }
}

void MathBox::update(float deltaTime)
{
    (void)deltaTime;
    // فعلاً چیزی برای آپدیت زمانی لازم نیست.
}

void MathBox::render(SDL_Renderer* renderer)
{
    if (boxTexture)
    {
        SDL_RenderTexture(renderer, boxTexture, nullptr, &rect);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 25, 25, 35, 230);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &rect);
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 230, 80, 255};

    renderText(renderer, questionText, rect.x + 30.0f, rect.y + 18.0f, white);

    for (int i = 0; i < 4; i++)
    {
        SDL_SetRenderDrawColor(renderer, 55, 55, 95, 235);
        SDL_RenderFillRect(renderer, &answerRects[i]);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &answerRects[i]);

        std::string answerText = std::to_string(answers[i]);
        renderText(renderer,
                   answerText,
                   answerRects[i].x + 20.0f,
                   answerRects[i].y + 8.0f,
                   yellow);
    }
}

int MathBox::handleEvent(const SDL_Event& event)
{
    if (event.type != SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        return 0;
    }

    float mouseX = static_cast<float>(event.button.x);
    float mouseY = static_cast<float>(event.button.y);

    for (int i = 0; i < 4; i++)
    {
        const SDL_FRect& r = answerRects[i];

        bool inside =
            mouseX >= r.x &&
            mouseX <= r.x + r.w &&
            mouseY >= r.y &&
            mouseY <= r.y + r.h;

        if (inside)
        {
            selectedAnswer = answers[i];
            bool isCorrect = (selectedAnswer == correctAnswer);

            generateQuestion();

            return isCorrect ? 1 : -1;
        }
    }

    return 0;
}

void MathBox::reset()
{
    selectedAnswer = -1;
    generateQuestion();
}

void MathBox::generateQuestion()
{
    int a = 1 + std::rand() % 10;
    int b = 1 + std::rand() % 10;

    int operation = std::rand() % 3;

    switch (operation)
    {
        case 0:
        {
            correctAnswer = a + b;
            questionText = std::to_string(a) + " + " + std::to_string(b) + " = ?";
            break;
        }

        case 1:
        {
            if (b > a)
            {
                std::swap(a, b);
            }

            correctAnswer = a - b;
            questionText = std::to_string(a) + " - " + std::to_string(b) + " = ?";
            break;
        }

        case 2:
        default:
        {
            correctAnswer = a * b;
            questionText = std::to_string(a) + " x " + std::to_string(b) + " = ?";
            break;
        }
    }

    answers.clear();
    answers.push_back(correctAnswer);

    while (answers.size() < 4)
    {
        int fakeAnswer = correctAnswer + (std::rand() % 21 - 10);

        if (fakeAnswer < 0)
        {
            fakeAnswer = std::abs(fakeAnswer);
        }

        bool duplicate = false;
        for (int value : answers)
        {
            if (value == fakeAnswer)
            {
                duplicate = true;
                break;
            }
        }

        if (!duplicate)
        {
            answers.push_back(fakeAnswer);
        }
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(answers.begin(), answers.end(), gen);

    float buttonW = 130.0f;
    float buttonH = 42.0f;

    float startX = rect.x + 40.0f;
    float startY = rect.y + 80.0f;

    answerRects[0] = {startX,               startY,               buttonW, buttonH};
    answerRects[1] = {startX + 150.0f,      startY,               buttonW, buttonH};
    answerRects[2] = {startX,               startY + 55.0f,       buttonW, buttonH};
    answerRects[3] = {startX + 150.0f,      startY + 55.0f,       buttonW, buttonH};
}

void MathBox::renderText(SDL_Renderer* renderer,
                         const std::string& text,
                         float x,
                         float y,
                         SDL_Color color)
{
    TTF_Font* activeFont = smallFont ? smallFont : font;

    if (!activeFont || text.empty())
    {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(activeFont,
                                                  text.c_str(),
                                                  text.length(),
                                                  color);

    if (!surface)
    {
        std::cout << "Failed to create text surface: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        std::cout << "Failed to create text texture: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect dst;
    dst.x = x;
    dst.y = y;
    dst.w = static_cast<float>(surface->w);
    dst.h = static_cast<float>(surface->h);

    SDL_RenderTexture(renderer, texture, nullptr, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}
