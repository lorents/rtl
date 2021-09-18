
#include "../animated.h"

#include "test.h"

using namespace rtl;

namespace
{
	class test : public rtl::test
	{
		void run() override
		{
			rtl::clock clock;
			dependency dep;
			clock.adjust(0.0);
			animated<double> anim(dep, clock, 0.0, 1.0, tween(10.0));
			assert(anim.is_valid() == false);
			assert_equals(anim.value(), 0.0);
			clock.adjust(1.0);
			assert_equals(anim.value(), 0.1);
			clock.adjust(5.0);
			assert_equals(anim.value(), 0.5);
			clock.adjust(7.0);
			assert_equals(anim.value(), 0.7);
			clock.adjust(10.0);
			assert_equals(anim.value(), 1.0);

			anim.try_animate(2.0, tween(10.0));
			assert(anim.is_valid() == false);
			assert_equals(anim.value(), 1.0);
			clock.adjust(11.0);
			assert_equals(anim.value(), 1.1);
			clock.adjust(15.0);
			assert_equals(anim.value(), 1.5);
			clock.adjust(17.0);
			assert_equals(anim.value(), 1.7);
			clock.adjust(20.0);
			assert_equals(anim.value(), 2.0);
		}
	};

	test t;
}