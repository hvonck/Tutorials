#include "ForwardRenderer.h"


ForwardRenderer::ForwardRenderer(HINSTANCE a_hInstance) :
	m_window(a_hInstance)
{
}


ForwardRenderer::~ForwardRenderer()
{
}


WINDOW_STATE ForwardRenderer::Run()
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

bool ForwardRenderer::Init()
{
	if (!m_window.Init())
		return false;

	if (!m_direct3D.Init(m_window.GetWidth(), m_window.GetHeight(), m_window.GetWindow()))
		return false;

	m_effect.Init(m_direct3D.GetDevice());
	OnResize(m_window.GetWidth(), m_window.GetHeight());
	
	return true;
}

void ForwardRenderer::StartFrame()
{
	m_effect.SetInputLayout(m_direct3D.GetDeviceContext());
	m_effect.SetProjectionMatrix(DirectX::XMLoadFloat4x4(&m_matProjection));

	ID3D11RenderTargetView* rtv = m_direct3D.GetRenderTarget();
	m_direct3D.GetDeviceContext()->RSSetViewports(1, &m_viewport);
	m_direct3D.GetDeviceContext()->OMSetRenderTargets(1, &rtv, nullptr);
	m_direct3D.GetDeviceContext()->ClearRenderTargetView(m_direct3D.GetRenderTarget(), Colors::Black);
	m_direct3D.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ForwardRenderer::Draw(const ObjectData& a_object, const DirectX::XMMATRIX& a_matModel)
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

void ForwardRenderer::SwapBuffers()
{
	m_direct3D.SwapBuffers();
}

void ForwardRenderer::OnResize(UINT a_width, UINT a_height)
{
	m_direct3D.OnResize(a_width, a_height);
	DirectX::XMStoreFloat4x4(&m_matProjection, DirectX::XMMatrixPerspectiveFovLH(FOV, m_window.GetAspectRatio(), 1.0f, 1000.0f));
	m_viewport = D3D11_VIEWPORT{ 0.0f, 0.0f, (float)a_width, (float)a_height, 0.0f, 1.0f };
}

void ForwardRenderer::bindBuffer(const ObjectData & a_object)
{
	static const UINT stride = sizeof(Vertex);
	static const UINT offset = 0;

	m_direct3D.GetDeviceContext()->IASetVertexBuffers(0, 1, &a_object.vertexBuffer, &stride, &offset);
	m_direct3D.GetDeviceContext()->IASetIndexBuffer(a_object.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}
