#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // BGRA 포맷
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;  // 안티앨리어싱
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1; // 더블 버퍼링
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	// create device and front/back buffers,
	// and swap chain and rendering context

	D3D11CreateDeviceAndSwapChain(
		nullptr,					// *pAdapter
		D3D_DRIVER_TYPE_HARDWARE,	// DriverType
		nullptr,					// Software
		0,							// Flags
		nullptr,					// *pFeatureLevels
		0,							// FeautreLevels
		D3D11_SDK_VERSION,			// SDKVersion
		&sd,						// *pSwapChainDesc
		&pSwap,						// **pSwapChain
		&pDevice,					// **ppDevice
		nullptr,					// *pFeatureLevel
		&pContext					// **ppImmediateContext
	);
	// gain access to texture subresouce in swap chain (back buffer)
	ID3D11Resource* pBackBuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&pTarget // render target
	);
	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if (pTarget != nullptr)
	{
		pTarget->Release();
	}

	if (pDevice != nullptr)
	{
		pContext->Release();
	}

	if (pSwap != nullptr)
	{
		pSwap->Release();
	}

	if (pContext != nullptr)
	{
		pContext->Release();
	}
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u); // syncinterval(VSYNC Hz), flags
}
