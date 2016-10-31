#pragma once
#include "DirectX_Include.h"
#include <unordered_map>

class Window
{
public:
	Window(HINSTANCE a_hInstance);
	~Window();

	bool Init();
	LRESULT MessageProcedure(HWND a_hWnd, UINT a_message, WPARAM a_wParam, LPARAM a_lParam);
	bool ShouldResize();

	//Getters.
	HWND			GetWindow()			const { return m_window; }
	UINT			GetHeight()			const { return m_height; }
	UINT			GetWidth()			const { return m_width;  }
	float			GetAspectRatio()	const { return (float)m_width / (float)m_height; }

private:
	HWND				m_window			= nullptr;
	HINSTANCE			m_hInstance			= nullptr;
	std::wstring		m_title				= L"Default Title";
	UINT				m_height			= 600;
	UINT				m_width				= 800;

	bool				m_shouldResize		= false;
	bool				m_isPaused			= false;
	bool				m_isMinimized		= false;
	bool				m_isMaximized		= false;
	bool				m_isResizing		= false;
};
