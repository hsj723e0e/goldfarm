#pragma once
#include <windows.h>
#include "inventory.h"

#define SHOP_ITEM_COUNT 8
#define SHOP_COLS 4
#define SHOP_ROWS 2

// 상점 건물 클릭 판정 (main.cpp에서도 사용)
static const int SHOP_BUILD_X = 500;
static const int SHOP_BUILD_Y = 400;
static const int SHOP_BUILD_W = 200;
static const int SHOP_BUILD_H = 150;

class Shop {
public:
    Shop();
    ~Shop();

    void Init(HWND hWnd);
    void Release();
    void Draw(HDC hdc, HWND hWnd);        // 상점 UI 전체 (배경 + 아이템 + 돈)
    void DrawBuilding(HDC hdc); // 500,500 위치 상점 건물 이미지
    void OnClick(int mx, int my);
    bool IsClicked(int mx, int my);
    void DrawMoneyUI(HDC hdc, HWND hWnd);

    int GetMoney() const { return money; }
    void SetMoney(int val) { money = val; } // 최종
    // 인벤토리 객체 포인터 연결
    void SetInventory(Wheat* w, Potato* p, Carrot* c, Strawberry* s) {
        pWheat = w; pPotato = p; pCarrot = c; pStrawberry = s;
    }

private:
    // 비트맵
    HDC     hItemDC[4];       // 상점.bmp (슬롯 아이템)
    HBITMAP hItemBit[4];
    HDC     hBuildingDC;   // 상점.bmp (건물 아이콘)
    HBITMAP hBuildingBit;
    HDC     hShopBgDC;     // 상점_배경.bmp (UI 배경)
    HBITMAP hShopBgBit;
    HDC     hLockDC;
    HBITMAP hLockBit;

    // 상점 UI 위치/크기
    int shopX, shopY, shopW, shopH;

    // 슬롯 8개 RECT
    RECT itemRects[SHOP_ITEM_COUNT];

    // 돈
    int money;

    void DrawBackground(HDC hdc, HWND hWnd);
    void DrawItems(HDC hdc, HWND hWnd);
    void DrawText_(HDC hdc, const wchar_t* text, int x, int y, COLORREF color, int fontSize);

    Wheat* pWheat = nullptr;
    Potato* pPotato = nullptr;
    Carrot* pCarrot = nullptr;
    Strawberry* pStrawberry = nullptr;
};