
#include "../dependant.h"
#include "../dependency.h"
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

			dependant.prepare_mark_and_sweep();

			dependant.mark_and_subscribe(&dependency1);
			dependant.mark_and_subscribe(&dependency2);

			dependant.sweep_and_unsubscribe();

			counter = 1;
			dependency1.invalidate();
			assert(counter == 0);

			counter = 1;
			dependency2.invalidate();
			assert(counter == 0);

			dependant.prepare_mark_and_sweep();

			dependant.mark_and_subscribe(&dependency2);
			dependant.mark_and_subscribe(&dependency3);

			dependant.sweep_and_unsubscribe();

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
