#include <rtl/animator.h>

#include <rtl/graph/visitor.h>

#include <assert.h>

using namespace rtl;

static_animator::static_animator(std::function<void()> handler):
	static_animator(*clock::current_clock, std::move(handler)) 
{}

static_animator::static_animator(rtl::clock& clock, std::function<void()> handler):
	clock(clock),
	dependant(this),
	handler(std::move(handler)),
	call_validate([this]() { validate(); })
{
	state = invalid;
	clock.subscribe(&call_validate);
}

static_animator::~static_animator()
{
	if (state != settled)
	{
		bool did_unsubscribe = clock.unsubscribe(&call_validate);
		assert(did_unsubscribe);
	}
}

void static_animator::invalidate()
{
	switch(state)
	{
		case invalid:
			break;
		case valid:
			state = invalid;
			break;
		case settled:
			state = invalid;
			clock.subscribe(&call_validate);
			break;
	}
}

void static_animator::validate()
{
	switch(state)
	{
		case invalid:
		{
			state = valid;
			handler();
			break;
		}
		case valid:
		{
			bool did_unsubscribe = clock.unsubscribe(&call_validate);
			assert(did_unsubscribe);
			state = settled;
			break;
		}
		case settled:
			assert(false);
	}
}

animator::animator(std::function<void()> handler): 
	animator(*clock::current_clock, std::move(handler)) 
{}

animator::animator(rtl::clock& clock, std::function<void()> handler): 
	static_animator(clock, [this, handler]()
	{
		visitor v( &dependant );
		active_visitor = &v;
		handler(); // this might delete us
		if (!v.was_canceled())
		{
			active_visitor = nullptr;
		}
	})
{}

animator::~animator()
{
	if (active_visitor)
	{
		active_visitor->cancel();
	}
}
