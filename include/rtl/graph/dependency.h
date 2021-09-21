#pragma once

#include "invalidatable.h"

#include <vector>
#include <source_location>

namespace rtl
{
	class dependant;

	class dependency : public invalidatable
	{
		unsigned int subscribed = 0;
		unsigned int visited = 0;

		std::vector<invalidatable*> subscribers;
		int invalidating_index = 0;

		thread_local static unsigned int current_flag;

	public:
		dependency(const dependency&) = delete;

		dependency(const std::source_location location = std::source_location::current());
		~dependency();

		static void push_marks();
		static void pop_marks();

		void mark_as_subscribed();
		void mark_as_visited();

		bool is_marked_as_subscribed() const;
		bool is_marked_as_visited() const;

		void clear_marks();

		void subscribe(invalidatable* subscriber, const std::source_location location = std::source_location::current());
		bool unsubscribe(invalidatable* subscriber, const std::source_location location = std::source_location::current());

		void invalidate() override;
	};
}