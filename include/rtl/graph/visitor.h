#pragma once

namespace rtl
{
	class dependant;
	class dependency;

	class visitor
	{
		thread_local static dependant* current_visitor;

		unsigned int our_flag;
		dependant* our_visitor;
		dependant* previous_visitor;

		bool canceled = false;
	public:
		visitor(const visitor&) = delete;

		visitor(dependant* visitor);
		~visitor();

		void cancel();
		
		bool was_canceled() const;

		static void visit(dependency* visited);
	};

	/**
	 * A guard to prevent current visitor from visiting external code.
	 * Functionally the same as putting a dummy visitor on the stack.
	 */
	class do_not_disturb : visitor
	{
	public:
		do_not_disturb(): visitor(nullptr) {}
	};
}
