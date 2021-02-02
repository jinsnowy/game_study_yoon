#include <d3d9.h>
#include <d3dx9.h>

// D3D 디바이스를 생성할 D3D 객체 변수
LPDIRECT3D9 g_pD3D = NULL;
// 랜더링에 사용될 D3D 디바이스
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

// g_pd3dDevice 를 초기화
HRESULT InitD3D(HWND hWnd)
{
	// 디바이스를 생성하기 위한 D3D 객체 생성
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;   // 디바이스 생성을 위한 구조체 (파라미터 셋팅)
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // 구조체 비우기

	d3dpp.Windowed = TRUE; // 창 모드
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 가장 효율적인 SWAP 효과?
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
		// 렌더링 내용
		
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
		Cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		Render();
		ValidateRect(hWnd, NULL);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}