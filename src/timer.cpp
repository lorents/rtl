#include <rtl/timer.h>

#include <rtl/clock.h>
#include <rtl/log.h>

#include <cassert>
#include <cmath>

using namespace rtl;

timer::timer(double timeout, std::function<void()> listener) :
	timer(*rtl::clock::current_clock, timeout, std::move(listener))
{
}

timer::timer(rtl::clock& clock, double timeout, std::function<void()> listener) :
	clock(clock), timeout(timeout), listener(listener)
{
	log::create(this);
	reset();
}

timer::~timer()
{
	bool is_subscribed = !std::isnan(next_time);
	if (is_subscribed)
	{
		bool did_unsubscribe = clock.unsubscribe(this);
		assert(did_unsubscribe);
	}
	log::destroy(this);
}

void timer::reset()
{
	bool is_subscribed = !std::isnan(next_time);
	next_time = clock.now() + timeout;
	if (!is_subscribed)
	{
		clock.subscribe(this);
	}
}

void timer::invalidate()
{
	if (clock.now() >= next_time)
	{
		bool did_unsubscribe = clock.unsubscribe(this);
		assert(did_unsubscribe);
		next_time = NAN;
		listener();
	}
}