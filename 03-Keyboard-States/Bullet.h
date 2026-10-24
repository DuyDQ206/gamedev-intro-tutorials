
#pragma once

#include "GameObject.h"
#include "Sprites.h"

// ID sprite cua vien dan
#define ID_SPRITE_BULLET 30002

// Toc do dan, pixel/giay
#define BULLET_SPEED 500.0f

class CBullet : public CGameObject
{
public:
    CBullet(float x, float y);

    void Update(DWORD dt) override;
    void Render() override;

    // Kiem tra dan da bay khoi man hinh chua
    bool IsOffScreen();
};
