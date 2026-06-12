#include <windows.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include "Player.h"
#include "Field.h"
#include "inventory.h" 
#include "shop.h"      
#include "museum.h" 
#include "Background.h" // 6/3배경 추가
#include "lab.h" // 최종
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

Player player;
Field fields[8];
HWND g_hWnd;

Background g_background; // 6/3 배경 추가

// --- 인벤토리 작물 객체 전역 선언 ---
Wheat           invWheat;
Potato          invPotato;
Carrot          invCarrot;
Strawberry      invStrawberry;

// --- 황금 작물 객체 ---
GoldWheat       invGoldWheat;
GoldPotato      invGoldPotato;
GoldCarrot      invGoldCarrot;
GoldStrawberry  invGoldStrawberry;

// --- 상점 객체 및 UI 상태 전역 선언 ---
Shop shop;
bool showShopUI = false;

// ★ 박물관 객체 및 UI 상태 추가
Museum museum;
bool showMuseumUI = false;

// ★ 연구소 객체 및 UI 상태 추가 //최종
Lab    lab;
bool   showLabUI = false;


// --- 기존 UI 상태 변수 ---
bool showPlantList = false;
int currentFieldListIndex = -1;

// --- 수확 연출 및 UI 관련 전역 변수 ---
HBITMAP hInvCrops[5];    // 일반 작물 UI 아이콘
HBITMAP hSpeCrops[5];    // 황금 작물 UI 아이콘
HBITMAP hEffectSprite;   // 수확 애니메이션 스프라이트 

bool showHarvestUI = false;
DWORD harvestUITime = 0;
CropType lastCropType = NONE;
bool lastWasGolden = false;
int lastCropCount = 0;

// --- 애니메이션 제어 변수 ---
bool playEffect = false;
int effectFrame = 0;
int effectX = 0, effectY = 0;
DWORD effectLastTime = 0;
const int MAX_EFFECT_FRAMES = 5;

