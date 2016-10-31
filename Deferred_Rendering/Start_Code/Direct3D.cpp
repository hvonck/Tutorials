#include "Direct3D.h"


Direct3D::Direct3D()
{
}


Direct3D::~Direct3D()
{
	ReleaseCOM(m_renderTargetView);
	ReleaseCOM(m_swapChain);

	if (m_d3dDeviceContext)
		m_d3dDeviceContext->ClearState();

	ReleaseCOM(m_d3dDeviceContext);
	ReleaseCOM(m_d3dDevice);
}

bool Direct3D::Init(UINT a_width, UINT a_height, HWND a_window)
{
	//Init Direct3D.
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//All feature levels that I care about.
	D3D_FEATURE_LEVEL necessaryFeatureLevel[]
	{
		D3D_FEATURE_LEVEL_11_0
	};


	//Get the correct GPU.
	IDXGIAdapter* dxgiAdapter = selectAdapter();

	D3D_FEATURE_LEVEL selectedFeatureLevel;
	HRESULT hr = D3D11CreateDevice(dxgiAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, createDeviceFlags, 
									necessaryFeatureLevel, 1, D3D11_SDK_VERSION, &m_d3dDevice, 
									&selectedFeatureLevel, &m_d3dDeviceContext);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"D3D11CreateDevice Failed.", L"D3D11 Initialisation Error", MB_OK);
		return false;
	}

	if (selectedFeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(NULL, L"Direct3D Feature Level 11 Unsupported.", L"D3D11 Initialisation Error", MB_OK);
		return false;
	}

	//Create the swapchain.
	UINT msaaQuality = 0;
	HR(m_d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality));
	if (!(msaaQuality > 0)) return false;

	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width						= a_width;
	swapChainDesc.BufferDesc.Height						= a_height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count		                = 1;
	swapChainDesc.SampleDesc.Quality	                = 0;
	swapChainDesc.BufferUsage	                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount	                        = 1;
	swapChainDesc.OutputWindow	                        = a_window;
	swapChainDesc.Windowed		                        = true;
	swapChainDesc.SwapEffect	                        = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags			                        = 0;
	


	//Finally create the swapchain.
	IDXGIFactory* dxgiFactory = nullptr;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
	HR(dxgiFactory->CreateSwapChain(m_d3dDevice, &swapChainDesc, &m_swapChain));

	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	OnResize(a_width, a_height);

	return true;
}

void Direct3D::OnResize(UINT a_width, UINT a_height)
{
	//NEVER forget to release the COMs first! NEVER.
	ReleaseCOM(m_renderTargetView);
	
	//Create the render target view.
	HR(m_swapChain->ResizeBuffers(1, a_width, a_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &m_renderTargetView));
	ReleaseCOM(backBuffer);

	//Set the depth stencil buffer and the viewport.
	m_d3dDeviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
}

void Direct3D::SwapBuffers()
{
	HR(m_swapChain->Present(0, 0));
}

IDXGIAdapter * Direct3D::selectAdapter()
{
	UINT index = 0;
	std::vector<IDXGIAdapter*> adapters;
	IDXGIFactory1* dxgiFactory1;
	IDXGIAdapter* dxgiAdapter;

	//Enumerate through all adapters.
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&dxgiFactory1);
	while (dxgiFactory1->EnumAdapters(index, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapters.push_back(dxgiAdapter);
		index++;
	}
	dxgiFactory1->Release();

	//Rate all of the adapters and select the best one.
	//NOTE: The reason that I put in > and not >= is because the last one (can be) is a software adapter. 
	//		(it only seems to be the case in x86 mode. In x64 mode the software adapter seems to be 
	//		superior all the time so I had to change something else for that.)
	IDXGIAdapter* selectedAdapter = nullptr;
	UINT lastScore = 0;

	for (auto it : adapters)
	{
		DXGI_ADAPTER_DESC desc;
		it->GetDesc(&desc);

		SIZE_T score = 0;
		score += desc.DedicatedSystemMemory;
		score += desc.DedicatedVideoMemory;

		if (score > lastScore)
		{
			lastScore = score;
			selectedAdapter = it;
		}
		else
		{
			ReleaseCOM(it);
		}
	}

	return selectedAdapter;
}
