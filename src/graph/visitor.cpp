#include <rtl/graph/visitor.h>

#include <rtl/graph/dependant.h>
#include <rtl/graph/dependency.h>

#include <assert.h>

using namespace rtl;

thread_local dependant* visitor::current_visitor = nullptr;

visitor::visitor(dependant* visitor) :
	previous_visitor(current_visitor)
{
	assert(visitor);
	current_visitor = visitor;
	current_visitor->prepare_mark_and_sweep();
	assert(current_visitor);
}

visitor::~visitor()
{
	current_visitor->sweep_and_unsubscribe();
	current_visitor = previous_visitor;
}

void visitor::visit(dependency* visited)
{
	if (current_visitor)
		current_visitor->mark_and_subscribe(visited);
}
