#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

using namespace std;

class CGameObject
{
protected:
	float x; 
	float y;

	float vx;
	float vy;

	int nx;	 

	int state;
	bool isDeleted = false;									
public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }

	float GetX() const { return x; }
	float GetY() const { return y; }
	void GetPosition(float& x, float& y) const { x = this->x; y = this->y; }

	void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }

	virtual void Delete() { isDeleted = true; }
	bool IsDeleted() const { return isDeleted; }

	CGameObject();
	CGameObject(float x, float y):CGameObject() { this->x = x; this->y = y; }

	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual ~CGameObject();
};

typedef CGameObject* LPGAMEOBJECT;
