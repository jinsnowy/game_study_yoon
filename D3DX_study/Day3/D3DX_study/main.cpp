#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h> 

// D3D ����̽��� ������ D3D ��ü ���� Driect3D
LPDIRECT3D9 g_pD3D = NULL;
// �������� ���� D3D ����̽� Direct3D Device
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
// ������ ������ ���� ���� Vertext Buffer
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

// x,y,z,rhw �� �׸��� diffuse �������� �̷����
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define UNIFORM_COLOR (0xFF00FFFF)
#define mat4f D3DXMATRIXA16
#define vec3f D3DXVECTOR3

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;  // 12 bytes
	DWORD color; // 8 bytes
}; // 20 bytes

void InitMatrix()
{
	/* world space conversion */
	mat4f matWorld;

	UINT iTime = timeGetTime() % 1000;

	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f; // 0 ~ 2pi 

	D3DXMatrixRotationY(&matWorld, fAngle); // rotation about Y axis

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld); // set this matrix as world

	/* view space conversion */
	mat4f matView;

	vec3f vEyePt(0.0f, -10.0f, -10.0f); // ���� ��ǥ�迡�� ī�޶� ��ġ
	vec3f vLookatPt(0.0f, 0.0f, 0.0f);  // ���� ��ǥ�迡�� ī�޶� ����
	vec3f vUpVec(0.0f, 1.0f, 0.0f);     // ���� ��ǥ�迡�� up vector

	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); // set this matrix as local pose

	/* perspective projection */
	mat4f matProj;

	// fov, ratio w/h , near z, far z
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}
// g_pVB ���� ���۸� �ʱ�ȭ
HRESULT InitVB()
{
	// �ﰢ���� �������ϱ� ���� 3���� ���� ����
	CUSTOMVERTEX vertices[] = {
		{0.0f, 0.0f, 0.0f,  (DWORD) UNIFORM_COLOR},	
		{1.0f, 0.0f, 0.0f, (DWORD) UNIFORM_COLOR},
		{0.0f, 1.0f, 0.0f,  (DWORD) UNIFORM_COLOR},
	};

	int num_vertices = sizeof(vertices) / sizeof(CUSTOMVERTEX);
	if (FAILED(g_pd3dDevice->CreateVertexBuffer( num_vertices * sizeof(CUSTOMVERTEX),
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
	if (FAILED(g_pVB->Lock(
		0,
		sizeof(vertices), // ���� ���� ������ ������ ũ��
		(void**)&pVertices, // �����͸� ������ �� �ִ� �����͸� ����
		0)))
	{
		return E_FAIL;
	}
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
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pd3dDevice)))
	{
		return E_FAIL;
	}
	
	// �ø� ���
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���� ��� off
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	// world, view, projection matrix
	InitMatrix();

	// ������ ����
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// ���� ������ ��� �ִ� ���� ���۸� ��� ��Ʈ������ �Ҵ�
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));

		// D3D���� ���� ���̴� ������ ����
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		// ���� ���� ���
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

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

