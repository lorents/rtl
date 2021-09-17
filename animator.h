#pragma once

#include "dependant.h"
#include "clock.h"

#include "invalidator.h"

#include <functional>
#include <mutex>
#include "log.h"

namespace rtl
{
	class clock;

	class static_animator : public invalidatable
	{
		clock& clock;

		dependant dependant;

		enum { invalid, valid, settled } state;

		std::function<void()> handler;

		invalidator call_validate;

	public:
		static_animator( const static_animator& ) = delete;

		static_animator(std::function<void()> handler);
		static_animator(rtl::clock& clock, std::function<void()> handler);

		~static_animator();

	protected:
		virtual void invalidate() override;
		virtual void validate();

		virtual void call_handler(rtl::dependant& dependant);
	};


	class animator : static_animator
	{
	public:
		animator( const animator& ) = delete;

		animator(std::function<void()> handler);
		animator(rtl::clock& clock, std::function<void()> handler);

	protected:
		virtual void call_handler(rtl::dependant& dependant) override;
	};
}
