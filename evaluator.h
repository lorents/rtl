#pragma once

#include "tween.h"

#include <functional>

namespace rtl
{
	template<typename T>
	class evaluator
	{
	public:
		virtual ~evaluator() = default;

		virtual const T& value() = 0;
		virtual const T& target() = 0;
		virtual bool is_valid() = 0;

		virtual bool try_set(const T&) { return false; }
		virtual bool try_set(std::function<T()>) { return false; }
		virtual bool try_animate(const T&, const tween&) { return false; }
		virtual bool try_retarget(const T& value) { return try_set(value); }
	};
}