#pragma once

#include <assert.h>
#include <source_location>
#include <map>
#include <string>
#include <memory>

namespace rtl
{
	class test
	{
	public:
		static std::unique_ptr<std::map<std::string, test*>> tests;

		test(const std::source_location location = std::source_location::current()) 
		{
			if (!tests) tests = std::make_unique<std::map<std::string, test*>>();
			(*tests)[location.file_name()] = this;
		}

		virtual void run() {}

		virtual ~test() = default;


		void assert_equals(double lhs, double rhs)
		{
			assert(std::abs(lhs - rhs) < 0.0001);
		}
	};
}