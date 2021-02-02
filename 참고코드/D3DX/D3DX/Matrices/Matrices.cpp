/**========================================================================
 * 행렬 사용
 * 행렬을 적용하고 변환하는 법을 공부하게 된다.
 *=========================================================================*/

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h> // TimeGetTime() 함수를 사용하기 위해서 포함하는 헤더

/**========================================================================
 * 전역 변수
 *=========================================================================*/

LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;

LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; 

struct CUSTOMVERTEX
{
	FLOAT x, y, z; 
	DWORD color;		
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

/**========================================================================
 * Direct3D 초기화
 *=========================================================================*/
HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS	d3dpp;			
	ZeroMemory(&d3dpp, sizeof(d3dpp));			

	d3dpp.Windowed = TRUE;						
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// 컬링 기능을 끈다, 삼각형의 앞면, 뒷면을 모두 랜더링한다.
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//정점에 색깔값이 있으므로 광원 기능을 끈다.
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

/**========================================================================
 * 정점 초기화 : 정점 버퍼를 생성하고 정점값을 채워 넣는다.
 *=========================================================================*/
HRESULT InitVB()
{
	CUSTOMVERTEX vertices[] =
	{
		//{ -1.0f, -1.0f, 0.0f, 0xffff0000, }, // x, y, z, color
		//{ 1.0f, -1.0f, 0.0f, 0xff00ff00, },
		//{ 0.0f, 1.0f, 0.0f, 0xff00ffff, },

		{ 0.0f, 0.0f, 0.0f, 0xffff0000, }, // x, y, z, color
		{ 1.0f, 0.0f, 0.0f, 0xff00ff00, },
		{ 0.0f, 1.0f, 0.0f, 0xff00ffff, },
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	void* pVertices;

	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}

/**========================================================================
 * 행렬 설정 : 행렬에는 3가지가 있으며, 각각 월드, 뷰, 프로젝션 행렬이다.
 *=========================================================================*/
void SetupMatrices()
{
	//월드 행렬
	D3DXMATRIXA16 matWorld;

	UINT iTime = timeGetTime() % 1000; // float 연산의 정밀도를 위해서 1000으로 나머지 연산한다.

	//FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f; // 1000밀리초마다 한 바퀴씩(2 * pi) 회전 애니메이션 행렬을 만든다.

	D3DXMatrixRotationY(&matWorld, GetTickCount() / 500.0f);

	//D3DXMatrixRotationY(&matWorld, fAngle); // Y축을 회전축으로 회전 행렬을 생성한다.

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld); // 생성한 회전 행렬을 월드 행렬로 디바이스에 설정한다.

	//뷰 행렬(카메라)을 정의하기 위해서는 3가지 값이 필요하다.
	D3DXVECTOR3 vEyePt(0.0f, -10.0f, -10.0f); //눈의 위치(0.0f, 3.0f, -5.0f)
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f); // 눈이 바라보는 위치(0.0f, 0.0f, 0.0f)
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // 윗 방향을 나타내는 상방 벡터(0.0f, 1.0f, 0.0f)

	//뷰 행렬(카메라)
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);// 3가지 Point를 이용해 뷰 행렬을 생성한다.

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); // 생성한 뷰 행렬을 디바이스에 설정한다.

	// 프로젝션 행렬을 정의하기 위해서는 시야각(FOV = Field Of View)과 종횡비(aspect ratio), 클리핑 평면의 값이 필요하다.
	D3DXMATRIXA16 matProj;
	//첫 번째 : 설정될 행렬
	//두 번째 : 시야각
	//세 번째 : 종횡비
	//네 번째 : 근접 클리핑
	//다섯 번째 : 원거리 클리핑
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj); // 생성한 프로젝션 행렬을 디바이스에 설정.
}


/**========================================================================
 * 초기화 객체들 Release
 *=========================================================================*/
void Cleanup()
{
	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

/**========================================================================
 * 화면 그리기
 *=========================================================================*/
void Render()
{
	if (NULL == g_pd3dDevice)
		return;

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	//월드, 뷰, 프로젝션 행렬을 설정한다.
	SetupMatrices();

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));

		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

/**========================================================================
 * WinProc
 *=========================================================================*/
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/**========================================================================
 * Window 생성
 *=========================================================================*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Matrices", NULL };

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(L"D3D Matrices", L"D3D Matrices", WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
		GetDesktopWindow(), NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitVB()))
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));

			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
			}
		}
	}

	UnregisterClass(L"D3D Matrices", wc.hInstance);
	return 0;
}



