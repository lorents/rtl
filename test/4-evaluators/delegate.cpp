#include <rtl/evaluators/delegate.h>

#include <rtl/graph/dependency.h>

#include "../test.h"

void rtl::tests::delegate_test()
{
	dependency dep;
	delegate<double> del(dep, []() { return 13.37; });
	assert_equals(del.value(), 13.37);
	assert_equals(del.target(), 13.37);
}
