#ifndef __SAMPLER_INTERFACE_H__
#define __SAMPLER_INTERFACE_H__

#include "IReferenceCounted.h"
#include "gfTypes.h"
#include "gfUtil.h"

namespace gf
{

	enum E_ADDRESS_MODE
	{
		EAM_WRAP = 1,
		EAM_MIRROR = 2,
		EAM_CLAMP = 3,
		EAM_BORDER = 4,
	};

	enum E_SAMPLER_FILTER
	{
		ESF_FILTER_MIN_MAG_MIP_POINT = 0,
		ESF_FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
		ESF_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
		ESF_FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
		ESF_FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
		ESF_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
		ESF_FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
		ESF_FILTER_MIN_MAG_MIP_LINEAR = 0x15,
		ESF_FILTER_ANISOTROPIC = 0x55,
		ESF_FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
		ESF_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
		ESF_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
		ESF_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
		ESF_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
		ESF_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
		ESF_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
		ESF_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
		ESF_FILTER_COMPARISON_ANISOTROPIC = 0xd5
	};

	struct SSamplerDesc
	{
		E_SAMPLER_FILTER	Filter;
		E_ADDRESS_MODE		AddressU;
		E_ADDRESS_MODE		AddressV;
		E_ADDRESS_MODE		AddressW;
		XMFLOAT4			BorderColor;
	};

	class ISampler : public IReferenceCounted
	{
	public:
		ISampler(const std::string& name)
			:mName(name)
		{
		}

		virtual void setAddressV(E_ADDRESS_MODE) = 0;
		virtual void setAddressU(E_ADDRESS_MODE) = 0;
		virtual void setAddressW(E_ADDRESS_MODE) = 0;

		virtual void setFilter(E_SAMPLER_FILTER) = 0;
		virtual void setBorderColor(const f32 rgba[]) = 0;

		virtual u32	 getSortCode() const = 0;

		const std::string& getName()
		{
			return mName;
		}

	private:
		std::string			mName;

	};

}

#endif

