#ifndef __SHADER_VARIABLE_INJECT_STRATEGY_INTERFACE_H__
#define __SHADER_VARIABLE_INJECT_STRATEGY_INTERFACE_H__

#include "IMeshNode.h"
#include "IPipeline.h"
#include "ISceneManager.h"
namespace gf
{
	class IShaderVariableInjection : public IReferenceCounted
	{
	public:
		virtual void inject(IMeshNode* mesh, IPipeline* pipeline, u32 subset = 0) const = 0;

		//_DECLARE_SINGLETON_INSTANCE(IShaderVariableInjection);
	};
}
#endif
