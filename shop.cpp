#include "shop.h"
#include "Player.h"
extern Player player;
#pragma comment(lib, "msimg32.lib")

// =====================================================================
//  레이아웃 상수
// =====================================================================

// 상점 UI 배경 출력 위치/크기
static const int SHOP_X = 0;
static const int SHOP_Y = 0;
static const int SHOP_BG_W = 700 ;
static const int SHOP_BG_H = 500;

// 슬롯 좌표 (상점_배경.bmp 안의 아이템 칸 위치)
static const int SLOT_START_X = 145;  // 첫 번째 슬롯 X
static const int SLOT_START_Y = 115;  // 첫 번째 슬롯 Y
static const int SLOT_W = 130;  // 슬롯 너비
static const int SLOT_H = 155;  // 슬롯 높이
static const int SLOT_GAP_X = 148;  // 슬롯 가로 간격
static const int SLOT_GAP_Y = 178;  // 슬롯 세로 간격

// 돈 UI (우상단)
static const int MONEY_UI_X = 660;
static const int MONEY_UI_Y = 10;
static const int MONEY_UI_W = 160;
static const int MONEY_UI_H = 36;

// =====================================================================
//  생성자 / 소멸자
// =====================================================================
Shop::Shop()
    : hBuildingDC(NULL), hBuildingBit(NULL),
    hShopBgDC(NULL), hShopBgBit(NULL), hLockBit(NULL),
    money(player.GetMoney()), shopX(SHOP_X), shopY(SHOP_Y)      // 6/8 서진씨
{
    for (int i = 0; i < 4; i++) {
        hItemDC[i] = NULL;
        hItemBit[i] = NULL;
    }
    ZeroMemory(itemRects, sizeof(itemRects));
}

Shop::~Shop() {
    Release();
}

