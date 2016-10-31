

//------------------------------------------------------------
//--------------------------GLOBALS---------------------------
//------------------------------------------------------------
SamplerState gSampler
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressV = WRAP;
};

Texture2D gTexDiffuse;
Texture2D gTexNormal;
Texture2D gTexPosition;

//------------------------------------------------------------
//---------------------------PER-X----------------------------
//------------------------------------------------------------
cbuffer cbPerObject
{
	float4x4 gModel;
	float4x4 gView;
	float4x4 gProjection;
	float4x4 gMIT;
	float4 gColor;
	float3 gLightPos;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 reflect;
};

cbuffer cbPerFrame
{
	float gMaxTessDistance;
	float gMinTessDistance;
	float gMinTessFactor;
	float gMaxTessFactor;
	float gHeightScale;
	float gTime;
};

//------------------------------------------------------------
//---------------------------VERTEX---------------------------
//------------------------------------------------------------
struct VertexIn
{
	float3 position  : POSITION;
	float3 normal : NORMAL;
	float3 tangentU : TANGENTU;
	float2 tex : TEXCOORD;
};

struct VertexOut
{
	float4 hPos : HPOS;
	float4 position  : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangentU : TANGENTU;
	float2 tex : TEXCOORD0;
};

//------------------------------------------------------------
//---------------------------PIXEL----------------------------
//------------------------------------------------------------
struct PixelOutDeferred
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 position : SV_Target2;
};

struct PixelOutBasic
{
	float4 color : SV_Target;
};

//------------------------------------------------------------
//------------------------TESSELLATION------------------------
//------------------------------------------------------------
struct DisVertOut
{
	float3 positionW  : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float2 tex : TEXCOORD;
	float tessFactor : TESS;
};

struct PatchTess
{
	float edgeTess[3] : SV_TessFactor;
	float insideTess : SV_InsideTessFactor;
};

struct DomainOut
{
	float4 position : SV_POSITION;
	float3 hPos : POSITION;
	float3 normal : NORMAL;
	float3 tangentU : TANGENT;
	float2 tex : TEXCOORD;
};

