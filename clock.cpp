#include "clock.h"

#include <assert.h>

namespace rtl
{
	thread_local clock clock::default_clock = {};
	thread_local clock* clock::current_clock = &default_clock;

	clock::clock(): dependency("clock.cpp", 13)
	{
	}

	void clock::adjust(double new_time)
	{
		if( !is_adjusted() )
		{
			last_interval = 0.0;
			current_time = new_time;
			reversed = false;
		}
		else
		{
			if( reversed && new_time > current_time )
			{
				reversed = false;
			}
			else if( !reversed && new_time < current_time )
			{
				reversed = true;
			}
			last_interval = new_time - current_time;
			current_time = new_time;
		}
		dependency.invalidate();
	}

	double clock::now() const
	{
		assert(is_adjusted());
		return current_time;
	}

	double clock::interval() const
	{
		assert(is_adjusted());
		return last_interval;
	}

	bool clock::is_adjusted() const
	{
		return !isnan(current_time);
	}

	bool clock::is_reversed() const
	{
		return reversed;
	}

	void clock::subscribe(invalidatable* subscriber)
	{
		dependency.subscribe(subscriber);
	}

	bool clock::unsubscribe(invalidatable* subscriber)
	{
		return dependency.unsubscribe(subscriber);
	}
}

