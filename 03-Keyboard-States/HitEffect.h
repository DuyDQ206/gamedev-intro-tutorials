#pragma once

#include "GameObject.h"
#include "Sprites.h"

#define ID_SPRITE_HIT 30004
#define HIT_EFFECT_DURATION 160 // millisecond
#define HIT_EFFECT_WIDTH 34.0f
#define HIT_EFFECT_HEIGHT 19.0f

class CHitEffect : public CGameObject
{
private:
    DWORD spawnTime;
    bool isFinished = false;
public:
    CHitEffect(float x, float y) : CGameObject(x, y)
    {
        spawnTime = GetTickCount();
    }

    void Update(DWORD dt) override
    {
        if (GetTickCount() - spawnTime >= HIT_EFFECT_DURATION)
        {
            isFinished = true;
            Delete();
        }
    }

    void Render() override
    {
        if (!isFinished)
        {
            CSprites* sprites = CSprites::GetInstance();
            LPSPRITE sprite = sprites->Get(ID_SPRITE_HIT);
            if (sprite != nullptr)
                sprite->Draw(x, y);
        }
    }

    bool IsFinished() const { return isFinished; }
};
