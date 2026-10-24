/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 03 - KEYBOARD AND OBJECT STATE

	This sample illustrates how to:

		1/ Process keyboard input
		2/ Control object state with keyboard events
================================================================ */

#include <windows.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Animation.h"
#include "Animations.h"
#include "Sprite.h"
#include "Sprites.h"

#include "Mario.h"
#include "Brick.h"

#include "SampleKeyEventHandler.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "HitEffect.h"
#include <cstdlib>
#include <ctime>

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"03 - Space Shooter"
#define WINDOW_ICON_PATH L"mario.ico"


#define BACKGROUND_COLOR D3DXCOLOR(30.0f/255, 30.0f/255, 30.0f/255, 1.0f)

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_PREVIEW 30

#define ID_SPRITE_BRICK 20001

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc.png"
#define TEXTURE_PATH_PREVIEW TEXTURES_DIR "\\preview.png"

#define ID_SPRITE_PLAYER 30001
#define ID_SPRITE_BULLET 30002
#define ID_SPRITE_ENEMY_SHIP 30003
#define ID_SPRITE_HIT 30004

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 720

#define MARIO_START_X 200.0f
#define MARIO_START_Y 10.0f

#define BRICK_X 0.0f
#define BRICK_Y GROUND_Y + 20.0f
#define NUM_BRICKS 50

HWND hMainWnd = NULL;
CMario* mario = NULL;
CPlayer* player = nullptr;

CSampleKeyHandler* keyHandler;

vector<LPGAMEOBJECT> objects;

DWORD lastEnemySpawnTime = 0;
const DWORD ENEMY_SPAWN_INTERVAL = 1000; // 1 giay
int score = 0;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object
*/

void LoadResources()
{
	// 1. Khoi tao cac bo quan ly tai nguyen
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();

	// 2. Tai texture duy nhat: preview.png
	textures->Add(
		ID_TEX_PREVIEW,
		TEXTURE_PATH_PREVIEW
	);

	LPTEXTURE texPreview =
		textures->Get(ID_TEX_PREVIEW);

	// 3. Dang ky sprite Player
	sprites->Add(
		ID_SPRITE_PLAYER,
		75, 50,
		118, 82,
		texPreview
	);

	// 4. Dang ky sprite Bullet
	sprites->Add(
		ID_SPRITE_BULLET,
		76, 226,
		83, 250,
		texPreview
	);

	// 5. Dang ky sprite Enemy
	sprites->Add(
		ID_SPRITE_ENEMY_SHIP,
		75, 463,
		115, 495,
		texPreview
	);

	// 6. Dang ky sprite hieu ung trung dan
	sprites->Add(
		ID_SPRITE_HIT,
		201, 260,
		224, 281,
		texPreview
	);

	// 7. Tao Player
	player = new CPlayer(
		SCREEN_WIDTH / 2.0f - 21.5f,
		SCREEN_HEIGHT - 80.0f
	);

	objects.push_back(player);
}

void ResetGame()
{
	score = 0;
	if (player != nullptr)
	{
		player->Reset();
	}

	for (int i = (int)objects.size() - 1; i >= 0; i--)
	{
		if (objects[i] != player)
		{
			delete objects[i];
			objects.erase(objects.begin() + i);
		}
	}

	lastEnemySpawnTime = GetTickCount();
}

