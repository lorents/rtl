#pragma once

#include "vec.h"
#include "animator.h"

#include "graph/visitor.h"

namespace rtl
{
	using std::make_shared;
	using std::unique_ptr;
	using std::shared_ptr;
	using std::vector;
	using std::initializer_list;
	using std::function;
	
    template<typename T>
	class list
	{
		function<event<T>(iterator&)> impl;
	public:
		list():
			list(vector<T>{})
		{}

		list( initializer_list<T> items ):
			list(make_shared<vec<T>>(std::move( items )))
		{}

		list( vector<T> items ):
			list(make_shared<vec<T>>(std::move( items )))
		{}
		
		list( vec<T>& vec ):
			impl([&](auto& it){ return vec.poll(it); })
		{}

		list( shared_ptr<vec<T>> vec ):
			impl([vec](auto& it){ return vec->poll(it); })
		{}
		
		list( function<event<T>(iterator&)> impl ):
			impl( std::move( impl ))
		{}

		event<T> poll( iterator& iterator )
		{
			return impl( iterator );
		}

		list<T> filter( function<bool(T&)> pred ) const
		{
			return fmap<T>( [=](T& item){ return pred(item) ? list({ item }) : list(); });
		}
		
		list<T> rfilter( function<bool(T&)> pred ) const
		{
			return rfmap<T>( [=](T& item){ return pred(item) ? list({ item }) : list(); });
		}
		
		template<typename U>
		list<U> map( function<U(T&)> func ) const
		{
			return fmap<U>( [=](T& item){ return list<U>({ func(item) }); });
		}

		template<typename U>
		list<U> rmap( function<U(T&)> func ) const
		{
			return rfmap<U>( [=](T& item){ return list<U>({ func(item) }); });
		}

		template<typename U>
		list<U> fmap( function<list<U>(T&)> func ) const
		{
			auto fmap = make_shared<fmap_t<U>>( *this, func );
			return { [fmap](auto& it){ return fmap->poll(it); } };
		}
		
		template<typename U>
		list<U> rfmap( function<list<U>(T&)> func ) const
		{
			auto fmap = make_shared<rfmap_t<U>>( *this, func );
			return { [fmap](auto& it){ return fmap->poll(it); } };
		}
	private:
		template<typename U>
		class fmap_t : public vec<U>
		{
			list<T> input;
			function<list<U>(T&)> func;

			struct source
			{
				list<U> source;
				iterator iterator;
				int offset;
			};
			
			vector<source> sources;

			iterator iterator;
			animator animator
			{
				[=]() { update(); }
			};

		public:
			fmap_t( list<T> input, function<list<U>(T&)> func ):
				input( std::move( input )),
				func( std::move( func ))
			{}

			void update()
			{
				while(true)
				{
					auto change = input.poll( iterator );
					if( change.is_insert() )
					{
						do_not_disturb please;
						sources.insert( sources.begin() + change.index, source{ func(*change.item), {}, 0 } );
					}
					else if( change.is_remove())
					{
						sources.erase(sources.begin() + change.index );
					}
					else
					{
						break;
					}
				}

				auto shift = 0;
				for( int i = 0; i< sources.size(); i++ )
				{
					auto& src = sources[i];
					src.offset += shift;
					while( true )
					{
						auto change = src.source.poll(src.iterator);
						if( change.is_insert())
						{
							vec<U>::insert( src.offset + change.index, *change.item );
							shift++;
						}
						else if( change.is_remove() )
						{
							vec<U>::remove( src.offset + change.index );
							shift--;
						}
						else
						{
							break;
						}
					}
				}
			}
		};
		template<typename U>
		class rfmap_t : public vec<U>
		{
			list<T> input;
			function<list<U>(T&)> func;

			struct source
			{
				iterator iterator;
				int offset;
				vec<list<U>> rsource;
				list<list<U>> rsource_list{ rsource };
				list<U> fsource{ rsource_list.template fmap<U>([](auto& l){ return l; }) };
				animator animator;
				
				source(std::function<list<U>()> func):
					animator([=](){
						if( rsource.size() > 0 ) rsource.remove(0);
						rsource.insert(0, func() );
					})
				{}
			};
			
			vector<unique_ptr<source>> sources;

			iterator iterator;
			animator animator
			{
				[=]() { update(); }
			};

		public:
			rfmap_t( list<T> input, function<list<U>(T&)> func ):
				input( std::move( input )),
				func( std::move( func ))
			{}

			void update()
			{
				while(true)
				{
					auto change = input.poll( iterator );
					if( change.is_insert() )
					{
						auto item = change.item;
						sources.insert(
							sources.begin() + change.index,
							make_unique<source>( [=](){ return func(*item); })
						);
					}
					else if( change.is_remove())
					{
						sources.erase(sources.begin() + change.index );
					}
					else
					{
						break;
					}
				}

				auto shift = 0;
				for( int i = 0; i< sources.size(); i++ )
				{
					auto& src = *sources[i];
					src.offset += shift;
					while( true )
					{
						auto change = src.fsource.poll(src.iterator);
						if( change.is_insert())
						{
							vec<U>::insert( src.offset + change.index, *change.item );
							shift++;
						}
						else if( change.is_remove() )
						{
							vec<U>::remove( src.offset + change.index );
							shift--;
						}
						else
						{
							break;
						}
					}
				}
			}
		};
	};
	
}
