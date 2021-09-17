
#include "animated.h"
#include "animator.h"
#include "clock.h"
#include "constant.h"
#include "delegate.h"
#include "dependant.h"
#include "dependency.h"
#include "dependency_visitor.h"
#include "easing.h"
#include "evaluator.h"
#include "invalidator.h"
#include "tween.h"
#include "var.h"


#include <memory>
#include <map>
#include <string>
#include <assert.h>

using std::map;
using std::string;
using std::unique_ptr;
using std::make_unique;

namespace rtl
{
	class test
	{
	public:
		virtual void run() {}

		virtual ~test() = default;


		void assert_equals(double lhs, double rhs)
		{
			assert(std::abs(lhs - rhs) < 0.0001);
		}
	};

	class dependency_test : public test
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


	class dependant_test : public test
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


	class dependency_visitor_test : public test
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

	class clock_test : public test
	{
		void run() override
		{
			clock clock;
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

	class animated_test : public test
	{
		void run() override
		{
			clock clock;
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

	class constant_test : public test
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

	class delegate_test : public test
	{
		void run() override
		{
			dependency dep;
			delegate<double> del(dep, []() { return 13.37; });
			assert_equals(del.value(), 13.37);
			assert_equals(del.target(), 13.37);
		}
	};

	class var_test : public test
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

	class animator_test : public test
	{
		void run() override
		{
			animator anim([]() 
			{
				// something
			});
		}
	};
	
	void tests_main(int argc, const char** argv)
	{
		map<string, unique_ptr<test>> tests;

		tests["dependency"]         = make_unique<dependency_test >();
		tests["dependant"]          = make_unique<dependant_test >();
		tests["dependency_visitor"] = make_unique<dependency_visitor_test >();
		tests["clock"]              = make_unique<clock_test >();     
		tests["animated"]           = make_unique<animated_test>();
		tests["constant"]           = make_unique<constant_test >();
		tests["delegate"]           = make_unique<delegate_test >();
		tests["var"]                = make_unique<var_test>();
		tests["animator"]           = make_unique<animator_test>();

		if (argc == 0)
		{
			for (auto it = tests.begin(); it != tests.end(); ++it)
			{
				it->second->run();
			}
		}
		else for (int i = 1; i < argc; i++)
		{
			tests[argv[i]]->run();
		}
	}

	void run_tests()
	{
		tests_main(0,nullptr);
	}
}