// =====================================================================
//  초기화
// =====================================================================
void Shop::Init(HWND hWnd) {
    HDC hdc = GetDC(hWnd);

    const wchar_t* itemImages[4] = {
    L"밀1.bmp",
    L"감자1.bmp",
    L"당근1.bmp",
    L"딸기1.bmp"
    };

    hBuildingDC = CreateCompatibleDC(hdc);
    hShopBgDC = CreateCompatibleDC(hdc);
    hLockDC = CreateCompatibleDC(hdc);

    // 이미지 로드
    hBuildingBit = (HBITMAP)LoadImage(NULL, L"상점.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hShopBgBit = (HBITMAP)LoadImage(NULL, L"상점_배경.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hLockBit = (HBITMAP)LoadImage(NULL, L"좌물쇠.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (hBuildingBit) SelectObject(hBuildingDC, hBuildingBit);
    if (hShopBgBit)   SelectObject(hShopBgDC, hShopBgBit);
    if (hLockBit)   SelectObject(hLockDC, hLockBit);

    for (int i = 0; i < 4; i++) {
        hItemDC[i] = CreateCompatibleDC(hdc);
        hItemBit[i] = (HBITMAP)LoadImage(NULL, itemImages[i], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hItemBit[i]) SelectObject(hItemDC[i], hItemBit[i]);
    }

    ReleaseDC(hWnd, hdc);

    // 슬롯 8개 RECT 계산
    for (int r = 0; r < SHOP_ROWS; r++) {
        for (int c = 0; c < SHOP_COLS; c++) {
            int idx = r * SHOP_COLS + c;
            int sx = SLOT_START_X + c * SLOT_GAP_X;
            int sy = SLOT_START_Y + r * SLOT_GAP_Y;
            itemRects[idx] = { sx, sy, sx + SLOT_W, sy + SLOT_H };
        }
    }
}

// =====================================================================
//  해제
// =====================================================================
void Shop::Release() {
    if (hBuildingBit) { DeleteObject(hBuildingBit); hBuildingBit = NULL; }
    if (hBuildingDC) { DeleteDC(hBuildingDC);      hBuildingDC = NULL; }

    if (hShopBgBit) { DeleteObject(hShopBgBit);   hShopBgBit = NULL; }
    if (hShopBgDC) { DeleteDC(hShopBgDC);         hShopBgDC = NULL; }

    if (hLockBit) { DeleteObject(hLockBit);      hLockBit = NULL; }
    if (hLockDC) { DeleteDC(hLockDC);           hLockDC = NULL; }

    for (int i = 0; i < 4; i++) {
        if (hItemBit[i]) { DeleteObject(hItemBit[i]); hItemBit[i] = NULL; }
        if (hItemDC[i]) { DeleteDC(hItemDC[i]);      hItemDC[i] = NULL; }
    }
}

// =====================================================================
//  전체 그리기
// =====================================================================
void Shop::Draw(HDC hdc, HWND hWnd) {
    DrawBackground(hdc, hWnd);
    DrawItems(hdc, hWnd);
    DrawMoneyUI(hdc, hWnd);
}

// =====================================================================
//  배경: 상점_배경.bmp 출력 (크기 버그 수정)
// =====================================================================
void Shop::DrawBackground(HDC hdc, HWND hWnd) {
    if (!hShopBgBit) return;

    BITMAP bm;
    GetObject(hShopBgBit, sizeof(bm), &bm);

    // 기존에는 windowWidth, windowHeight로 창 전체에 배경 이미지를 억지로 늘려버려서 
    // 클릭 좌표(itemRects)와 화면상에 그려지는 상점 아이템 위치가 불일치했습니다.
    // 이를 정해진 UI 크기(SHOP_BG_W, SHOP_BG_H)로 고정하여 그립니다.
    TransparentBlt(hdc,
        SHOP_X, SHOP_Y, SHOP_BG_W, SHOP_BG_H,
        hShopBgDC,
        0, 0, bm.bmWidth, bm.bmHeight,
        RGB(255, 255, 255));
}

// =====================================================================
//  아이템 슬롯 8개에 이미지 출력
// =====================================================================
void Shop::DrawItems(HDC hdc, HWND hWnd) {
    bool isUnlocked[4] = {
        player.GetUnlockWheat(),
        player.GetUnlockPotato(),
        player.GetUnlockCarrot(),
        player.GetUnlockStrawberry()
    };
    for (int i = 0; i < SHOP_ITEM_COUNT; i++) {
        int col = i % 4;  // 0~3열
        RECT& r = itemRects[i];
        int pad = 5;

        if (isUnlocked[col]) {
            if (!hItemBit[col]) continue;
            BITMAP bm;
            GetObject(hItemBit[col], sizeof(bm), &bm);
            TransparentBlt(hdc,
                r.left + pad, r.top + pad,
                SLOT_W - pad * 2, SLOT_H - pad * 2,
                hItemDC[col],
                0, 0, bm.bmWidth, bm.bmHeight,
                RGB(255, 255, 255));
        }
        else {
            if (!hLockBit) continue;
            BITMAP bm;
            GetObject(hLockBit, sizeof(bm), &bm);
            TransparentBlt(hdc,
                r.left + pad, r.top + pad,
                SLOT_W - pad * 2, SLOT_H - pad * 2,
                hLockDC,
                0, 0, bm.bmWidth, bm.bmHeight,
                RGB(255, 255, 255));
        }
    }
}

// =====================================================================
//  상점 건물 이미지
// =====================================================================
void Shop::DrawBuilding(HDC hdc) {
    if (!hBuildingBit) return;

    BITMAP bm;
    GetObject(hBuildingBit, sizeof(bm), &bm);

    TransparentBlt(hdc,
        SHOP_BUILD_X, SHOP_BUILD_Y,
        SHOP_BUILD_W, SHOP_BUILD_H,
        hBuildingDC,
        0, 0, bm.bmWidth, bm.bmHeight,
        RGB(255, 255, 255));
}

// =====================================================================
//  돈 UI (우상단)
// =====================================================================
void Shop::DrawMoneyUI(HDC hdc, HWND hWnd) {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    int dynamicX = clientRect.right - MONEY_UI_W - 30;
    int dynamicY = clientRect.top + 20;

    HBRUSH hBg = CreateSolidBrush(RGB(30, 20, 5));
    HPEN   hPen = CreatePen(PS_SOLID, 2, RGB(200, 160, 40));
    HPEN   hOld = (HPEN)SelectObject(hdc, hPen);
    SelectObject(hdc, hBg);
    RoundRect(hdc,
        dynamicX, dynamicY,
        dynamicX + MONEY_UI_W, dynamicY + MONEY_UI_H,
        18, 18);
    SelectObject(hdc, hOld);
    DeleteObject(hPen);
    DeleteObject(hBg);

    HBRUSH hCoin = CreateSolidBrush(RGB(255, 200, 30));
    HPEN   hPen2 = CreatePen(PS_SOLID, 1, RGB(180, 130, 0));
    HPEN   hOld2 = (HPEN)SelectObject(hdc, hPen2);
    SelectObject(hdc, hCoin);
    Ellipse(hdc,
        dynamicX + 8, dynamicY + 7,
        dynamicX + 28, dynamicY + MONEY_UI_H - 7);
    SelectObject(hdc, hOld2);
    DeleteObject(hPen2);
    DeleteObject(hCoin);

    wchar_t buf[32];
    wsprintf(buf, L"%d", money);
    DrawText_(hdc, buf,
        dynamicX + 36, dynamicY + 6,
        RGB(255, 220, 80), 18);
}

// =====================================================================
//  클릭 처리 (명시적 POINT 사용)
// =====================================================================
bool Shop::IsClicked(int mx, int my) {
    POINT pt = { mx, my };
    RECT panel = { SHOP_X, SHOP_Y, SHOP_X + SHOP_BG_W, SHOP_Y + SHOP_BG_H };
    return PtInRect(&panel, pt) == TRUE;
}

void Shop::OnClick(int mx, int my) {
    POINT pt = { mx, my };
    for (int i = 0; i < SHOP_ITEM_COUNT; i++) {
        if (PtInRect(&itemRects[i], pt)) {
            int row = i / 4;  // 0=구매, 1=판매
            int col = i % 4;  // 0=밀, 1=감자, 2=당근, 3=딸기

            bool isUnlocked[4] = {
                player.GetUnlockWheat(),
                player.GetUnlockPotato(),
                player.GetUnlockCarrot(),
                player.GetUnlockStrawberry()
            };

            // 주석 처리 되어있던 메시지 박스를 풀어서 피드백을 제공합니다.
            if (!isUnlocked[col]) {
                MessageBox(NULL, L"밭을 먼저 해금해야 해당 작물을 거래할 수 있습니다!", L"상점 잠금", MB_OK | MB_ICONWARNING);
                break;
            }

            if (row == 0) {
                // 1행: 구매
                if (money >= 10) {
                    money -= 10;
                    if (col == 0) pWheat->AddCount(1);
                    else if (col == 1) pPotato->AddCount(1);
                    else if (col == 2) pCarrot->AddCount(1);
                    else if (col == 3) pStrawberry->AddCount(1);
                }
                else {
                    MessageBox(NULL, L"돈이 부족합니다!", L"알림", MB_OK);
                }
            }
            else {
                // 2행: 판매
                int cnt = 0;
                if (col == 0) cnt = pWheat->GetCount();
                else if (col == 1) cnt = pPotato->GetCount();
                else if (col == 2) cnt = pCarrot->GetCount();
                else if (col == 3) cnt = pStrawberry->GetCount();

                if (cnt > 0) {
                    money += 10;
                    if (col == 0) pWheat->SubCount(1);
                    else if (col == 1) pPotato->SubCount(1);
                    else if (col == 2) pCarrot->SubCount(1);
                    else if (col == 3) pStrawberry->SubCount(1);
                }
                else {
                    MessageBox(NULL, L"판매할 작물이 인벤토리에 없습니다!", L"알림", MB_OK);
                }
            }
            break; // 아이템을 찾았으므로 for 루프 탈출
        }
    }
}

// =====================================================================
//  텍스트 출력 헬퍼
// =====================================================================
void Shop::DrawText_(HDC hdc, const wchar_t* text, int x, int y,
    COLORREF color, int fontSize) {
    HFONT hFont = CreateFont(
        fontSize, 0, 0, 0,
        FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Arial");

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, x, y, text, (int)wcslen(text));
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}