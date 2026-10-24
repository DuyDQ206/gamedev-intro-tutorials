#include "Player.h"
#include "Sprite.h"

CPlayer::CPlayer(float x, float y)
    : CGameObject(x, y)
{
    vx = 0.0f;
    vy = 0.0f;
    health = PLAYER_MAX_HEALTH;
    dead = false;
    lastShootTime = 0;
    lastDamageTime = 0;
}

CPlayer::~CPlayer()
{
    for (size_t i = 0; i < bullets.size(); i++)
    {
        delete bullets[i];
    }
    bullets.clear();
}

void CPlayer::Reset()
{
    health = PLAYER_MAX_HEALTH;
    dead = false;
    lastShootTime = 0;
    lastDamageTime = 0;
    x = SCREEN_WIDTH / 2.0f;
    y = SCREEN_HEIGHT - 60.0f;
    vx = 0.0f;
    vy = 0.0f;

    for (size_t i = 0; i < bullets.size(); i++)
    {
        delete bullets[i];
    }
    bullets.clear();
}

void CPlayer::Move(float dx, float dy)
{
    if (dead)
    {
        vx = 0.0f;
        vy = 0.0f;
        return;
    }
    vx = dx * PLAYER_SPEED;
    vy = dy * PLAYER_SPEED;
}

void CPlayer::Shoot()
{
    if (dead)
        return;

    DWORD now = GetTickCount();
    if (now - lastShootTime >= 180) // Cooldown 180ms
    {
        // Ban dan tu chinh giua mui tau cua nguoi choi
        float bulletX = x;
        float bulletY = y - (PLAYER_HEIGHT + BULLET_HEIGHT) / 2.0f;

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
        if (bullets[i]->IsOffScreen() || bullets[i]->IsDeleted())
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
    if (!dead)
    {
        float elapsed = dt / 1000.0f;

        x += vx * elapsed;
        y += vy * elapsed;

        // Gioi han phi thuyen trong man hinh (x, y la toa do tam cua tau)
        float halfW = PLAYER_WIDTH / 2.0f;
        float halfH = PLAYER_HEIGHT / 2.0f;

        if (x < halfW)
            x = halfW;
        if (x > SCREEN_WIDTH - halfW)
            x = SCREEN_WIDTH - halfW;

        if (y < halfH)
            y = halfH;
        if (y > SCREEN_HEIGHT - halfH)
            y = SCREEN_HEIGHT - halfH;
    }

    UpdateBullets(dt);
}

void CPlayer::Render()
{
    if (!dead)
    {
        DWORD now = GetTickCount();
        bool isFlashing = (now - lastDamageTime < PLAYER_INVULNERABLE_TIME);

        // Nhap nhay khi bat tu
        if (!isFlashing || ((now / 100) % 2 != 0))
        {
            CSprites* sprites = CSprites::GetInstance();
            LPSPRITE sprite = sprites->Get(ID_SPRITE_PLAYER);

            if (sprite != nullptr)
                sprite->Draw(x, y);
        }
    }

    RenderBullets();
}

void CPlayer::TakeDamage(int damage)
{
    if (dead || damage <= 0)
        return;

    DWORD now = GetTickCount();
    if (now - lastDamageTime < PLAYER_INVULNERABLE_TIME)
        return;

    health -= damage;
    lastDamageTime = now;

    if (health <= 0)
    {
        health = 0;
        dead = true;
    }
}

bool CPlayer::IsDead() const
{
    return dead;
}

bool CPlayer::IsInvulnerable() const
{
    if (dead)
        return false;
    return (GetTickCount() - lastDamageTime < PLAYER_INVULNERABLE_TIME);
}

int CPlayer::GetHealth() const
{
    return health;
}
