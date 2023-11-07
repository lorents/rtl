#pragma once

#include <assert.h>
#include <map>
#include <string>
#include <memory>

#ifdef RTL_USE_SOURCE_LOCATION
#include <source_location>
#else
#include <sstream>
#endif

namespace rtl
{
	class tests
	{
		static void dependency_test();
		static void dependant_test();
		static void visitor_test();
		static void animated_test();
		static void constant_test();
		static void delegate_test();
		static void clock_test();
		static void timer_test();
		static void animator_test();
		static void var_test();
		static void vec_test();
		static void list_test();

		static void assert_equals(double lhs, double rhs)
		{
			assert(std::abs(lhs - rhs) < 0.0001);
		}
	public:

		static void run()
		{
			dependency_test();
			dependant_test();
			visitor_test();
			animated_test();
			constant_test();
			delegate_test();
			clock_test();
			timer_test();
			animator_test();
			var_test();
			vec_test();
			list_test();
		}

	};
}