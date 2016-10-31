#include "DeferredRenderer.h"


DeferredRenderer::DeferredRenderer(HINSTANCE a_hInstance) :
	m_window(a_hInstance)
{
}


DeferredRenderer::~DeferredRenderer()
{
}


WINDOW_STATE DeferredRenderer::Run()
{
	MSG msg = { 0 };

	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			return WINDOW_STATE::CLOSED;

		return WINDOW_STATE::PAUSED;
	}
	else
	{
		if (m_window.ShouldResize())
			OnResize(m_window.GetWidth(), m_window.GetHeight());

		return WINDOW_STATE::RUNNING;
	}
}

bool DeferredRenderer::Init()
{
	if (!m_window.Init())
		return false;

	if (!m_direct3D.Init(m_window.GetWidth(), m_window.GetHeight(), m_window.GetWindow()))
		return false;

	m_effect.Init(m_direct3D.GetDevice());
	OnResize(m_window.GetWidth(), m_window.GetHeight());
	initScreenQuad();

	return true;
}

void DeferredRenderer::BindFirstPass()
{
	m_effect.SetInputLayout(m_direct3D.GetDeviceContext());
	m_direct3D.GetDeviceContext()->RSSetViewports(1, &m_viewport);
	
	//Set the rendertargets.
	ID3D11RenderTargetView* renderTargets[] = {
		m_graphicsBuffer[0].renderTargetView,
		m_graphicsBuffer[1].renderTargetView,
		m_graphicsBuffer[2].renderTargetView
	};
	m_direct3D.GetDeviceContext()->OMSetRenderTargets(BUFFER_COUNT, renderTargets, m_depthStencilView);

	//Clear the rendertargets.
	m_direct3D.GetDeviceContext()->ClearRenderTargetView(m_graphicsBuffer[0].renderTargetView, Colors::LightSteelBlue);
	m_direct3D.GetDeviceContext()->ClearRenderTargetView(m_graphicsBuffer[1].renderTargetView, Colors::Black);
	m_direct3D.GetDeviceContext()->ClearRenderTargetView(m_graphicsBuffer[2].renderTargetView, Colors::Black);
	m_direct3D.GetDeviceContext()->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_effect.SetProjectionMatrix(DirectX::XMLoadFloat4x4(&m_matProjection));
	m_effect.SelectTechBasic();
	m_direct3D.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_effect.ApplyPass(m_direct3D.GetDeviceContext());
}

void DeferredRenderer::BindLastPass()
{
	ID3D11RenderTargetView* rtv = m_direct3D.GetRenderTarget();
	m_direct3D.GetDeviceContext()->OMSetRenderTargets(1, &rtv, nullptr);
	m_direct3D.GetDeviceContext()->ClearRenderTargetView(m_direct3D.GetRenderTarget(), Colors::White);
	m_direct3D.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_effect.SetModelMatrix(DirectX::XMMatrixIdentity());

	m_effect.SetDiffuse(m_graphicsBuffer[0].shaderResourceView);
	m_effect.SetNormal(m_graphicsBuffer[1].shaderResourceView);
	m_effect.SetPosition(m_graphicsBuffer[2].shaderResourceView);
	
	m_effect.SelectTechLighting();
	m_effect.ApplyPass(m_direct3D.GetDeviceContext());

	bindBuffer(m_screenQuad);
	m_direct3D.GetDeviceContext()->DrawIndexed(m_screenQuad.indexCount, m_screenQuad.indexOffset, m_screenQuad.vertexOffset);

	m_effect.SetDiffuse(nullptr);
	m_effect.SetNormal(nullptr);
	m_effect.SetPosition(nullptr);
	m_effect.ApplyPass(m_direct3D.GetDeviceContext());
}

