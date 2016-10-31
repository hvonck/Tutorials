#include "ForwardRenderer.h"
#include "DebugModels.h"
#include <crtdbg.h>

#include "GeometryGenerator.h"

void Update(ForwardRenderer* a_app);
void Init();

//Global for now.
//I mean, this is just a throw-away test scene.
DirectX::XMFLOAT4X4 sphereWorld[10];
DirectX::XMFLOAT4X4 boxWorld;
DirectX::XMFLOAT4X4 centerSphere;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	ForwardRenderer app(hInstance);
	if (!app.Init())
		return -1;

	Init();

	DebugModels models(&app);
	
	//Main loop.
	WINDOW_STATE state = WINDOW_STATE::RUNNING;
	do 
	{
		state = app.Run();

		if (state == WINDOW_STATE::RUNNING)
		{
			//UPDATE CODE:
			Update(&app);

			//DRAWING CODE:
			app.StartFrame();

			app.Draw(models.GetModelCube(),		DirectX::XMLoadFloat4x4(&boxWorld));
			app.Draw(models.GetModelSphere(),	DirectX::XMLoadFloat4x4(&centerSphere));
			for (UINT i = 0; i < 10; i++)
			{
				app.Draw(models.GetModelSphere(),   DirectX::XMLoadFloat4x4(&sphereWorld[i]));
			}

			app.SwapBuffers();
		}

	} while (state != WINDOW_STATE::CLOSED);

	models.Release();

	return 0;
}

void Update(ForwardRenderer* a_app)
{
	static float m_theta = 0.95f;
	static float m_phi = 1.57f;
	static float m_radius = 20.0f;
	
	//Update code.
	float x = m_radius * sinf(m_phi) * cosf(m_theta);
	float y = m_radius * sinf(m_phi) * sinf(m_theta);
	float z = m_radius * cosf(m_phi);

	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	DirectX::XMFLOAT4X4 matView;
	DirectX::XMStoreFloat4x4(&matView, V);
	a_app->SetMatView(matView);
}

void Init()
{
	DirectX::XMMATRIX boxScale = DirectX::XMMatrixScaling(2.0f, 1.0f, 2.0f);
	DirectX::XMMATRIX boxOffset = DirectX::XMMatrixTranslation(0.0f, 0.5f, 10.0f);
	DirectX::XMStoreFloat4x4(&boxWorld, DirectX::XMMatrixMultiply(boxScale, boxOffset));

	DirectX::XMMATRIX centerSphereScale = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f);
	DirectX::XMMATRIX centerSphereOffset = DirectX::XMMatrixTranslation(0.0f, 2.0f, 10.0f);
	DirectX::XMStoreFloat4x4(&centerSphere, DirectX::XMMatrixMultiply(centerSphereScale, centerSphereOffset));

	for (UINT i = 0; i < 5; i++)
	{
		DirectX::XMStoreFloat4x4(&sphereWorld[i * 2 + 0], DirectX::XMMatrixTranslation(-5.0f, 1.5f, -10.0f + (float)i * 5.0f));
		DirectX::XMStoreFloat4x4(&sphereWorld[i * 2 + 1], DirectX::XMMatrixTranslation(5.0f, 1.5f, -10.0f + (float)i * 5.0f));
	}
}