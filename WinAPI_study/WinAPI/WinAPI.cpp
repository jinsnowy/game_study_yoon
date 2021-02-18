// WinAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI.h"
#include <list>
#include <cmath>

using namespace std;

#define MAX_LOADSTRING 100
#define PI 3.141592

enum MOVE_DIR {
    MD_BACK = -1,
    MD_NONE,
    MD_FRONT
};

typedef struct _tagSphere
{
    float x, y;
    float r;
}SPHERE, * PSPHERE;

typedef struct _tagRectangle {
    float left, top, right, bottom;
}RECTANGLE, *PRECTANGLE;

typedef struct _tagBullet {
    SPHERE      tSphere;
    float       fDist;
    float       fLimitDist;
    float       fAngle;
}BULLET, *PBULLET;

typedef struct _tagMonster {
    SPHERE      tSphere;
    float       fSpeed;
    float       fTime;
    float       fLimitTime;
    int         iDir;
}MONSTER, *PMONSTER;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND g_hWnd;
HDC g_hDC;
bool g_bLoop = true;
SPHERE      g_tPlayer = { 50.f, 50.f, 50.f };
POINT       g_tGunPos;
float       g_fGunLength = 70.0f;
float       g_fPlayerAngle;
MONSTER     g_tMonster;

// 플레이어 총알
list<BULLET> g_PlayerBulletList;

// 몬스터 총알
list<BULLET> g_MonsterBulletList;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 시간을 구하기 위한 변수
LARGE_INTEGER   g_tSecond;
LARGE_INTEGER   g_tTime;
float           g_fDeltaTime;

void Run();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    
    // 화면용 DC 생성
    g_hDC = GetDC(g_hWnd);

    g_tMonster.tSphere.x = 800.0f - 50.0f;
    g_tMonster.tSphere.y = 50.f;
    g_tMonster.tSphere.r = 50.0f;
    g_tMonster.fSpeed = 300.0f;
    g_tMonster.fLimitTime = 1.f;
    g_tMonster.iDir = MD_FRONT;

    // 플레이어 총구의 위치를 구해준다.
    g_tGunPos.x = g_tPlayer.x + cosf(g_fPlayerAngle) * g_fGunLength;
    g_tGunPos.y = g_tPlayer.y + sinf(g_fPlayerAngle) * g_fGunLength;
    MSG msg;

    QueryPerformanceFrequency(&g_tSecond);
    QueryPerformanceCounter(&g_tTime);
    // 기본 메시지 루프입니다:
    while (g_bLoop)
    {
        // PeekMessage는 메세지가 메시지큐에 없어도 바로 빠져나온다.
        // 있을 경우만 true, 메세지가 없는 시간이 윈도우의 데드타임
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // 윈도우 데드타임일 경우
        else {
            static int i = 0;
            i++;
            if (i > 100)
            {
                i = 0;
                Run();
            }

        }
    }
 
    ReleaseDC(g_hWnd, g_hDC);
    return (int) msg.wParam;
}