void ShowInventory(HDC hdc) //최종
{

    invWheat.Draw(hdc);
    invPotato.Draw(hdc);
    invCarrot.Draw(hdc);
    invStrawberry.Draw(hdc);

    invWheat.Count_Draw(hdc);
    invPotato.Count_Draw(hdc);
    invCarrot.Count_Draw(hdc);
    invStrawberry.Count_Draw(hdc);

    invGoldWheat.Draw(hdc);
    invGoldPotato.Draw(hdc);
    invGoldCarrot.Draw(hdc);
    invGoldStrawberry.Draw(hdc);

    invGoldWheat.Count_Draw(hdc);
    invGoldPotato.Count_Draw(hdc);
    invGoldCarrot.Count_Draw(hdc);
    invGoldStrawberry.Count_Draw(hdc);
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    srand((unsigned int)time(NULL));

    WNDCLASS wndClass = { 0 };
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = WndProc;
    wndClass.lpszClassName = L"FarmGame";
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wndClass);

    RECT wr = { 0, 0, 800, 600 };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    g_hWnd = CreateWindow(L"FarmGame", L"농장 경영 게임", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, hInstance, NULL);

    ShowWindow(g_hWnd, nCmdShow);

    MSG Message;
    while (true)
    {
        if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
        {
            if (Message.message == WM_QUIT) break;
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        else
        {
            player.Update();
            for (int i = 0; i < 8; i++) fields[i].UpdateGrowth();

            g_background.Update(); // ★ 배경 시간 업데이트 수행       6/3 

            if (playEffect && (GetTickCount() - effectLastTime > 60)) {
                effectFrame++;
                effectLastTime = GetTickCount();
                if (effectFrame >= MAX_EFFECT_FRAMES) {
                    playEffect = false;
                }
            }

            InvalidateRect(g_hWnd, NULL, FALSE);
            Sleep(16);
        }
    }
    return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc, hMemDC;
    static HBITMAP hBit, hOldBit;
    PAINTSTRUCT ps;
    RECT rect;

    switch (iMessage) {
    case WM_CREATE:
    {
        player.Init(hWnd);
        g_background.Init(hWnd);    // 6/3
        int startX = 200, startY = 150;
        int gapX = 100, gapY = 100;
        for (int i = 0; i < 8; i++) {
            bool isFirst = (i == 0);
            fields[i].Init(hWnd, startX + (i % 4) * gapX, startY + (i / 4) * gapY, isFirst);
        }

        // 일반 작물 초기화
        invWheat.InitWheat(hWnd);
        invPotato.InitPotato(hWnd);
        invCarrot.InitCarrot(hWnd);
        invStrawberry.InitStrawberry(hWnd);

        // 황금 작물 초기화
        invGoldWheat.InitGoldWheat(hWnd);
        invGoldPotato.InitGoldPotato(hWnd);
        invGoldCarrot.InitGoldCarrot(hWnd);
        invGoldStrawberry.InitGoldStrawberry(hWnd);

        // 상점 초기화
        shop.Init(hWnd);
        shop.SetInventory(&invWheat, &invPotato, &invCarrot, &invStrawberry);

        // ★ 박물관 초기화 및 인벤토리 연결
        museum.Init(hWnd);
        museum.SetInventory(&invWheat, &invPotato, &invCarrot, &invStrawberry,
            &invGoldWheat, &invGoldPotato, &invGoldCarrot, &invGoldStrawberry);

        lab.Init(hWnd); // 최종

        
        hInvCrops[WHEAT] = (HBITMAP)LoadImage(NULL, L"밀1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hInvCrops[POTATO] = (HBITMAP)LoadImage(NULL, L"감자1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hInvCrops[CARROT] = (HBITMAP)LoadImage(NULL, L"당근1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hInvCrops[STRAWBERRY] = (HBITMAP)LoadImage(NULL, L"딸기1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        hSpeCrops[WHEAT] = (HBITMAP)LoadImage(NULL, L"spe_wheat.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hSpeCrops[POTATO] = (HBITMAP)LoadImage(NULL, L"spe_potato.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hSpeCrops[CARROT] = (HBITMAP)LoadImage(NULL, L"spe_carrot.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hSpeCrops[STRAWBERRY] = (HBITMAP)LoadImage(NULL, L"spe_strawberry.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        hEffectSprite = (HBITMAP)LoadImage(NULL, L"수확애니메이션.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        int mx = LOWORD(lParam);
        int my = HIWORD(lParam);
        POINT pt = { mx, my };

        // 1. 상점 UI 클릭 처리
        if (showShopUI) {
            if (shop.IsClicked(mx, my)) {
                shop.OnClick(mx, my);
                return 0;
            }
            else {
                showShopUI = false;
            }
        }

        // ★ 2. 박물관 UI 클릭 처리
        if (showMuseumUI) {
            if (museum.IsClicked(mx, my)) {
                museum.OnClick(mx, my);
                return 0;
            }
            else {
                showMuseumUI = false;
            }
        }

        // ★ 3. 연구소 UI 클릭 처리        // 최종
        if (showLabUI) {
            if (lab.IsClicked(mx, my)) {
                lab.OnClick(mx, my);
                return 0;
            }
            else {
                showLabUI = false;
            }
        }

        // 3. 상점 건물 클릭 확인
        RECT buildRect = { SHOP_BUILD_X, SHOP_BUILD_Y, SHOP_BUILD_X + SHOP_BUILD_W, SHOP_BUILD_Y + SHOP_BUILD_H };
        if (PtInRect(&buildRect, pt)) {
            double dist = sqrt(pow(player.GetX() - (SHOP_BUILD_X + SHOP_BUILD_W / 2), 2) + pow(player.GetY() - (SHOP_BUILD_Y + SHOP_BUILD_H / 2), 2));
            if (dist < 300) {
                showShopUI = true;
                showMuseumUI = false; // 상점을 열면 박물관은 닫음
                showLabUI = false;  // 최종
                showPlantList = false;
            }
            else {
                MessageBox(hWnd, L"상점과 너무 멉니다! 상점에 더 가까이 다가가세요.", L"알림", MB_OK);
            }
            return 0;
        }

        // ★ 4. 박물관 건물 클릭 확인
        RECT museumRect = { MUSEUM_BUILD_X, MUSEUM_BUILD_Y, MUSEUM_BUILD_X + MUSEUM_BUILD_W, MUSEUM_BUILD_Y + MUSEUM_BUILD_H };
        if (PtInRect(&museumRect, pt)) {
            double dist = sqrt(pow(player.GetX() - (MUSEUM_BUILD_X + MUSEUM_BUILD_W / 2), 2) + pow(player.GetY() - (MUSEUM_BUILD_Y + MUSEUM_BUILD_H / 2), 2));
            if (dist < 300) {
                showMuseumUI = true;
                showShopUI = false; // 박물관을 열면 상점은 닫음
                showLabUI = false;  // 최종

                showPlantList = false;
            }
            else {
                MessageBox(hWnd, L"박물관과 너무 멉니다! 박물관에 더 가까이 다가가세요.", L"알림", MB_OK);
            }
            return 0;
        }

        // ★ 5. 연구소 건물 클릭 확인    //최종 전체 바꿔
        RECT labRect = { LAB_BUILD_X, LAB_BUILD_Y, LAB_BUILD_X + LAB_BUILD_W, LAB_BUILD_Y + LAB_BUILD_H };
        if (PtInRect(&labRect, pt)) {
            double dist = sqrt(pow(player.GetX() - (LAB_BUILD_X + LAB_BUILD_W / 2), 2) + pow(player.GetY() - (LAB_BUILD_Y + LAB_BUILD_H / 2), 2));
            if (dist < 300) {
                showLabUI = true;
                showShopUI = false; // 박물관을 열면 상점은 닫음
                showMuseumUI = false;
                showPlantList = false;
            }
            else {
                MessageBox(hWnd, L"연구소과 너무 멉니다! 연구소에 더 가까이 다가가세요.", L"알림", MB_OK);
            }
            return 0;
        }

        // 5. 밭 클릭 확인
        bool clickedAnyField = false;
        for (int i = 0; i < 8; i++) {
            if (fields[i].IsClicked(mx, my)) {
                clickedAnyField = true;
                double dist = sqrt(pow(player.GetX() + 25 - fields[i].GetCenterX(), 2) +
                    pow(player.GetY() + 40 - fields[i].GetCenterY(), 2));
                if (dist < 600) {
                    if (fields[i].IsHarvestable()) {
                        lastCropType = fields[i].Harvest();
                        lastWasGolden = (rand() % 100 < 10);
                        
                        int gained_crop = player.GetCropCount();        // 6/8 서진씨에게 보냅니다 
                        lastCropCount = gained_crop;        // 6/8 서진씨

                        // 수확 시 황금/일반 여부에 따라 인벤토리 다르게 증가
                        if (lastWasGolden) {                
                            switch (lastCropType) {
                            case WHEAT: invGoldWheat.AddCount(gained_crop); // 5/31 16시 모든 작물(골드는 밀도 잠금)잠금으로 바꿨고, 1개이상 얻으면 잠금해제 골드작물부분 6/8도 //최종 오타수정
                                if (!player.GetUnlockGoldWheat())      player.SetUnlockGoldWheat(true);      break;
                            case POTATO:     invGoldPotato.AddCount(gained_crop);   // 6/8 서진씨 gained_crop추가함
                                if (!player.GetUnlockGoldPotato())     player.SetUnlockGoldPotato(true);     break;
                            case CARROT:     invGoldCarrot.AddCount(gained_crop); // 6/8 서진씨
                                if (!player.GetUnlockGoldCarrot())     player.SetUnlockGoldCarrot(true);     break;
                            case STRAWBERRY: invGoldStrawberry.AddCount(gained_crop); // 6/8 서진씨
                                if (!player.GetUnlockGoldStrawberry()) player.SetUnlockGoldStrawberry(true); break;
                            }
                        }
                        else {
                            switch (lastCropType) {
                            case WHEAT:      invWheat.AddCount(gained_crop);                // 5/31 16시 모든 작물(밀제외)잠금으로 바꿨고, 1개이상 얻으면 잠금해제 일반작물부분
                                if (!player.GetUnlockWheat())      player.SetUnlockWheat(true);      break;
                            case POTATO:     invPotato.AddCount(gained_crop); // 6/8 서진씨
                                if (!player.GetUnlockPotato())     player.SetUnlockPotato(true);     break;
                            case CARROT:     invCarrot.AddCount(gained_crop); // 6/8 서진씨
                                if (!player.GetUnlockCarrot())     player.SetUnlockCarrot(true);     break;
                            case STRAWBERRY: invStrawberry.AddCount(gained_crop); // 6/8 서진씨
                                if (!player.GetUnlockStrawberry()) player.SetUnlockStrawberry(true); break;
                            }
                        }

                        showHarvestUI = true;
                        harvestUITime = GetTickCount();

                        playEffect = true;
                        effectFrame = 0;
                        effectX = fields[i].GetX();
                        effectY = fields[i].GetY();
                        effectLastTime = GetTickCount();
                    }
                    else if (fields[i].GetIsUnlocked()) {
                        showPlantList = true;
                        currentFieldListIndex = i;
                    }
                    else {
                        if (MessageBox(hWnd, L"해금하시겠습니까?", L"알림", MB_YESNO) == IDYES) {
                            fields[i].SetUnlocked(true);
                        }
                    }
                }
                else {
                    MessageBox(hWnd, L"너무 멉니다! 밭에 더 가까이 다가가세요.", L"알림", MB_OK);
                    showPlantList = false;
                }
                break;
            }
        }

        if (!clickedAnyField) {
            showPlantList = false;
        }
        break;
    }
    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE) {
            if (showShopUI) showShopUI = false;
            if (showMuseumUI) showMuseumUI = false; // ★ 박물관 ESC 닫기
            if (showPlantList) showPlantList = false;
            if (showLabUI)showLabUI = false;    // 최종
        }

        else if (wParam == VK_ADD || wParam == VK_OEM_PLUS) {       // 최종 + 키누르면 인벤토리 갯수 999개
            invWheat.SubCount(invWheat.GetCount()); // 0개로 초기화 후
            invWheat.AddCount(999);

            invPotato.SubCount(invPotato.GetCount());
            invPotato.AddCount(999);

            invCarrot.SubCount(invCarrot.GetCount());
            invCarrot.AddCount(999);

            invStrawberry.SubCount(invStrawberry.GetCount());
            invStrawberry.AddCount(999);

            // 황금 작물 999개로 변경
            invGoldWheat.SubCount(invGoldWheat.GetCount());
            invGoldWheat.AddCount(999);

            invGoldPotato.SubCount(invGoldPotato.GetCount());
            invGoldPotato.AddCount(999);

            invGoldCarrot.SubCount(invGoldCarrot.GetCount());
            invGoldCarrot.AddCount(999);

            invGoldStrawberry.SubCount(invGoldStrawberry.GetCount());
            invGoldStrawberry.AddCount(999);

            // (선택 사항) 변경 후 화면 갱신을 위해 재호출
            InvalidateRect(hWnd, NULL, FALSE);
        }
        else if (wParam == VK_MULTIPLY) { //최종  * 키누르면 돈 999
            shop.SetMoney(999);


            InvalidateRect(hWnd, NULL, FALSE);
        }
        if (showPlantList && currentFieldListIndex != -1) {
            if (wParam == '1') { fields[currentFieldListIndex].Plant(WHEAT); showPlantList = false; }
            else if (wParam == '2') { fields[currentFieldListIndex].Plant(POTATO); showPlantList = false; }
            else if (wParam == '3') { fields[currentFieldListIndex].Plant(CARROT); showPlantList = false; }
            else if (wParam == '4') { fields[currentFieldListIndex].Plant(STRAWBERRY); showPlantList = false; }
        }

        
        break;
    }
    case WM_ERASEBKGND:     // 6/3 깜빡해결
        return 1;

    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hdc = BeginPaint(hWnd, &ps);
        hMemDC = CreateCompatibleDC(hdc);
        hBit = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        hOldBit = (HBITMAP)SelectObject(hMemDC, hBit);

        g_background.Draw(hMemDC, rect);

        wchar_t fortuneMsg[64];
        wsprintf(fortuneMsg, L"행운: %d", player.Getfortunate());
        SetBkMode(hMemDC, TRANSPARENT);
        SetTextColor(hMemDC, RGB(0, 0, 0));
        TextOut(hMemDC, 110, 300, fortuneMsg, wcslen(fortuneMsg));

        // 건물 그리기
        shop.DrawBuilding(hMemDC);
        museum.DrawBuilding(hMemDC); // ★ 박물관 건물 그리기
        lab.DrawBuilding(hMemDC);   // 최종

        for (int i = 0; i < 8; i++) fields[i].Draw(hMemDC);
        player.Draw(hMemDC);

        // 일반 작물 UI 그리기
        invWheat.Draw(hMemDC);
        invWheat.Count_Draw(hMemDC);
        invPotato.Draw(hMemDC);
        invPotato.Count_Draw(hMemDC);
        invCarrot.Draw(hMemDC);
        invCarrot.Count_Draw(hMemDC);
        invStrawberry.Draw(hMemDC);
        invStrawberry.Count_Draw(hMemDC);

        // 황금 작물 UI 그리기
        invGoldWheat.Draw(hMemDC);
        invGoldWheat.Count_Draw(hMemDC);
        invGoldPotato.Draw(hMemDC);
        invGoldPotato.Count_Draw(hMemDC);
        invGoldCarrot.Draw(hMemDC);
        invGoldCarrot.Count_Draw(hMemDC);
        invGoldStrawberry.Draw(hMemDC);
        invGoldStrawberry.Count_Draw(hMemDC);

        // ★ UI 출력 (상점 우선, 다음 박물관, 아무것도 안 열려있으면 기본 상점 돈 출력)
        if (showShopUI) {
            shop.Draw(hMemDC, hWnd);
        }
        else if (showMuseumUI) {
            museum.Draw(hMemDC, hWnd);

        }
        else if (showLabUI) {   //최종
            lab.Draw(hMemDC, hWnd);
        }
        ShowInventory(hMemDC); //최종
        shop.DrawMoneyUI(hMemDC, hWnd);     // 6/8 서진씨에게 보냅니다
        

        if (showPlantList && currentFieldListIndex != -1) {
            int listX = fields[currentFieldListIndex].GetX() + 30;
            int listY = fields[currentFieldListIndex].GetY() - 80;
            HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 240));
            RECT bgRect = { listX - 10, listY - 5, listX + 180, listY + 85 };
            FillRect(hMemDC, &bgRect, hBrush);
            DeleteObject(hBrush);

            SetBkMode(hMemDC, TRANSPARENT);
            SetTextColor(hMemDC, RGB(0, 0, 0));
            TextOut(hMemDC, listX, listY, L"[ 숫자키를 눌러 심기 ]", 14);
            TextOut(hMemDC, listX, listY + 20, L"1. 밀(기본)", 8);
            TextOut(hMemDC, listX, listY + 35, L"2. 감자 (돈 필요)", 12);
            TextOut(hMemDC, listX, listY + 50, L"3. 당근 (돈 필요)", 12);
            TextOut(hMemDC, listX, listY + 65, L"4. 딸기 (돈 필요)", 12);
        }

        HDC hUIDC = CreateCompatibleDC(hdc);

        if (playEffect && hEffectSprite) {
            SelectObject(hUIDC, hEffectSprite);
            BITMAP bm;
            GetObject(hEffectSprite, sizeof(BITMAP), &bm);
            int frameWidth = bm.bmWidth / MAX_EFFECT_FRAMES;
            int frameHeight = bm.bmHeight;
            int srcX = effectFrame * frameWidth;
            TransparentBlt(hMemDC, effectX, effectY, 100, 100, hUIDC, srcX, 0, frameWidth, frameHeight, RGB(0, 0, 0));
        }

        if (showHarvestUI) {
            if (GetTickCount() - harvestUITime > 1500) {
                showHarvestUI = false;
            }
            else {
                int uiX = player.GetX() + 25;
                int uiY = player.GetY() - 40;

                HBITMAP hIcon = lastWasGolden ? hSpeCrops[lastCropType] : hInvCrops[lastCropType];
                if (hIcon) {
                    SelectObject(hUIDC, hIcon);
                    BITMAP bm;
                    GetObject(hIcon, sizeof(BITMAP), &bm);
                    TransparentBlt(hMemDC, uiX, uiY, 40, 40, hUIDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
                }

                const wchar_t* cropName = L"";
                switch (lastCropType) {
                case WHEAT:      cropName = L"밀"; break;
                case POTATO:     cropName = L"감자"; break;
                case CARROT:     cropName = L"당근"; break;
                case STRAWBERRY: cropName = L"딸기"; break;
                }

                wchar_t countMsg[64];
                if (lastWasGolden) {
                    wsprintf(countMsg, L"황금 %s + %d", cropName, lastCropCount); // 6/8 11시 추가
                }
                else {
                    wsprintf(countMsg, L"%s + %d", cropName, lastCropCount); // 6/8 11시 추가
                }

                SetBkMode(hMemDC, TRANSPARENT);
                SetTextColor(hMemDC, lastWasGolden ? RGB(255, 165, 0) : RGB(0, 0, 0));
                TextOut(hMemDC, uiX + 50, uiY + 10, countMsg, wcslen(countMsg));
            }
        }
        DeleteDC(hUIDC);

        BitBlt(hdc, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);


        SelectObject(hMemDC, hOldBit);
        DeleteObject(hBit);
        DeleteDC(hMemDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        for (int i = 0; i < 8; i++) fields[i].Release();
        player.Release();
        shop.Release();
        museum.Release(); // ★ 박물관 해제
        lab.Release(); //최종

        for (int i = 0; i < 5; i++) {
            if (hInvCrops[i]) DeleteObject(hInvCrops[i]);
            if (hSpeCrops[i]) DeleteObject(hSpeCrops[i]);
        }
        if (hEffectSprite) DeleteObject(hEffectSprite);

        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
// 6812