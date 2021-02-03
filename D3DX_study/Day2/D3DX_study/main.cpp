#include <d3d9.h>
#include <d3dx9.h>

// D3D ����̽��� ������ D3D ��ü ���� Driect3D
LPDIRECT3D9 g_pD3D = NULL;
// �������� ���� D3D ����̽� Direct3D Device
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
// ������ ������ ���� ���� Vertext Buffer
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

// x,y,z,rhw �� �׸��� diffuse �������� �̷����
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;  // 12 bytes
	DWORD color; // 8 bytes
}; // 20 bytes

// g_pVB ���� ���۸� �ʱ�ȭ
HRESULT InitVB()
{
	// �ﰢ���� �������ϱ� ���� 3���� ���� ����
	CUSTOMVERTEX vertices[] = {
		{150.0f, 50.0f,  0.5f, 1.0f, (DWORD) 0xFFFF0000},
		{250.0f, 250.0f, 0.5f, 1.0f, (DWORD) 0xFF00FF00},
		{50.0f,  250.0f, 0.5f, 1.0f, (DWORD) 0xFF00FFFF}
	};

	// ���� ���� ����
	/*
		STDMETHOD(CreateVertexBuffer)
		(THIS_ UINT Length, �޸� �Ҵ� ������
		DWORD Usage,        ���� ���� ���� Ȥ�� ó�� ��� (SW, HW)
		DWORD FVF,          ���� ���� ����ü�� ���� ����� FVF �÷��� ��
		D3DPOOL Pool,       ���� ���۰� ����� �޸� ��ġ (����/�ý��� �޸�)
		IDirect3DVertexBuffer9** ppVertexBuffer,  ���� ����
		HANDLE* pSharedHandle)  ��ȯ�� ���� ������ �������̽�
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

	// ������ ���ۿ� ������ ����
	// ���� ������ Lock() �Լ��� ȣ���Ͽ� �����͸� ���´�.
	void* pVertices;

	/*
		(Lock)
		(THIS_ UINT OffsetToLock,   Lock�� �� ������ ������
		UINT SizeToLock,			Lock�� �� ������ ũ��
		void** ppbData,				�а� �� �� �ְ� �� �޸� ������ ������
		DWORD Flags)				Lock�� ������ �� �Բ� ����ϴ� �÷���
		PURE;
	*/
	if (FAILED(g_pVB->Lock(
		0,
		sizeof(vertices), // ���� ���� ������ ������ ũ��
		(void**)&pVertices, // �����͸� ������ �� �ִ� �����͸� ����
		0)))
	{
		return E_FAIL;
	}

	// �޸� �� ����
	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}

// g_pd3dDevice�� �ʱ�ȭ
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

	// �ĸ� ���� �Ķ���(0, 0, 255)�� �����.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);

	// ������ ����
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// ���� ������ ��� �ִ� ���� ���۸� ��� ��Ʈ������ �Ҵ�
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));

		// D3D���� ���� ���̴� ������ ����
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		// ���� ���� ���
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

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
		// Render();
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

	// Direct3D ����̽� �ʱ�ȭ && Vertex Buffer �ʱ�ȭ
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
			LPMSG lpMsg, // MSG ����ü ������
			HWND hWnd,   // ������ �ڵ�
			UINT wMsgFilterMin, // ù��° �޼���
			UINT wMsgFilterMax, // ������ �޼���
			UINT wRemoveMsg     // ���� �÷���
		)

		GetMessage()�� ������
		GetMessage() �޼��� ���ŵ� ���� return (blocking �Լ�) 
		PeekMessage() �޼��� ���ž� �ǵ� return (non-blocking �Լ�)
		*/

		while (msg.message != WM_QUIT)
		{
			// PM_REMOVE return �� �޼��� ť���� �޼��� ���� 
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				// �޼��� ���� (Ű���� � ���� ������)
				TranslateMessage(&msg);

				// �޼��� ó���Լ� MsgProc�� ����
				DispatchMessage(&msg);
			}
			else Render(); // ValidateRect() ��� Direct3D���� �������ϴ� ����� ä��
		}
	}

	//��ϵ� ������Ʈ winclass ������.
	UnregisterClass("D3D Init", wc.hInstance);
	return 0;
}

