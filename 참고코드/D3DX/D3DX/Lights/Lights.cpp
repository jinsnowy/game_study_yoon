/**========================================================================
 * 광원 설정
 * 조명을 사용하여 더 극적인 연출하는 공부를 한다.
 *=========================================================================*/

#include <Windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>

/**========================================================================
 * 전역 변수
 *=========================================================================*/

LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;

LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; //정점을 보관할 정점 버퍼

//광원을 사용하기 위해서는 법선 벡터의 정보가 추가 되어야 한다.
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;	//정점의 3차원 좌표
	D3DXVECTOR3 nomarl;			//정점의 법선 벡터
};

// 사용자 정점 구조체에 관한 정보를 나타내는 FVF값
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL)

/**========================================================================
 * Direct3D 초기화
 *=========================================================================*/
HRESULT InitD3D(HWND hWnd)
{
	// 디바이스를 생성하기 위한 D3D 객체 생성
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS	d3dpp;				
	ZeroMemory(&d3dpp, sizeof(d3dpp));			

	d3dpp.Windowed = TRUE;						
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	//z버퍼 사용을 위한 설정
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	//컬링 기능을 켠다.
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//z 버퍼 기능을 켠다.
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

/**========================================================================
 * 정점 초기화 : 정점 버퍼를 생성하고 정점값을 채워 넣는다.
 *
 *=========================================================================*/
HRESULT InitVB()
{
	// 실린더를 그리기 위해 아래 위의 원을 그리는 정점버퍼를 준비한다.
	// 50 * 2 만큼의 크기로..
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
											D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	CUSTOMVERTEX* pVertices;

	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	//정점 버퍼에 들어갈 정점 데이터를 만들어 삽입한다.
	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);

		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
		pVertices[2 * i + 0].nomarl = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));
		pVertices[2 * i + 1].nomarl = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
	}

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

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixRotationX(&matWorld, timeGetTime() / 500.0f); 

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld); // 생성한 회전 행렬을 월드 행렬로 디바이스에 설정한다.

	//뷰 행렬(카메라)을 정의하기 위해서는 3가지 값이 필요하다.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f); //눈의 위치(0.0f, 3.0f, -5.0f)
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
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj); // 생성한 프로젝션 행렬을 디바이스에 설정.
}
/**========================================================================
 * 광원 설정
 *=========================================================================*/
void SetupLights()
{
	//재질 설정
	//재질은 디바이스에 단 하나만 설정될 수 있다.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_pd3dDevice->SetMaterial(&mtrl);

	//광원 설정
	D3DXVECTOR3 vecDir;
	//방향성 광원(directional light)이 향한 빛의 방향
	//광원 구조체
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	//광원의 확산광 색깔의 밝기를 지정한다.
	//광원의 종류를 설정한다(포인트 라이트, 다이렉션 라이트, 스포트 라이트)
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	//광원의 방향 설정
	vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f), 1.0f, sinf(timeGetTime() / 350.0f));

	//광원의 방향을 단위 벡터로 만든다.
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Range = 1000.0f; // 광원이 다다를 수 있는 최대거리
	g_pd3dDevice->SetLight(0, &light); // 디바이스에 광원 0번을 설치
	g_pd3dDevice->LightEnable(0, TRUE); // 광원 0번을 활성화 한다.
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);// 광원 설정을 활성화 한다.

	//환경 광원의 값 설정
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

/**========================================================================
 * 초기화 객체들 Release
 *=========================================================================*/
void Cleanup()
{
	//해제 순서가 중요 인터페이스 생성의 역순으로 해제하자.
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

	// 후면 버퍼를 파란색(0, 0, 255)으로 지운다.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	SetupLights();

	SetupMatrices();

	// 랜더링 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// 정점 정보가 담겨 있는 정점 버퍼를 출력 스트림으로 할당한다.
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));

		// D3D에게 정점 셰이더 정보를 지정한다. 대부분의 경우에는 FVF만 지정한다.
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		// 기하 정보를 출력하기 위한 DrawPrimitive() 함수를 호출한다.
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);

		//렌더링 종료
		g_pd3dDevice->EndScene();
	}

	// 후면 버퍼를 보이는 화면으로 전환.
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
	//윈도우 클래스 등록
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Light", NULL };

	//winclass 레지스터에 등록
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(L"D3D Light", L"D3D Light", WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
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

	//등록된 레지스트 winclass 릴리즈.
	UnregisterClass(L"D3D Light", wc.hInstance);
	return 0;
}



