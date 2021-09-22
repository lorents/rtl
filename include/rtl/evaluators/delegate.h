#pragma once

#include <rtl/evaluator.h>

#include <rtl/graph/invalidatable.h>
#include <rtl/graph/dependant.h>
#include <rtl/graph/visitor.h>

namespace rtl
{
	template<typename T>
	class delegate : public evaluator<T>, invalidatable
	{
		T current_value;
		bool valid = false;

		invalidatable& dependency;
		dependant dependant;

		std::function<T()> evaluate;

	public:
		delegate(const delegate<T>&) = delete;

		delegate(
				class invalidatable& dependency,
				std::function<T()> evaluate):
				dependency(dependency),
				dependant(this),
				evaluate(evaluate)
		{
			dependency.invalidate();
		}

		const T& value() override { validate(); return current_value; }
		const T& target() override { validate(); return current_value; }
		bool is_valid() override { return valid; }

		bool try_set(const T& value) override
		{
			if (evaluate == nullptr && current_value == value)
			{
				return true;
			}

			evaluate = nullptr;
			current_value = value;
			valid = true;

			dependency.invalidate();
			return true;
		}

		bool try_set( std::function<T()> new_evaluate) override
		{
			evaluate = new_evaluate;
			valid = false;

			dependency.invalidate();
			return true;
		}

	private:

		void* log_id() override
		{
			return &dependency;
		}

		void invalidate() override
		{
			if (!valid) { return; }
			valid = false;

			dependency.invalidate();
		}

		void validate()
		{
			if (valid) { return; }
			valid = true;

			visitor v(&dependant);
			current_value = evaluate();
		}
	};
}