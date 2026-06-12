#include "lab.h"
#include "Field.h"
#include "inventory.h"
#include "Player.h"
#include <tchar.h>
#pragma comment(lib, "msimg32.lib")

extern Player        player;
extern Field         fields[8]; 
extern Potato          invPotato;
extern Carrot          invCarrot;
extern Strawberry      invStrawberry;
extern GoldPotato      invGoldPotato;
extern GoldCarrot      invGoldCarrot;
extern GoldStrawberry  invGoldStrawberry;
// =====================================================================
//  레이아웃 상수
// =====================================================================
static const int LAB_X = 0;
static const int LAB_Y = 0;
static const int LAB_BG_W = 690;
static const int LAB_BG_H = 600;

// 노드 색상
static const COLORREF NODE_COLOR[LAB_RES_COUNT] = {
    RGB(0, 220,   0),   // RES_GROW_SPEED - 초록 (당근)
    RGB(255, 165,   0),   // RES_MOVE_SPEED - 주황 (감자)
    RGB(0, 180, 255),   // RES_CROP_LUCK  - 파랑 (딸기)
};

// 재료 비용 [카테고리][티어]
// I  : 일반 10개
// II : 황금  5개
// III: 황금 10개
static const CostInfo COST[LAB_RES_COUNT][LAB_TIER_COUNT] = {
    { {10,0}, {0,5}, {0,10} },  // RES_GROW_SPEED (당근)
    { {10,0}, {0,5}, {0,10} },  // RES_MOVE_SPEED (감자)
    { {10,0}, {0,5}, {0,10} },  // RES_CROP_LUCK  (딸기)
};

// =====================================================================
//  생성자 / 소멸자
// =====================================================================
Lab::Lab()
    : hBuildingDC(NULL), hBuildingBit(NULL),
    hLabBgDC(NULL), hLabBgBit(NULL),
    hCheckDC(NULL), hCheckBit(NULL),
    growSpeed(0), moveSpeed(0), cropLuck(0)
{
    ZeroMemory(nodes, sizeof(nodes));
}

Lab::~Lab() {
    Release();
}

// =====================================================================
//  초기화
// =====================================================================
void Lab::Init(HWND hWnd) {
    HDC hdc = GetDC(hWnd);

    hBuildingDC = CreateCompatibleDC(hdc);
    hLabBgDC = CreateCompatibleDC(hdc);
    hCheckDC = CreateCompatibleDC(hdc);

    hBuildingBit = (HBITMAP)LoadImage(NULL, L"연구소.bmp",
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBuildingBit) SelectObject(hBuildingDC, hBuildingBit);

    hLabBgBit = (HBITMAP)LoadImage(NULL, L"연구소_배경.bmp",
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hLabBgBit) SelectObject(hLabBgDC, hLabBgBit);

    hCheckBit = (HBITMAP)LoadImage(NULL, L"체크.bmp",
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hCheckBit) SelectObject(hCheckDC, hCheckBit);

    ReleaseDC(hWnd, hdc);

    // 노드 위치 (연구소_배경.bmp 기준)
    nodes[RES_MOVE_SPEED][0] = { RES_MOVE_SPEED, 0, false, {446, 137}, 35, 2 };
    nodes[RES_MOVE_SPEED][1] = { RES_MOVE_SPEED, 1, false, {558, 101}, 35, 2 };
    nodes[RES_MOVE_SPEED][2] = { RES_MOVE_SPEED, 2, false, {678,  71}, 35, 3 };

    nodes[RES_GROW_SPEED][0] = { RES_GROW_SPEED, 0, false, { 196, 322}, 35, 200 };
    nodes[RES_GROW_SPEED][1] = { RES_GROW_SPEED, 1, false, {146, 414}, 35, 300 };
    nodes[RES_GROW_SPEED][2] = { RES_GROW_SPEED, 2, false, {89, 502}, 35, 500 };

    nodes[RES_CROP_LUCK][0] = { RES_CROP_LUCK,  0, false, {472, 389}, 35, 20 };
    nodes[RES_CROP_LUCK][1] = { RES_CROP_LUCK,  1, false, {581, 419}, 35, 30 };
    nodes[RES_CROP_LUCK][2] = { RES_CROP_LUCK,  2, false, {698, 459}, 35, 50 };
}

// =====================================================================
//  해제
// =====================================================================
void Lab::Release() {
    if (hBuildingBit) { DeleteObject(hBuildingBit); hBuildingBit = NULL; }
    if (hBuildingDC) { DeleteDC(hBuildingDC);      hBuildingDC = NULL; }
    if (hLabBgBit) { DeleteObject(hLabBgBit);     hLabBgBit = NULL; }
    if (hLabBgDC) { DeleteDC(hLabBgDC);          hLabBgDC = NULL; }
    if (hCheckBit) { DeleteObject(hCheckBit);     hCheckBit = NULL; }
    if (hCheckDC) { DeleteDC(hCheckDC);          hCheckDC = NULL; }
}

