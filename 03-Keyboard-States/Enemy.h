#pragma once

#include "GameObject.h"
#include "Sprites.h"

#define ENEMY_SPEED 120.0f
#define ID_SPRITE_ENEMY_SHIP 30003
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 640

#define ENEMY_WIDTH 39.0f
#define ENEMY_HEIGHT 34.0f

class CEnemy : public CGameObject
{
private:
    int health = 1;
    bool dead = false;
public:
    CEnemy(float x, float y);

    void Update(DWORD dt) override;
    void Render() override;
    void TakeDamage(int damage);
    bool IsDead() const;
    bool IsOffScreen();
};
