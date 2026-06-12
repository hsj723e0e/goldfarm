#pragma once
#include <windows.h>
#pragma comment(lib, "msimg32.lib")

#define LAB_RES_COUNT  3
#define LAB_TIER_COUNT 3

// 연구소 건물 위치/크기 
static int LAB_BUILD_X= 200;
static int LAB_BUILD_Y= 500;
static int LAB_BUILD_W=  80;
static int LAB_BUILD_H=  80;

enum ResearchCategory {
    RES_GROW_SPEED = 0,  // 초록 - 당근
    RES_MOVE_SPEED,      // 주황 - 감자
    RES_CROP_LUCK,       // 파랑 - 딸기
};

static const int RESEARCH_VALUE[LAB_TIER_COUNT] = { 100, 300, 500 };

struct ResearchNode {
    ResearchCategory category;
    int   tier;
    bool  unlocked;
    POINT pos;
    int   radius;
    int value;
};

struct CostInfo {
    int normalCount;  // 일반 작물 수량
    int goldCount;    // 황금 작물 수량
};

class Lab {
public:
    Lab();
    ~Lab();

    void Init(HWND hWnd);
    void Release();
    void Draw(HDC hdc, HWND hWnd);
    void DrawBuilding(HDC hdc);
    void OnClick(int mx, int my);
    bool IsClicked(int mx, int my);

    int GetGrowSpeed() const { return growSpeed; }
    int GetMoveSpeed() const { return moveSpeed; }
    int GetCropLuck()  const { return cropLuck; }

private:
    HDC     hBuildingDC;
    HBITMAP hBuildingBit;
    HDC     hLabBgDC;
    HBITMAP hLabBgBit;
    HDC     hCheckDC;
    HBITMAP hCheckBit;

    ResearchNode nodes[LAB_RES_COUNT][LAB_TIER_COUNT];

    int growSpeed;
    int moveSpeed;
    int cropLuck;

    bool HitTest(ResearchNode* node, int x, int y);
    bool HasEnoughMaterial(ResearchCategory cat, int tier);
    void ConsumeMaterial(ResearchCategory cat, int tier);
    void ApplyResearch(ResearchCategory cat, int tier);
    void DrawNode(HDC hdc, ResearchNode* node, COLORREF color);
    void DrawText_(HDC hdc, const wchar_t* text, int x, int y,
        COLORREF color, int fontSize);
};
