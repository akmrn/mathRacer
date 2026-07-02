#include "Player.h"

#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <cmath>

Player::Player(SDL_Renderer* renderer,
               const char* texturePath,
               float x,
               float y)
{
    texture = IMG_LoadTexture(renderer, texturePath);

    if (!texture)
    {
        std::cout << "Failed to load player texture from: " << texturePath << std::endl;
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }

    rect.x = x;
    rect.y = y;
    rect.w = 90.0f;
    rect.h = 55.0f;

    speed = 60.0f;
}

Player::~Player()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Player::update(float deltaTime)
{
    /*
        فعلاً کنترل مستقیم کیبورد برای playerها غیرفعال است،
        چون ماشین اصلی فقط با جواب درست جلو می‌رود
        و پلیس هم با moveToward تعقیب می‌کند.
    */
}

void Player::render(SDL_Renderer* renderer)
{
    if (texture)
    {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Player::moveForward(float amount)
{
    rect.x += amount;
}

void Player::moveToward(const Player& target, float deltaTime)
{
    float targetX = target.getX();
    float targetY = target.getY();

    float dx = targetX - rect.x;
    float dy = targetY - rect.y;

    float length = std::sqrt(dx * dx + dy * dy);

    if (length > 0.0f)
    {
        dx /= length;
        dy /= length;

        rect.x += dx * speed * deltaTime;
        rect.y += dy * speed * deltaTime;
    }
}

bool Player::checkCollision(const Player& other) const
{
    SDL_FRect otherRect;
    otherRect.x = other.getX();
    otherRect.y = other.getY();
    otherRect.w = rect.w;
    otherRect.h = rect.h;

    return SDL_HasRectIntersectionFloat(&rect, &otherRect);
}

bool Player::reachedGoal(float finishLineX) const
{
    return rect.x + rect.w >= finishLineX;
}

void Player::reset(float x, float y)
{
    rect.x = x;
    rect.y = y;
}

float Player::getX() const
{
    return rect.x;
}

float Player::getY() const
{
    return rect.y;
}
