#pragma once
#include "DirectX_Include.h"
#include <vector>

class GeometryGenerator
{
public:
	GeometryGenerator();
	~GeometryGenerator();

	void GenerateBox(float a_width, float a_height, float a_depth, MeshData& a_meshData);
	void GenerateSphere(float a_radius, UINT a_sliceCount, UINT a_stackCount, MeshData& a_meshData);
	void GenerateFullscreenQuad(MeshData& a_meshData);
};

