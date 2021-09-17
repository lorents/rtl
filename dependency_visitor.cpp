#include "dependency_visitor.h"

#include "dependant.h"
#include "dependency.h"

#include <assert.h>

using namespace rtl;

thread_local dependant* dependency_visitor::current_visitor = nullptr;

dependency_visitor::dependency_visitor(dependant* visitor) :
	visitor(visitor),
	previous_visitor(current_visitor)
{
	assert(visitor);
	current_visitor = visitor;
	current_visitor->prepare_mark_and_sweep();
	assert(current_visitor);
}

dependency_visitor::~dependency_visitor()
{
	assert(current_visitor==visitor);
	current_visitor->sweep_and_unsubscribe();
	current_visitor = previous_visitor;
}

void dependency_visitor::visit(dependency* visited)
{
	if (current_visitor)
		current_visitor->mark_and_subscribe(visited);
}
