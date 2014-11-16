#ifndef __GAME_FINAL_SHADER_HEADER__
#define __GAME_FINAL_SHADER_HEADER__

#ifndef GF_TEXTURE
#define GF_TEXTURE GF_TEXTURE_0
#endif

struct SDirectionalLight
{
	float4	Diffuse;
	float4	Specular;
	float4	Direction;
};

struct SPointLight
{
	float4	Diffuse;
	float4	Specular;
	float3	Position;
	float	Range;
	float3  Attenuations;
	float   _pad;
};

struct SSpotLight
{
	float4	Diffuse;
	float4	Specular;
	float4	Direction;
	float3	Position;
	float	Range;
	float3	Attenuations;
	float	Falloff;
	float	InnerCone;
	float	OuterCone;
	float2  _pad;
};

struct SMaterial
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emissive;
};

cbuffer gf_cb_world
{
	float4x4 GF_WORLD;
	float4x4 GF_INV_WORLD;
	float4x4 GF_TRAN_WORLD;
	float4x4 GF_INV_TRAN_WORLD;
};

cbuffer gf_cb_view
{
	float4x4 GF_VIEW;
	float4x4 GF_INV_VIEW;
	float4x4 GF_TRAN_VIEW;
	float4x4 GF_INV_TRAN_VIEW;
};

cbuffer gf_cb_world_view
{
	float4x4 GF_WORLD_VIEW;
	float4x4 GF_INV_WORLD_VIEW;
	float4x4 GF_TRAN_WORLD_VIEW;
	float4x4 GF_INV_TRAN_WORLD_VIEW;
};

cbuffer gf_cb_wvp
{
	float4x4 GF_WVP;
	float4x4 GF_INV_WVP;
	float4x4 GF_TRAN_WVP;
	float4x4 GF_INV_TRAN_WVP;
};

cbuffer gf_cb_proj
{
	float4x4 GF_PROJ;
	float4x4 GF_INV_PROJ;
	float4x4 GF_TRAN_PROJ;
	float4x4 GF_INV_TRAN_PROJ;
};

cbuffer gf_cb_view_proj
{
	float4x4 GF_VIEW_PROJ;
	float4x4 GF_INV_VIEW_PROJ;
	float4x4 GF_TRAN_VIEW_PROJ;
	float4x4 GF_INV_TRAN_VIEW_PROJ;
};

cbuffer gf_cb_camera_pos
{
	float3 GF_CAMERA_POS;
	float4 GF_CAMERA_SEGMENTS;
};

cbuffer gf_cb_material
{
	float4 GF_MTRL_AMBIENT;
	float4 GF_MTRL_DIFFUSE;
	float4 GF_MTRL_SPECULAR;
	float4 GF_MTRL_EMISSIVE;
};

cbuffer gf_cb_screen_size
{
	float4  GF_SCREEN_SIZE;
};

cbuffer gf_cb_scene_info
{
	float4 GF_AMBIENT;
};

cbuffer gf_cb_point_lights
{
	SPointLight		GF_POINT_LIGHTS[32];
	uint			GF_POINT_LIGHTS_NUM;
};

cbuffer gf_cb_directional_lights
{
	SDirectionalLight		GF_DIR_LIGHTS[8];
	uint					GF_DIR_LIGHTS_NUM;
};

Texture2D GF_TEXTURE_0;
Texture2D GF_TEXTURE_1;
Texture2D GF_TEXTURE_2;
Texture2D GF_TEXTURE_3;
Texture2D GF_TEXTURE_4;
Texture2D GF_TEXTURE_5;
Texture2D GF_TEXTURE_6;
Texture2D GF_TEXTURE_7;



/* @lightDir: from object to light.
   @return: if light can reach the object. */
bool ComputeIrradianceOfPointLight(float3 pos, SPointLight light, out float3 lightDir,
	out float4 diffuse, out float4 specular)
{
	lightDir = light.Position - pos;
	float dist = length(lightDir);
	if(dist > light.Range)
		return false;

	lightDir /= dist;
	float distAtten = 1.0 / dot(light.Attenuations, float3(1, dist, dist * dist));

	diffuse = light.Diffuse * distAtten;
	specular = light.Specular * distAtten;

	return true;
}


void PhoneShading(float3 pos, float3 lightDir, float3 normal, 
	inout float4 diffuse, inout float4 specular, float power)
{
	float3 reflectDir = reflect(-lightDir, normal);
	float3 viewDir = normalize(GF_CAMERA_POS - pos);

	float atten = saturate(dot(normal, lightDir));
	float specAtten = saturate(dot(viewDir, reflectDir));

	diffuse = diffuse * atten;
	specular = specular * atten * pow(specAtten, power);
}



float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f * normalMapSample - 1.0f;
	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);
	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);
	return bumpedNormalW;
}



#endif