#include "Player.h"
#pragma comment(lib, "msimg32.lib") 
#define MONEY 25   // 6/8 서진씨
Player::Player()
    : x(100), y(100), frameX(0), frameY(0),
    frameWidth(16), frameHeight(32), animCount(0), scale(2), fortunate(0), money(MONEY) , speed(5), // 최종 speed추가
    hBitmap(NULL), hMemDC(NULL)
{
        is_unlock_wheat = true;        // 캐릭터의 해금여부를 바꾸고 싶다면 이걸 건드리세요
        is_unlock_potato = false;
        is_unlock_carrot = false;
        is_unlock_strawberry =false;
        is_unlock_gold_wheat = false;
        is_unlock_gold_potato = false;
        is_unlock_gold_carrot = false;
        is_unlock_gold_strawberry = false;
}

Player::~Player()
{
    Release();
}

void Player::Init(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hdc);
    ReleaseDC(hWnd, hdc);

    // 폴더 내에 있는 "주인공.bmp" 파일을 직접 로드
    hBitmap = (HBITMAP)LoadImage(NULL, L"주인공.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SelectObject(hMemDC, hBitmap);
}

void Player::Update()
{
    bool isMoving = false;  // 최종 speed 5 표시 지울 것

    // 키보드 입력에 따른 이동 및 방향(frameY) 설정
    // 이미지 행 순서: 0(아래), 1(오른쪽), 2(위), 3(왼쪽)
    if (GetAsyncKeyState('A') & 0x8000) {
        x -= speed;
        frameY = 3;
        isMoving = true;
    }
    else if (GetAsyncKeyState('D') & 0x8000) {
        x += speed;
        frameY = 1;
        isMoving = true;
    }
    else if (GetAsyncKeyState('W') & 0x8000) {
        y -= speed;
        frameY = 2;
        isMoving = true;
    }
    else if (GetAsyncKeyState('S') & 0x8000) {
        y += speed;
        frameY = 0;
        isMoving = true;
    }

    // 이동 중일 때만 걷는 애니메이션(frameX) 변경
    if (isMoving) {
        animCount++;
        if (animCount > 3) { // 숫자가 커질수록 다리 움직임이 느려집니다.
            frameX++;
            if (frameX > 3) frameX = 0; // 프레임이 4개를 넘어가면 처음으로
            animCount = 0;
        }
    }
    else {
        frameX = 0; // 가만히 있을 때는 차렷 자세 (첫 번째 열)
    }
}

void Player::Draw(HDC hdc)
{
    // 원본 스프라이트에서 자를 위치 계산
    int startX = frameX * frameWidth;
    int startY = frameY * frameHeight;

    // RGB(10, 10, 10)을 투명하게 처리하며 화면에 그림 (scale 배율로 확대)
    TransparentBlt(
        hdc, x, y, frameWidth * scale, frameHeight * scale, // 화면에 그려질 목적지 x, y, 가로, 세로
        hMemDC, startX, startY, frameWidth, frameHeight,    // 원본 이미지에서 가져올 x, y, 가로, 세로
        RGB(10, 10, 10)                                     // 투명 처리할 색상 (Color Key)
    );
}

void Player::Release()
{
    if (hBitmap) DeleteObject(hBitmap);
    if (hMemDC) DeleteDC(hMemDC);
}
    
int Player::GetCropCount() const {                              // 5/31 추가했어 서진씨
    // 1. 백 단위로 확정 개수 구하기
    // 예: 51 -> 0 / 153 -> 1
    int fixedCount = static_cast<int>(fortunate) / 100;

    // 2. 십 단위 이하로 확률(%) 구하기
    // 예: 51 -> 51% / 153 -> 53%
    int prob = static_cast<int>(fortunate) % 100;

    // 3. [확정 개수 + 1]을 기본 시작 값으로 설정
    // 예: 51일 때는 기본 1개 / 153일 때는 기본 2개
    int finalCount = fixedCount + 1;

    // 4. 말씀하신 대로 prob 확률을 뚫으면! 그때 1개를 더 더해줍니다.
    if (rand() % 100 < prob) {
        finalCount++; // ⭕ 확률 성공 시 1개 더 추가!
    }

    return finalCount;
}
//06041851