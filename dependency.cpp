#include "dependency.h"
#include "invalidatable.h"

#include "log.h"

#include <assert.h>

using namespace rtl;

thread_local unsigned int dependency::current_flag = 1u;

void dependency::push_marks()
{
	current_flag <<= 1u;
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
	subscribed &= ~current_flag;
	visited &= ~current_flag;
}

dependency::dependency(const char* file, int line)
{
	log::create(this, file, line);
}

dependency::~dependency()
{
	log::destroy(this, "", 0);
	// mark and sweep will fail if we go away in the meantime
	assert(subscribers.size() == 0);
}

void dependency::subscribe(invalidatable* subscriber)
{
	log::subscribe(this, subscriber->log_id(), "", 0);
	assert(std::find(subscribers.begin(), subscribers.end(), subscriber) == subscribers.end());
	subscribers.push_back(subscriber);
}

bool dependency::unsubscribe(invalidatable* subscriber)
{
	log::unsubscribe(this, subscriber->log_id(), "", 0);
	auto it = std::find(subscribers.begin(), subscribers.end(), subscriber);
	if (it == subscribers.end())
	{
		return false;
	}

	bool offsets_iteration = it <= subscribers.begin() + invalidating_index;
	if (offsets_iteration)
	{
		invalidating_index--;
	}

	subscribers.erase(it);
	return true;
}

void dependency::invalidate()
{
	for (invalidating_index = 0; invalidating_index < subscribers.size(); ++invalidating_index)
	{
		subscribers[invalidating_index]->invalidate();
	}
}
