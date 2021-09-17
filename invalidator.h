#pragma once

#include "invalidatable.h"

#include <functional>

namespace rtl
{
	class invalidator : public invalidatable
	{
		std::function<void()> handler;
	public:
		invalidator(const invalidator&) = delete;

		invalidator(std::function<void()> handler) :
			handler(std::move(handler))
		{}

		void invalidate() override
		{
			handler();
		}
	};
}