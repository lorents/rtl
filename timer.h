#pragma once

#include "clock.h"
#include "invalidatable.h"
#include "log.h"

#include <functional>
#include <assert.h>

namespace rtl
{
	class timer : invalidatable
	{
		rtl::clock& clock;
		double timeout;
		double next_time = NAN;
		std::function<void()> listener;

	public:
		timer(const timer&) = delete;

		timer(double timeout, std::function<void()> listener ):
			timer(*rtl::clock::current_clock, timeout, std::move(listener))
		{
		}

		timer(rtl::clock& clock,  double timeout, std::function<void()> listener ):
			clock(clock), timeout( timeout ), listener( listener )
		{
			log::create(this, "timer", 0);
			reset();
		}

		~timer()
		{
			bool is_subscribed = !isnan(next_time);
			if (is_subscribed)
			{
				bool did_unsubscribe = clock.unsubscribe( this );
				assert(did_unsubscribe);
			}
			log::destroy(this, "", 0);
		}

		void reset()
		{
			bool is_subscribed = !isnan(next_time);
			next_time = clock.now() + timeout;
			if (!is_subscribed)
			{
				clock.subscribe( this );
			}
		}

	private:
		void invalidate() override
		{
			if( clock.now() >= next_time )
			{
				bool did_unsubscribe = clock.unsubscribe( this );
				assert(did_unsubscribe);
				next_time = NAN;
				listener();
			}
		}
	};
}