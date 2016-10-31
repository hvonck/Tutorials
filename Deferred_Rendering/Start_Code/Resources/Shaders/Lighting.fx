

//------------------------------------------------------------
//-------------------------BLINN-PHONG------------------------
//------------------------------------------------------------

//Not here yet.


//------------------------------------------------------------
//-----------------------SHADOW-MAPPING-----------------------
//------------------------------------------------------------
Material CalculateShadow(float3 a_direction, float3 a_toEye, float3 a_normal)
{
	Material mat;

	mat.ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	mat.diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	mat.specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -a_direction;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	float diffuseFactor = dot(lightVec, a_normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, a_normal);
		float specFactor = pow(max(dot(v, a_toEye), 0.0f), 200);

		mat.diffuse = diffuseFactor;
		mat.specular = specFactor;
	}

	return mat;
}



//------------------------------------------------------------
//-----------------------NORMAL-MAPPING-----------------------
//------------------------------------------------------------
float4 CalculateNormal(float3 a_normalSample, float3 a_normal, float3 a_tangentU)
{
	float3 normalT = 2.0f * a_normalSample - 1.0f;

	float3 N = a_normal;
	float3 T = normalize(a_tangentU - dot(a_tangentU, N)*N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float4 normal = float4(mul(normalT, TBN), 1.0f);

	return normal;
}

PixelOutDeferred NormalMappingPS(VertexOut pIn)
{
	PixelOutDeferred pOut;
	//Color.
	pOut.color = gTexDiffuse.Sample(gSampler, pIn.tex) * gColor;

	//Normal.
	float3 normalSample = gTexNormal.Sample(gSampler, pIn.tex).rgb;
	pOut.normal = CalculateNormal(normalSample, pIn.normal, pIn.tangentU);

	//Position.
	pOut.position = mul(pIn.hPos, gModel);

	return pOut;
}