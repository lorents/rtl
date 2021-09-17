#pragma once

#include <vector>

namespace rtl
{
	class invalidatable;
	class dependant;

	class dependency
	{
		unsigned int subscribed = 0;
		unsigned int visited = 0;

		std::vector<invalidatable*> subscribers;
		int invalidating_index = 0;

		thread_local static unsigned int current_flag;

	public:
		static void push_marks();
		static void pop_marks();

		void mark_as_subscribed();
		void mark_as_visited();

		bool is_marked_as_subscribed() const;
		bool is_marked_as_visited() const;

		void clear_marks();

		dependency(const dependency&) = delete;
		dependency(const char* file = "", int line = 0);
		~dependency();

		void subscribe(invalidatable* subscriber);
		bool unsubscribe(invalidatable* subscriber);

		void invalidate();
	};
}