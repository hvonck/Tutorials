#include "Window.h"

//Global message procedure.
//DO NOT REMOVE.
namespace
{
	Window* g_Window;
}

LRESULT CALLBACK MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_Window->MessageProcedure(hwnd, msg, wParam, lParam);
}


Window::Window(HINSTANCE a_hInstance) :
	m_hInstance(a_hInstance)
{
	m_hInstance = GetModuleHandle(0);
	g_Window = this;
}


Window::~Window()
{
}

bool Window::Init()
{
	//Create the window info.
	WNDCLASSEX winClassex{};
	winClassex.cbSize			= sizeof(WNDCLASSEX);
	winClassex.style			= CS_HREDRAW | CS_VREDRAW;
	winClassex.lpfnWndProc		= MainWindowProcedure;
	winClassex.cbClsExtra		= 0;
	winClassex.cbWndExtra		= 0;
	winClassex.hInstance		= m_hInstance;
	winClassex.hIcon			= LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	winClassex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	winClassex.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
	winClassex.lpszMenuName		= NULL;
	winClassex.lpszClassName	= L"D3DWndClassName";
	winClassex.hIconSm			= LoadIcon(winClassex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//Register the window.
	if (!RegisterClassEx(&winClassex))
	{
		MessageBox(NULL, L"Call to RegisterClassEx failed!", L"Window Creation Error Message", NULL);
		return false;
	}

	//Ajust the screen size.
	RECT R = { 0, 0, m_width, m_height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width	= R.right - R.left;
	int height	= R.bottom - R.top;

	//Create the window.
	m_window = CreateWindow(winClassex.lpszClassName, m_title.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width,
		height, NULL, NULL, m_hInstance, (LPVOID)this);

	if (!m_window)
	{
		MessageBox(NULL, L"Call to CreateWindow failed!", L"Window Creation Error Message", NULL);
		return false;
	}

	//Display the window.
	ShowWindow(m_window, SW_SHOW);
	UpdateWindow(m_window);

	return true;
}

LRESULT Window::MessageProcedure(HWND a_hWnd, UINT a_message, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_message)
	{
	case WM_ACTIVATE:
		if (LOWORD(a_wParam) == WA_INACTIVE)
		{
			m_isPaused = true;
		}
		else
		{
			m_isPaused = false;
		}
		return 0;

	case WM_SIZE:
		m_width = LOWORD(a_lParam);
		m_height = HIWORD(a_lParam);
		if (a_wParam == SIZE_MINIMIZED)
		{
			m_isPaused = true;
			m_isMinimized = true;
			m_isMaximized = false;
		}
		else if (a_wParam == SIZE_MAXIMIZED)
		{
			m_isPaused = false;
			m_isMinimized = false;
			m_isMaximized = true;

			m_shouldResize = true;
		}
		else if (a_wParam == SIZE_RESTORED)
		{
			if (m_isMinimized)
			{
				m_isPaused = false;
				m_isMinimized = false;

				m_shouldResize = true;
			}
			else if (m_isMaximized)
			{
				m_isPaused = false;
				m_isMaximized = false;

				m_shouldResize = true;
			}
		}
		else if (m_isResizing)
		{
			//Do nothing, otherwise we will re-create the rendertargets
			//A million different types because of the dragging.
		}
		else
		{
			m_shouldResize = true;
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		m_isPaused = true;
		m_isResizing = true;
		return 0;

	case WM_EXITSIZEMOVE:
		m_isPaused = false;
		m_isResizing = false;
		m_shouldResize = true;
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)a_lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)a_lParam)->ptMinTrackSize.y = 200;
		return 0;
	}

	return DefWindowProc(a_hWnd, a_message, a_wParam, a_lParam);
}

bool Window::ShouldResize()
{
	if (m_shouldResize)
	{
		m_shouldResize = false;
		return true;
	}
	return false;
}
