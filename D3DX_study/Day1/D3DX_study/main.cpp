#include <d3d9.h>
#include <d3dx9.h>

// D3D ����̽��� ������ D3D ��ü ����
LPDIRECT3D9 g_pD3D = NULL;
// �������� ���� D3D ����̽�
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

// g_pd3dDevice �� �ʱ�ȭ
HRESULT InitD3D(HWND hWnd)
{
	// ����̽��� �����ϱ� ���� D3D ��ü ����
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;   // ����̽� ������ ���� ����ü (�Ķ���� ����)
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // ����ü ����

	d3dpp.Windowed = TRUE; // â ���
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // ���� ȿ������ SWAP ȿ��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // �ĸ���� ���� ?

	// ����̽��� �����ؼ� ����
	// ����Ʈ ���� ī�带 ����ϰ�, HAL ����̽��� �����Ѵ�.
	// ���� ó���� ��� ī�忡�� �����ϴ� SWó���� �����Ѵ�.
	/*
	STDMETHOD(CreateDevice)(THIS_ UINT Adapter,  = D3DADAPTER_DEFAULT (����� ����� ����, ����� �Ѱ��� DEFAULT)
	D3DDEVTYPE DeviceType,  = (D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_REF) HAL Ÿ�� ��� ����̽�
	HWND hFocusWindow, = ����̽��� ����� ������ �ڵ�, �ֻ��� �ڵ�� ����
	DWORD BehaviorFlags, = (D3DCREATE_HARDWARE_VERTEXPROCESSING, D3DCREATE_SOFTWARE_VERTEXPROCESSING, D3DCREATE_MIXED_VERTEX_PROCESSING)
	                       ���� ���μ����� �ϵ����/����Ʈ����/ȥ�� ��� ����� ä��
	D3DPRESENT_PARAMETERS* pPresentationParameters, = Direct3D Device�� �����ϱ� ���� ���� �Ķ����
	IDirect3DDevice9** ppReturnedDeviceInterface)  = ���� �����Ǵ� Direct3D ����̽� �����
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
	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

void Render()
{
	if (NULL == g_pd3dDevice)
		return;

	// �ĸ� ���� �Ķ���(0, 0, 255)�� �����.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);

	// ������ ����
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// ������ ����
		
		// ������ ����
		g_pd3dDevice->EndScene();
	}

	// �ĸ� ���۸� ���̴� ȭ������ ��ȯ
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
		// ����
		Cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		// �׸���
		Render();
		ValidateRect(hWnd, NULL);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
  Window ����
*/

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	//������ Ŭ���� ���
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Init", NULL };

	//winclass �������Ϳ� ���
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("D3D Init", "D3D Init", WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
		GetDesktopWindow(), NULL, NULL, wc.hInstance, NULL);

	// Direc3D ����̽� �ʱ�ȭ
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		/*
		typedef struct tagMSG {
			HWND        hwnd;
			UINT        message;
			WPARAM      wParam;
			LPARAM      lParam;
			DWORD       time;
			POINT       pt;
		#ifdef _MAC
			DWORD       lPrivate;
		#endif
		} MSG, *PMSG, NEAR *NPMSG, FAR *LPMSG;
		*/

		MSG msg;

		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//��ϵ� ������Ʈ winclass ������.
	UnregisterClass("D3D Init", wc.hInstance);
	return 0;
}

