#include "Field.h"
#pragma comment(lib, "msimg32.lib")

// (1) 생성자 수정 (isGolden 값인 false 추가)
Field::Field() : x(0), y(0), isUnlocked(false), fieldWidth(100), fieldHeight(100) {
    currentCrop = { NONE, 0, 0, 3000, false }; // 여기 false 추가
    hLockFieldBit = NULL; hUnlockFieldBit = NULL;
    for (int i = 0; i < 5; i++) hCropBit[i] = NULL;
}


void Field::Init(HWND hWnd, int sx, int sy, bool unlocked) {
    x = sx; y = sy; isUnlocked = unlocked;
    GrowSpeed = 3000;   //최종

    HDC hdc = GetDC(hWnd);
    hFieldDC = CreateCompatibleDC(hdc);
    hCropDC = CreateCompatibleDC(hdc);
    hLockFieldBit = (HBITMAP)LoadImage(NULL, L"lock_field.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hUnlockFieldBit = (HBITMAP)LoadImage(NULL, L"unlock_field.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCropBit[WHEAT] = (HBITMAP)LoadImage(NULL, L"밀.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCropBit[POTATO] = (HBITMAP)LoadImage(NULL, L"감자.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCropBit[CARROT] = (HBITMAP)LoadImage(NULL, L"당근.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hCropBit[STRAWBERRY] = (HBITMAP)LoadImage(NULL, L"딸기.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    ReleaseDC(hWnd, hdc);
}

void Field::UpdateGrowth() {
    if (currentCrop.stage == 0 || currentCrop.stage >= 4) return;
    if (GetTickCount() - currentCrop.lastGrowthTime > currentCrop.growthInterval) {
        currentCrop.stage++;
        currentCrop.lastGrowthTime = GetTickCount();
    }
}

// (2) Plant 함수 수정 (isGolden 값인 false 추가)
void Field::Plant(CropType type) {
    if (isUnlocked && type != NONE)
        currentCrop = { type, 1, GetTickCount(), (DWORD)GrowSpeed, false }; // 여기 false 추가 // 최종
}


void Field::Draw(HDC hdc) {
    HBITMAP hCurrentField = isUnlocked ? hUnlockFieldBit : hLockFieldBit;
    SelectObject(hFieldDC, hCurrentField);
    TransparentBlt(hdc, x, y, fieldWidth, fieldHeight, hFieldDC, 0, 0, 100, 100, RGB(255, 255, 255));

    if (isUnlocked && currentCrop.type != NONE && currentCrop.stage > 0) {
        SelectObject(hCropDC, hCropBit[currentCrop.type]);
        int cropSourceX = (currentCrop.stage - 1) * 100;
        TransparentBlt(hdc, x, y, fieldWidth, fieldHeight, hCropDC, cropSourceX, 0, 100, 100, RGB(255, 255, 255));
    }
}

bool Field::IsClicked(int mx, int my) {
    RECT rc = { x, y, x + fieldWidth, y + fieldHeight };
    return PtInRect(&rc, { mx, my });
}

Field::~Field() { Release(); }
void Field::Release() {
    if (hLockFieldBit) DeleteObject(hLockFieldBit);
    if (hUnlockFieldBit) DeleteObject(hUnlockFieldBit);
    for (int i = 0; i < 5; i++) if (hCropBit[i]) DeleteObject(hCropBit[i]);
    DeleteDC(hFieldDC); DeleteDC(hCropDC);
}
// Field.cpp의 맨 아래에 추가
bool Field::IsHarvestable() {
    return (isUnlocked && currentCrop.type != NONE && currentCrop.stage >= 4);
}

CropType Field::Harvest() {
    // 1. 초기화하기 전에 무슨 작물이었는지 기억해둠
    CropType harvestedType = currentCrop.type;

    // 2. 수확 시 상태 초기화
    currentCrop.type = NONE;
    currentCrop.stage = 0;
    currentCrop.lastGrowthTime = 0;
    currentCrop.isGolden = false;

    // 3. 기억해둔 작물 타입 반환
    return harvestedType;
}