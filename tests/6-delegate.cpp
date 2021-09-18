
#include "../delegate.h"

#include "test.h"

using namespace rtl;

namespace
{
	class test : public rtl::test
	{
		void run() override
		{
			dependency dep;
			delegate<double> del(dep, []() { return 13.37; });
			assert_equals(del.value(), 13.37);
			assert_equals(del.target(), 13.37);
		}
	};

	test t;
}
