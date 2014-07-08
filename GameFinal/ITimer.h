#ifndef __ITIMER_H_INCLUDE__
#define __ITIMER_H_INCLUDE__

#include "gfTypes.h"
#include "gfUtil.h"
#include "IReferenceCounted.h"
namespace gf
{
	class ITimer : public IReferenceCounted
	{
	public:
		virtual u32 tick() = 0;
		virtual u32 getElapseTime() const = 0;

		virtual void start() = 0; // Call when unpaused.
		virtual void stop() = 0;  // Call when paused.

		virtual void reset() = 0;
	};
}
#endif