bool CheckCollision(
	float x1, float y1, float w1, float h1,
	float x2, float y2, float w2, float h2)
{
	return x1 < x2 + w2 &&
		x1 + w1 > x2 &&
		y1 < y2 + h2 &&
		y1 + h1 > y2;
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/

void Update(DWORD dt)
{
	// 1. Cap nhat tat ca doi tuong hien tai
	for (int i = 0; i < (int)objects.size(); i++)
	{
		objects[i]->Update(dt);
	}

	// 2. Tao enemy moi moi 1 giay neu player con song
	DWORD now = GetTickCount();

	if (player != nullptr && !player->IsDead())
	{
		if (now - lastEnemySpawnTime >= ENEMY_SPAWN_INTERVAL)
		{
			float enemyX = (float)(rand() % (SCREEN_WIDTH - 40));
			float enemyY = 0.0f;

			CEnemy* enemy = new CEnemy(enemyX, enemyY);
			objects.push_back(enemy);

			lastEnemySpawnTime = now;
		}
	}

	// 3. Xu ly va cham: Dan (Bullet) va cham voi Quai (Enemy)
	if (player != nullptr)
	{
		std::vector<CBullet*>& bullets = player->GetBullets();
		for (size_t b = 0; b < bullets.size(); b++)
		{
			CBullet* bullet = bullets[b];
			if (bullet->IsDeleted() || bullet->IsOffScreen())
				continue;

			for (size_t i = 0; i < objects.size(); i++)
			{
				CEnemy* enemy = dynamic_cast<CEnemy*>(objects[i]);
				if (enemy != nullptr && !enemy->IsDead() && !enemy->IsDeleted())
				{
					// Dan: rong 7, cao 24. Quai: rong 40, cao 32
					if (CheckCollision(bullet->GetX(), bullet->GetY(), 7.0f, 24.0f,
									   enemy->GetX(), enemy->GetY(), 40.0f, 32.0f))
					{
						enemy->TakeDamage(1);
						bullet->Delete();

						// Tao hieu ung trung dan (HitEffect rong 23, cao 21)
						float hitX = enemy->GetX() + (40.0f - 23.0f) / 2.0f;
						float hitY = enemy->GetY() + (32.0f - 21.0f) / 2.0f;
						objects.push_back(new CHitEffect(hitX, hitY));

						score += 10;
						break;
					}
				}
			}
		}
	}

	// 4. Xu ly va cham: Quai (Enemy) va cham voi Nguoi choi (Player)
	if (player != nullptr && !player->IsDead())
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			CEnemy* enemy = dynamic_cast<CEnemy*>(objects[i]);
			if (enemy != nullptr && !enemy->IsDead() && !enemy->IsDeleted())
			{
				// Player: rong 43, cao 32. Enemy: rong 40, cao 32
				if (CheckCollision(player->GetX(), player->GetY(), 43.0f, 32.0f,
								   enemy->GetX(), enemy->GetY(), 40.0f, 32.0f))
				{
					if (!player->IsInvulnerable())
					{
						player->TakeDamage(1);
						enemy->TakeDamage(1);

						// Hieu ung no tai vi tri va cham
						float hitX = (player->GetX() + enemy->GetX()) / 2.0f;
						float hitY = (player->GetY() + enemy->GetY()) / 2.0f;
						objects.push_back(new CHitEffect(hitX, hitY));
					}
				}
			}
		}
	}

	// 5. Xoa enemy da bay khoi man hinh hoac da chet, xoa hit effect da xong
	for (int i = (int)objects.size() - 1; i >= 0; i--)
	{
		LPGAMEOBJECT obj = objects[i];
		if (obj == player)
			continue;

		if (obj->IsDeleted())
		{
			delete obj;
			objects.erase(objects.begin() + i);
		}
	}

	// 6. Cap nhat tieu de cua so de nguoi choi theo doi Mau (HP) va Diem
	if (hMainWnd != NULL && player != nullptr)
	{
		WCHAR title[128];
		if (player->IsDead())
		{
			wsprintf(title, L"GAME OVER! Diem: %d | Nhan [R] de choi lai", score);
		}
		else
		{
			wsprintf(title, L"Space Shooter - Mau (HP): %d/3 | Diem: %d", player->GetHealth(), score);
		}
		SetWindowText(hMainWnd, title);
	}
}


void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

	for (int i = 0; i < (int)objects.size(); i++)
	{
		objects[i]->Render();
	}

	spriteHandler->End();
	pSwapChain->Present(0, 0);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			CGame::GetInstance()->ProcessKeyboard();
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
	hMainWnd = hWnd;

	SetDebugWindow(hWnd);

	CGame* game = CGame::GetInstance();
	game->Init(hWnd, hInstance);

	keyHandler = new CSampleKeyHandler();
	game->InitKeyboard(keyHandler);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	srand((unsigned int)time(NULL));
	LoadResources();

	Run();

	return 0;
}
