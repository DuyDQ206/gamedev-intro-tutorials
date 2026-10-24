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

    if (IsOffScreen())
    {
        Delete();
    }
}

void CEnemy::Render()
{
    if (dead)
        return;

    CSprites* sprites = CSprites::GetInstance();

    LPSPRITE sprite = sprites->Get(ID_SPRITE_ENEMY_SHIP);

    if (sprite != nullptr)
        sprite->Draw(x, y);
}

void CEnemy::TakeDamage(int damage)
{
    if (dead || damage <= 0)
        return;

    health -= damage;

    if (health <= 0)
    {
        health = 0;
        dead = true;
        Delete();
    }
}

bool CEnemy::IsDead() const
{
    return dead;
}

bool CEnemy::IsOffScreen()
{
    return y > SCREEN_HEIGHT;
}
