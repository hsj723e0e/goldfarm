#include "museum.h"
#include "Player.h"
#include "SoundManager.h" //최종
#pragma comment(lib, "msimg32.lib")
extern Player player;

// 레이아웃 상수
static const int MUSEUM_X = 120;
static const int MUSEUM_Y = 120;
static const int MUSEUM_BG_W = 700;
static const int MUSEUM_BG_H = 500;

// 행운 이미지 출력 위치/크기
static const int FORTUNATE_X = 150;
static const int FORTUNATE_Y = 50;
static const int FORTUNATE_W = 300;
static const int FORTUNATE_H = 300;

// 슬롯 위치 (행운 이미지 아래 빈칸 4개)
static const int SLOT_START_X = 100;
static const int SLOT_START_Y = 370;
static const int SLOT_W = 80;
static const int SLOT_H = 80;
static const int SLOT_GAP = 100;

// 돈 UI
static const int MONEY_UI_W = 160;
static const int MONEY_UI_H = 36;

// =====================================================================
// 생성자 / 소멸자
// =====================================================================
Museum::Museum()
    : hBuildingDC(NULL), hBuildingBit(NULL),
    hMuseumBgDC(NULL), hMuseumBgBit(NULL),
    museumX(0), museumY(0), museumW(MUSEUM_BG_W), museumH(MUSEUM_BG_H),
    fortunateStage(0), money(player.GetMoney()) // 6/8 서진씨
{
    for (int i = 0; i < FORTUNATE_COUNT; i++) {
        hFortunateDC[i] = NULL;
        hFortunateBit[i] = NULL;
    }
    for (int s = 0; s < FORTUNATE_COUNT; s++)
        for (int c = 0; c < MUSEUM_SLOT_COUNT; c++)
            slotDonated[s][c] = false;

    ZeroMemory(slotRects, sizeof(slotRects));
}

Museum::~Museum() {
    Release();
}

