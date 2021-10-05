# RTL - Reactive Template Library

_It solves all my C++ animation problems_  
-- me

## Using RTL

### Lazy, caching, reactive variables ###

Use `rtl::var<T>`

```cpp
#include <rtl/var.h>
...

rtl::var<float> bar = 2.0f;
rtl::var<float> foo = 3.0f;
rtl::var<float> foobar = [&](){ return foo() + bar(); };

printf("foobar = %f\n", foobar());

foo = 2.0f;
printf("foobar = %f\n", foobar());

bar = 1.0f;
printf("foobar = %f\n", foobar());
```

### Animation and time

RTL uses the `rtl::clock` class as a source of time, and if no instance is explicitly provided for `rtl::var` and `rtl::animator` they will use whatever the thread static `rtl::clock::current_clock` points to. 

This clock is by default pointing to the thread static `rtl::clock::default_clock`. However, we need to update this clock every frame by calling `rtl::clock::default_clock.adjust()` with the current time in seconds:

```cpp
#include <rtl/clock.h>
...

rtl:clock::default_clock.adjust(get_time_in_seconds()); 
```

**Reacting to value changes**

With `rtl::clock` set up, you can now use `rtl::animator` to react to changes. 

Even if multiple dependencies change the animator callback is run at max once per frame, avoiding wrong intermediate states. 

```cpp
#include <rtl/animator.h>
...

rtl::animator foo_printer = [&]()
{
	printf("foo = %f\n", foo());
};
rtl::animator bar_printer = [&]()
{
	printf("bar = %f\n", bar());
};
rtl::animator foobar_printer = [&]()
{
	printf("foobar = %f\n", foobar());
};

foo = 3.0f; // prints nothing yet
bar = 2.0f; // prints nothing yet

// later..
rtl:clock::default_clock.adjust(get_time_in_seconds()); // prints foo = 3, bar = 2, foobar = 5

bar = 3.0f; // prints nothing again

rtl:clock::default_clock.adjust(get_time_in_seconds()); // prints bar = 3, foobar = 6
```

**Triggering animations**

```cpp
foo.animate(1.0f, 0.5, &rtl::easing::linear, [&]()
{
	foo.animate(0.0f, 0.5, &rtl::easing::linear, [&]()
	{
		printf("animation complete!\n");
	});
});
```

Again note that `rtl::clock::frame_clock.adjust()` must have been called at least once before triggering any animations.

## Extending RTL

While it is often easier to just use an internal `rtl::var<T>` or `rtl::animator`, RTL can be extended at a lower level through the _graph_ API.

**Adding new value producers**

0. Include relevant headers
	```cpp
	#include <rtl/graph/dependency.h>
	#include <rtl/graph/visitor.h>
	```

1. Make an `rtl::dependency`
	```cpp
	dependency my_dependency;
	```

2. When evaluated, visit current `rtl::visitor`
	```cpp
	visitor::visit(&my_dependency);
	```

3. When changed, invalidate the `rtl::dependency`
	```cpp
	my_dependency.invalidate();
	```

**Adding new value consumers**

0. Include relevant headers
	```cpp
	#include <rtl/graph/dependant.h>
	#include <rtl/graph/visitor.h>
	```

1. Implement `rtl::invalidatable`, e.g. using `rtl::invalidator`
	```cpp
	#include <rtl/invalidator.h>

	invalidator my_invalidator = []()
	{
		// one of the dependencies was invalidated
		...
	};
	```

2. Make an `rtl::dependant`
	```cpp
	dependant my_dependant(&my_invalidator);
	```

3. Guard any calls to reactive functions with an instance of  `rtl::visitor`
	```cpp
	{
		visitor my_visitor(&my_dependant);

		// evaluate dependencies here
		...
	}
	```

## Limitations

Inteded for single-threaded use

## Licence 

```
MIT-licence here
```
