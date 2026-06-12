#pragma comment(lib, "msimg32.lib")
#include <string>
#include "inventory.h"
#include "Player.h"
extern Player player;
#define SIZE 40
#define SET_X 10
#define SET_Y 10
#define COUNT 2
static int GOLD_SIZE = SIZE;
static int GOLD_SET_X = SET_X + 4 * SIZE;
static int GOLD_SET_Y = SET_Y;
static HDC     hLockDC = NULL;
static HBITMAP hLockBit = NULL;
static HDC     hGoldLockDC = NULL;
static HBITMAP hGoldLockBit = NULL;
// =====================================================================
// 1. 밀 (Wheat) 구현
// =====================================================================
Wheat::Wheat() : pos_x(SET_X), pos_y(SET_Y), size(SIZE), hMemDC(NULL), hBit(NULL), count_x(pos_x), count_y(pos_y + size), count_size(SIZE / 2), count(COUNT) {}
Wheat::~Wheat() {
    if (hBit) DeleteObject(hBit);
    if (hMemDC) DeleteDC(hMemDC);
}
void Wheat::InitWheat(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hdc);
    hBit = (HBITMAP)LoadImage(NULL, L"밀1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBit) SelectObject(hMemDC, hBit);

    if (!hLockBit) {
        hLockDC = CreateCompatibleDC(hdc);
        hLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hLockBit) SelectObject(hLockDC, hLockBit);
    }

    ReleaseDC(hWnd, hdc);
}
void Wheat::Draw(HDC hBufferDC) {
    if (!hMemDC || !hBit) return;
    BITMAP bm;
    if (player.GetUnlockWheat()) {
        // 해금 상태: 밀 이미지
        GetObject(hBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
    else {
        // 잠금 상태: 자물쇠 이미지
        if (!hLockBit) return;
        GetObject(hLockBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
}
void Wheat::Count_Draw(HDC hdc) {
    wchar_t buf[32];

    wsprintf(buf, L"%d", count);
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    int digits = std::to_string(count).length();
    TextOut(hdc, count_x - digits * 4 + count_size, count_y, buf, lstrlen(buf));
    SetBkMode(hdc, TRANSPARENT);

}

// =====================================================================
// 2. 감자 (Potato) 구현
// =====================================================================

Potato::Potato() : pos_x(SET_X + SIZE * 1), pos_y(SET_Y), size(SIZE), hMemDC(NULL), hBit(NULL), count_x(pos_x), count_y(pos_y + size), count_size(SIZE / 2), count(COUNT) {}

Potato::~Potato() {
    if (hBit) DeleteObject(hBit);
    if (hMemDC) DeleteDC(hMemDC);
}

void Potato::InitPotato(HWND hWnd) {
    HDC hdc = GetDC(hWnd);

    hMemDC = CreateCompatibleDC(hdc);

    hBit = (HBITMAP)LoadImage(
        NULL,
        L"감자1.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    if (hBit) SelectObject(hMemDC, hBit);

    if (!hLockBit) {
        hLockDC = CreateCompatibleDC(hdc);
        hLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hLockBit) SelectObject(hLockDC, hLockBit);
    }

    ReleaseDC(hWnd, hdc);
}

void Potato::Draw(HDC hBufferDC) {
    if (!hMemDC || !hBit) return;

    BITMAP bm;
    if (player.GetUnlockPotato()) {
        // 해금 상태: 밀 이미지
        GetObject(hBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
    else {
        // 잠금 상태: 자물쇠 이미지
        if (!hLockBit) return;
        GetObject(hLockBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
}

void Potato::Count_Draw(HDC hdc) {
    wchar_t buf[32];

    wsprintf(buf, L"%d", count);
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    int digits = std::to_string(count).length();
    TextOut(hdc, count_x - digits * 4 + count_size, count_y, buf, lstrlen(buf));
    SetBkMode(hdc, TRANSPARENT);
}

// =====================================================================
// 3. 당근 (Carrot) 구현
// =====================================================================

Carrot::Carrot() : pos_x(SET_X + SIZE * 2), pos_y(SET_Y), size(SIZE), hMemDC(NULL), hBit(NULL), count_x(pos_x), count_y(pos_y + size), count_size(SIZE / 2), count(COUNT) {}
Carrot::~Carrot() {
    if (hBit) DeleteObject(hBit);
    if (hMemDC) DeleteDC(hMemDC);
}

void Carrot::InitCarrot(HWND hWnd) {
    HDC hdc = GetDC(hWnd);

    hMemDC = CreateCompatibleDC(hdc);

    hBit = (HBITMAP)LoadImage(
        NULL,
        L"당근1.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    if (!hLockBit) {
        hLockDC = CreateCompatibleDC(hdc);
        hLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hLockBit) SelectObject(hLockDC, hLockBit);
    }


    if (hBit) SelectObject(hMemDC, hBit);

    ReleaseDC(hWnd, hdc);
}

void Carrot::Draw(HDC hBufferDC) {
    if (!hMemDC || !hBit) return;

    BITMAP bm;
    if (player.GetUnlockCarrot()) {
        // 해금 상태: 밀 이미지
        GetObject(hBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
    else {
        // 잠금 상태: 자물쇠 이미지
        if (!hLockBit) return;
        GetObject(hLockBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
}

void Carrot::Count_Draw(HDC hdc) {
    wchar_t buf[32];

    wsprintf(buf, L"%d", count);
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    int digits = std::to_string(count).length();
    TextOut(hdc, count_x - digits * 4 + count_size, count_y, buf, lstrlen(buf));
    SetBkMode(hdc, TRANSPARENT);
}

// =====================================================================
// 4. 딸기 (Strawberry) 구현
// =====================================================================

Strawberry::Strawberry() : pos_x(SET_X + SIZE * 3), pos_y(SET_Y), size(SIZE), hMemDC(NULL), hBit(NULL), count_x(pos_x), count_y(pos_y + size), count_size(SIZE / 2), count(COUNT) {}

Strawberry::~Strawberry() {
    if (hBit) DeleteObject(hBit);
    if (hMemDC) DeleteDC(hMemDC);
}

void Strawberry::InitStrawberry(HWND hWnd) {
    HDC hdc = GetDC(hWnd);

    hMemDC = CreateCompatibleDC(hdc);

    hBit = (HBITMAP)LoadImage(
        NULL,
        L"딸기1.bmp",
        IMAGE_BITMAP,
        0, 0,
        LR_LOADFROMFILE
    );

    if (hBit) SelectObject(hMemDC, hBit);

    if (!hLockBit) {
        hLockDC = CreateCompatibleDC(hdc);
        hLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hLockBit) SelectObject(hLockDC, hLockBit);
    }

    ReleaseDC(hWnd, hdc);
}

void Strawberry::Draw(HDC hBufferDC) {
    if (!hMemDC || !hBit) return;

    BITMAP bm;
    if (player.GetUnlockStrawberry()) {
        // 해금 상태: 밀 이미지
        GetObject(hBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
    else {
        // 잠금 상태: 자물쇠 이미지
        if (!hLockBit) return;
        GetObject(hLockBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
}

void Strawberry::Count_Draw(HDC hdc) {
    wchar_t buf[32];

    wsprintf(buf, L"%d", count);
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    int digits = std::to_string(count).length();
    TextOut(hdc, count_x - digits * 4 + count_size, count_y, buf, lstrlen(buf));
    SetBkMode(hdc, TRANSPARENT);
}
// =====================================================================
// 1. 황금밀 (GoldWheat) 구현
// =====================================================================
GoldWheat::GoldWheat() : pos_x(GOLD_SET_X), pos_y(GOLD_SET_Y), size(GOLD_SIZE), hMemDC(NULL), hBit(NULL), count_x(pos_x), count_y(pos_y + GOLD_SIZE), count_size(GOLD_SIZE / 2), count(COUNT) {}
GoldWheat::~GoldWheat() {
    if (hBit) DeleteObject(hBit);
    if (hMemDC) DeleteDC(hMemDC);
}
void GoldWheat::InitGoldWheat(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hdc);
    hBit = (HBITMAP)LoadImage(NULL, L"황금밀.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBit) SelectObject(hMemDC, hBit);

    if (!hGoldLockBit) {
        hGoldLockDC = CreateCompatibleDC(hdc);
        hGoldLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hGoldLockBit) SelectObject(hGoldLockDC, hGoldLockBit);
    }
    ReleaseDC(hWnd, hdc);
}
void GoldWheat::Draw(HDC hBufferDC) {
    if (!hMemDC || !hBit) return;
    BITMAP bm;
    if (player.GetUnlockGoldWheat()) {
        GetObject(hBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
    else {
        if (!hGoldLockBit) return;
        GetObject(hGoldLockBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hGoldLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
}
void GoldWheat::Count_Draw(HDC hdc) {
    wchar_t buf[32];
    wsprintf(buf, L"%d", count);
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    int digits = std::to_string(count).length();
    TextOut(hdc, count_x - digits * 4 + count_size, count_y, buf, lstrlen(buf));
    SetBkMode(hdc, TRANSPARENT);
}

// =====================================================================
// 2. 황금감자 (GoldPotato) 구현
// =====================================================================
GoldPotato::GoldPotato() : pos_x(GOLD_SET_X + GOLD_SIZE * 1), pos_y(GOLD_SET_Y), size(GOLD_SIZE), hMemDC(NULL), hBit(NULL), count_x(pos_x), count_y(pos_y + GOLD_SIZE), count_size(GOLD_SIZE / 2), count(COUNT) {}
GoldPotato::~GoldPotato() {
    if (hBit) DeleteObject(hBit);
    if (hMemDC) DeleteDC(hMemDC);
}
void GoldPotato::InitGoldPotato(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hdc);
    hBit = (HBITMAP)LoadImage(NULL, L"황금감자.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBit) SelectObject(hMemDC, hBit);

    if (!hGoldLockBit) {
        hGoldLockDC = CreateCompatibleDC(hdc);
        hGoldLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hGoldLockBit) SelectObject(hGoldLockDC, hGoldLockBit);
    }
    ReleaseDC(hWnd, hdc);
}
void GoldPotato::Draw(HDC hBufferDC) {
    if (!hMemDC || !hBit) return;
    BITMAP bm;
    if (player.GetUnlockGoldPotato()) {
        GetObject(hBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
    else {
        if (!hGoldLockBit) return;
        GetObject(hGoldLockBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hGoldLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
}
void GoldPotato::Count_Draw(HDC hdc) {
    wchar_t buf[32];
    wsprintf(buf, L"%d", count);
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    int digits = std::to_string(count).length();
    TextOut(hdc, count_x - digits * 4 + count_size, count_y, buf, lstrlen(buf));
    SetBkMode(hdc, TRANSPARENT);
}

// =====================================================================
// 3. 황금당근 (GoldCarrot) 구현
// =====================================================================
GoldCarrot::GoldCarrot() : pos_x(GOLD_SET_X + GOLD_SIZE * 2), pos_y(GOLD_SET_Y), size(GOLD_SIZE), hMemDC(NULL), hBit(NULL), count_x(pos_x), count_y(pos_y + GOLD_SIZE), count_size(GOLD_SIZE / 2), count(COUNT) {}
GoldCarrot::~GoldCarrot() {
    if (hBit) DeleteObject(hBit);
    if (hMemDC) DeleteDC(hMemDC);
}
void GoldCarrot::InitGoldCarrot(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hdc);
    hBit = (HBITMAP)LoadImage(NULL, L"황금당근.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBit) SelectObject(hMemDC, hBit);

    if (!hGoldLockBit) {
        hGoldLockDC = CreateCompatibleDC(hdc);
        hGoldLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hGoldLockBit) SelectObject(hGoldLockDC, hGoldLockBit);
    }
    ReleaseDC(hWnd, hdc);
}
void GoldCarrot::Draw(HDC hBufferDC) {
    if (!hMemDC || !hBit) return;
    BITMAP bm;
    if (player.GetUnlockGoldCarrot()) {
        GetObject(hBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
    else {
        if (!hGoldLockBit) return;
        GetObject(hGoldLockBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hGoldLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
}
void GoldCarrot::Count_Draw(HDC hdc) {
    wchar_t buf[32];
    wsprintf(buf, L"%d", count);
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    int digits = std::to_string(count).length();
    TextOut(hdc, count_x - digits * 4 + count_size, count_y, buf, lstrlen(buf));
    SetBkMode(hdc, TRANSPARENT);
}

// =====================================================================
// 4. 황금딸기 (GoldStrawberry) 구현
// =====================================================================
GoldStrawberry::GoldStrawberry() : pos_x(GOLD_SET_X + GOLD_SIZE * 3), pos_y(GOLD_SET_Y), size(GOLD_SIZE), hMemDC(NULL), hBit(NULL), count_x(pos_x), count_y(pos_y + GOLD_SIZE), count_size(GOLD_SIZE / 2), count(COUNT) {}
GoldStrawberry::~GoldStrawberry() {
    if (hBit) DeleteObject(hBit);
    if (hMemDC) DeleteDC(hMemDC);
}
void GoldStrawberry::InitGoldStrawberry(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hdc);
    hBit = (HBITMAP)LoadImage(NULL, L"황금딸기.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBit) SelectObject(hMemDC, hBit);

    if (!hGoldLockBit) {
        hGoldLockDC = CreateCompatibleDC(hdc);
        hGoldLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hGoldLockBit) SelectObject(hGoldLockDC, hGoldLockBit);
    }
    ReleaseDC(hWnd, hdc);
}
void GoldStrawberry::Draw(HDC hBufferDC) {
    if (!hMemDC || !hBit) return;
    BITMAP bm;
    if (player.GetUnlockGoldStrawberry()) {
        GetObject(hBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
    else {
        if (!hGoldLockBit) return;
        GetObject(hGoldLockBit, sizeof(BITMAP), &bm);
        TransparentBlt(hBufferDC, pos_x, pos_y, size, size,
            hGoldLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
    }
}
void GoldStrawberry::Count_Draw(HDC hdc) {
    wchar_t buf[32];
    wsprintf(buf, L"%d", count);
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    int digits = std::to_string(count).length();
    TextOut(hdc, count_x - digits * 4 + count_size, count_y, buf, lstrlen(buf));
    SetBkMode(hdc, TRANSPARENT);
}
// 플레이어를 따라오는 인벤토리

//#pragma comment(lib, "msimg32.lib")
//#include "inventory.h"
//#include "Player.h"
//extern Player player;
//#define SIZE  30
//static HDC     hLockDC = NULL;
//static HBITMAP hLockBit = NULL;
//// =====================================================================
//// 1. 밀 (Wheat) 구현
//// =====================================================================
//Wheat::Wheat() : size(SIZE), hMemDC(NULL), hBit(NULL), count_x(pos_x), count_y(pos_y + size), count_size(SIZE / 2), count(0) {}
//Wheat::~Wheat() {
//    if (hBit) DeleteObject(hBit);
//    if (hMemDC) DeleteDC(hMemDC);
//}
//void Wheat::InitWheat(HWND hWnd) {
//    HDC hdc = GetDC(hWnd);
//    hMemDC = CreateCompatibleDC(hdc);
//    hBit = (HBITMAP)LoadImage(NULL, L"밀.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//    if (hBit) SelectObject(hMemDC, hBit);
//
//    if (!hLockBit) {
//        hLockDC = CreateCompatibleDC(hdc);
//        hLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//        if (hLockBit) SelectObject(hLockDC, hLockBit);
//    }
//
//    ReleaseDC(hWnd, hdc);
//}
//void Wheat::Draw(HDC hBufferDC) {
//    if (!hMemDC || !hBit) return;
//    BITMAP bm;
//
//    int px = player.GetX() + 40;
//    int py = player.GetY() - 10;
//
//    if (player.GetUnlockWheat()) {
//        // 해금 상태: 밀 이미지
//        GetObject(hBit, sizeof(BITMAP), &bm);
//        TransparentBlt(hBufferDC, px, py, size, size,
//            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
//    }
//    else {
//        // 잠금 상태: 자물쇠 이미지
//        if (!hLockBit) return;
//        GetObject(hLockBit, sizeof(BITMAP), &bm);
//        TransparentBlt(hBufferDC, px, py, size, size,
//            hLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
//    }
//}
//void Wheat::Count_Draw(HDC hdc) {
//    wchar_t buf[32];
//
//    wsprintf(buf, L"%d", count);
//
//    int px = player.GetX() + 40;
//    int py = player.GetY() - 10;
//
//    SetBkMode(hdc, TRANSPARENT);
//    SetTextColor(hdc, RGB(0, 0, 0));
//
//    TextOut(hdc, px + (SIZE / 2), py + size, buf, lstrlen(buf));
//}
//
//// =====================================================================
//// 2. 감자 (Potato) 구현
//// =====================================================================
//Potato::Potato() : size(SIZE), hMemDC(NULL), hBit(NULL), count(0) {}
//Potato::~Potato() {
//    if (hBit) DeleteObject(hBit);
//    if (hMemDC) DeleteDC(hMemDC);
//}
//void Potato::InitPotato(HWND hWnd) {
//    HDC hdc = GetDC(hWnd);
//    hMemDC = CreateCompatibleDC(hdc);
//    hBit = (HBITMAP)LoadImage(NULL, L"감자.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//    if (hBit) SelectObject(hMemDC, hBit);
//
//    if (!hLockBit) {
//        hLockDC = CreateCompatibleDC(hdc);
//        hLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//        if (hLockBit) SelectObject(hLockDC, hLockBit);
//    }
//    ReleaseDC(hWnd, hdc);
//}
//void Potato::Draw(HDC hBufferDC) {
//    if (!hMemDC || !hBit) return;
//    BITMAP bm;
//
//    // 밀 아이콘 오른쪽 옆에 나란히 배치하기 위해 SIZE * 1 만큼 더해줍니다.
//    int px = player.GetX() + 40 + (SIZE * 1);
//    int py = player.GetY() - 10;
//
//    if (player.GetUnlockPotato()) {
//        GetObject(hBit, sizeof(BITMAP), &bm);
//        TransparentBlt(hBufferDC, px, py, size, size,
//            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
//    }
//    else {
//        if (!hLockBit) return;
//        GetObject(hLockBit, sizeof(BITMAP), &bm);
//        TransparentBlt(hBufferDC, px, py, size, size,
//            hLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
//    }
//}
//void Potato::Count_Draw(HDC hdc) {
//    wchar_t buf[32];
//    wsprintf(buf, L"%d", count);
//
//    int px = player.GetX() + 40 + (SIZE * 1);
//    int py = player.GetY() - 10;
//
//    SetBkMode(hdc, TRANSPARENT);
//    SetTextColor(hdc, RGB(0, 0, 0));
//    TextOut(hdc, px + (SIZE / 2), py + size, buf, lstrlen(buf));
//}
//
//// =====================================================================
//// 3. 당근 (Carrot) 구현
//// =====================================================================
//Carrot::Carrot() : size(SIZE), hMemDC(NULL), hBit(NULL), count(0) {}
//Carrot::~Carrot() {
//    if (hBit) DeleteObject(hBit);
//    if (hMemDC) DeleteDC(hMemDC);
//}
//void Carrot::InitCarrot(HWND hWnd) {
//    HDC hdc = GetDC(hWnd);
//    hMemDC = CreateCompatibleDC(hdc);
//    hBit = (HBITMAP)LoadImage(NULL, L"당근.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//    if (hBit) SelectObject(hMemDC, hBit);
//
//    if (!hLockBit) {
//        hLockDC = CreateCompatibleDC(hdc);
//        hLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//        if (hLockBit) SelectObject(hLockDC, hLockBit);
//    }
//    ReleaseDC(hWnd, hdc);
//}
//void Carrot::Draw(HDC hBufferDC) {
//    if (!hMemDC || !hBit) return;
//    BITMAP bm;
//
//    // 밀 아이콘 오른쪽 옆에 나란히 배치하기 위해 SIZE * 2 만큼 더해줍니다.
//    int px = player.GetX() + 40 + (SIZE * 2);
//    int py = player.GetY() - 10;
//
//    if (player.GetUnlockCarrot()) {
//        GetObject(hBit, sizeof(BITMAP), &bm);
//        TransparentBlt(hBufferDC, px, py, size, size,
//            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
//    }
//    else {
//        if (!hLockBit) return;
//        GetObject(hLockBit, sizeof(BITMAP), &bm);
//        TransparentBlt(hBufferDC, px, py, size, size,
//            hLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
//    }
//}
//void Carrot::Count_Draw(HDC hdc) {
//    wchar_t buf[32];
//    wsprintf(buf, L"%d", count);
//
//    int px = player.GetX() + 40 + (SIZE * 2);
//    int py = player.GetY() - 10;
//
//    SetBkMode(hdc, TRANSPARENT);
//    SetTextColor(hdc, RGB(0, 0, 0));
//    TextOut(hdc, px + (SIZE / 2), py + size, buf, lstrlen(buf));
//}
//
//// =====================================================================
//// 4. 딸기 (Strawberry) 구현
//// =====================================================================
//Strawberry::Strawberry() : size(SIZE), hMemDC(NULL), hBit(NULL), count(0) {}
//Strawberry::~Strawberry() {
//    if (hBit) DeleteObject(hBit);
//    if (hMemDC) DeleteDC(hMemDC);
//}
//void Strawberry::InitStrawberry(HWND hWnd) {
//    HDC hdc = GetDC(hWnd);
//    hMemDC = CreateCompatibleDC(hdc);
//    hBit = (HBITMAP)LoadImage(NULL, L"딸기.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//    if (hBit) SelectObject(hMemDC, hBit);
//
//    if (!hLockBit) {
//        hLockDC = CreateCompatibleDC(hdc);
//        hLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//        if (hLockBit) SelectObject(hLockDC, hLockBit);
//    }
//    ReleaseDC(hWnd, hdc);
//}
//void Strawberry::Draw(HDC hBufferDC) {
//    if (!hMemDC || !hBit) return;
//    BITMAP bm;
//
//    // 밀 아이콘 오른쪽 옆에 나란히 배치하기 위해 SIZE * 3 만큼 더해줍니다.
//    int px = player.GetX() + 40 + (SIZE * 3);
//    int py = player.GetY() - 10;
//
//    if (player.GetUnlockStrawberry()) {
//        GetObject(hBit, sizeof(BITMAP), &bm);
//        TransparentBlt(hBufferDC, px, py, size, size,
//            hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
//    }
//    else {
//        if (!hLockBit) return;
//        GetObject(hLockBit, sizeof(BITMAP), &bm);
//        TransparentBlt(hBufferDC, px, py, size, size,
//            hLockDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
//    }
//}
//void Strawberry::Count_Draw(HDC hdc) {
//    wchar_t buf[32];
//    wsprintf(buf, L"%d", count);
//
//    int px = player.GetX() + 40 + (SIZE * 3);
//    int py = player.GetY() - 10;
//
//    SetBkMode(hdc, TRANSPARENT);
//    SetTextColor(hdc, RGB(0, 0, 0));
//    TextOut(hdc, px + (SIZE / 2), py + size, buf, lstrlen(buf));
//}