void DeferredRenderer::Draw(const ObjectData& a_object, const DirectX::XMMATRIX& a_matModel)
{
	//Don't draw an uninitialised object.
	if (a_object.vertexBuffer != nullptr)
	{
		m_effect.SetModelMatrix(a_matModel);
		m_effect.ApplyPass(m_direct3D.GetDeviceContext());

		bindBuffer(a_object);
		m_direct3D.GetDeviceContext()->DrawIndexed(a_object.indexCount, a_object.indexOffset, a_object.vertexOffset);
	}
}

void DeferredRenderer::SwapBuffers()
{
	m_direct3D.SwapBuffers();
}

void DeferredRenderer::OnResize(UINT a_width, UINT a_height)
{
	m_direct3D.OnResize(a_width, a_height);

	destroyGraphicsBuffer();
	initGraphicsBuffer();

	DirectX::XMStoreFloat4x4(&m_matProjection, DirectX::XMMatrixPerspectiveFovLH(FOV, m_window.GetAspectRatio(), 1.0f, 1000.0f));
	m_viewport = D3D11_VIEWPORT{ 0.0f, 0.0f, (float)a_width, (float)a_height, 0.0f, 1.0f };
}

void DeferredRenderer::bindBuffer(const ObjectData & a_object)
{
	static const UINT stride = sizeof(Vertex);
	static const UINT offset = 0;

	m_direct3D.GetDeviceContext()->IASetVertexBuffers(0, 1, &a_object.vertexBuffer, &stride, &offset);
	m_direct3D.GetDeviceContext()->IASetIndexBuffer(a_object.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeferredRenderer::initGraphicsBuffer()
{
	//Generate the render target textures.
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width            = m_window.GetWidth();
	textureDesc.Height           = m_window.GetHeight();
	textureDesc.MipLevels        = 1;
	textureDesc.ArraySize        = 1;
	textureDesc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage            = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	for (UINT i = 0; i < BUFFER_COUNT; i++)
		HR(m_direct3D.GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_graphicsBuffer[i].texture));

	//Generate the render target views.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format        = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	for (UINT i = 0; i < BUFFER_COUNT; i++)
		HR(m_direct3D.GetDevice()->CreateRenderTargetView(m_graphicsBuffer[i].texture, &renderTargetViewDesc, &m_graphicsBuffer[i].renderTargetView));

	//Generate the shader resource views.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format              = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (UINT i = 0; i < BUFFER_COUNT; i++)
		HR(m_direct3D.GetDevice()->CreateShaderResourceView(m_graphicsBuffer[i].texture, &shaderResourceViewDesc, &m_graphicsBuffer[i].shaderResourceView));

	//Generate the depth stencil buffer texture.
	ID3D11Texture2D* depthStencilTexture = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc{};
	depthStencilBufferDesc.Width            = m_window.GetWidth();
	depthStencilBufferDesc.Height           = m_window.GetHeight();
	depthStencilBufferDesc.MipLevels        = 1;
	depthStencilBufferDesc.ArraySize        = 1;
	depthStencilBufferDesc.Format           = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.Usage            = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags        = D3D11_BIND_DEPTH_STENCIL;
	
	HR(m_direct3D.GetDevice()->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format        = depthStencilBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	HR(m_direct3D.GetDevice()->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &m_depthStencilView));

	ReleaseCOM(depthStencilTexture);
}

void DeferredRenderer::destroyGraphicsBuffer()
{
	ReleaseCOM(m_depthStencilView);

	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		ReleaseCOM(m_graphicsBuffer[i].texture);
		ReleaseCOM(m_graphicsBuffer[i].renderTargetView);
		ReleaseCOM(m_graphicsBuffer[i].shaderResourceView);
	}
}

#include "GeometryGenerator.h"
void DeferredRenderer::initScreenQuad()
{
	GeometryGenerator gen;
	MeshData meshData;
	gen.GenerateFullscreenQuad(meshData);
	DirectXHelper::GenerateVertexBuffer(m_direct3D.GetDevice(), meshData, m_screenQuad);
}
