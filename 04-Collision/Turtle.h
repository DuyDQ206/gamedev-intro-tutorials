#pragma once
#include "GameObject.h"

#define TURTLE_GRAVITY 0.002f
#define TURTLE_WALKING_SPEED 0.05f


#define TURTLE_BBOX_WIDTH 16
#define TURTLE_BBOX_HEIGHT 14
#define TURTLE_BBOX_HEIGHT_DIE 7

#define TURTLE_DIE_TIMEOUT 500

#define TURTLE_STATE_WALKING 100
#define TURTLE_STATE_DIE 200

#define ID_ANI_TURTLE_WALKING 5100
#define ID_ANI_TURTLE_DIE 5101

class CTurtle : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CTurtle(float x, float y);
	virtual void SetState(int state);
};