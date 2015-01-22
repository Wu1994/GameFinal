#ifndef __MY_DEFERRED_SHADING_SHADER_HEADER__
#define __MY_DEFERRED_SHADING_SHADER_HEADER__ 

Texture2D gDepthBuffer;
Texture2D gGBuffer0;
Texture2D gGBuffer1;
Texture2D gGBuffer2;
Texture2D gGBuffer3;
SamplerState gSamplerState;

cbuffer cbPerFrame
{
	SPointLight gPointLight;
	SDirectionalLight gLight;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex : TEXCOORD;
};


float4 point_light_ps_main(VertexOut pin) : SV_TARGET
{
	float4 posH;
	posH.xy = pin.Tex * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
	posH.z = gDepthBuffer.Sample(gSamplerState, pin.Tex).r; 
	posH.w = 1.0f;

	clip(1.0f - posH.z);

	float4 posW = mul(posH, GF_INV_VIEW_PROJ);
	posW = posW / posW.w;
	float3 PosW = posW.xyz;

	float3 normal = gGBuffer0.Sample(gSamplerState, pin.Tex).xyz * 2.0f - 1.0f;
	float4 diffuseMtrl = gGBuffer1.Sample(gSamplerState, pin.Tex);
	float4 specularMtrl = gGBuffer2.Sample(gSamplerState, pin.Tex);
	float4 ambientMtrl = gGBuffer3.Sample(gSamplerState, pin.Tex);

	float4 diffuseSum = float4(0, 0, 0, 0);
	float4 specularSum = float4(0, 0, 0, 0);
	float4 diffuse;
	float4 specular;
	float3 lightDir;
	float4 Color = GF_AMBIENT * ambientMtrl;

	for(int i = 0; i < GF_POINT_LIGHTS_NUM; i++)
	{
		SPointLight pointLight = GF_POINT_LIGHTS[i];
		if(ComputeIrradianceOfPointLight(PosW, pointLight, lightDir, diffuse, specular))
		{
			BlinnPhoneShading(PosW, lightDir, normal, diffuse, specular, pointLight.Specular.w);
			//PhoneShading(PosW, lightDir, normal, diffuse, specular, pointLight.Specular.w);

			diffuseSum += diffuse;
			specularSum += specular;
		}
	}

	return Color + diffuseSum * diffuseMtrl + specularSum * specularMtrl;
}


float4 ps_main(VertexOut pin) : SV_TARGET
{
	float4 posH;
	posH.xy = pin.Tex * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
	posH.z = gDepthBuffer.Sample(gSamplerState, pin.Tex).r; 
	posH.w = 1.0f;

	clip(1.0f - posH.z);

	float4 posW = mul(posH, GF_INV_VIEW_PROJ);
	posW = posW / posW.w;
	float3 PosW = posW.xyz;

	//PosW = ComputeWorldPosFromDepthBuffer(gDepthBuffer, gSamplerState, pin.Tex);
	float3 normal = gGBuffer0.Sample(gSamplerState, pin.Tex).xyz * 2.0f - 1.0f;
	float4 diffuseMtrl = gGBuffer1.Sample(gSamplerState, pin.Tex);
	float4 specularMtrl = gGBuffer2.Sample(gSamplerState, pin.Tex);
	float4 ambientMtrl = gGBuffer3.Sample(gSamplerState, pin.Tex);

	float4 diffuse = gLight.Diffuse;
	float4 specular = gLight.Specular;

	float NdotL = dot(normal, -gLight.Direction.xyz);

	if(NdotL < 0)
	{
		return GF_AMBIENT * ambientMtrl;
	}

	PhoneShading(PosW, -gLight.Direction.xyz, normal,
		diffuse, specular, gLight.Specular.w);

#ifdef SHADOW_ON

	float shadowFactor = CalcShadowFactor_Jitter(GF_DIR_SHADOW_MAP_1, GF_SHADOW_MAP_SIZE_1, 
		GF_DIR_SHADOW_MAP_TRANSFORMS_1, pin.PosH.xy/pin.PosH.w * 29.454f, PosW, 10.0f);

	return GF_AMBIENT * ambientMtrl + 
		diffuse * diffuseMtrl * shadowFactor + specular * specularMtrl * shadowFactor;
#else
	return GF_AMBIENT * ambientMtrl + 
		diffuse * diffuseMtrl + specular * specularMtrl;
#endif
	
}


#endif