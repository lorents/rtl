
#include <rtl/animator.h>

#include "test.h"

using namespace rtl;

namespace
{
	class test : public rtl::test
	{
		void run() override
		{
			animator anim([]() 
			{
				// something
			});
		}
	};

	test t;	
}
