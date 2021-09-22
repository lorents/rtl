#pragma once

#include <rtl/evaluator.h>

#include <rtl/graph/invalidatable.h>

namespace rtl
{
	template<typename T>
	class constant : public evaluator<T>
	{
		invalidatable& dependency;

		T constant_value;
	public:
		constant( const constant<T>& ) = delete;

		constant(rtl::invalidatable& dependency, const T& value) : dependency(dependency), constant_value(value)
		{
			dependency.invalidate();
		}

		const T& value() override { return constant_value; }
		const T& target() override { return constant_value; }
		bool is_valid() override { return true; }

		bool try_set(const T& value) override
		{
			if( constant_value == value )
			{
				return true;
			}

			constant_value = value;
			dependency.invalidate();
			return true;
		}
	};
}

