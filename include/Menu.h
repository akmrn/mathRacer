#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>

// وضعیت‌هایی که برنامه می‌تواند داشته باشد
enum class Scene {
    MENU,
    GAME,
    QUIT
};

struct Button {
    SDL_FRect rect;
    SDL_Texture* textTexture;
    SDL_FRect textRect;
    std::string label;
    bool isHovered;
};

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();
    Scene handleEvents(); // مدیریت ورودی‌ها و بازگرداندن وضعیت بعدی
    void render(SDL_Renderer* renderer);

private:
    TTF_Font* font;
    SDL_Texture* backgroundTex;
    std::vector<Button> buttons;
};
