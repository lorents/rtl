#pragma once

#include "dependency.h"

#include <cmath>
#include <mutex>

namespace rtl
{
	class invalidatable;

	class clock
	{
		dependency dependency;
		double current_time = NAN;
		double last_interval = NAN;
		bool reversed = false;
	public:
		thread_local static clock* current_clock;

		thread_local static clock default_clock;

		clock( const clock& ) = delete;
		clock();

		void adjust(double new_time);

		double now() const;
		double interval() const;

		bool is_adjusted() const;
		bool is_reversed() const;

		void subscribe(invalidatable* subscriber);
		bool unsubscribe(invalidatable* subscriber);

	};
}
