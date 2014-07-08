#include "stdafx.h"
#include "D3DUtil.h"

namespace gf
{
	DXGI_FORMAT getDxgiFormat(E_GI_FORMAT format)
	{
		switch (format)
		{
		case EGF_R32G32B32A32_FLOAT:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case EGF_R32G32B32_FLOAT:		return DXGI_FORMAT_R32G32B32_FLOAT;
		case EGF_R32G32_FLOAT:			return DXGI_FORMAT_R32G32_FLOAT;
		case EGF_R32_FLOAT:				return DXGI_FORMAT_R32_FLOAT;
		case EGF_R8_UINT:				return DXGI_FORMAT_R8_UINT;
		case EGF_R8G8_UINT:				return DXGI_FORMAT_R8G8_UINT;
			//	case EGF_R8G8B8_UINT:			return DXGI_FORMAT_R8G8B8_UINT;
		case EGF_R8G8B8A8_UINT:			return DXGI_FORMAT_R8G8B8A8_UINT;
		case EGF_R16_FLOAT:				return DXGI_FORMAT_R16_FLOAT;
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	D3D11_USAGE getD3d11Usage(E_MEMORY_USAGE usage)
	{
		switch (usage)
		{
		case EMU_STATIC:	return D3D11_USAGE_IMMUTABLE;
		case EMU_DEFAULT:	return D3D11_USAGE_DEFAULT;
		case EMU_DYNAMIC:	return D3D11_USAGE_DYNAMIC;
		case EMU_STAGING:	return D3D11_USAGE_STAGING;
		}
		return D3D11_USAGE_IMMUTABLE;
	}

}

