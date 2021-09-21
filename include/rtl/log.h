#pragma once

#include <string>
#include <functional>
#include <source_location>

namespace rtl
{
	struct log_event
	{
		void* subject;
		std::string verb;
		void* object;
		std::source_location location;
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
		log_consumer(std::function<void(log_event)> consume);
		~log_consumer();
	};

	class log
	{
	public:
		static void event(void* subject, std::string verb, void* object, const std::source_location location = std::source_location::current());

		static inline void event(void* subject, std::string verb, const std::source_location location = std::source_location::current())
		{
			event(subject, verb, nullptr, location);
		}
		static inline  void event(std::string verb, const std::source_location location = std::source_location::current())
		{
			event(nullptr, verb, nullptr, location);
		}
		static inline  void create(void* subject, const std::source_location location = std::source_location::current())
		{
			event(subject, "create", location);
		}
		static inline  void destroy(void* subject, const std::source_location location = std::source_location::current())
		{
			event(subject, "destroy", location);
		}
		static inline  void subscribe(void* subject, void* object, const std::source_location location = std::source_location::current())
		{
			event(subject, "subscribe", object, location);
		}
		static inline  void unsubscribe(void* subject, void* object, const std::source_location location = std::source_location::current())
		{
			event(subject, "unsubscribe", object, location);
		}
		static inline void populate(void* subject, void* object, const std::source_location location = std::source_location::current())
		{
			event(subject, "populate", object, location);
		}
		static inline void depopulate(void* subject, void* object, const std::source_location location = std::source_location::current())
		{
			event(subject, "depopulate", object, location);
		}
		static inline void evaluate(void* subject, const std::source_location location = std::source_location::current())
		{
			event(subject, "evaluate", location);
		}
		static inline void invalidate(void* subject, const std::source_location location = std::source_location::current())
		{
			event(subject, "invalidate", location);
		}
	};
}