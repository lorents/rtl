#pragma once

namespace rtl
{
	class dependant;
	class dependency;

	class dependency_visitor
	{
		thread_local static dependant* current_visitor;

		dependant* previous_visitor;

		dependant* visitor; // only for debugging
	public:
		dependency_visitor(const dependency_visitor&) = delete;
		dependency_visitor(dependant* visitor);
		~dependency_visitor();

		static void visit(dependency* visited);
	};
}
