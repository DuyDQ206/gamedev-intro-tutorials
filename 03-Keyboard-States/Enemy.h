
#pragma once

#include "GameObject.h"
#include "Sprites.h"

#define ENEMY_SPEED 100.0f
#define ID_SPRITE_ENEMY_SHIP 30003
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 720

class CEnemy : public CGameObject
{
public:
    CEnemy(float x, float y);

    void Update(DWORD dt) override;
    void Render() override;

    bool IsOffScreen();
};
