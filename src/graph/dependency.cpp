#include <rtl/graph/dependency.h>
#include <rtl/graph/visitor.h>

#include <rtl/graph/invalidatable.h>

#include <rtl/log.h>

#include <assert.h>

using namespace rtl;

thread_local unsigned int dependency::current_flag = 1u;


dependency::dependency(
#ifdef RTL_USE_SOURCE_LOCATION
	const std::source_location location
#endif
)
{
	log::create(this
#ifdef RTL_USE_SOURCE_LOCATION
		, location
#endif
	);
}

dependency::~dependency()
{
	log::destroy(this);
	// mark and sweep will fail if we go away in the meantime
	assert(subscribers.size() == 0);
}

unsigned int dependency::push_marks()
{
	current_flag <<= 1u;
	return current_flag;
}
void dependency::pop_marks()
{
	current_flag >>= 1u;
}

void dependency::mark_as_subscribed()
{
	subscribed |= current_flag;
}

void dependency::mark_as_visited()
{
	visited |= current_flag;
}

bool dependency::is_marked_as_subscribed() const
{
	return subscribed & current_flag;
}

bool dependency::is_marked_as_visited() const
{
	return visited & current_flag;
}

void dependency::clear_marks()
{
	clear_marks(current_flag);
}

void dependency::clear_marks(unsigned int flag)
{
	subscribed &= ~flag;
	visited &= ~flag;
}


void dependency::subscribe(
	invalidatable* subscriber
#ifdef RTL_USE_SOURCE_LOCATION
	, const std::source_location location
#endif
)
{
	log::subscribe(this, subscriber->log_id()
#ifdef RTL_USE_SOURCE_LOCATION
		, location
#endif
	);
	assert(std::find(subscribers.begin(), subscribers.end(), subscriber) == subscribers.end());
	subscribers.push_back(subscriber);
}

bool dependency::unsubscribe(
		invalidatable* subscriber
#ifdef RTL_USE_SOURCE_LOCATION
		, const std::source_location location
#endif
)
{
	log::unsubscribe(this, subscriber->log_id()
#ifdef RTL_USE_SOURCE_LOCATION
		, location
#endif
	);
	auto it = std::find(subscribers.begin(), subscribers.end(), subscriber);
	if (it == subscribers.end())
	{
		return false;
	}

	bool offsets_iteration = (it - subscribers.begin()) <= invalidating_index;
	if (offsets_iteration)
	{
		next_invalidating_index = invalidating_index;
	}

	subscribers.erase(it);
	return true;
}

void dependency::invalidate()
{
	for (invalidating_index = 0; invalidating_index < subscribers.size(); invalidating_index = next_invalidating_index)
	{
		next_invalidating_index = invalidating_index + 1;
		subscribers[invalidating_index]->invalidate(); // unsubscribe may alter next_invalidating_index
	}
}

void dependency::visit() const
{
	visitor::visit(const_cast<rtl::dependency*>(this));
}
