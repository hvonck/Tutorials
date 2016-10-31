SamplerState gSampler;

cbuffer cbPerObject
{
	float4x4 gModel;
	float4x4 gView;
	float4x4 gProjection;
};

struct VertexIn
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD;
};

struct VertexOut
{
	float4 hPos		: HPOS;
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
	float2 tex		: TEXCOORD0;
};

struct PixelOutBasic
{
	float4 color : SV_Target;
};


VertexOut BasicVS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.hPos     = float4(vIn.position, 1.0f);
	vOut.position = mul(vOut.hPos, mul(gModel, mul(gView, gProjection)));
	vOut.normal   = mul(normalize(vIn.normal), (float3x3)gModel);
	vOut.tex      = vIn.tex;

	return vOut;
}

PixelOutBasic BasicPS(VertexOut pIn)
{
	PixelOutBasic pOut;
	pOut.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return pOut;
}

technique11 BasicTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, BasicVS()));
		SetPixelShader(CompileShader(ps_5_0, BasicPS()));
	}
};