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
		bool is_subscribed_to_clock;

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
				is_subscribed_to_clock = true;
				clock.subscribe(this);
			}
			else
			{
				is_subscribed_to_clock = false;
			}

			dependency.invalidate();
		}

		~animated()
		{
			if (is_subscribed_to_clock)
			{
				bool did_unsubscribe = clock.unsubscribe(this);
				assert(did_unsubscribe);
			}
		}

		const T& value() override { validate(); return current_value; }
		const T& target() override { return target_value; }
		bool is_valid() override { return idle; }

		bool try_set(const T& value) override
		{
			if( !is_subscribed_to_clock && idle && current_value == value )
			{
				return true;
			}

			tween = {};

			current_value = target_value = value;
			idle = true;

			if (is_subscribed_to_clock)
			{
				bool did_unsubscribe = clock.unsubscribe(this);
				assert(did_unsubscribe);
				is_subscribed_to_clock = false;
			}

			dependency.invalidate();
			return true;
		}

		bool try_animate(const T& target, const rtl::tween& new_tween) override
		{
			bool should_subscribe_to_clock = new_tween.delay != 0.0;

			tween = new_tween;

			initial_value = current_value;
			target_value = target;
			started = clock.now() + tween.delay;

			idle = should_subscribe_to_clock;

			if (is_subscribed_to_clock && !should_subscribe_to_clock)
			{
				bool did_unsubscribe = clock.unsubscribe(this);
				assert(did_unsubscribe);
				is_subscribed_to_clock = false;
			}
			else if(!is_subscribed_to_clock && should_subscribe_to_clock)
			{
				is_subscribed_to_clock = true;
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
			if (clock.now() - started > tween.duration)
			{
				bool did_unsubscribe = clock.unsubscribe(this);
				assert(did_unsubscribe);
				is_subscribed_to_clock = false;
				auto continuation = std::move(tween.completed);
				if(continuation) continuation();
			}
			else if (clock.now() >= started )
			{
				bool did_unsubscribe = clock.unsubscribe(this);
				assert(did_unsubscribe);
				is_subscribed_to_clock = false;
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
				is_subscribed_to_clock = true;
				clock.subscribe(this);
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
