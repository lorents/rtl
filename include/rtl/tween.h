#pragma once

#include "easing.h"

#include <functional>

namespace rtl
{
	class tween
	{
	public:
		double duration;
		easing::function easing;
		double delay;
		std::function<void()> completed;

		tween(
				double duration,
				easing::function easing,
				std::function<void()> completed
		):
				duration(duration),
				easing(easing),
				delay(0.0),
				completed(completed)
		{}

		tween(
			double duration = 0.0,
			easing::function easing = easing::linear,
			double delay = 0.0,
			std::function<void()> completed = nullptr
		):
			duration(duration),
			easing(easing),
			delay(delay),
			completed(completed)
		{}
	};
}
