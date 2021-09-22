#pragma once

#include <assert.h>
#include <map>
#include <string>
#include <memory>

#ifdef RTL_USE_SOURCE_LOCATION
#include <source_location>
#else
#include <sstream>
#endif

namespace rtl
{
	class test
	{
	public:
		static std::unique_ptr<std::map<std::string, test*>> tests;

		test(
#ifdef RTL_USE_SOURCE_LOCATION
			const std::source_location location = std::source_location::current()
#endif
		)
		{
			if (!tests) tests = std::make_unique<std::map<std::string, test*>>();
#ifdef RTL_USE_SOURCE_LOCATION
			(*tests)[location.file_name()] = this;
#else
			const void * address = static_cast<const void*>(this);
			std::stringstream ss;
			ss << address;
			(*tests)[ss.str()] = this;
#endif
		}

		virtual void run() {}

		virtual ~test() = default;


		void assert_equals(double lhs, double rhs)
		{
			assert(std::abs(lhs - rhs) < 0.0001);
		}
	};
}