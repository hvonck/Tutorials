#pragma once
#define _USE_MATH_DEFINES

#include <DirectXPackedVector.h>
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <windowsx.h>
#include <windows.h>
#include <assert.h>
#include <d3d11.h>
#include <string>
#include <vector>
#include <math.h>

#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
{															\
	HRESULT hr = (x);										\
	if (FAILED(hr))											\
	{														\
		LPWSTR output;										\
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |			\
			FORMAT_MESSAGE_IGNORE_INSERTS |					\
			FORMAT_MESSAGE_ALLOCATE_BUFFER,					\
			NULL,											\
			hr,												\
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),		\
			(LPTSTR)&output,								\
			0,												\
			NULL);											\
		MessageBox(NULL, output, L"Error", MB_OK);			\
	}														\
}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

typedef float RGBA[4];


struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 textureCoord;
};

struct TextureRenderTarget
{
	ID3D11Texture2D*			texture				= nullptr;
	ID3D11RenderTargetView*		renderTargetView	= nullptr;
	ID3D11ShaderResourceView*	shaderResourceView	= nullptr;
};

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<UINT>	indices;
};

struct ObjectData
{
	ID3D11Buffer*	vertexBuffer = nullptr;
	ID3D11Buffer*	indexBuffer  = nullptr;
	UINT			indexCount	 = 0;
	UINT			indexOffset	 = 0;
	UINT			vertexOffset = 0;

	ObjectData*		parent		 = nullptr;

	void Release()
	{
		if (parent == nullptr)
		{
			ReleaseCOM(vertexBuffer);
			ReleaseCOM(indexBuffer);
		}
	}

	~ObjectData()
	{
		Release();
	}
};

static const D3D11_INPUT_ELEMENT_DESC VERTEX_DESCRIPTION[]{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,								 D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal),		 D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(Vertex, textureCoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

namespace Colors
{
	static const RGBA White   = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const RGBA Black   = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const RGBA Red     = { 1.0f, 0.0f, 0.0f, 1.0f };
	static const RGBA Green   = { 0.0f, 1.0f, 0.0f, 1.0f };
	static const RGBA Blue    = { 0.0f, 0.0f, 1.0f, 1.0f };
	static const RGBA Yellow  = { 1.0f, 1.0f, 0.0f, 1.0f };
	static const RGBA Cyan    = { 0.0f, 1.0f, 1.0f, 1.0f };
	static const RGBA Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	static const RGBA Silver         = { 0.75f, 0.75f, 0.75f, 1.0f };
	static const RGBA LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
	static const RGBA Lilac          = { 0.71f, 0.4f,  0.82f, 1.0f };
}

namespace DXColors
{
	static const DirectX::XMFLOAT4 White   = (DirectX::XMFLOAT4)Colors::White;
	static const DirectX::XMFLOAT4 Black   = (DirectX::XMFLOAT4)Colors::Black;
	static const DirectX::XMFLOAT4 Red     = (DirectX::XMFLOAT4)Colors::Red;
	static const DirectX::XMFLOAT4 Green   = (DirectX::XMFLOAT4)Colors::Green;
	static const DirectX::XMFLOAT4 Blue    = (DirectX::XMFLOAT4)Colors::Blue;
	static const DirectX::XMFLOAT4 Yellow  = (DirectX::XMFLOAT4)Colors::Yellow;
	static const DirectX::XMFLOAT4 Cyan    = (DirectX::XMFLOAT4)Colors::Cyan;
	static const DirectX::XMFLOAT4 Magenta = (DirectX::XMFLOAT4)Colors::Magenta;

	static const DirectX::XMFLOAT4 Silver         = (DirectX::XMFLOAT4)Colors::Silver;
	static const DirectX::XMFLOAT4 LightSteelBlue = (DirectX::XMFLOAT4)Colors::LightSteelBlue;
	static const DirectX::XMFLOAT4 Lilac          = (DirectX::XMFLOAT4)Colors::Lilac;
}


#include "DirectXHelper.h"