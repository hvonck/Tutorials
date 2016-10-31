#include "DirectXHelper.h"
#include <fstream>

HRESULT DirectXHelper::CompileShader(LPCWSTR a_srcFile, LPCSTR a_entryPoint, LPCSTR a_profile, ID3DBlob** a_blob)
{
	if (!a_srcFile || !&a_entryPoint || !a_profile || !a_blob)
		return E_INVALIDARG;

	*a_blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[]{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT res = D3DCompileFromFile(a_srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		a_entryPoint, a_profile, flags, 0, &shaderBlob, &errorBlob);

	if (FAILED(res))
	{
		if (errorBlob)
			OutputDebugStringA((char*)errorBlob->GetBufferPointer()),
			errorBlob->Release();
		if (shaderBlob)
			shaderBlob->Release();

		return res;
	}

	*a_blob = shaderBlob;

	return res;
}

HRESULT DirectXHelper::CompileShaderFromData(const std::wstring & a_filePath, ID3D11Device * a_d3dDevice, ID3DX11Effect ** a_effect)
{
	std::ifstream fin(a_filePath.c_str(), std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	return D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, a_d3dDevice, a_effect);
}

void DirectXHelper::GenerateVertexBuffer(ID3D11Device* a_d3dDevice, const MeshData & a_meshData, ObjectData & a_objectData)
{
	static D3D11_BUFFER_DESC vertexBufferDesc { 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	static D3D11_BUFFER_DESC indexBufferDesc  { 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER,  0, 0, 0 };
	static D3D11_SUBRESOURCE_DATA vertexSubresourceData {};
	static D3D11_SUBRESOURCE_DATA indexSubresourceData  {};

	if (a_meshData.indices.empty() || a_meshData.vertices.empty())
		return;

	a_objectData.indexCount = a_meshData.indices.size();

	vertexBufferDesc.ByteWidth = sizeof(Vertex) * a_meshData.vertices.size();
	vertexSubresourceData.pSysMem = a_meshData.vertices.data();
	HR(a_d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &a_objectData.vertexBuffer));

	indexBufferDesc.ByteWidth = sizeof(UINT) * a_meshData.indices.size();
	indexSubresourceData.pSysMem = a_meshData.indices.data();
	HR(a_d3dDevice->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &a_objectData.indexBuffer));
}
