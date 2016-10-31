#pragma once
#include "DirectX_Include.h"

#include "Window.h"
#include "Direct3D.h"
#include "Effect.h"

const float FOV = 45.0f;

enum class WINDOW_STATE
{
	CLOSED,
	RUNNING,
	PAUSED
};

class ForwardRenderer
{
public:
	ForwardRenderer(HINSTANCE a_hInstance);
	~ForwardRenderer();

	WINDOW_STATE Run();
	bool Init();

	void StartFrame();
	void Draw(const ObjectData& a_object, const DirectX::XMMATRIX& a_matModel);
	void SwapBuffers();

	void OnResize(UINT a_width, UINT a_height);

	void SetMatView(DirectX::XMFLOAT4X4& a_matView) { m_effect.SetViewMatrix(DirectX::XMLoadFloat4x4(&a_matView)); }
	ID3D11Device* GetDevice() const { return m_direct3D.GetDevice(); }

private:
	void bindBuffer(const ObjectData& a_object);

private:
	Window              m_window;
	Direct3D            m_direct3D;
	Effect              m_effect;
	
	DirectX::XMFLOAT4X4 m_matProjection;
	D3D11_VIEWPORT      m_viewport;
};