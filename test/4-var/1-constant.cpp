#include <rtl/var/constant.h>

#include <rtl/graph/dependency.h>

#include "../test.h"

using namespace rtl;

namespace
{
	class test : public rtl::test
	{
		void run() override
		{
			dependency dep;
			constant<double> con(dep, 13.37);
			assert_equals(con.value(), 13.37);
			assert_equals(con.target(), 13.37);
			assert(con.try_set(777));
			assert_equals(con.value(), 777);
		}
	};

	test t;
}
