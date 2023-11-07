#pragma once

#include <memory>

namespace rtl
{
	using std::shared_ptr;
	
	template<typename T>
	struct event
	{
		uint64_t timestamp;
		enum { insert, remove, idle } type;
		size_t index;
		shared_ptr<T> item;
		
		bool is_insert() const { return type == insert; }
		bool is_remove() const { return type == remove; }
	};
	
}
