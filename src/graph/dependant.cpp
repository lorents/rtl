#include <rtl/graph/dependant.h>

#include <rtl/graph/dependency.h>

#include <assert.h>

using namespace rtl;

dependant::dependant(invalidatable* subscriber) : subscriber(subscriber)
{}

dependant::~dependant()
{
	for (dependency* dependency : dependencies)
	{
		bool did_unsubscribe = dependency->unsubscribe(subscriber);
		assert(did_unsubscribe);
	}
}

unsigned int dependant::prepare_mark_and_sweep()
{
	auto flag = dependency::push_marks();

	for (dependency* dep : dependencies)
	{
		dep->mark_as_subscribed();
	}
	
	return flag;
}

void dependant::mark_and_subscribe(dependency* dep)
{
	dep->mark_as_visited();
	if (!dep->is_marked_as_subscribed())
	{
		dep->subscribe(subscriber);
		dep->mark_as_subscribed();
		dependencies.push_back(dep);
	}
}

void dependant::sweep_and_unsubscribe()
{
	int i = 0;
	for (dependency* dep : dependencies)
	{
		if (!dep->is_marked_as_visited())
		{
			assert(dep->is_marked_as_subscribed());
			bool did_unsubscribe = dep->unsubscribe(subscriber);
			assert(did_unsubscribe);
		}
		else
		{
			assert(dep->is_marked_as_subscribed());
			dependencies[i] = dep;
			i++;
		}
		dep->clear_marks();
	}
	dependencies.resize(i);

	dependency::pop_marks();
}

void dependant::unsubscribe_all(unsigned int flag)
{
	int i = 0;
	for (dependency* dep : dependencies)
	{
		bool did_unsubscribe = dep->unsubscribe(subscriber);
		dep->clear_marks( flag );
	}
	dependencies.resize(0);
}
