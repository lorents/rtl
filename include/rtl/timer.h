#pragma once

#include "graph/invalidatable.h"

#include <functional>

namespace rtl
{
	class clock;

	class timer : invalidatable
	{
		rtl::clock& clock;
		double timeout;
		double next_time = std::numeric_limits<double>::quiet_NaN();
		std::function<void()> listener;

	public:
		timer(const timer&) = delete;

		timer(double timeout, std::function<void()> listener);
		timer(rtl::clock& clock, double timeout, std::function<void()> listener);

		~timer();

		void reset();

	private:
		void invalidate() override;
	};
}