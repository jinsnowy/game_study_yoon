#include "CCore.h"
#include "CTimer.h"
#include "../Scene/CSceneManager.h"

CCore* CCore::m_pInst = 0;
BOOL CCore::m_bLoop = TRUE;

CCore::CCore()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
   // _CrtSetBreakAlloc(239);
}
CCore::~CCore()
{
    DESTROY_SINGLE(CTimer);
    DESTROY_SINGLE(CSceneManager);
}

BOOL CCore::Create()
{
    m_hWnd = CreateWindowW(L"Game", L"Game", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hWnd)
    {
        return FALSE;
    }

    // 실제 윈도우 타이틀바나 메뉴를 포함한 윈도우의 크기를
    // 구해준다.
    RECT rc = { 0,0,m_tRS.iW, m_tRS.iH };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // 위에서 구해준 크기로 윈도우 클라이언트 영역의 크기를
    // 원하는 크기로 맞춰줘야한다.
    SetWindowPos(m_hWnd, HWND_TOPMOST,
        100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        m_bLoop = false;
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);

}

ATOM CCore::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Game";
    wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

bool CCore::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

    MyRegisterClass();

    // 해상도 설정
    m_tRS.iW = 1280;
    m_tRS.iH = 720;

    Create();

    // 화면 DC를 만들어 준다.
    m_hDC = GetDC(m_hWnd);


    // 타이머 초기화
    if (!GET_SINGLE(CTimer)->Init())
        return false;

    // 장면관리자 초기화
    if (!GET_SINGLE(CSceneManager)->Init())
        return false;
   

	return true;
}

int CCore::Run()
{
    MSG msg;

    // 기본 메시지 루프입니다:
    while (m_bLoop)
    {
        // PeekMessage는 메세지가 메시지큐에 없어도 바로 빠져나온다.
        // 있을 경우만 true, 메세지가 없는 시간이 윈도우의 데드타임
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // 윈도우 데드타임일 경우
        else {
            Logic();
        }
    }
    return (int)msg.wParam;
}

void CCore::Logic()
{
     // 타이머 갱신
    GET_SINGLE(CTimer)->Update();

    float fDeltaTime = GET_SINGLE(CTimer)->GetDeltaTime();
    
    Input(fDeltaTime);
    Update(fDeltaTime);
    LateUpdate(fDeltaTime);
    Collision(fDeltaTime);
    Render(fDeltaTime);
}

void CCore::Input(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->Input(fDeltaTime);
}

int CCore::Update(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->Input(fDeltaTime);
    return 0;
}

int CCore::LateUpdate(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->LateUpdate(fDeltaTime);
    return 0;
}

void CCore::Collision(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->Collision(fDeltaTime);
}

void CCore::Render(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->Render(m_hDC, fDeltaTime);
}
