
#include "../dependant.h"
#include "../dependency.h"
#include "../dependency_visitor.h"
#include "../invalidator.h"

#include "test.h"

using namespace rtl;

namespace
{
	class test : public rtl::test
	{
		void run() override
		{
			dependency dependency1;
			dependency dependency2;
			dependency dependency3;

			int counter = 0;
			invalidator inv([&]() { assert(counter-- > 0);  });
			dependant dependant(&inv);

			{
				dependency_visitor v(&dependant);
				dependency_visitor::visit(&dependency1);
				dependency_visitor::visit(&dependency2);
			}

			counter = 1;
			dependency1.invalidate();
			assert(counter == 0);

			counter = 1;
			dependency2.invalidate();
			assert(counter == 0);

			{
				dependency_visitor v(&dependant);
				dependency_visitor::visit(&dependency2);
				dependency_visitor::visit(&dependency3);
			}

			counter = 0;
			dependency1.invalidate();
			assert(counter == 0);

			counter = 1;
			dependency2.invalidate();
			assert(counter == 0);

			counter = 1;
			dependency3.invalidate();
			assert(counter == 0);

		}
	};

	test t;
}
