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
#define quatf D3DXQUATERNION

vec3f g_aniPos[2];
quatf g_aniRot[2];

mat4f g_matTMParent;
mat4f g_matRParent;

mat4f g_matTMChild;
mat4f g_matRChild;

constexpr float rot_delta = 0.1f;
float g_fRot = 0.0f;

// g_pd3dDevice�� �ʱ�ȭ
HRESULT InitD3D(HWND hWnd)
{
	// ����̽��� �����ϱ� ���� D3D ��ü ����
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;   // ����̽� ������ ���� ����ü (�Ķ���� ����)
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // ����ü ����

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	d3dpp.EnableAutoDepthStencil = TRUE; // ���̰� �ִ� z���۰� �ʿ��ϹǷ� �����Ѵ�.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

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

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

void SetupCamera()
{
	mat4f matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	/* view space conversion */
	vec3f vEyePt(3.0f, 0.0f, -10.0f);
	vec3f vLookatPt(3.0f, 0.0f, 0.0f);
	vec3f vUpVec(0.0f, 1.0f, 0.0f);

	mat4f matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	
	/* perspective projection */
	mat4f matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 0.01f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void Animate()
{
	static float t = 0;
	float x, y, z;
	quatf quat;

	t = (GetTickCount() % 1000) / 1000.0f;  // 1 �� �ֱ� �ִϸ��̼�

	vec3f v;
	// ���� ����
	D3DXVec3Lerp(&v, &g_aniPos[0], &g_aniPos[1], t);
	// translation
	D3DXMatrixTranslation(&g_matTMParent, v.x, v.y, v.z);

	// ���� ���� ����
	D3DXQuaternionSlerp(&quat, &g_aniRot[0], &g_aniRot[1], t);
	// ������� ȸ�� ��İ����� ��ȯ
	D3DXMatrixRotationQuaternion(&g_matRParent, &quat);


	// �ڽ� ������Ʈ ȸ��
	D3DXMatrixRotationZ(&g_matRChild, GetTickCount() / 500.0f);

	// �ڽ� ������Ʈ �̵�
	D3DXMatrixTranslation(&g_matTMChild, 3, 3, 3);
}

void DrawMesh(mat4f* pMat)
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

// g_pVB ���� ���۸� �ʱ�ȭ
HRESULT InitVB()
{
	//// �ﰢ���� �������ϱ� ���� 3���� ���� ����
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

	// ������ ���ۿ� ������ ����
	// ���� ������ Lock() �Լ��� ȣ���Ͽ� �����͸� ���´�.
	void* pVertices;
	if (FAILED(g_pVB->Lock(
		0,
		sizeof(cube_vertices), // ���� ���� ������ ������ ũ��
		(void**)&pVertices, // �����͸� ������ �� �ִ� �����͸� ����
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
// ���� ���� �ʱ�ȭ
HRESULT InitGeometry()
{
	if (FAILED(InitVB()))
		return E_FAIL;

	if (FAILED(InitIB()))
		return E_FAIL;

	g_aniPos[0] = D3DXVECTOR3(0, 0, 0); // ��ġ ��ȭ�� ��� �� ���� ��
	g_aniPos[1] = D3DXVECTOR3(5, 5, 5); // ��ġ ��ȭ�� ��� �� ���� ��

	FLOAT Yaw = D3DX_PI * 90.0f / 180.0f; // Y�� 90�� ȸ��
	FLOAT Pitch = 0;
	FLOAT Roll = 0;

	D3DXQuaternionRotationYawPitchRoll(&g_aniRot[0], Yaw, Pitch, Roll); // �����(Y�� 90��)

	Yaw = 0;
	Pitch = D3DX_PI * 90.0f / 180.0f; // X�� 90�� ȸ��
	Roll = 0;
	D3DXQuaternionRotationYawPitchRoll(&g_aniRot[1], Yaw, Pitch, Roll); // �����(X�� 90��)
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

	// ������ ����
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// �θ� ��ȯ ���
		matWorld = g_matRParent * g_matTMParent;

		mat4f matworld = g_matTMParent * g_matRParent;
		DrawMesh(&matWorld);

		// �ڽ� ��ȯ ���
		matWorld = g_matRChild * g_matTMChild * g_matRParent * g_matTMParent;
		
		DrawMesh(&matWorld);

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
	if (SUCCEEDED(InitD3D(hWnd)) && SUCCEEDED(InitGeometry()))
	{
		SetupCamera();

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			// PM_REMOVE return �� �޼��� ť���� �޼��� ���� 
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else Render(); // ValidateRect() ��� Direct3D���� �������ϴ� ����� ä��
		}
	}

	//��ϵ� ������Ʈ winclass ������.
	UnregisterClass("D3D Init", wc.hInstance);
	return 0;
}

