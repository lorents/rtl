#include <rtl/evaluators/constant.h>

#include <rtl/graph/dependency.h>

#include "../test.h"

void rtl::tests::constant_test()
{
	dependency dep;
	constant<double> con(dep, 13.37);
	assert_equals(con.value(), 13.37);
	assert_equals(con.target(), 13.37);
	assert(con.try_set(777));
	assert_equals(con.value(), 777);
}
