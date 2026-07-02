#ifndef MATHBOX_H
#define MATHBOX_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

class MathBox
{
public:
    MathBox(SDL_Renderer* renderer,
            const char* boxTexturePath,
            const char* fontPath,
            float x,
            float y,
            float w,
            float h);

    ~MathBox();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    int handleEvent(const SDL_Event& event);
    void reset();

private:
    void generateQuestion();
    void renderText(SDL_Renderer* renderer,
                    const std::string& text,
                    float x,
                    float y,
                    SDL_Color color);

private:
    SDL_Texture* boxTexture;
    SDL_FRect rect;

    TTF_Font* font;
    TTF_Font* smallFont;

    std::string questionText;
    std::vector<int> answers;
    int correctAnswer;

    SDL_FRect answerRects[4];
    int selectedAnswer;
};

#endif
