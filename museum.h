#pragma once
#include <windows.h>
#include "inventory.h"

#define MUSEUM_SLOT_COUNT 4
#define FORTUNATE_COUNT 3

static const int MUSEUM_BUILD_X = 600;
static const int MUSEUM_BUILD_Y = 200;
static const int MUSEUM_BUILD_W = 200;
static const int MUSEUM_BUILD_H = 150;

class Museum {
public:
    Museum();
    ~Museum();
    void Init(HWND hWnd);
    void Release();
    void Draw(HDC hdc, HWND hWnd);
    void DrawBuilding(HDC hdc);
    void DrawMoneyUI(HDC hdc, HWND hWnd);
    void OnClick(int mx, int my);
    bool IsClicked(int mx, int my);
    int  GetMoney() const { return money; }

    void SetInventory(Wheat* w, Potato* p, Carrot* c, Strawberry* s,
        GoldWheat* gw, GoldPotato* gp, GoldCarrot* gc, GoldStrawberry* gs) {
        pWheat = w; pPotato = p; pCarrot = c; pStrawberry = s;
        pGoldWheat = gw; pGoldPotato = gp; pGoldCarrot = gc; pGoldStrawberry = gs;
    }

private:
    // 비트맵
    HDC     hBuildingDC;
    HBITMAP hBuildingBit;
    HDC     hMuseumBgDC;
    HBITMAP hMuseumBgBit;
    HDC     hFortunateDC[FORTUNATE_COUNT];
    HBITMAP hFortunateBit[FORTUNATE_COUNT];
    HDC     hCompleteDC;
    HBITMAP hCompleteBit;
    HWND g_hWnd;

    // 박물관 UI 위치
    int museumX, museumY, museumW, museumH;

    // 행운 단계 (0=행운1, 1=행운2, 2=행운3)
    int fortunateStage;

    // 각 단계별 슬롯 기증 상태 [단계][슬롯]
    bool slotDonated[FORTUNATE_COUNT][MUSEUM_SLOT_COUNT];

    // 슬롯 RECT (4개)
    RECT slotRects[MUSEUM_SLOT_COUNT];

    int money;

    void DrawBackground(HDC hdc, HWND hWnd);
    void DrawFortunate(HDC hdc, HWND hWnd);
    void DrawSlots(HDC hdc, HWND hWnd);
    void DrawText_(HDC hdc, const wchar_t* text, int x, int y, COLORREF color, int fontSize);

    // 인벤토리 포인터
    Wheat* pWheat = nullptr;
    Potato* pPotato = nullptr;
    Carrot* pCarrot = nullptr;
    Strawberry* pStrawberry = nullptr;
    GoldWheat* pGoldWheat = nullptr;
    GoldPotato* pGoldPotato = nullptr;
    GoldCarrot* pGoldCarrot = nullptr;
    GoldStrawberry* pGoldStrawberry = nullptr;
};

