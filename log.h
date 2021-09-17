#pragma once

#include <string>
#include <mutex>
#include <functional>

namespace rtl
{
	struct log_event
	{
		void* subject;
		std::string verb;
		void* object;
		const char* file;
		int line;
	};

	class log_consumer
	{
		friend class log;

		log_consumer* prev_consumer;
		log_consumer* next_consumer;

		static log_consumer* first_consumer;
		static log_consumer* last_consumer;

		std::function<void(log_event)> consume;
	public:
		log_consumer(std::function<void(log_event)> consume) : consume(std::move(consume))
		{
			prev_consumer = last_consumer;
			if(prev_consumer)
			{
				prev_consumer->next_consumer = this;
			}
			next_consumer = nullptr;
			last_consumer = this;
			if(first_consumer == nullptr)
			{
				first_consumer = this;
			}
		}
		~log_consumer()
		{
			if(prev_consumer)
			{
				prev_consumer->next_consumer = next_consumer;
			}
			if(next_consumer)
			{
				next_consumer->prev_consumer = prev_consumer;
			}
			if(first_consumer == this)
			{
				first_consumer = nullptr;
			}
			if(last_consumer == this)
			{
				last_consumer = prev_consumer;
			}
		}
	};

	class log
	{
	public:
		static void event(void* subject, std::string verb, void* object, const char* file, int line)
		{
			for(auto it = log_consumer::first_consumer; it != nullptr; it = it->next_consumer)
			{
				it->consume({ subject, verb, object, file, line });
			}
		}
		static void event(void* subject, std::string verb, const char* file, int line)
		{
			event(subject, verb, nullptr, file, line);
		}
		static void event(std::string verb, const char* file, int line)
		{
			event(nullptr, verb, nullptr, file, line);
		}
		static void create(void* subject, const char* file, int line)
		{
			event(subject, "create", file, line);
		}
		static void destroy(void* subject, const char* file, int line)
		{
			event(subject, "destroy", file, line);
		}
		static void subscribe(void* subject, void* object, const char* file, int line)
		{
			event(subject, "subscribe", object, file, line);
		}
		static void unsubscribe(void* subject, void* object, const char* file, int line)
		{
			event(subject, "unsubscribe", object, file, line);
		}
	};
}