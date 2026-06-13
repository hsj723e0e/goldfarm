#pragma once
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// =====================================================================
//  현재 BGM 상태
// =====================================================================
enum BGMState {
    BGM_NONE,
    BGM_MAIN,       // 배경음악.wav
    BGM_SHOP,       // 상점배경음악.wav
    BGM_MUSEUM,     // 박물관배경음악.wav
    BGM_LAB         // 연구소배경음악.wav
};

class SoundManager {
public:
    // -----------------------------------------------------------------
    //  초기화: ShowWindow() 이후에 호출할 것
    // -----------------------------------------------------------------
    static void Init() {
        s_currentBGM = BGM_NONE;
        PlayBGM(BGM_MAIN);
    }

    static void Shutdown() {
        StopBGM();
        mciSendString(L"close sfx_shop", NULL, 0, NULL);
        mciSendString(L"close sfx_plant", NULL, 0, NULL);
        mciSendString(L"close sfx_harvest", NULL, 0, NULL);
        mciSendString(L"close sfx_donate", NULL, 0, NULL);
        mciSendString(L"close sfx_levelup", NULL, 0, NULL);
    }

    // -----------------------------------------------------------------
    //  BGM 전환 (같은 BGM이면 무시)
    // -----------------------------------------------------------------
    static void PlayBGM(BGMState state) {
        if (s_currentBGM == state) return;

        StopBGM();
        s_currentBGM = state;

        const wchar_t* file = nullptr;
        switch (state) {
        case BGM_MAIN:   file = L"배경음악.wav";       break;
        case BGM_SHOP:   file = L"상점배경음악.wav";   break;
        case BGM_MUSEUM: file = L"박물관배경음악.wav";  break;
        case BGM_LAB:    file = L"연구소배경음악.wav";  break;
        default: return;
        }

        wchar_t cmd[512];

        // ★ waveaudio 대신 mpegvideo → repeat 지원됨
        wsprintf(cmd, L"open \"%s\" type mpegvideo alias bgm", file);
        if (mciSendString(cmd, NULL, 0, NULL) != 0) {
            s_currentBGM = BGM_NONE;
            return;
        }
        mciSendString(L"play bgm from 0 repeat", NULL, 0, NULL);
    }

    static void StopBGM() {
        mciSendString(L"stop bgm", NULL, 0, NULL);
        mciSendString(L"close bgm", NULL, 0, NULL);
        s_currentBGM = BGM_NONE;
    }

    // -----------------------------------------------------------------
    //  장소 진입 함수
    // -----------------------------------------------------------------
    static void OnEnterMain() { PlayBGM(BGM_MAIN); }
    static void OnEnterShop() { PlayBGM(BGM_SHOP); }
    static void OnEnterMuseum() { PlayBGM(BGM_MUSEUM); }
    static void OnEnterLab() { PlayBGM(BGM_LAB); }

    // -----------------------------------------------------------------
    //  효과음 (wav → PlaySound / mp3·ogg → MCI)
    // -----------------------------------------------------------------

    // 상점 구매/판매 (ogg)
    static void PlaySFX_Shop() {
        PlaySFX_MCI(L"상점 효과음.wav", L"sfx_plant", L"waveaudio");
    }

    // 심기 (mp3)
    static void PlaySFX_Plant() {
        PlaySFX_MCI(L"심기효과음.wav", L"sfx_plant", L"waveaudio");
    }

    // 수확 (wav)
    static void PlaySFX_Harvest() {
        PlaySFX_MCI(L"수확효과음.wav", L"sfx_harvest", L"waveaudio");
    }

    // 기증 효과음 (wav) — 연구소·박물관 슬롯 하나 채울 때
    static void PlaySFX_Donate() {
        PlaySFX_MCI(L"연구효과음.wav", L"sfx_donate", L"waveaudio");
    }

    // 단계 완료 (mp3) — 박물관 다음 단계 / 연구 3개 완료
    static void PlaySFX_LevelUp() {
        PlaySFX_MCI(L"연구단계증가.wav", L"sfx_levelup", L"waveaudio");
    }

    static BGMState GetCurrentBGM() { return s_currentBGM; }

private:
    static BGMState s_currentBGM;

    // mp3 / ogg 효과음 공통 헬퍼
    static void PlaySFX_MCI(const wchar_t* file, const wchar_t* alias, const wchar_t* type = L"mpegvideo") {
        wchar_t cmd[512];
        wsprintf(cmd, L"close %s", alias);
        mciSendString(cmd, NULL, 0, NULL);

        wsprintf(cmd, L"open \"%s\" type mpegvideo alias %s", file, alias);
        if (mciSendString(cmd, NULL, 0, NULL) != 0) return;

        wsprintf(cmd, L"play %s from 0", alias);
        mciSendString(cmd, NULL, 0, NULL);
    }
};

__declspec(selectany) BGMState SoundManager::s_currentBGM = BGM_NONE;
