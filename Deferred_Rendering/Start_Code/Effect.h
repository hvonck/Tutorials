#pragma once
#include "DirectX_Include.h"

class Effect
{
public:
	Effect();
	~Effect();

	void Init(ID3D11Device* a_d3dDevice, const std::wstring& a_filePath = L"Resources\\Shaders\\deferredShader.cso");

	//Setters.
	void SetInputLayout(ID3D11DeviceContext* a_d3dDeviceContext)	{ a_d3dDeviceContext->IASetInputLayout(m_inputLayout);  }

	void SetViewMatrix(const DirectX::XMMATRIX& a_view)				{ m_view->SetMatrix((float*)&a_view);					}
	void SetProjectionMatrix(const DirectX::XMMATRIX& a_projection) { m_projection->SetMatrix((float*)&a_projection);		}
	void SetModelMatrix(const DirectX::XMMATRIX& a_model)			{ m_model->SetMatrix((float*)&a_model);					}

	void ApplyPass(ID3D11DeviceContext* a_d3dDeviceContext);
	D3DX11_TECHNIQUE_DESC GetSelectedTechDescription() const;

private:
	ID3DX11Effect*							m_effect		= nullptr;
	ID3DX11EffectTechnique*					m_techBasic		= nullptr;
	ID3D11InputLayout*						m_inputLayout	= nullptr;

	ID3DX11EffectMatrixVariable*			m_model			= nullptr;
	ID3DX11EffectMatrixVariable*			m_view			= nullptr;
	ID3DX11EffectMatrixVariable*			m_projection	= nullptr;
};
