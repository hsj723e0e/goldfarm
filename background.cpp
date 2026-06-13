#include "Background.h"
#include <math.h>

Background::Background()
    : m_timeState(EARLY_MORNING), m_timeProgress(0.0f), m_currentBgColor(RGB(190, 195, 230))
    , m_hSunBit(NULL), m_sunX(0), m_sunY(0), m_isSunVisible(true)
    , m_hBgBit(NULL), m_bgWidth(0), m_bgHeight(0)
    , m_hSquirrelBit(NULL), m_squirrelFrame(0)
    , m_squirrelTimer(0.0f), m_squirrelAniTimer(0.0f)
    , m_squirrelVisible(false), m_squirrelX(900)
    , m_hBirdBit(NULL), m_birdFrame(0)
    , m_birdAniTimer(0.0f), m_birdTimer(0.0f)
    , m_birdVisible(false), m_birdX(0), m_birdY(0){
}

Background::~Background() {
    if (m_hSunBit)      DeleteObject(m_hSunBit);
    if (m_hBgBit)       DeleteObject(m_hBgBit);
    if (m_hSquirrelBit) DeleteObject(m_hSquirrelBit);
    if (m_hBirdBit) DeleteObject(m_hBirdBit);
}

void Background::Init(HWND hWnd) {
    m_hSunBit = (HBITMAP)LoadImage(NULL, L"태양.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_hBgBit = (HBITMAP)LoadImage(NULL, L"배경33.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_hSquirrelBit = (HBITMAP)LoadImage(NULL, L"다람쥐2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_hBirdBit = (HBITMAP)LoadImage(NULL, L"새그림.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (m_hBgBit) {
        BITMAP bm;
        GetObject(m_hBgBit, sizeof(BITMAP), &bm);
        m_bgWidth = bm.bmWidth;
        m_bgHeight = bm.bmHeight;
    }
}

COLORREF Background::LerpColor(COLORREF startColor, COLORREF endColor, float ratio) {
    BYTE sR = GetRValue(startColor); BYTE sG = GetGValue(startColor); BYTE sB = GetBValue(startColor);
    BYTE eR = GetRValue(endColor);   BYTE eG = GetGValue(endColor);   BYTE eB = GetBValue(endColor);

    BYTE r = (BYTE)(sR + (eR - sR) * ratio);
    BYTE g = (BYTE)(sG + (eG - sG) * ratio);
    BYTE b = (BYTE)(sB + (eB - sB) * ratio);

    return RGB(r, g, b);
}

void Background::UpdateSunPosition() {
    const float PI = 3.14159265f;
    float totalProgress = 0.0f;

    if (m_timeState == EARLY_MORNING) {
        totalProgress = m_timeProgress * 0.25f;
    }
    else if (m_timeState == DAY) {
        totalProgress = 0.25f + m_timeProgress * 0.25f;
    }
    else if (m_timeState == AFTERNOON) {
        totalProgress = 0.5f + m_timeProgress * 0.25f;
    }
    else if (m_timeState == EVENING) {
        totalProgress = 0.75f + m_timeProgress * 0.25f;
    }
    else {
        m_isSunVisible = false;
        return;
    }

    m_isSunVisible = true;
    m_sunX = (int)(-400 + totalProgress * 1400);
    float sinVal = sinf(totalProgress * PI);
    m_sunY = (int)(200 - sinVal * 200);
}

void Background::Update() {
    m_timeProgress += (m_timeState == MIDNIGHT) ? 0.0006f : 0.0015f;

    // ✅ 상태 전환을 먼저 처리 (clamp도 같이)
    if (m_timeProgress >= 1.0f) {
        m_timeProgress = 0.0f;
        m_timeState = (TimeState)((m_timeState + 1) % 5);
    }

    COLORREF startColor, endColor;
    switch (m_timeState) {
    case EARLY_MORNING: startColor = RGB(200, 180, 160); endColor = RGB(255, 255, 255); break;
    case DAY:           startColor = RGB(255, 255, 255); endColor = RGB(255, 220, 180); break;
    case AFTERNOON:     startColor = RGB(255, 220, 180); endColor = RGB(230, 110, 80); break;
    case EVENING:       startColor = RGB(230, 110, 80); endColor = RGB(30, 35, 70); break;
    case MIDNIGHT:      startColor = RGB(30, 35, 70); endColor = RGB(200, 180, 160); break;
    }


    m_currentBgColor = LerpColor(startColor, endColor, m_timeProgress);
    UpdateSunPosition();

    // 다람쥐 타이머
    if (!m_squirrelVisible) {
        m_squirrelTimer += 1.0f;
        if (m_squirrelTimer >= 800.0f) { // 약 13초 (16ms * 800)
            m_squirrelVisible = true;
            m_squirrelTimer = 0.0f;
            m_squirrelFrame = 0;
            m_squirrelX = -100;
        }
    }
    else {
        // 프레임 전환 (약 0.1초마다)
        m_squirrelAniTimer += 1.0f;
        if (m_squirrelAniTimer >= 6.0f) {
            m_squirrelFrame = (m_squirrelFrame + 1) % 6;
            m_squirrelAniTimer = 0.0f;
        }
        // 왼쪽으로 이동
        m_squirrelX += 3;
        if (m_squirrelX > 250) {
            m_squirrelVisible = false;
            m_squirrelTimer = 0.0f;
        }
    }
    // 새 타이머
    if (!m_birdVisible) {
        m_birdTimer += 1.0f;
        if (m_birdTimer >= 600.0f) { // 약 10초마다 등장
            m_birdVisible = true;
            m_birdTimer = 0.0f;
            m_birdFrame = 0;
            m_birdX = -100; // 화면 중앙 시작 - screenWidth 없으므로 아래 참고
            m_birdY = 30; // 높이는 원하는 값으로
        }
    }
    else {
        m_birdAniTimer += 1.0f;
        if (m_birdAniTimer >= 6.0f) {
            m_birdFrame = (m_birdFrame + 1) % 12; // 1x12
            m_birdAniTimer = 0.0f;     
        }
        m_birdX += 3; // 오른쪽으로 이동
        if (m_birdX > 1100) { // 화면 밖으로 나가면 숨김
            m_birdVisible = false;
            m_birdTimer = 0.0f;
        }
    }
}

void Background::Draw(HDC hMemDC, RECT rect) {
    int screenWidth = rect.right - rect.left;
    int screenHeight = rect.bottom - rect.top;
    
    // 1. 배경 이미지 그리기
    if (m_hBgBit) {
        HDC hBgDC = CreateCompatibleDC(hMemDC);
        HBITMAP hOldBg = (HBITMAP)SelectObject(hBgDC, m_hBgBit);
        StretchBlt(hMemDC, 0, 0, screenWidth, screenHeight, hBgDC, 0, 0, m_bgWidth, m_bgHeight, SRCCOPY);
        SelectObject(hBgDC, hOldBg);
        DeleteDC(hBgDC);
    }

    // 2. 시간대별 색상 오버레이
    if (m_currentBgColor != RGB(255, 255, 255)) {
        HDC hColorDC = CreateCompatibleDC(hMemDC);
        HBITMAP hColorBit = CreateCompatibleBitmap(hMemDC, screenWidth, screenHeight);
        HBITMAP hOldColor = (HBITMAP)SelectObject(hColorDC, hColorBit);

        HBRUSH hBrush = CreateSolidBrush(m_currentBgColor);
        RECT r = { 0, 0, screenWidth, screenHeight };
        FillRect(hColorDC, &r, hBrush);
        DeleteObject(hBrush);

        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.AlphaFormat = 0;

        float alpha = 0.0f;
        switch (m_timeState) {
        case EARLY_MORNING: alpha = 50.0f - m_timeProgress * 50.0f;   break; // 50→0
        case DAY:           alpha = 0.0f;                              break;
        case AFTERNOON:     alpha = 0.0f + m_timeProgress * 80.0f;   break; // 0→80
        case EVENING:       alpha = 80.0f + m_timeProgress * 40.0f;   break; // 80→120
        case MIDNIGHT:      alpha = 120.0f - m_timeProgress * 70.0f;  break; // 120→50
        }
        bf.SourceConstantAlpha = (BYTE)alpha;

        GdiAlphaBlend(hMemDC, 0, 0, screenWidth, screenHeight,
            hColorDC, 0, 0, screenWidth, screenHeight, bf);

        SelectObject(hColorDC, hOldColor);
        DeleteObject(hColorBit);
        DeleteDC(hColorDC);
    }

    // 3. 태양 그리기
    //if (m_isSunVisible && m_hSunBit) {
    //    HDC hSunDC = CreateCompatibleDC(hMemDC);
    //    HBITMAP hOldBit = (HBITMAP)SelectObject(hSunDC, m_hSunBit);

    //    BITMAP bm;
    //    GetObject(m_hSunBit, sizeof(BITMAP), &bm);
    //    TransparentBlt(hMemDC, m_sunX, m_sunY, 60, 60,
    //        hSunDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));

    //    SelectObject(hSunDC, hOldBit);
    //    DeleteDC(hSunDC);
    //}

    //// 4. 다람쥐 그리기
    //if (m_squirrelVisible && m_hSquirrelBit) {
    //    BITMAP bm;
    //    GetObject(m_hSquirrelBit, sizeof(BITMAP), &bm);

    //    int frameW = bm.bmWidth / 3; // 4열
    //    int frameH = bm.bmHeight / 2; // 2행

    //    int col = m_squirrelFrame % 3;
    //    int row = m_squirrelFrame / 3;

    //    HDC hSqDC = CreateCompatibleDC(hMemDC);
    //    HBITMAP hOldBit = (HBITMAP)SelectObject(hSqDC, m_hSquirrelBit);

    //    TransparentBlt(hMemDC, m_squirrelX, 500, frameW/3, frameH/3,
    //        hSqDC, col * frameW, row * frameH, frameW, frameH,
    //        RGB(255, 255, 255));

    //    SelectObject(hSqDC, hOldBit);
    //    DeleteDC(hSqDC);
    //}
    //// 5. 새 그리기
    //if (m_birdVisible && m_hBirdBit) {
    //    BITMAP bm;
    //    GetObject(m_hBirdBit, sizeof(BITMAP), &bm);

    //    int frameW = bm.bmWidth / 12; // 1x12
    //    int frameH = bm.bmHeight;

    //    HDC hBirdDC = CreateCompatibleDC(hMemDC);
    //    HBITMAP hOldBit = (HBITMAP)SelectObject(hBirdDC, m_hBirdBit);

    //    TransparentBlt(hMemDC, m_birdX, m_birdY, frameW/2, frameH/2,
    //        hBirdDC, m_birdFrame * frameW, 0, frameW, frameH,
    //        RGB(0, 0, 0));

    //    SelectObject(hBirdDC, hOldBit);
    //    DeleteDC(hBirdDC);
    //}
}

// 06032148