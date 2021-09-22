#pragma once

namespace rtl
{
	class dependant;
	class dependency;

	class visitor
	{
		thread_local static dependant* current_visitor;

		dependant* previous_visitor;

	public:
		visitor(const visitor&) = delete;

		visitor(dependant* visitor);
		~visitor();

		static void visit(dependency* visited);
	};
}
