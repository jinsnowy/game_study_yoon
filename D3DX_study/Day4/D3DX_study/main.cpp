#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>

LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIB = NULL;

struct CUSTOMVERTEX
{
	FLOAT x, y, z;  // 12 bytes
	DWORD color; // 8 bytes
}; // 20 bytes

struct TRIANGLEINDEX
{
	WORD _0, _1, _2;
}; // 6 bytes

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define mat4f D3DXMATRIXA16
#define vec3f D3DXVECTOR3

mat4f g_matTMParent; // 부모 기본 행렬
mat4f g_matRParent;  // 부모 회전 행렬
mat4f g_matTMChild;  // 자식 기본 행렬
mat4f g_matRChild;   // 자식 회전 행렬

void SetupCamera()
{
	mat4f matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	/* view space conversion */
	mat4f matView;
	vec3f vEyePt(0.0f, 0.0f, -5.0f);    // 월드 좌표계에서 카메라 위치
	vec3f vLookatPt(0.0f, 0.0f, 0.0f);  // 월드 좌표계에서 카메라 방향
	vec3f vUpVec(0.0f, 1.0f, 0.0f);     // 월드 좌표계에서 up vector

	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); // set this matrix as local pose

	/* perspective projection */
	mat4f matProj;

	// fov, ratio w/h , near z, far z
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 0.01f, 100.0f);

	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void Animate()
{
	// 부모 원점 그대로
	D3DXMatrixIdentity(&g_matTMParent);

	// 부모 y축 회전
	D3DXMatrixRotationY(&g_matRParent, GetTickCount() / 500.0f);

	// 자식은 translation 변환
	D3DXMatrixTranslation(&g_matTMChild, 3, 3, 3);

	// 자식 z축 회전
	D3DXMatrixRotationZ(&g_matRChild, GetTickCount() / 500.0f);
}

void DrawMesh(mat4f* pMat)
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

// g_pVB 정점 버퍼를 초기화
HRESULT InitVB()
{
	//// 삼각형을 렌더링하기 위해 3개의 정점 선언
	const CUSTOMVERTEX cube_vertices[8] = { 1.0f,1.0f,1.0f,0xffff0000,
											1.0f,1.0f,-1.0f,0xff00ff00,
											-1.0f,1.0f,-1.0f,0xff0000ff,
											-1.0f,1.0f,1.0f,0xffffff00,

											1.0f,-1.0f,1.0f,0xffff0000,
											1.0f,-1.0f,-1.0f,0xff00ff00,
											-1.0f,-1.0f,-1.0f,0xff0000ff,
											-1.0f,-1.0f,1.0f,0xffffff00 };

	int num_vertices = sizeof(cube_vertices) / sizeof(CUSTOMVERTEX);
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
		sizeof(cube_vertices), // 내가 만든 정점의 데이터 크기
		(void**)&pVertices, // 데이터를 접근할 수 있는 포인터를 얻어옴
		0)))
	{
		return E_FAIL;
	}
	memcpy(pVertices, cube_vertices, sizeof(cube_vertices));
	g_pVB->Unlock();
	return S_OK;
}

HRESULT InitIB()
{
	TRIANGLEINDEX cube_indices[] = {
								{1,2,3}, {0,1,3}, // up
								{2,6,7}, {3,2,7}, // left
								{5,1,0}, {4,5,0}, // right
								{5,6,2}, {1,5,2}, // front
								{0,3,7}, {4,0,7}, // back
								{6,5,4}, {7,6,4}, // down
	};

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(12 * sizeof(TRIANGLEINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL)))
	{
		return E_FAIL;
	}

	void* pIndices;
	if (FAILED(g_pIB->Lock(0, sizeof(cube_indices), (void**)&pIndices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pIndices, cube_indices, sizeof(cube_indices));

	g_pIB->Unlock();

	return S_OK;
}
// 기하 정보 초기화
HRESULT InitGeometry()
{
	if (FAILED(InitVB()))
		return E_FAIL;

	if (FAILED(InitIB()))
		return E_FAIL;

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

	d3dpp.EnableAutoDepthStencil = TRUE; // 깊이가 있는 z버퍼가 필요하므로 설정한다.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

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
	
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//Z 버퍼 기능을 켠다.
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

void Cleanup()
{
	if (g_pIB != NULL)
		g_pIB->Release();

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

	mat4f matWorld;

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(225, 225, 225), 1.0f, 0);

	Animate();

	// 랜더링 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// 부모 pose
		matWorld = g_matRParent * g_matTMParent;

		DrawMesh(&matWorld);

		matWorld = g_matRChild * g_matTMChild * g_matRParent * g_matTMParent;
		
		DrawMesh(&matWorld);

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
	if (SUCCEEDED(InitD3D(hWnd)) && SUCCEEDED(InitGeometry()))
	{
		SetupCamera();

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			// PM_REMOVE return 후 메세지 큐에서 메세지 삭제 
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else Render(); // ValidateRect() 대신 Direct3D에서 렌더링하는 방식을 채택
		}
	}

	//등록된 레지스트 winclass 릴리즈.
	UnregisterClass("D3D Init", wc.hInstance);
	return 0;
}

