#include <rtl/log.h>

#include <source_location>

using namespace rtl;

log_consumer* log_consumer::first_consumer = nullptr;
log_consumer* log_consumer::last_consumer = nullptr;

log_consumer::log_consumer(std::function<void(log_event)> consume) : consume(std::move(consume))
{
	prev_consumer = last_consumer;
	if (prev_consumer)
	{
		prev_consumer->next_consumer = this;
	}
	next_consumer = nullptr;
	last_consumer = this;
	if (first_consumer == nullptr)
	{
		first_consumer = this;
	}
}

log_consumer::~log_consumer()
{
	if (prev_consumer)
	{
		prev_consumer->next_consumer = next_consumer;
	}
	if (next_consumer)
	{
		next_consumer->prev_consumer = prev_consumer;
	}
	if (first_consumer == this)
	{
		first_consumer = nullptr;
	}
	if (last_consumer == this)
	{
		last_consumer = prev_consumer;
	}
}

void log::event(void* subject, std::string verb, void* object, const std::source_location location)
{
	for (auto it = log_consumer::first_consumer; it != nullptr; it = it->next_consumer)
	{
		it->consume({ subject, verb, object, location });
	}
}
