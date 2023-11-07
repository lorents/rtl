#pragma once

#include <memory>

namespace rtl
{
	using std::shared_ptr;

	struct iterator
	{
		shared_ptr<void> data;
	};
}
