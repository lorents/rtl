#pragma once

#include "dependency.h"
#include "dependency_visitor.h"

#include "evaluator.h"

#include "constant.h"
#include "animated.h"
#include "delegate.h"

#include "tween.h"
#include "easing.h"

#include "log.h"

#include <functional>
#include <memory>
#include <assert.h>

namespace rtl
{
	template<typename T>
	class var
	{
		dependency dependency;

		std::unique_ptr<evaluator<T>> evaluator;

	public:
		var(const var<T>&) = delete;

		var(): var(T()) {}

		var(const T& value): dependency("", 0), evaluator(std::make_unique<constant<T>>(dependency, value)) {}
		var(std::function<T()> evaluate): dependency("", 0), evaluator(std::make_unique<delegate<T>>(dependency, evaluate)) {}

		const T& operator=(const T& new_value)
		{
			if (!evaluator->try_set(new_value))
			{
				evaluator = std::make_unique<constant<T>>(dependency, new_value);
			}
			return new_value;
		}

		void operator=(std::function<T()> new_evaluate)
		{
			if (!evaluator->try_set(new_evaluate))
			{
				evaluator = std::make_unique<delegate<T>>(dependency, new_evaluate);
			}
		}

		void animate(const T& target, double duration, std::function<void()> complete = nullptr)
		{
			animate(*rtl::clock::current_clock, target, {duration, easing::default_easing, 0.0, std::move(complete) });
		}

		void animate(const T& target, double duration, easing::function easing, std::function<void()> complete = nullptr)
		{
			animate(*rtl::clock::current_clock, target, {duration, easing, 0.0, std::move(complete) });
		}

		void animate(const T& target, double duration, easing::function easing, double delay, std::function<void()> complete = nullptr)
		{
			animate(*rtl::clock::current_clock, target, {duration, easing, delay, std::move(complete) });
		}

		void animate(const T& target, tween tween)
		{
			animate(*clock::current_clock, target, std::move(tween));
		}

		void animate(rtl::clock& clock, const T& target, double duration, std::function<void()> complete = nullptr)
		{
			animate(clock, target, { duration, easing::default_easing, 0.0, std::move(complete) });
		}

		void animate(rtl::clock& clock, const T& target, double duration, easing::function easing, std::function<void()> complete = nullptr)
		{
			animate(clock, target, { duration, easing, 0.0, std::move(complete) });
		}

		void animate(rtl::clock& clock, const T& target, double duration, easing::function easing, double delay, std::function<void()> complete = nullptr)
		{
			animate(clock, target, { duration, easing, delay, std::move(complete) });
		}

		void animate(rtl::clock& clock, const T& target, tween tween)
		{
			if (tween.duration == 0.0 && tween.delay == 0.0)
			{
				*this = target;
				return;
			}

			assert(clock.is_adjusted());
			if (clock.is_reversed())
			{
				tween.duration = -tween.duration;
				tween.delay = -tween.delay;
			}
			if (!evaluator->try_animate(target, tween))
			{
				evaluator = std::make_unique<animated<T>>(dependency, clock, evaluator->value(), target, std::move(tween));
			}
		}

		void retarget(const T& target)
		{
			if(!evaluator->try_retarget(target))
			{
				evaluator = std::make_unique<constant<T>>(dependency, target);
			}
		}

		const T& target() const
		{
			return evaluator->target();
		}

		bool is_valid() const
		{
			return evaluator->is_valid();
		}

		operator const T& () const
		{
			return operator()();
		}

		const T& operator()() const
		{
			dependency_visitor::visit(const_cast<rtl::dependency*>(&dependency));
			return evaluator->value();
		}

		const T& peek() const
		{
			return evaluator->value();
		}

		void subscribe(invalidatable* subscriber)
		{
			dependency.subscribe(subscriber);
		}

		bool unsubscribe(invalidatable* subscriber)
		{
			return dependency.unsubscribe(subscriber);
		}
	};
}

