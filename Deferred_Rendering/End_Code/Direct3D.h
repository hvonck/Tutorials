#pragma once
#include "DirectX_Include.h"

class Direct3D
{
public:
	Direct3D();
	~Direct3D();

	bool Init(UINT a_width, UINT a_height, HWND a_window);
	void OnResize(UINT a_width, UINT a_height);
	void SwapBuffers();

	//Getters.
	ID3D11Device*			GetDevice()			const { return m_d3dDevice;			}
	ID3D11DeviceContext*	GetDeviceContext()	const { return m_d3dDeviceContext;	}
	IDXGISwapChain*			GetSwapChain()		const { return m_swapChain;			}
	ID3D11RenderTargetView*	GetRenderTarget()	const { return m_renderTargetView;	}

private:
	IDXGIAdapter* selectAdapter();

private:	
	ID3D11Device*			m_d3dDevice			= nullptr;
	ID3D11DeviceContext*	m_d3dDeviceContext	= nullptr;
	IDXGISwapChain*			m_swapChain			= nullptr;
	ID3D11RenderTargetView* m_renderTargetView	= nullptr;
};