// =====================================================================
// 초기화
// =====================================================================
void Museum::Init(HWND hWnd) {
    HDC hdc = GetDC(hWnd);
    g_hWnd = hWnd;

    hBuildingDC = CreateCompatibleDC(hdc);
    hMuseumBgDC = CreateCompatibleDC(hdc);
    hCompleteDC = CreateCompatibleDC(hdc);

    hBuildingBit = (HBITMAP)LoadImage(NULL, L"박물관.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hMuseumBgBit = (HBITMAP)LoadImage(NULL, L"박물관배경.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCompleteBit = (HBITMAP)LoadImage(NULL, L"박물관완성이미지.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (hBuildingBit) SelectObject(hBuildingDC, hBuildingBit);
    if (hMuseumBgBit) SelectObject(hMuseumBgDC, hMuseumBgBit);
    if (hCompleteBit) SelectObject(hCompleteDC, hCompleteBit);

    const wchar_t* fortunateImages[FORTUNATE_COUNT] = {
        L"행운1.bmp", L"행운2.bmp", L"행운3.bmp"
    };
    for (int i = 0; i < FORTUNATE_COUNT; i++) {
        hFortunateDC[i] = CreateCompatibleDC(hdc);
        hFortunateBit[i] = (HBITMAP)LoadImage(NULL, fortunateImages[i],
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hFortunateBit[i]) SelectObject(hFortunateDC[i], hFortunateBit[i]);
    }

    ReleaseDC(hWnd, hdc);

    // 슬롯 RECT 계산
    for (int i = 0; i < MUSEUM_SLOT_COUNT; i++) {
        int sx = SLOT_START_X + i * SLOT_GAP;
        slotRects[i] = { sx, SLOT_START_Y, sx + SLOT_W, SLOT_START_Y + SLOT_H };
    }
}

// =====================================================================
// 해제
// =====================================================================
void Museum::Release() {
    if (hBuildingBit) { DeleteObject(hBuildingBit); hBuildingBit = NULL; }
    if (hBuildingDC) { DeleteDC(hBuildingDC);      hBuildingDC = NULL; }
    if (hMuseumBgBit) { DeleteObject(hMuseumBgBit); hMuseumBgBit = NULL; }
    if (hMuseumBgDC) { DeleteDC(hMuseumBgDC);      hMuseumBgDC = NULL; }
    if (hCompleteBit) { DeleteObject(hCompleteBit); hCompleteBit = NULL; }
    if (hCompleteDC) { DeleteDC(hCompleteDC);      hCompleteDC = NULL; }
    for (int i = 0; i < FORTUNATE_COUNT; i++) {
        if (hFortunateBit[i]) { DeleteObject(hFortunateBit[i]); hFortunateBit[i] = NULL; }
        if (hFortunateDC[i]) { DeleteDC(hFortunateDC[i]);      hFortunateDC[i] = NULL; }
    }
}

// =====================================================================
// 전체 그리기
// =====================================================================
void Museum::Draw(HDC hdc, HWND hWnd) {
    DrawBackground(hdc, hWnd);
    DrawFortunate(hdc, hWnd);
    DrawSlots(hdc, hWnd);
    DrawMoneyUI(hdc, hWnd);
}

// =====================================================================
// 배경
// =====================================================================
void Museum::DrawBackground(HDC hdc, HWND hWnd) {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int W = clientRect.right;
    int H = clientRect.bottom;

    // 비율로 배경 위치/크기 계산
    int bgX = W * 0.05;
    int bgY = H * 0.05;
    int bgW = W * 0.9;
    int bgH = H * 0.9;
    BITMAP bm;

    if (!hMuseumBgBit) return;
    GetObject(hMuseumBgBit, sizeof(bm), &bm);
    TransparentBlt(hdc, bgX, bgY,
        bgW, bgH,
        hMuseumBgDC, 0, 0, bm.bmWidth, bm.bmHeight,
        RGB(255, 255, 255));
    if (fortunateStage >= FORTUNATE_COUNT) {
        if (!hCompleteDC || !hCompleteBit) return;
        GetObject(hCompleteBit, sizeof(bm), &bm);
        StretchBlt(hdc, W / 4, H / 4,
            bgW / 2, bgH / 2,
            hCompleteDC, 0, 0, bm.bmWidth, bm.bmHeight,
            SRCCOPY);  // ← 투명처리 없이 그냥 덮어씀
        return;
    }
}

// =====================================================================
// 행운 이미지 + 단계 관리
// =====================================================================
void Museum::DrawFortunate(HDC hdc, HWND hWnd) {
    if (fortunateStage >= FORTUNATE_COUNT) return;
    if (!hFortunateBit[fortunateStage]) return;


    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int W = clientRect.right;
    int H = clientRect.bottom;

    // 비율로 위치/크기 계산
    int fx = W * 0.3;   // 윈도우 너비의 30% 위치
    int fy = H * 0.05;  // 윈도우 높이의 5% 위치
    int fw = W * 0.35;  // 윈도우 너비의 35% 크기
    int fh = H * 0.55;  // 윈도우 높이의 55% 크기

    BITMAP bm;
    GetObject(hFortunateBit[fortunateStage], sizeof(bm), &bm);
    TransparentBlt(hdc, fx, fy,
        fw, fh,
        hFortunateDC[fortunateStage],
        0, 0, bm.bmWidth, bm.bmHeight,
        RGB(255, 255, 255));
}

// =====================================================================
// 슬롯 그리기 (빈칸 + 체크 표시)
// =====================================================================
void Museum::DrawSlots(HDC hdc, HWND hWnd) {
    if (fortunateStage >= FORTUNATE_COUNT) return;

    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int W = clientRect.right;
    int H = clientRect.bottom;
    int slotW = W * 0.1;
    int slotH = H * 0.13;
    int startX = W * 0.22;
    int startY = H * 0.72;
    int gap = W * 0.13;

    int required[FORTUNATE_COUNT] = { 1, 3, 5 };
    int need = required[fortunateStage];

    for (int i = 0; i < MUSEUM_SLOT_COUNT; i++) {
        RECT r = { startX + i * gap, startY,
                         startX + i * gap + slotW, startY + slotH };
        slotRects[i] = r;

        if (slotDonated[fortunateStage][i]) {
            // 기증 완료 - 어두운 색 + 체크
            HBRUSH hBrush = CreateSolidBrush(RGB(180, 120, 30));
            FillRect(hdc, &r, hBrush);
            DeleteObject(hBrush);

            // ████ 채워진 표시
            SetTextColor(hdc, RGB(220, 160, 40));
            SetBkMode(hdc, TRANSPARENT);
            HFONT hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
            HFONT hOld = (HFONT)SelectObject(hdc, hFont);
            DrawText(hdc, L"████", -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectObject(hdc, hOld);
            DeleteObject(hFont);
        }
        else {
            // 빈칸 - 밝은 노란색
            HBRUSH hBrush = CreateSolidBrush(RGB(230, 180, 60));
            FillRect(hdc, &r, hBrush);
            DeleteObject(hBrush);

            // 테두리
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(180, 130, 0));
            HPEN hOld = (HPEN)SelectObject(hdc, hPen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, r.left, r.top, r.right, r.bottom);
            SelectObject(hdc, hOld);
            DeleteObject(hPen);
        }
    }
}

// =====================================================================
// 건물 이미지
// =====================================================================
void Museum::DrawBuilding(HDC hdc) {
    if (!hBuildingBit) return;
    BITMAP bm;
    GetObject(hBuildingBit, sizeof(bm), &bm);
    TransparentBlt(hdc,
        MUSEUM_BUILD_X, MUSEUM_BUILD_Y,
        MUSEUM_BUILD_W, MUSEUM_BUILD_H,
        hBuildingDC,
        0, 0, bm.bmWidth, bm.bmHeight,
        RGB(255, 255, 255));
}

// =====================================================================
// 돈 UI
// =====================================================================
void Museum::DrawMoneyUI(HDC hdc, HWND hWnd) {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int dynamicX = clientRect.right - MONEY_UI_W - 30;
    int dynamicY = clientRect.top + 20;

    HBRUSH hBg = CreateSolidBrush(RGB(30, 20, 5));
    HPEN   hPen = CreatePen(PS_SOLID, 2, RGB(200, 160, 40));
    HPEN   hOld = (HPEN)SelectObject(hdc, hPen);
    SelectObject(hdc, hBg);
    RoundRect(hdc, dynamicX, dynamicY,
        dynamicX + MONEY_UI_W, dynamicY + MONEY_UI_H, 18, 18);
    SelectObject(hdc, hOld);
    DeleteObject(hPen);
    DeleteObject(hBg);

    HBRUSH hCoin = CreateSolidBrush(RGB(255, 200, 30));
    HPEN   hPen2 = CreatePen(PS_SOLID, 1, RGB(180, 130, 0));
    HPEN   hOld2 = (HPEN)SelectObject(hdc, hPen2);
    SelectObject(hdc, hCoin);
    Ellipse(hdc, dynamicX + 8, dynamicY + 7,
        dynamicX + 28, dynamicY + MONEY_UI_H - 7);
    SelectObject(hdc, hOld2);
    DeleteObject(hPen2);
    DeleteObject(hCoin);

    wchar_t buf[32];
    wsprintf(buf, L"%d", money);
    DrawText_(hdc, buf, dynamicX + 36, dynamicY + 6, RGB(255, 220, 80), 18);
}

// =====================================================================
// 클릭 판정
// =====================================================================
bool Museum::IsClicked(int mx, int my) {
    RECT clientRect;
    GetClientRect(g_hWnd, &clientRect);
    int W = clientRect.right;
    int H = clientRect.bottom;

    RECT panel = {
        (int)(W * 0.1), (int)(H * 0.1),
        (int)(W * 0.9), (int)(H * 0.9)
    };
    return PtInRect(&panel, { mx, my }) == TRUE;
}

void Museum::OnClick(int mx, int my) {
    if (fortunateStage >= FORTUNATE_COUNT) return;

    int required[FORTUNATE_COUNT] = { 1, 3, 5 };
    int need = required[fortunateStage];  // ← 현재 단계 요구량

    for (int i = 0; i < MUSEUM_SLOT_COUNT; i++) {
        if (slotDonated[fortunateStage][i]) continue;  // 이미 기증됨
        if (!PtInRect(&slotRects[i], { mx, my })) continue;

        // 슬롯마다 필요한 황금작물: 0=황금밀, 1=황금감자, 2=황금당근, 3=황금딸기
        bool canDonate = false;
        if (i == 0 && pGoldWheat && pGoldWheat->GetCount() > need) canDonate = true;
        else if (i == 1 && pGoldPotato && pGoldPotato->GetCount() > need) canDonate = true;
        else if (i == 2 && pGoldCarrot && pGoldCarrot->GetCount() > need) canDonate = true;
        else if (i == 3 && pGoldStrawberry && pGoldStrawberry->GetCount() > need) canDonate = true;

        if (canDonate) {
            // 황금작물 차감
            if (i == 0) pGoldWheat->SubCount(need);
            else if (i == 1) pGoldPotato->SubCount(need);
            else if (i == 2) pGoldCarrot->SubCount(need);
            else if (i == 3) pGoldStrawberry->SubCount(need);

            slotDonated[fortunateStage][i] = true;
            SoundManager::PlaySFX_Donate(); //최종

            // 4개 모두 기증 완료 → 다음 단계로
            bool allDone = true;
            for (int j = 0; j < MUSEUM_SLOT_COUNT; j++)
                if (!slotDonated[fortunateStage][j]) { allDone = false; break; }
            if (allDone) {
                fortunateStage++;
                SoundManager::PlaySFX_LevelUp();
                if (fortunateStage == 1) {
                    player.Addfortunate(20);
                }
                else if (fortunateStage == 2) {
                    player.Addfortunate(30);
                }
                else if (fortunateStage == 3) {
                    player.Addfortunate(50);
                }
            }
        }
        else {
            MessageBox(NULL, L"황금 작물이 부족합니다!", L"알림", MB_OK);
        }
        break;
    }
}

// =====================================================================
// 텍스트 헬퍼
// =====================================================================
void Museum::DrawText_(HDC hdc, const wchar_t* text, int x, int y,
    COLORREF color, int fontSize) {
    HFONT hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    HFONT hOld = (HFONT)SelectObject(hdc, hFont);
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, x, y, text, (int)wcslen(text));
    SelectObject(hdc, hOld);
    DeleteObject(hFont);
}
