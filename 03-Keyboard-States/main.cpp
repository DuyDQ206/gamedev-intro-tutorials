/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 03 - KEYBOARD AND OBJECT STATE

	This sample illustrates how to:

		1/ Process keyboard input
		2/ Control object state with keyboard events
================================================================ */

#include <windows.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

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

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Space Shooter"
#define WINDOW_ICON_PATH L"mario.ico"

// Mau nen khong gian vu tru
#define BACKGROUND_COLOR D3DXCOLOR(18.0f/255, 20.0f/255, 32.0f/255, 1.0f)

#define ID_TEX_PREVIEW 30

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_PREVIEW TEXTURES_DIR "\\preview.png"

#define ID_SPRITE_PLAYER 30001
#define ID_SPRITE_BULLET 30002
#define ID_SPRITE_ENEMY_SHIP 30003
#define ID_SPRITE_HIT 30004

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 640

HWND hMainWnd = NULL;
CMario* mario = NULL;
CPlayer* player = nullptr;

CSampleKeyHandler* keyHandler;

vector<LPGAMEOBJECT> objects;

DWORD lastEnemySpawnTime = 0;
const DWORD ENEMY_SPAWN_INTERVAL = 900; // 0.9 giay
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
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();

	// 1. Tai texture preview.png da duoc lam trong suot
	textures->Add(
		ID_TEX_PREVIEW,
		TEXTURE_PATH_PREVIEW
	);

	LPTEXTURE texPreview = textures->Get(ID_TEX_PREVIEW);

	// 2. Dang ky sprite Player (canh chuan doi xung: rong 41, cao 31)
	sprites->Add(
		ID_SPRITE_PLAYER,
		72, 50,
		112, 80,
		texPreview
	);

	// 3. Dang ky sprite Bullet (Laser dung chuan: rong 6, cao 24)
	sprites->Add(
		ID_SPRITE_BULLET,
		72, 230,
		77, 253,
		texPreview
	);

	// 4. Dang ky sprite Enemy (canh chuan doi xung huong xuong: rong 39, cao 34)
	sprites->Add(
		ID_SPRITE_ENEMY_SHIP,
		72, 460,
		110, 493,
		texPreview
	);

	// 5. Dang ky sprite hieu ung trung dan / no (rong 34, cao 19)
	sprites->Add(
		ID_SPRITE_HIT,
		196, 258,
		229, 276,
		texPreview
	);

	// 6. Tao Player tai chinh giua mep duoi man hinh
	player = new CPlayer(
		SCREEN_WIDTH / 2.0f,
		SCREEN_HEIGHT - 60.0f
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

/*
	Kiem tra va cham dua tren toa do tam cua hai doi tuong
*/
bool CheckCollision(
	float x1, float y1, float w1, float h1,
	float x2, float y2, float w2, float h2)
{
	return (fabs(x1 - x2) * 2.0f < (w1 + w2)) &&
		   (fabs(y1 - y2) * 2.0f < (h1 + h2));
}

/*
	Update world status for this frame
*/
void Update(DWORD dt)
{
	// 1. Cap nhat tat ca doi tuong
	for (int i = 0; i < (int)objects.size(); i++)
	{
		objects[i]->Update(dt);
	}

	// 2. Tao enemy moi neu player con song
	DWORD now = GetTickCount();

	if (player != nullptr && !player->IsDead())
	{
		if (now - lastEnemySpawnTime >= ENEMY_SPAWN_INTERVAL)
		{
			float halfEnemyW = ENEMY_WIDTH / 2.0f;
			float enemyX = halfEnemyW + (float)(rand() % (int)(SCREEN_WIDTH - ENEMY_WIDTH));
			float enemyY = -ENEMY_HEIGHT / 2.0f;

			CEnemy* enemy = new CEnemy(enemyX, enemyY);
			objects.push_back(enemy);

			lastEnemySpawnTime = now;
		}
	}

	// 3. Xu ly va cham: Dan (Bullet) ban trung Quai (Enemy)
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
					if (CheckCollision(bullet->GetX(), bullet->GetY(), BULLET_WIDTH, BULLET_HEIGHT,
									   enemy->GetX(), enemy->GetY(), ENEMY_WIDTH, ENEMY_HEIGHT))
					{
						enemy->TakeDamage(1);
						bullet->Delete();

						// Hieu ung no tai tam cua quai
						objects.push_back(new CHitEffect(enemy->GetX(), enemy->GetY()));

						score += 10;
						break;
					}
				}
			}
		}
	}

	// 4. Xu ly va cham: Quai (Enemy) va cham Nguoi choi (Player)
	if (player != nullptr && !player->IsDead())
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			CEnemy* enemy = dynamic_cast<CEnemy*>(objects[i]);
			if (enemy != nullptr && !enemy->IsDead() && !enemy->IsDeleted())
			{
				if (CheckCollision(player->GetX(), player->GetY(), PLAYER_WIDTH, PLAYER_HEIGHT,
								   enemy->GetX(), enemy->GetY(), ENEMY_WIDTH, ENEMY_HEIGHT))
				{
					if (!player->IsInvulnerable())
					{
						player->TakeDamage(1);
						enemy->TakeDamage(1);

						// Hieu ung no ngay tai diem tiep xuc
						float hitX = (player->GetX() + enemy->GetX()) / 2.0f;
						float hitY = (player->GetY() + enemy->GetY()) / 2.0f;
						objects.push_back(new CHitEffect(hitX, hitY));
					}
				}
			}
		}
	}

	// 5. Xoa enemy da chet hoac ra khoi man hinh, va hit effect da hoan thanh
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

	// 6. Cap nhat tieu de cua so (Hien thi HP va Score)
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
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	// Tinh toan kich thuoc cua so de vung Client Area chinh xac ScreenWidth x ScreenHeight
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	RECT wr = { 0, 0, ScreenWidth, ScreenHeight };
	AdjustWindowRect(&wr, dwStyle, FALSE);

	int winWidth = wr.right - wr.left;
	int winHeight = wr.bottom - wr.top;
	int winX = (GetSystemMetrics(SM_CXSCREEN) - winWidth) / 2;
	int winY = (GetSystemMetrics(SM_CYSCREEN) - winHeight) / 2;

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			dwStyle,
			winX,
			winY,
			winWidth,
			winHeight,
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

	srand((unsigned int)time(NULL));
	LoadResources();

	Run();

	return 0;
}
