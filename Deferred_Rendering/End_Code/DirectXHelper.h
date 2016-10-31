#pragma once
#include "DirectX_Include.h"

class DirectXHelper
{
public:
	static HRESULT CompileShader(LPCWSTR a_srcFile, LPCSTR a_entryPoint, LPCSTR a_profile, ID3DBlob** a_blob);
	static HRESULT CompileShaderFromData(const std::wstring& a_filePath, ID3D11Device* a_d3dDevice, ID3DX11Effect** a_effect);
	static void GenerateVertexBuffer(ID3D11Device* a_d3dDevice, const MeshData& a_meshData, ObjectData& a_objectData);
	
	template<typename T>
	static inline T Clamp(const T& a_type, const T& a_min, const T& a_max) 
		{ return a_type < a_min ? a_min : (a_type > a_max ? a_max : a_type); }

};

