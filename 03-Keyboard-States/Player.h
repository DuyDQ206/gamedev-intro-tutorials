#pragma once

#include "GameObject.h"
#include "Sprites.h"
#include <vector>
#include "Bullet.h"

// Kich thuoc man hinh game
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 720

// Toc do di chuyen, don vi pixel/giay
#define PLAYER_SPEED 300.0f
#define ID_SPRITE_PLAYER 30001

class CPlayer : public CGameObject
{
private:
    std::vector<CBullet*> bullets;
    DWORD lastShootTime = 0;
public:
    CPlayer(float x, float y);
    virtual ~CPlayer();

    void Update(DWORD dt) override;
    void Render() override;
    void Shoot();
    void UpdateBullets(DWORD dt);
    void RenderBullets();
    // Di chuyen theo hai truc
    void Move(float dx, float dy);
    std::vector<CBullet*>& GetBullets() { return bullets; }
};
