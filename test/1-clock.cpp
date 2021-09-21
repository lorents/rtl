
#include <rtl/clock.h>

#include "test.h"

using namespace rtl;


namespace
{
	class test : public rtl::test
	{
		void run() override
		{
			rtl::clock clock;
			assert(!clock.is_adjusted());
			clock.adjust(0);
			assert(clock.is_adjusted());
			assert(!clock.is_reversed());
			assert_equals(clock.now(), 0);
			clock.adjust(1);
			assert_equals(clock.now(), 1);
			assert(!clock.is_reversed());
			clock.adjust(0);
			assert_equals(clock.now(), 0);
			assert(clock.is_reversed());
			clock.adjust(0);
			assert_equals(clock.now(), 0);
			assert(clock.is_reversed());
			clock.adjust(2);
			assert_equals(clock.now(), 2);
			assert(!clock.is_reversed());
		}
	};

	test t;
}