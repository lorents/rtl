#pragma once

#include "var.h"
#include "tween.h"

namespace rtl
{
	template<typename T>
	class ref
	{
		struct function_pair
		{
			std::function<T()> getter;
			std::function<void(T,tween)> setter;
		};

		void* var_or_function_pair;

		bool has_var : 1;
		bool owns_var : 1;

	public:
		ref( var<T>& target )
		{
			has_var = true;
			owns_var = false;
			var_or_function_pair = &target;
		}

		ref( const T& value )
		{
			has_var = true;
			owns_var = true;
			var_or_function_pair = new var<T>(value);
		}

		ref(std::function<T()> getter, std::function<void(T,tween)> setter)
		{
			has_var = false;
			var_or_function_pair = new function_pair{ std::move(getter), std::move(setter) };
		}

		ref( ref<T>&& rhs)
		{
			has_var = rhs.has_var;
			owns_var = rhs.owns_var;
			var_or_function_pair = rhs.var_or_function_pair;

			// prevent deletion of adopted pointer
			rhs.has_var = true;
			rhs.owns_var = false;
		}

		ref(const ref<T>&) = delete;

		~ref()
		{
			if(!has_var)
				delete reinterpret_cast<function_pair*>(var_or_function_pair);
			else if(owns_var)
				delete reinterpret_cast<var<T>*>(var_or_function_pair);
		}

		void operator=(const T& new_value)
		{
			if(has_var)
				reinterpret_cast<var<T>*>(var_or_function_pair)->operator=(new_value);
			else
				reinterpret_cast<function_pair*>(var_or_function_pair)->setter(new_value, tween());
		}

		void animate(const T& target, double duration, std::function<void()> complete = nullptr)
		{
			animate(target, {duration, easing::default_easing, 0.0, std::move(complete) });
		}

		void animate(const T& target, double duration, easing::function easing, std::function<void()> complete = nullptr)
		{
			animate(target, {duration, easing, 0.0, std::move(complete) });
		}

		void animate(const T& target, double duration, easing::function easing, double delay, std::function<void()> complete = nullptr)
		{
			animate(target, {duration, easing, delay, std::move(complete) });
		}

		void animate(const T& target, tween tween)
		{
			if(has_var)
				reinterpret_cast<var<T>*>(var_or_function_pair)->animate(target, tween);
			else
				reinterpret_cast<function_pair*>(var_or_function_pair)->setter(target, tween);
		}

		operator const T& () const
		{
			return operator()();
		}

		T operator()() const
		{
			return has_var
				? reinterpret_cast<var<T>*>(var_or_function_pair)->operator()()
				: reinterpret_cast<function_pair*>(var_or_function_pair)->getter();
		}
	};
}
