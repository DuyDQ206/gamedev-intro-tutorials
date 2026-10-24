#pragma once

#include "GameObject.h"
#include "Sprites.h"
#include <vector>
#include "Bullet.h"

// Kich thuoc man hinh game
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 640

// Kich thuoc sprite Player
#define PLAYER_WIDTH 41.0f
#define PLAYER_HEIGHT 31.0f

// Toc do di chuyen, don vi pixel/giay
#define PLAYER_SPEED 320.0f
#define ID_SPRITE_PLAYER 30001
#define PLAYER_MAX_HEALTH 3
#define PLAYER_INVULNERABLE_TIME 1500 // 1.5s bat tu sau khi dinh don

class CPlayer : public CGameObject
{
private:
    std::vector<CBullet*> bullets;
    DWORD lastShootTime = 0;
    DWORD lastDamageTime = 0;
    int health = PLAYER_MAX_HEALTH;
    bool dead = false;
public:
    CPlayer(float x, float y);
    virtual ~CPlayer();

    void Update(DWORD dt) override;
    void Render() override;
    void Shoot();
    void UpdateBullets(DWORD dt);
    void RenderBullets();
    void TakeDamage(int damage);
    bool IsDead() const;
    bool IsInvulnerable() const;
    int GetHealth() const;
    void Reset();

    // Di chuyen theo hai truc
    void Move(float dx, float dy);
    std::vector<CBullet*>& GetBullets() { return bullets; }
};