// =====================================================================
//  재료 확인
// =====================================================================
bool Lab::HasEnoughMaterial(ResearchCategory cat, int tier) {
    CostInfo cost = COST[cat][tier];
    switch (cat) {
    case RES_MOVE_SPEED: // 감자
        if (cost.normalCount > 0) return invPotato.GetCount() >= cost.normalCount;
        if (cost.goldCount > 0) return invGoldPotato.GetCount() >= cost.goldCount;
        break;
    case RES_GROW_SPEED: // 당근
        if (cost.normalCount > 0) return invCarrot.GetCount() >= cost.normalCount;
        if (cost.goldCount > 0) return invGoldCarrot.GetCount() >= cost.goldCount;
        break;
    case RES_CROP_LUCK:  // 딸기
        if (cost.normalCount > 0) return invStrawberry.GetCount() >= cost.normalCount;
        if (cost.goldCount > 0) return invGoldStrawberry.GetCount() >= cost.goldCount;
        break;
    }
    return false;
}

// =====================================================================
//  재료 차감
// =====================================================================
void Lab::ConsumeMaterial(ResearchCategory cat, int tier) {
    CostInfo cost = COST[cat][tier];
    switch (cat) {
    case RES_MOVE_SPEED:
        if (cost.normalCount > 0) invPotato.SubCount(cost.normalCount);
        if (cost.goldCount > 0) invGoldPotato.SubCount(cost.goldCount);
        break;
    case RES_GROW_SPEED:
        if (cost.normalCount > 0) invCarrot.SubCount(cost.normalCount);
        if (cost.goldCount > 0) invGoldCarrot.SubCount(cost.goldCount);
        break;
    case RES_CROP_LUCK:
        if (cost.normalCount > 0) invStrawberry.SubCount(cost.normalCount);
        if (cost.goldCount > 0) invGoldStrawberry.SubCount(cost.goldCount);
        break;
    }
}

// =====================================================================
//  히트 테스트
// =====================================================================
bool Lab::HitTest(ResearchNode* node, int x, int y) {
    int dx = x - node->pos.x;
    int dy = y - node->pos.y;
    return (dx * dx + dy * dy) <= (node->radius * node->radius);
}

// =====================================================================
//  연구 적용
// =====================================================================
void Lab::ApplyResearch(ResearchCategory cat, int tier) {
    int val = nodes[cat][tier].value;
    switch (cat) {
    case RES_GROW_SPEED:
        for (int i = 0; i < 8; i++)
            fields[i].AddGrowSpeed(val);
        growSpeed += val;  // ← 이게 텍스트에 표시됨
        break;
    case RES_MOVE_SPEED:
        player.AddSpeed(val);
        moveSpeed += val;  // ← 이게 텍스트에 표시됨
        break;
    case RES_CROP_LUCK:
        player.Addfortunate(val);
        cropLuck += val;   // ← 이게 텍스트에 표시됨
        break;
    }
}

// =====================================================================
//  클릭 판정
// =====================================================================
bool Lab::IsClicked(int mx, int my) {
    RECT panel = { LAB_X, LAB_Y, LAB_X + LAB_BG_W, LAB_Y + LAB_BG_H };
    return PtInRect(&panel, { mx, my }) == TRUE;
}

void Lab::OnClick(int mx, int my) {
    for (int cat = 0; cat < LAB_RES_COUNT; cat++) {
        for (int tier = 0; tier < LAB_TIER_COUNT; tier++) {
            ResearchNode* node = &nodes[cat][tier];

            if (!HitTest(node, mx, my)) continue;

            // 선행 노드 미해금이면 반응 없음
            if (tier >= 1 && !nodes[cat][0].unlocked) return;
            if (tier >= 2 && !nodes[cat][1].unlocked) return;

            // 이미 해금된 경우 무시
            if (node->unlocked) return;

            // 재료 부족이면 반응 없음
            if (!HasEnoughMaterial((ResearchCategory)cat, tier)) return;

            // 재료 차감 후 해금
            ConsumeMaterial((ResearchCategory)cat, tier);
            node->unlocked = true;
            ApplyResearch((ResearchCategory)cat, tier);
            return;
        }
    }
}

