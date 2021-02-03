#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h> 

// D3D 디바이스를 생성할 D3D 객체 변수 Driect3D
LPDIRECT3D9 g_pD3D = NULL;
// 랜더링에 사용될 D3D 디바이스 Direct3D Device
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
// 정점을 보관할 정점 버퍼 Vertext Buffer
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

// x,y,z,rhw 값 그리고 diffuse 색상으로 이루어짐
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

	vec3f vEyePt(0.0f, -10.0f, -10.0f); // 월드 좌표계에서 카메라 위치
	vec3f vLookatPt(0.0f, 0.0f, 0.0f);  // 월드 좌표계에서 카메라 방향
	vec3f vUpVec(0.0f, 1.0f, 0.0f);     // 월드 좌표계에서 up vector

	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); // set this matrix as local pose

	/* perspective projection */
	mat4f matProj;

	// fov, ratio w/h , near z, far z
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}
// g_pVB 정점 버퍼를 초기화
HRESULT InitVB()
{
	// 삼각형을 렌더링하기 위해 3개의 정점 선언
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

	// 정점의 버퍼에 데이터 쓰기
	// 정점 버퍼의 Lock() 함수를 호출하여 포인터를 얻어온다.
	void* pVertices;
	if (FAILED(g_pVB->Lock(
		0,
		sizeof(vertices), // 내가 만든 정점의 데이터 크기
		(void**)&pVertices, // 데이터를 접근할 수 있는 포인터를 얻어옴
		0)))
	{
		return E_FAIL;
	}
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
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pd3dDevice)))
	{
		return E_FAIL;
	}
	
	// 컬링 기능
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 광원 기능 off
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

	// 후면 버퍼 파란색(0, 0, 255)로 지운다.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);

	// world, view, projection matrix
	InitMatrix();

	// 랜더링 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// 정점 정보가 담겨 있는 정점 버퍼를 출력 스트림으로 할당
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));

		// D3D에게 정점 셰이더 정보를 지정
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		// 기하 정보 출력
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

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

