
#include "Enemy.h"
#include "Sprite.h"
#include "GameObject.h"
CEnemy::CEnemy(float x, float y): CGameObject(x, y)
{
    vx = 0.0f;
    vy = ENEMY_SPEED;
}

void CEnemy::Update(DWORD dt)
{
    float elapsed = dt / 1000.0f;

    x += vx * elapsed;
    y += vy * elapsed;
}

void CEnemy::Render()
{
    CSprites* sprites = CSprites::GetInstance();

    LPSPRITE sprite =
        sprites->Get(ID_SPRITE_ENEMY_SHIP);

    if (sprite != nullptr)
        sprite->Draw(x, y);
}

bool CEnemy::IsOffScreen()
{
    return y > SCREEN_HEIGHT;
}
