#pragma once

namespace rtl
{
	class invalidatable
	{
	public:
		virtual void* log_id() { return this; }

		virtual void invalidate() = 0;

		virtual ~invalidatable() = default;
	};
}
