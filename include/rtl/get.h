#pragma once


namespace rtl
{
	template< typename T >
	class get
	{
		std::function<T()> mGet;
	public:
		get( var<T>& ref ) : mGet([&ref](){ return ref(); })
		{
		}
		
		get( const T& value )
		{
			mGet = [value](){ return value; };
		}
		
		get( std::function<T()> getter )
		{
			mGet = std::move(getter);
		}
		
		T operator ()() const
		{
			return mGet();
		}
	};
}
