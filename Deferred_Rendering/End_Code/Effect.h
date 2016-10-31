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
	void SetDiffuse(ID3D11ShaderResourceView* a_diffuse)			{ m_diffuse->SetResource(a_diffuse);					}
	void SetNormal(ID3D11ShaderResourceView* a_normal)				{ m_normal->SetResource(a_normal);						}
	void SetPosition(ID3D11ShaderResourceView* a_position)			{ m_position->SetResource(a_position);					}

	void SelectTechBasic()                                          { m_techSelected = m_techBasic;                         }
	void SelectTechLighting()                                       { m_techSelected = m_techLighting;                      }

	void ApplyPass(ID3D11DeviceContext* a_d3dDeviceContext);
	D3DX11_TECHNIQUE_DESC GetSelectedTechDescription() const;

private:
	ID3DX11Effect*							m_effect		= nullptr;
	ID3D11InputLayout*						m_inputLayout	= nullptr;

	ID3DX11EffectTechnique*					m_techBasic		= nullptr;
	ID3DX11EffectTechnique*					m_techLighting	= nullptr;
	ID3DX11EffectTechnique*					m_techSelected	= nullptr;

	ID3DX11EffectMatrixVariable*			m_model			= nullptr;
	ID3DX11EffectMatrixVariable*			m_view			= nullptr;
	ID3DX11EffectMatrixVariable*			m_projection	= nullptr;

	ID3DX11EffectShaderResourceVariable*    m_diffuse		= nullptr;
	ID3DX11EffectShaderResourceVariable*    m_normal		= nullptr;
	ID3DX11EffectShaderResourceVariable*    m_position		= nullptr;
};
