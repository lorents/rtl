#pragma once

#include <rtl/evaluator.h>

#include <rtl/graph/invalidatable.h>

#include <rtl/clock.h>
#include <rtl/tween.h>

namespace rtl
{
	template<typename T>
	class animated : public evaluator<T>, invalidatable
	{
		T current_value;
		bool idle;

		T initial_value;
		T target_value;
		tween tween;
		double started;

		invalidatable& dependency;
		clock& clock;

	public:
		animated( const animated& ) = delete;

		animated(
				class invalidatable& dependency,
				class clock& clock,
				const T& initial,
				const T& target,
				rtl::tween tween
		):
				dependency( dependency ),
				clock( clock ),
				current_value(initial),
				initial_value(initial),
				target_value(target),
				tween(std::move(tween))
		{
			started = clock.now() + tween.delay;
			idle = false;

			if (tween.delay != 0.0)
			{
				idle = true;
				clock.subscribe(this);
			}

			dependency.invalidate();
		}

		~animated()
		{
			bool is_subscribed_to_clock = tween.delay != 0.0;

			if (is_subscribed_to_clock)
			{
				clock.unsubscribe(this);
			}
		}

		const T& value() override { validate(); return current_value; }
		const T& target() override { return target_value; }
		bool is_valid() override { return idle; }

		bool try_set(const T& value) override
		{
			bool is_subscribed_to_clock = tween.delay != 0.0;

			if( !is_subscribed_to_clock && idle && current_value == value )
			{
				return true;
			}

			tween = {};

			current_value = target_value = value;
			idle = true;

			if (is_subscribed_to_clock)
			{
				clock.unsubscribe(this);
			}

			dependency.invalidate();
			return true;
		}

		bool try_animate(const T& target, const rtl::tween& new_tween) override
		{
			bool is_subscribed_to_clock = tween.delay != 0.0;
			bool should_subscribe_to_clock = new_tween.delay != 0.0;

			tween = new_tween;

			initial_value = current_value;
			target_value = target;
			started = clock.now() + tween.delay;

			idle = should_subscribe_to_clock;

			if (is_subscribed_to_clock && !should_subscribe_to_clock)
			{
				clock.unsubscribe(this);
			}
			else if(!is_subscribed_to_clock && should_subscribe_to_clock)
			{
				clock.subscribe(this);
			}

			dependency.invalidate();
			return true;
		}

		bool try_retarget(const T& target) override
		{
			(idle ? current_value : target_value) = target;
			dependency.invalidate();
			return true;
		}

	private:
		void invalidate() override
		{
			if (clock.now() >= started )
			{
				clock.unsubscribe(this);
				tween.delay = 0.0;
				idle = false;
				dependency.invalidate();
			}
		}

		void validate()
		{
			if (idle) { return; }

			if (clock.now() - started > tween.duration)
			{
				current_value = target_value;
				idle = true;
				if(tween.completed) tween.completed();
				tween.completed = nullptr;
			}
			else
			{
				current_value = evaluate(clock.now());
				idle = false;
				dependency.invalidate();
			}
		}

		T evaluate(double t)
		{
			return lerp(initial_value, target_value, tween.easing(float((t - started) / tween.duration)));
		}

		static T lerp( T lower, T upper, float weight )
		{
			return lower + static_cast< T >(( upper - lower ) * weight );
		}
	};
}
