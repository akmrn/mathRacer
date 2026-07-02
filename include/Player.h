#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>

class Player
{
public:
    Player(SDL_Renderer* renderer,
           const char* texturePath,
           float x,
           float y);

    ~Player();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    void moveForward(float amount);
    void moveToward(const Player& target, float deltaTime);

    bool checkCollision(const Player& other) const;
    bool reachedGoal(float finishLineX) const;

    void reset(float x, float y);

    float getX() const;
    float getY() const;

private:
    SDL_Texture* texture;
    SDL_FRect rect;

    float speed;
};

#endif
