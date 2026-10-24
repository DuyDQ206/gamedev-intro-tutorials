#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "Player.h"

extern CMario* mario;
extern CPlayer* player;
extern void ResetGame();

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (player != nullptr)
			player->Shoot();
		break;
	case DIK_R:
		if (player != nullptr && player->IsDead())
			ResetGame();
		break;
	case DIK_S:
		if (mario != nullptr)
			mario->SetState(MARIO_STATE_JUMP);
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_S:
		if (mario != nullptr)
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		if (mario != nullptr)
			mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();

	if (player == NULL)
		return;

	float dx = 0.0f;
	float dy = 0.0f;

	if (game->IsKeyDown(DIK_LEFT) ||
		game->IsKeyDown(DIK_A))
		dx -= 1.0f;

	if (game->IsKeyDown(DIK_RIGHT) ||
		game->IsKeyDown(DIK_D))
		dx += 1.0f;

	if (game->IsKeyDown(DIK_UP) ||
		game->IsKeyDown(DIK_W))
		dy -= 1.0f;

	if (game->IsKeyDown(DIK_DOWN) ||
		game->IsKeyDown(DIK_S))
		dy += 1.0f;

	// Chuan hoa toc do di cheo
	if (dx != 0.0f && dy != 0.0f)
	{
		dx *= 0.70710678f;
		dy *= 0.70710678f;
	}

	player->Move(dx, dy);

	if (game->IsKeyDown(DIK_SPACE))
	{
		player->Shoot();
	}
}
