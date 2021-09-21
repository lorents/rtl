#include <rtl/graph/dependency.h>

#include "../test.h"

using namespace rtl;

namespace
{
	class test : public rtl::test
	{
		void run() override
		{
			dependency dependency;

			dependency::push_marks();
			{
				dependency.mark_as_subscribed();

				dependency::push_marks();
				{
					dependency.mark_as_visited();

					dependency::push_marks();
					{
						dependency.mark_as_visited();
						dependency.mark_as_subscribed();

						dependency::pop_marks();
					}

					dependency::pop_marks();
				}

				assert(dependency.is_marked_as_subscribed());
				assert(!dependency.is_marked_as_visited());

				dependency::push_marks();
				{
					assert(!dependency.is_marked_as_subscribed());
					assert(dependency.is_marked_as_visited());

					dependency::push_marks();
					{
						assert(dependency.is_marked_as_subscribed());
						assert(dependency.is_marked_as_visited());

						dependency.clear_marks();
						assert(!dependency.is_marked_as_subscribed());
						assert(!dependency.is_marked_as_visited());

						dependency::pop_marks();
					}
					assert(!dependency.is_marked_as_subscribed());
					assert(dependency.is_marked_as_visited());

					dependency.clear_marks();
					assert(!dependency.is_marked_as_subscribed());
					assert(!dependency.is_marked_as_visited());

					dependency::pop_marks();
				}

				assert(dependency.is_marked_as_subscribed());
				assert(!dependency.is_marked_as_visited());

				dependency.clear_marks();
				assert(!dependency.is_marked_as_subscribed());
				assert(!dependency.is_marked_as_visited());

				dependency::pop_marks();
			}
		}
	};

	test t;
}