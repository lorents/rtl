#include <rtl/var.h>

#include "../test.h"

using namespace rtl;

namespace
{
	class test : public rtl::test
	{
		void run() override
		{
			var<double> foo = 5.0;
			var<double> bar = 7.0;
			var<double> fooplusbar = {[&]() { return foo + bar; }};
			assert_equals(foo, 5.0);
			assert_equals(bar, 7.0);
			assert_equals(fooplusbar, 5.0 + 7.0);
			foo = 7;
			assert_equals(fooplusbar, 7.0 + 7.0);
			fooplusbar = [&]() { return foo(); };
			assert_equals(fooplusbar, 7.0);
		}
	};

	test t;
}
