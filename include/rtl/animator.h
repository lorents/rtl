#pragma once

#include "graph/dependant.h"

#include "log.h"
#include "clock.h"
#include "invalidator.h"

#include <functional>
#include <mutex>

namespace rtl
{
	class static_animator : public invalidatable
	{
		clock& clock;

		enum { invalid, valid, settled } state;

		std::function<void()> handler;

		invalidator call_validate;

	public:
		static_animator( const static_animator& ) = delete;

		static_animator(std::function<void()> handler);
		static_animator(rtl::clock& clock, std::function<void()> handler);

		~static_animator();

	protected:
		dependant dependant;

		virtual void invalidate() override;
		virtual void validate();
	};


	class animator : static_animator
	{
	public:
		animator( const animator& ) = delete;

		animator(std::function<void()> handler);
		animator(rtl::clock& clock, std::function<void()> handler);
	};
}