void Run()
{
    // DeltaTime 
    LARGE_INTEGER tTime;
    QueryPerformanceCounter(&tTime);

    g_fDeltaTime = (tTime.QuadPart - g_tTime.QuadPart)
                        / (float) g_tSecond.QuadPart;

    g_tTime = tTime;
    static float fTimeScale = 1.f;

    if (GetAsyncKeyState(VK_F1) & 0x8000)
    {
        fTimeScale -= g_fDeltaTime;

        if (fTimeScale < 0.f)
            fTimeScale = 0.f;
    }
    if (GetAsyncKeyState(VK_F2) & 0x8000)
    {
        fTimeScale += g_fDeltaTime;

        if (fTimeScale > 1.f)
            fTimeScale = 1.f;
    }

    // 플레이어 초당 이동속도 : 300
    float fSpeed = 400.f * g_fDeltaTime * fTimeScale;

    if (GetAsyncKeyState('D') & 0x8000)
    {
        g_fPlayerAngle += PI * g_fDeltaTime * fTimeScale;
    }
    if (GetAsyncKeyState('A') & 0x8000)
    {
        g_fPlayerAngle -= PI * g_fDeltaTime * fTimeScale;
    }
    if (GetAsyncKeyState('W') & 0x8000)
    {
        g_tPlayer.x += fSpeed * cosf(g_fPlayerAngle);
        g_tPlayer.y += fSpeed * sinf(g_fPlayerAngle);
    }
    if (GetAsyncKeyState('S') & 0x8000)
    {
        g_tPlayer.x -= fSpeed * cosf(g_fPlayerAngle);
        g_tPlayer.y -= fSpeed * sinf(g_fPlayerAngle);
    }

    // 총구 위치를 구한다.
    g_tGunPos.x = g_tPlayer.x + cosf(g_fPlayerAngle) * g_fGunLength;
    g_tGunPos.y = g_tPlayer.y + sinf(g_fPlayerAngle) * g_fGunLength;

    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        BULLET tBullet;

        tBullet.tSphere.x = g_tGunPos.x + cosf(g_fPlayerAngle) * 25.f;
        tBullet.tSphere.y = g_tGunPos.y + sinf(g_fPlayerAngle) * 25.f;
        tBullet.tSphere.r = 25.f;
        tBullet.fDist = 0.f;
        tBullet.fLimitDist = 500.0f;
        tBullet.fAngle = g_fPlayerAngle;

        g_PlayerBulletList.push_back(tBullet);
    }

    if (GetAsyncKeyState('1') & 0x8000)
    {
        float fAngle = g_fPlayerAngle - PI / 6.f;

        for (int i = 0; i < 3; ++i)
        {
            BULLET tBullet;

            tBullet.tSphere.x = g_tGunPos.x + cosf(fAngle) * 25.f;
            tBullet.tSphere.y = g_tGunPos.y + sinf(fAngle) * 25.f;
            tBullet.tSphere.r = 25.f;
            tBullet.fDist = 0.f;
            tBullet.fLimitDist = 500.0f;
            tBullet.fAngle = fAngle;

            g_PlayerBulletList.push_back(tBullet);
            fAngle += PI / 6.f;
        }
    }

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        // 마우스 위치를 얻어온다
        POINT ptMouse;
        // 마우스 위치는 스크린 좌표 기준으로 받아온다.
        GetCursorPos(&ptMouse);

        // 스크린 좌표를 클라이언트 좌표롤 변환한다.
        ScreenToClient(g_hWnd, &ptMouse);

        // 플레이어와 충돌처리한다.
        /*if (g_tPlayerRC.left <= ptMouse.x && ptMouse.x <= g_tPlayerRC.right &&
            g_tPlayerRC.top <= ptMouse.y && ptMouse.y <= g_tPlayerRC.bottom)
        {
            MessageBox(NULL, L"플레이어 클릭", L"마우스 클릭", MB_OK);
        }*/

        float fMX = g_tMonster.tSphere.x - ptMouse.x;
        float fMY = g_tMonster.tSphere.y - ptMouse.y;
        float fMDist = sqrtf(fMX * fMX + fMY * fMY);
        if (g_tMonster.tSphere.r >= fMDist)
        {
            MessageBox(NULL, L"몬스터 클릭", L"마우스 클릭", MB_OK);
        }
    }

    RECT rcWindow;
    GetClientRect(g_hWnd, &rcWindow);
    SetRect(&rcWindow, 0, 0, 800, 600);

   /* if (g_tPlayerRC.top < 0)
    {
        g_tPlayerRC.top = 0;
        g_tPlayerRC.bottom = 100;
    }

    else if (g_tPlayerRC.bottom > rcWindow.bottom)
    {
        g_tPlayerRC.top = rcWindow.bottom - 100;
        g_tPlayerRC.bottom = rcWindow.bottom;
    }

    else if (g_tPlayerRC.left < rcWindow.left)
    {
        g_tPlayerRC.left= rcWindow.left;
        g_tPlayerRC.right = rcWindow.left + 100;
    }

    else if (g_tPlayerRC.right > rcWindow.right)
    {
        g_tPlayerRC.left = rcWindow.right - 100;
        g_tPlayerRC.right = rcWindow.right;
    }*/

    // 몬스터 이동
    g_tMonster.tSphere.y +=
        g_tMonster.fSpeed * g_fDeltaTime *
        fTimeScale * g_tMonster.iDir;
    //g_tMonster.tSphere.bottom += g_tMonster.fSpeed * g_fDeltaTime *
    //    fTimeScale * g_tMonster.iDir;

    if (g_tMonster.tSphere.y + g_tMonster.tSphere.r >= 600)
    {
        g_tMonster.iDir = MD_BACK;
        g_tMonster.tSphere.y = 550;
    }
    else if (g_tMonster.tSphere.y - g_tMonster.tSphere.r <= 0)
    {
        g_tMonster.iDir = MD_FRONT;
        g_tMonster.tSphere.y = 50.f;
    }

    // 몬스터 총알 발사 로직
    g_tMonster.fTime += g_fDeltaTime * fTimeScale;
    if (g_tMonster.fTime >= g_tMonster.fLimitTime)
    {
        g_tMonster.fTime -= g_tMonster.fLimitTime;

        BULLET tBullet = {};

        tBullet.tSphere.x = g_tMonster.tSphere.x - g_tMonster.tSphere.r - 25.f;
        tBullet.tSphere.y = g_tMonster.tSphere.y;
        tBullet.tSphere.r = 25.f;

        tBullet.fDist = 0.f;
        tBullet.fLimitDist = 800.0f;

        g_MonsterBulletList.push_back(tBullet);
    }

    // 플레이어 총알 이동
    list<BULLET>::iterator iter;
    list<BULLET>::iterator iterEnd = g_PlayerBulletList.end();

    const float fBulletSpeed = 600.0f * g_fDeltaTime * fTimeScale;
    for (iter = g_PlayerBulletList.begin(); iter != iterEnd;)
    {
        iter->tSphere.x += fBulletSpeed * cosf(iter->fAngle);
        iter->tSphere.y += fBulletSpeed * sinf(iter->fAngle);

        iter->fDist += fBulletSpeed;

        float fx = iter->tSphere.x - g_tMonster.tSphere.x;
        float fy = iter->tSphere.y - g_tMonster.tSphere.y;
        float fDist = sqrtf(fx * fx + fy * fy);


        if (fDist <= iter->tSphere.r + g_tMonster.tSphere.r)
        {
            iter = g_PlayerBulletList.erase(iter);
            iterEnd = g_PlayerBulletList.end();
        }

        if (iter->fDist >= iter->fLimitDist)
        {
            iter = g_PlayerBulletList.erase(iter);
            iterEnd = g_PlayerBulletList.end();
        }
        else if (iter->tSphere.x - iter->tSphere.r >= 800)
        {
            iter = g_PlayerBulletList.erase(iter);
            iterEnd = g_PlayerBulletList.end();
        }
        else
        {
            ++iter;
        }
    }

    iterEnd = g_MonsterBulletList.end();
    for (iter = g_MonsterBulletList.begin(); iter != iterEnd;)
    {
        iter->tSphere.x -= fBulletSpeed;
        iter->fDist += fBulletSpeed;


        if (iter->fDist >= iter->fLimitDist)
        {
            iter = g_MonsterBulletList.erase(iter);
            iterEnd = g_MonsterBulletList.end();
        }
        else if (iter->tSphere.x + iter->tSphere.r <= 0)
        {
            iter = g_MonsterBulletList.erase(iter);
            iterEnd = g_MonsterBulletList.end();

        }
        

        /* else if (g_tPlayerRC.left <= iter->rc.right &&
            g_tPlayerRC.right >= iter->rc.left &&
            g_tPlayerRC.top <= iter->rc.bottom &&
            g_tPlayerRC.bottom >= iter->rc.top)
        {
            iter = g_MonsterBulletList.erase(iter);
            iterEnd = g_MonsterBulletList.end();
        }*/
        else ++iter;
    }


    // 출력
    // Rectangle(g_hDC, 0, 0, 800, 600);
    Ellipse(g_hDC, g_tMonster.tSphere.x - g_tMonster.tSphere.r,
                    g_tMonster.tSphere.y - g_tMonster.tSphere.r,
                    g_tMonster.tSphere.x + g_tMonster.tSphere.r,
                    g_tMonster.tSphere.y + g_tMonster.tSphere.r);
    // 포신
    MoveToEx(g_hDC, g_tPlayer.x, g_tPlayer.y, NULL);
    LineTo(g_hDC, g_tGunPos.x, g_tGunPos.y);

    Ellipse(g_hDC, g_tPlayer.x - g_tPlayer.r,
                     g_tPlayer.y - g_tPlayer.r,
                     g_tPlayer.x + g_tPlayer.r,
                     g_tPlayer.y + g_tPlayer.r);

    iterEnd = g_PlayerBulletList.end();
    for (iter = g_PlayerBulletList.begin(); iter != iterEnd; iter++)
    {
        Ellipse(g_hDC, iter->tSphere.x - iter->tSphere.r,
            iter->tSphere.y - iter->tSphere.r,
            iter->tSphere.x + iter->tSphere.r,
            iter->tSphere.y + iter->tSphere.r);
    }

    iterEnd = g_MonsterBulletList.end();
    for (iter = g_MonsterBulletList.begin(); iter != iterEnd; iter++)
    {
        Ellipse(g_hDC, iter->tSphere.x - iter->tSphere.r,
            iter->tSphere.y - iter->tSphere.r,
            iter->tSphere.x + iter->tSphere.r,
            iter->tSphere.y + iter->tSphere.r);
    }
}


//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   // 실제 윈도우 타이틀바나 메뉴를 포함한 윈도우의 크기를
   // 구해준다.
   RECT rc = { 0,0,800,600 };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
   
   // 위에서 구해준 크기로 윈도우 클라이언트 영역의 크기를
   // 원하는 크기로 맞춰줘야한다.
   SetWindowPos(hWnd, HWND_TOPMOST,
       100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        g_bLoop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
