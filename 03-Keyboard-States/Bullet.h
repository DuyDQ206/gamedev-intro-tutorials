#pragma once

#include "GameObject.h"
#include "Sprites.h"

// ID sprite cua vien dan
#define ID_SPRITE_BULLET 30002

// Kich thuoc vien dan
#define BULLET_WIDTH 6.0f
#define BULLET_HEIGHT 24.0f

// Toc do dan, pixel/giay
#define BULLET_SPEED 550.0f

class CBullet : public CGameObject
{
public:
    CBullet(float x, float y);

    void Update(DWORD dt) override;
    void Render() override;

    // Kiem tra dan da bay khoi man hinh chua
    bool IsOffScreen();
};
