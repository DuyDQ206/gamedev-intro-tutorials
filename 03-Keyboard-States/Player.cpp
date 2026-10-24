#include "Player.h"
#include "Sprite.h"

CPlayer::CPlayer(float x, float y)
    : CGameObject(x, y)
{
    vx = 0.0f;
    vy = 0.0f;
}

CPlayer::~CPlayer()
{
    for (size_t i = 0; i < bullets.size(); i++)
    {
        delete bullets[i];
    }
    bullets.clear();
}

void CPlayer::Move(float dx, float dy)
{
    // Luu huong di chuyen theo tung truc
    vx = dx * PLAYER_SPEED;
    vy = dy * PLAYER_SPEED;
}

void CPlayer::Shoot()
{
    DWORD now = GetTickCount();
    if (now - lastShootTime >= 200) // Cooldown 200ms
    {
        // Player sprite width=43, height=32; Bullet width=7, height=24
        float bulletX = x + (43.0f - 7.0f) / 2.0f;
        float bulletY = y - 24.0f;

        bullets.push_back(new CBullet(bulletX, bulletY));
        lastShootTime = now;
    }
}

void CPlayer::UpdateBullets(DWORD dt)
{
    for (size_t i = 0; i < bullets.size(); i++)
    {
        bullets[i]->Update(dt);
    }

    for (int i = (int)bullets.size() - 1; i >= 0; i--)
    {
        if (bullets[i]->IsOffScreen())
        {
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
        }
    }
}

void CPlayer::RenderBullets()
{
    for (size_t i = 0; i < bullets.size(); i++)
    {
        bullets[i]->Render();
    }
}

void CPlayer::Update(DWORD dt)
{
    // Chuyen doi dt tu milliseconds sang seconds
    float elapsed = dt / 1000.0f;

    x += vx * elapsed;
    y += vy * elapsed;

    // Gioi han phi thuyen trong man hinh
    if (x < 0.0f)
        x = 0.0f;

    if (y < 0.0f)
        y = 0.0f;

    if (x > SCREEN_WIDTH - 43.0f)
        x = SCREEN_WIDTH - 43.0f;

    if (y > SCREEN_HEIGHT - 32.0f)
        y = SCREEN_HEIGHT - 32.0f;

    UpdateBullets(dt);
}

void CPlayer::Render()
{
    CSprites* sprites = CSprites::GetInstance();

    LPSPRITE sprite = sprites->Get(ID_SPRITE_PLAYER);

    if (sprite != nullptr)
        sprite->Draw(x, y);

    RenderBullets();
}
