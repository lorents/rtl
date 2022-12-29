#include <rtl/graph/visitor.h>

#include <rtl/graph/dependant.h>
#include <rtl/graph/dependency.h>

#include <assert.h>

using namespace rtl;

thread_local dependant* visitor::current_visitor = nullptr;

visitor::visitor(dependant* visitor) :
	our_visitor(visitor),
	previous_visitor(current_visitor)
{
	current_visitor = our_visitor;
	if (our_visitor)
	{
		our_flag = our_visitor->prepare_mark_and_sweep();
	}
}

visitor::~visitor()
{
	if (current_visitor)
	{
		if (canceled)
		{
			dependency::pop_marks();
		}
		else
		{
			current_visitor->sweep_and_unsubscribe();
		}
	}
	current_visitor = previous_visitor;
}

void visitor::cancel()
{
	canceled = true;
	if (our_visitor)
	{
		our_visitor->unsubscribe_all(our_flag);
	}
}

bool visitor::was_canceled() const
{
	return canceled;
}

void visitor::visit(dependency* visited)
{
	if (current_visitor)
		current_visitor->mark_and_subscribe(visited);
}
