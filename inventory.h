#pragma once
#include <windows.h>
#include "Player.h"

// 1. 밀 클래스
class Wheat {
public:
    Wheat();
    ~Wheat();
    int  GetCount() const { return count; }
    void AddCount(int n) { count += n; }
    void SubCount(int n) { if (count >= n) count -= n; }

    void InitWheat(HWND hWnd);
    void Draw(HDC hBufferDC);
    void Count_Draw(HDC hdc);
    //void Lock_Draw(HDC hdc, HWND hWnd);  // ← 추가

private:
    int pos_x, pos_y;
    int size;

    int count_x, count_y;
    int count_size;
    int count;

    HDC     hMemDC;
    HBITMAP hBit;
};

// 2. 감자 클래스
class Potato {
public:
    Potato();
    ~Potato();
    int  GetCount() const { return count; }
    void AddCount(int n) { count += n; }
    void SubCount(int n) { if (count >= n) count -= n; }

    void InitPotato(HWND hWnd);
    void Draw(HDC hBufferDC);
    void Count_Draw(HDC hdc);
    //void Lock_Draw(HDC hdc);  // ← 추가

private:
    int pos_x, pos_y;
    int size;

    int count_x, count_y;
    int count_size;
    int count;

    HDC     hMemDC;
    HBITMAP hBit;
};

// 3. 당근 클래스
class Carrot {
public:
    Carrot();
    ~Carrot();

    int  GetCount() const { return count; }
    void AddCount(int n) { count += n; }
    void SubCount(int n) { if (count >= n) count -= n; }

    void InitCarrot(HWND hWnd);
    void Draw(HDC hBufferDC);
    void Count_Draw(HDC hdc);
    // void Lock_Draw(HDC hdc);  // ← 추가

private:
    int pos_x, pos_y;
    int size;

    int count_x, count_y;
    int count_size;
    int count;

    HDC     hMemDC;
    HBITMAP hBit;
};

// 4. 딸기 클래스
class Strawberry {
public:
    Strawberry();
    ~Strawberry();

    int  GetCount() const { return count; }
    void AddCount(int n) { count += n; }
    void SubCount(int n) { if (count >= n) count -= n; }

    void InitStrawberry(HWND hWnd);
    void Draw(HDC hBufferDC);
    void Count_Draw(HDC hdc);
    //void Lock_Draw(HDC hdc);  // ← 추가

private:
    int pos_x, pos_y;
    int size;

    int count_x, count_y;
    int count_size;
    int count;

    HDC     hMemDC;
    HBITMAP hBit;
};


// 1. 황금밀 클래스
class GoldWheat {
public:
    GoldWheat();
    ~GoldWheat();
    int  GetCount() const { return count; }
    void AddCount(int n) { count += n; }
    void SubCount(int n) { if (count >= n) count -= n; }
    void InitGoldWheat(HWND hWnd);
    void Draw(HDC hBufferDC);
    void Count_Draw(HDC hdc);
private:
    int pos_x, pos_y;
    int size;
    int count_x, count_y;
    int count_size;
    int count;
    HDC     hMemDC;
    HBITMAP hBit;
};

// 2. 황금감자 클래스
class GoldPotato {
public:
    GoldPotato();
    ~GoldPotato();
    int  GetCount() const { return count; }
    void AddCount(int n) { count += n; }
    void SubCount(int n) { if (count >= n) count -= n; }
    void InitGoldPotato(HWND hWnd);
    void Draw(HDC hBufferDC);
    void Count_Draw(HDC hdc);
private:
    int pos_x, pos_y;
    int size;
    int count_x, count_y;
    int count_size;
    int count;
    HDC     hMemDC;
    HBITMAP hBit;
};

// 3. 황금당근 클래스
class GoldCarrot {
public:
    GoldCarrot();
    ~GoldCarrot();
    int  GetCount() const { return count; }
    void AddCount(int n) { count += n; }
    void SubCount(int n) { if (count >= n) count -= n; }
    void InitGoldCarrot(HWND hWnd);
    void Draw(HDC hBufferDC);
    void Count_Draw(HDC hdc);
private:
    int pos_x, pos_y;
    int size;
    int count_x, count_y;
    int count_size;
    int count;
    HDC     hMemDC;
    HBITMAP hBit;
};

// 4. 황금딸기 클래스
class GoldStrawberry {
public:
    GoldStrawberry();
    ~GoldStrawberry();
    int  GetCount() const { return count; }
    void AddCount(int n) { count += n; }
    void SubCount(int n) { if (count >= n) count -= n; }
    void InitGoldStrawberry(HWND hWnd);
    void Draw(HDC hBufferDC);
    void Count_Draw(HDC hdc);
private:
    int pos_x, pos_y;
    int size;
    int count_x, count_y;
    int count_size;
    int count;
    HDC     hMemDC;
    HBITMAP hBit;
};