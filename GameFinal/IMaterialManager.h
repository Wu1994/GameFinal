#ifndef __MATERIAL_MANAGER_INTERFACE_H__
#define __MATERIAL_MANAGER_INTERFACE_H__

#include "IReferenceCounted.h"
#include "IMaterial.h"

namespace gf
{

	class IMaterialManager : public IReferenceCounted
	{
	public:
		virtual IMaterial* create(const std::string& name,
			IPipeline* pipelines[]) = 0;

		virtual IMaterial* create(const std::string& name,
			IPipeline* pipeline) = 0;

		virtual bool add(IMaterial& material) = 0;
		//virtual IMaterial* load() = 0;
		virtual IMaterial* get(const std::string& name, bool bLoadIfNotExist = true) = 0;

		virtual bool destroy(const std::string& name) = 0;

		virtual bool destroy(IMaterial* material) = 0;

		_DECLARE_SINGLETON_INSTANCE(IMaterialManager);
	};

}

#endif
