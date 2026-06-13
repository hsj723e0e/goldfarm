#pragma once
#include <windows.h>

enum TimeState {
    EARLY_MORNING,
    DAY,
    AFTERNOON,
    EVENING,
    MIDNIGHT
};

class Background {
public:
    Background();
    ~Background();

    void Init(HWND hWnd);
    void Update();
    void Draw(HDC hMemDC, RECT rect);

private:
    COLORREF LerpColor(COLORREF startColor, COLORREF endColor, float ratio);
    void UpdateSunPosition();

    // 시간
    TimeState m_timeState;
    float     m_timeProgress;
    COLORREF  m_currentBgColor;

    // 배경 이미지
    HBITMAP m_hBgBit;
    int     m_bgWidth;
    int     m_bgHeight;

    // 태양
    HBITMAP m_hSunBit;
    int     m_sunX;
    int     m_sunY;
    bool    m_isSunVisible;

    // 다람쥐
    HBITMAP m_hSquirrelBit;
    int     m_squirrelFrame;
    float   m_squirrelTimer;
    float   m_squirrelAniTimer;
    bool    m_squirrelVisible;
    int     m_squirrelX;

    // 새 관련 멤버변수 추가
    HBITMAP m_hBirdBit;
    int     m_birdFrame;
    float   m_birdAniTimer;
    float   m_birdTimer;
    bool    m_birdVisible;
    int     m_birdX;
    int     m_birdY;
};
