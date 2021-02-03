#include <d3d9.h>
#include <d3dx9.h>

// D3D 디바이스를 생성할 D3D 객체 변수 Driect3D
LPDIRECT3D9 g_pD3D = NULL;
// 랜더링에 사용될 D3D 디바이스 Direct3D Device
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
// 정점을 보관할 정점 버퍼 Vertext Buffer
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

// x,y,z,rhw 값 그리고 diffuse 색상으로 이루어짐
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;  // 12 bytes
	DWORD color; // 8 bytes
}; // 20 bytes

// g_pVB 정점 버퍼를 초기화
HRESULT InitVB()
{
	// 삼각형을 렌더링하기 위해 3개의 정점 선언
	CUSTOMVERTEX vertices[] = {
		{150.0f, 50.0f,  0.5f, 1.0f, (DWORD) 0xFFFF0000},
		{250.0f, 250.0f, 0.5f, 1.0f, (DWORD) 0xFF00FF00},
		{50.0f,  250.0f, 0.5f, 1.0f, (DWORD) 0xFF00FFFF}
	};

	// 정점 버퍼 생성
	/*
		STDMETHOD(CreateVertexBuffer)
		(THIS_ UINT Length, 메모리 할당 사이즈
		DWORD Usage,        정점 버퍼 종류 혹은 처리 방식 (SW, HW)
		DWORD FVF,          정점 정보 구조체에 따라 선언된 FVF 플래그 값
		D3DPOOL Pool,       정점 버퍼가 저장될 메모리 위치 (비디오/시스템 메모리)
		IDirect3DVertexBuffer9** ppVertexBuffer,  정점 버퍼
		HANDLE* pSharedHandle)  반환될 정점 버퍼의 인터페이스
		PURE;
	*/
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
												0,
												D3DFVF_CUSTOMVERTEX,
												D3DPOOL_DEFAULT,
												&g_pVB,
												NULL)))
	{
		return E_FAIL;
	}

	// 정점의 버퍼에 데이터 쓰기
	// 정점 버퍼의 Lock() 함수를 호출하여 포인터를 얻어온다.
	void* pVertices;

	/*
		(Lock)
		(THIS_ UINT OffsetToLock,   Lock을 할 버퍼의 시작점
		UINT SizeToLock,			Lock을 할 버퍼의 크기
		void** ppbData,				읽고 쓸 수 있게 된 메모리 영역의 포인터
		DWORD Flags)				Lock을 수행할 때 함께 사용하는 플래그
		PURE;
	*/
	if (FAILED(g_pVB->Lock(
		0,
		sizeof(vertices), // 내가 만든 정점의 데이터 크기
		(void**)&pVertices, // 데이터를 접근할 수 있는 포인터를 얻어옴
		0)))
	{
		return E_FAIL;
	}

	// 메모리 값 복사
	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}

// g_pd3dDevice를 초기화
HRESULT InitD3D(HWND hWnd)
{
	// 디바이스를 생성하기 위한 D3D 객체 생성
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;   // 디바이스 생성을 위한 구조체 (파라미터 셋팅)
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // 구조체 비우기

	d3dpp.Windowed = TRUE; // 창 모드
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 가장 효율적인 SWAP 효과
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // 후면버퍼 생성 ?

	// 디바이스를 설정해서 생성
	// 디폴트 비디오 카드를 사용하고, HAL 디바이스를 생성한다.
	// 정점 처리는 모든 카드에서 지원하는 SW처리로 생성한다.
	/*
		STDMETHOD(CreateDevice)(THIS_ UINT Adapter,  = D3DADAPTER_DEFAULT (출력할 모니터 설정, 모니터 한개면 DEFAULT)
		D3DDEVTYPE DeviceType,  = (D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_REF) HAL 타입 고급 디바이스
		HWND hFocusWindow, = 디바이스가 출력할 윈도우 핸들, 최상위 핸들로 설정
		DWORD BehaviorFlags, = (D3DCREATE_HARDWARE_VERTEXPROCESSING, D3DCREATE_SOFTWARE_VERTEXPROCESSING, D3DCREATE_MIXED_VERTEX_PROCESSING)
							   정점 프로세싱을 하드웨어/소프트웨어/혼합 사용 방식을 채택
		D3DPRESENT_PARAMETERS* pPresentationParameters, = Direct3D Device를 설정하기 위한 전달 파라미터
		IDirect3DDevice9** ppReturnedDeviceInterface)  = 최종 생성되는 Direct3D 디바이스 결과물
		PURE;
	*/
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pd3dDevice)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void Cleanup()
{
	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

void Render()
{
	if (NULL == g_pd3dDevice)
		return;

	// 후면 버퍼 파란색(0, 0, 255)로 지운다.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);

	// 랜더링 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// 정점 정보가 담겨 있는 정점 버퍼를 출력 스트림으로 할당
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));

		// D3D에게 정점 셰이더 정보를 지정
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		// 기하 정보 출력
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		// 렌더링 종료
		g_pd3dDevice->EndScene();
	}

	// 후면 버퍼를 보이는 화면으로 전환
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

/*
  WinProc
*/
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		// 종료
		Cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		// 그리기
		// Render();
		ValidateRect(hWnd, NULL);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
  Window 생성
*/

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	//윈도우 클래스 등록
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
						GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
						"D3D Init", NULL };

	//winclass 레지스터에 등록
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("D3D Init", "D3D Init", WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
							  GetDesktopWindow(), NULL, NULL, wc.hInstance, NULL);

	// Direct3D 디바이스 초기화 && Vertex Buffer 초기화
	if (SUCCEEDED(InitD3D(hWnd)) && SUCCEEDED(InitVB()))
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		//while (GetMessage(&msg, NULL, 0, 0))
		//{
		//	TranslateMessage(&msg);
		//	DispatchMessage(&msg);
		//}
		/*
		BOOL PeekMessage(
			LPMSG lpMsg, // MSG 구조체 포인터
			HWND hWnd,   // 윈도우 핸들
			UINT wMsgFilterMin, // 첫번째 메세지
			UINT wMsgFilterMax, // 마지막 메세지
			UINT wRemoveMsg     // 제거 플래그
		)

		GetMessage()와 차이점
		GetMessage() 메세지 수신될 때만 return (blocking 함수) 
		PeekMessage() 메세지 수신안 되도 return (non-blocking 함수)
		*/

		while (msg.message != WM_QUIT)
		{
			// PM_REMOVE return 후 메세지 큐에서 메세지 삭제 
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				// 메세지 가공 (키보드 어떤 글자 눌렀나)
				TranslateMessage(&msg);

				// 메세지 처리함수 MsgProc로 전달
				DispatchMessage(&msg);
			}
			else Render(); // ValidateRect() 대신 Direct3D에서 렌더링하는 방식을 채택
		}
	}

	//등록된 레지스트 winclass 릴리즈.
	UnregisterClass("D3D Init", wc.hInstance);
	return 0;
}

