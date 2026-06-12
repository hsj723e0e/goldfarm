#pragma once
#include <windows.h>

// 작물 종류 정의
enum CropType { NONE = 0, WHEAT, POTATO, CARROT, STRAWBERRY };

// 작물 정보 구조체
struct Crop {
    CropType type;
    int stage;              // 성장 단계 (0~4)
    DWORD lastGrowthTime;   // 마지막 성장 시간
    DWORD growthInterval;   // 성장 주기
    bool isGolden;          // 황금 작물 여부
};

class Field {
private:
    int x, y, fieldWidth, fieldHeight;
    int centerX, centerY;
    bool isUnlocked;
    Crop currentCrop;

    // GDI 관련 객체
    HDC hFieldDC, hCropDC;
    HBITMAP hLockFieldBit, hUnlockFieldBit;
    HBITMAP hCropBit[5];        // 일반 작물 4종 + 빈칸
    HBITMAP hGoldenCropBit[5];  // 황금 작물 4종

    int GrowSpeed; //최종

public:
    Field();
    ~Field();

    // 기본 기능
    void Init(HWND hWnd, int sx, int sy, bool unlocked);
    void UpdateGrowth();
    void Plant(CropType type);
    void Draw(HDC hdc);
    bool IsClicked(int mx, int my);
    void Release();

    // 수확 시스템 추가 함수
    bool IsHarvestable(); // 작물이 다 자랐는지(4단계) 확인
    CropType Harvest();       // 수확 처리 (작물 초기화)

    // Getter/Setter
    bool GetIsUnlocked() { return isUnlocked; }
    void SetUnlocked(bool val) { isUnlocked = val; }
    int GetX() { return x; }
    int GetY() { return y; }
    int GetCenterX() { return centerX; }
    int GetCenterY() { return centerY; }
    void AddGrowSpeed(int val) { GrowSpeed -= val; } //최종
};