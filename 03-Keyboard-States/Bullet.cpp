
#include "Bullet.h"
#include "Sprite.h"

CBullet::CBullet(float x, float y)
    : CGameObject(x, y)
{
    // Dan bay thang len tren
    vx = 0.0f;
    vy = -BULLET_SPEED;
}

void CBullet::Update(DWORD dt)
{
    // dt duoc tinh bang milliseconds
    float elapsed = dt / 1000.0f;

    // Cap nhat vi tri vien dan
    x += vx * elapsed;
    y += vy * elapsed;
}

void CBullet::Render()
{
    CSprites* sprites = CSprites::GetInstance();

    LPSPRITE sprite = sprites->Get(ID_SPRITE_BULLET);

    if (sprite != nullptr)
        sprite->Draw(x, y);
}

bool CBullet::IsOffScreen()
{
    // Tam thoi su dung chieu cao man hinh 720 pixel
    return y < -16.0f;
}
