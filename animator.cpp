#include "animator.h"

#include "clock.h"
#include "dependency_visitor.h"

#include <assert.h>

using namespace rtl;

static_animator::static_animator(std::function<void()> handler):
	static_animator(*clock::current_clock, std::move(handler)) {}

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
			call_handler(dependant);
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

void static_animator::call_handler(rtl::dependant&)
{
	handler(); // this might invalidate again e.g. during animation
}

animator::animator(std::function<void()> handler) : static_animator(std::move(handler)) {}

animator::animator(rtl::clock& clock, std::function<void()> handler) : static_animator(clock, std::move(handler)) {}

void animator::call_handler(rtl::dependant& dep)
{
	dependency_visitor v(&dep);
	static_animator::call_handler(dep); // this might invalidate again e.g. during animation
}