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
    float elapsed = dt / 1000.0f;

    x += vx * elapsed;
    y += vy * elapsed;

    if (IsOffScreen())
    {
        Delete();
    }
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
    return y < -BULLET_HEIGHT;
}
