#include "GeometryGenerator.h"

//0Complete-: Not finished.									 /\
1Complete-: Added Position.								    //\\
2Complete-: Added Normals.								   ///\\\
3Complete-: Added TexCoords.							   ///\\\
4Complete-: Added TangentU.								  ////\\\\
														  ////\\\\
Second number specifies the version that it is becomming./////\\\\\
2Complete3: Somewhere inbetween 2 and 3.			     /////\\\\\
3Complete3: Just version 3.							    //////\\\\\\
															 ||

GeometryGenerator::GeometryGenerator()
{
}


GeometryGenerator::~GeometryGenerator()
{
}

//--------------------------------------------------
//--------------------4COMPLETE4--------------------
//--------------------------------------------------
void GeometryGenerator::GenerateBox(float a_width, float a_height, float a_depth, MeshData & a_meshData)
{
	a_meshData.vertices.clear();
	a_meshData.indices.clear();

	Vertex v[24];

	float w2 = 0.5f * a_width;
	float h2 = 0.5f * a_height;
	float d2 = 0.5f * a_depth;

	// Fill in the front face vertex data.
	v[0]  = { { -w2, -h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };
	v[1]  = { { -w2, +h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } };
	v[2]  = { { +w2, +h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } };
	v[3]  = { { +w2, -h2, -d2 }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } };

	// Fill in the back face vertex data.
	v[4]  = { { -w2, -h2, +d2 }, { 0.0f, 0.0f,  1.0f }, { 1.0f, 1.0f } };
	v[5]  = { { +w2, -h2, +d2 }, { 0.0f, 0.0f,  1.0f }, { 0.0f, 1.0f } };
	v[6]  = { { +w2, +h2, +d2 }, { 0.0f, 0.0f,  1.0f }, { 0.0f, 0.0f } };
	v[7]  = { { -w2, +h2, +d2 }, { 0.0f, 0.0f,  1.0f }, { 1.0f, 0.0f } };

	// Fill in the top face vertex data.
	v[8]  = { { -w2, +h2, -d2 }, { 0.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } };
	v[9]  = { { -w2, +h2, +d2 }, { 0.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } };
	v[10] = { { +w2, +h2, +d2 }, { 0.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } };
	v[11] = { { +w2, +h2, -d2 }, { 0.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } };

	// Fill in the bottom face vertex data.
	v[12] = { { -w2, -h2, -d2 }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } };
	v[13] = { { +w2, -h2, -d2 }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } };
	v[14] = { { +w2, -h2, +d2 }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } };
	v[15] = { { -w2, -h2, +d2 }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } };

	// Fill in the left face vertex data.
	v[16] = { { -w2, -h2, +d2 }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };
	v[17] = { { -w2, +h2, +d2 }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };
	v[18] = { { -w2, +h2, -d2 }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };
	v[19] = { { -w2, -h2, -d2 }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };

	// Fill in the right face vertex data.
	v[20] = { { +w2, -h2, -d2 }, {  1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };
	v[21] = { { +w2, +h2, -d2 }, {  1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };
	v[22] = { { +w2, +h2, +d2 }, {  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };
	v[23] = { { +w2, -h2, +d2 }, {  1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };

	a_meshData.vertices.assign(&v[0], &v[24]);

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	a_meshData.indices.assign(&i[0], &i[36]);
}

//--------------------------------------------------
//--------------------4COMPLETE4--------------------
//--------------------------------------------------
void GeometryGenerator::GenerateSphere(float a_radius, UINT a_sliceCount, UINT a_stackCount, MeshData & a_meshData)
{
	a_meshData.vertices.clear();
	a_meshData.indices.clear();

	//Vertices
	Vertex topVertex   { { 0.0f,  a_radius, 0.0f }, { 0.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } };
	Vertex bottomVertex{ { 0.0f, -a_radius, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } };

	a_meshData.vertices.push_back(topVertex);

	float phiStep   =        M_PI / (float)a_stackCount;
	float thetaStep = 2.0f * M_PI / (float)a_sliceCount;

	for (UINT stack = 1; stack < a_stackCount; stack++)
	{
		float phi = (float)stack * phiStep;

		for (UINT slice = 0; slice < a_sliceCount + 1; slice++)
		{
			float theta = (float)slice * thetaStep;

			Vertex vertex;
			vertex.position = { a_radius * sinf(phi) * cosf(theta),
								a_radius * cosf(phi),
								a_radius * sinf(phi) * sinf(theta) };
			
			vertex.textureCoord = { theta / DirectX::XM_2PI, phi / (float)M_PI };

			DirectX::XMStoreFloat3(&vertex.normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vertex.position)));

			a_meshData.vertices.push_back(vertex);
		}
	}
	a_meshData.vertices.push_back(bottomVertex);


	//Indices.
	//Top.
	for (UINT slice = 0; slice < a_sliceCount + 1; slice++)
	{
		a_meshData.indices.push_back(0);
		a_meshData.indices.push_back(slice + 1);
		a_meshData.indices.push_back(slice);
	}

	//Middle.
	UINT baseIndex = 1;
	UINT ringVertexCount = a_sliceCount + 1;
	for (UINT stack = 0; stack < a_stackCount - 2; stack++)
	{
		for (UINT slice = 0; slice < a_sliceCount; slice++)
		{
			a_meshData.indices.push_back(baseIndex +  stack      * ringVertexCount + slice);
			a_meshData.indices.push_back(baseIndex +  stack      * ringVertexCount + slice + 1);
			a_meshData.indices.push_back(baseIndex + (stack + 1) * ringVertexCount + slice);

			a_meshData.indices.push_back(baseIndex + (stack + 1) * ringVertexCount + slice);
			a_meshData.indices.push_back(baseIndex +  stack      * ringVertexCount + slice + 1);
			a_meshData.indices.push_back(baseIndex + (stack + 1) * ringVertexCount + slice + 1);
		}
	}

	//Bottom.
	UINT southPoleIndex = (UINT)a_meshData.vertices.size() - 1;
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT slice = 0; slice < a_sliceCount; slice++)
	{
		a_meshData.indices.push_back(southPoleIndex);
		a_meshData.indices.push_back(baseIndex + slice);
		a_meshData.indices.push_back(baseIndex + slice + 1);
	}
}

//--------------------------------------------------
//--------------------4COMPLETE4--------------------
//--------------------------------------------------
void GeometryGenerator::GenerateFullscreenQuad(MeshData& a_meshData)
{
	a_meshData.vertices.resize(4);
	a_meshData.indices.resize(6);

	// Position coordinates specified in NDC space.
	a_meshData.vertices[0] = { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };
	a_meshData.vertices[1] = { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } };
	a_meshData.vertices[2] = { {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } };
	a_meshData.vertices[3] = { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } };

	a_meshData.indices[0] = 0;
	a_meshData.indices[1] = 1;
	a_meshData.indices[2] = 2;

	a_meshData.indices[3] = 0;
	a_meshData.indices[4] = 2;
	a_meshData.indices[5] = 3;
}
