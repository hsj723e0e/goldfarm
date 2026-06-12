#pragma once
#include <windows.h>

class Player
{
private:
    int x, y;
    int frameX, frameY;
    int frameWidth;
    int frameHeight;
    int animCount;
    int scale;
    int speed;      //최종

    int fortunate;      // 05.27 18:30 to 서진씨
    int money;  // 6/8 서진씨
    bool is_unlock_wheat;
    bool is_unlock_potato;
    bool is_unlock_carrot;
    bool is_unlock_strawberry;
    bool is_unlock_gold_wheat;
    bool is_unlock_gold_potato;
    bool is_unlock_gold_carrot;
    bool is_unlock_gold_strawberry;
    HBITMAP hBitmap;
    HDC hMemDC;

public:
    Player();
    ~Player();
    int GetX() { return x; }
    int GetY() { return y; }
    void Addfortunate(int add) { fortunate += add; } // 05.27 
    int Getfortunate() { return fortunate; }    // 05.27 임시 테스트용
    void Init(HWND hWnd);
    void Update();
    void Draw(HDC hdc);
    void Release();
    bool GetUnlockWheat()            const { return is_unlock_wheat; }
    bool GetUnlockPotato()           const { return is_unlock_potato; }
    bool GetUnlockCarrot()           const { return is_unlock_carrot; }
    bool GetUnlockStrawberry()       const { return is_unlock_strawberry; }
    bool GetUnlockGoldWheat()        const { return is_unlock_gold_wheat; }
    bool GetUnlockGoldPotato()       const { return is_unlock_gold_potato; }
    bool GetUnlockGoldCarrot()       const { return is_unlock_gold_carrot; }
    bool GetUnlockGoldStrawberry()   const { return is_unlock_gold_strawberry; }
    void SetUnlockWheat(bool value) { is_unlock_wheat = value; }    // to 서진씨  5/31 15시 함수 8개 추가함
    void SetUnlockPotato(bool value) { is_unlock_potato = value; }
    void SetUnlockCarrot(bool value) { is_unlock_carrot = value; }
    void SetUnlockStrawberry(bool value) { is_unlock_strawberry = value; }
    void SetUnlockGoldWheat(bool value) { is_unlock_gold_wheat = value; }
    void SetUnlockGoldPotato(bool value) { is_unlock_gold_potato = value; }
    void SetUnlockGoldCarrot(bool value) { is_unlock_gold_carrot = value; }
    void SetUnlockGoldStrawberry(bool value) { is_unlock_gold_strawberry = value; }
    int GetCropCount() const;   // 5/31 이것도
    int GetMoney()const { return money; }  // 6/8 이것도
    void AddSpeed(int val) { speed += val; } //최종
};