// =====================================================================
//  노드 그리기 (테두리만, 연결선 없음)
// =====================================================================
void Lab::DrawNode(HDC hdc, ResearchNode* node, COLORREF color) {
    int cx = node->pos.x;
    int cy = node->pos.y;
    int r = node->radius;

    HPEN   pen = CreatePen(PS_SOLID, 3, color);
    HBRUSH brush = node->unlocked
        ? CreateSolidBrush(color)           // 해금: 색으로 채움
        : (HBRUSH)GetStockObject(NULL_BRUSH);
    HPEN   oldPen = (HPEN)SelectObject(hdc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);

    // 해금 시 체크 표시
    if (node->unlocked) {
        HPEN checkPen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
        HPEN oldCp = (HPEN)SelectObject(hdc, checkPen);
        MoveToEx(hdc, cx - r / 3, cy, NULL);
        LineTo(hdc, cx - r / 8, cy + r / 3);
        LineTo(hdc, cx + r / 3, cy - r / 4);
        SelectObject(hdc, oldCp);
        DeleteObject(checkPen);
    }

    // 미해금 시 필요 재료 수량 표시
    if (!node->unlocked) {
        CostInfo cost = COST[node->category][node->tier];
        wchar_t buf[32]; // 문자열 버퍼를 조금 더 늘려줌

        // 1) 카테고리에 따른 작물 이름 판정
        const wchar_t* cropName = L"";
        switch (node->category) {
        case RES_GROW_SPEED: cropName = L"당근"; break;
        case RES_MOVE_SPEED: cropName = L"감자"; break;
        case RES_CROP_LUCK:  cropName = L"딸기"; break;
        }

        // 2) 일반/황금 티어에 따른 문자열 포맷팅
        if (cost.normalCount > 0)
            wsprintf(buf, L"%s %d", cropName, cost.normalCount); // 예: 당근 10
        else
            wsprintf(buf, L"금%s %d", cropName, cost.goldCount); // 예: 금감자 5

        HFONT hFont = CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
        HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

        SetTextColor(hdc, RGB(255, 235, 100)); // 글씨가 잘 보이도록 부드러운 노란색 계열
        SetBkMode(hdc, TRANSPARENT);

        // 3) 글자 길이에 맞춰 중앙 정렬되도록 살짝 보정 (TextOut 좌표 수정)
        // 기존 cx - 10 대신 글자 수에 맞춰 조금 더 왼쪽에서 시작하도록 조정
        TextOut(hdc, cx - (lstrlen(buf) * 4), cy - r - 25, buf, lstrlen(buf));

        SelectObject(hdc, oldFont);
        DeleteObject(hFont);
    }
}

// =====================================================================
//  전체 그리기
// =====================================================================
void Lab::Draw(HDC hdc, HWND hWnd) {
    RECT cr;
    GetClientRect(hWnd, &cr);
    int w = cr.right;
    int h = cr.bottom;

    // 1) 배경
    if (hLabBgBit) {
        RECT cr;
        GetClientRect(hWnd, &cr);
        BITMAP bm;
        GetObject(hLabBgBit, sizeof(bm), &bm);
        TransparentBlt(hdc,
            LAB_X, LAB_Y, cr.right, cr.bottom,
            hLabBgDC,
            0, 0, bm.bmWidth, bm.bmHeight,
            RGB(255, 255, 255));
    }

    // 2) 노드 그리기 (연결선 없음)
    for (int cat = 0; cat < LAB_RES_COUNT; cat++) {
        for (int tier = 0; tier < LAB_TIER_COUNT; tier++) {
            DrawNode(hdc, &nodes[cat][tier], NODE_COLOR[cat]);
        }
    }

    // 3) 현재 수치 텍스트
    wchar_t buf[64];
    int textX = (int)(w * 0.5);   // 왼쪽에서 1%
    int textY1 = (int)(h * 0.85);  // 아래에서 85%
    int textY2 = (int)(h * 0.90);  // 아래에서 90%
    int textY3 = (int)(h * 0.95);  // 아래에서 95%

    wsprintf(buf, L"성장속도 +%d", growSpeed);
    DrawText_(hdc, buf, textX, textY1, RGB(0, 220, 0), 30);

    wsprintf(buf, L"이동속도 +%d", moveSpeed);
    DrawText_(hdc, buf, textX, textY2, RGB(255, 165, 0), 30);

    wsprintf(buf, L"작물행운 +%d", cropLuck);
    DrawText_(hdc, buf, textX, textY3, RGB(0, 180, 255), 30);
}

// =====================================================================
//  건물 그리기
// =====================================================================
void Lab::DrawBuilding(HDC hdc) {
    if (!hBuildingBit) return;
    BITMAP bm;
    GetObject(hBuildingBit, sizeof(bm), &bm);
    TransparentBlt(hdc,
        LAB_BUILD_X, LAB_BUILD_Y,
        LAB_BUILD_W, LAB_BUILD_H,
        hBuildingDC,
        0, 0, bm.bmWidth, bm.bmHeight,
        RGB(255, 255, 255));
}

// =====================================================================
//  텍스트 헬퍼
// =====================================================================
void Lab::DrawText_(HDC hdc, const wchar_t* text, int x, int y,
    COLORREF color, int fontSize) {
    HFONT hFont = CreateFont(
        fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, x, y, text, (int)wcslen(text));
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}
