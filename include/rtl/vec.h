#pragma once

#include "iterator.h"
#include "event.h"

#include "graph/dependency.h"

#include <functional>

namespace rtl
{
	using std::make_shared;
	using std::shared_ptr;
	using std::vector;
	using std::initializer_list;
	using std::function;
	
	static thread_local uint64_t generation;
	
    template<typename T>
    class vec
    {
		dependency dependency;

        vector<event<T>> changes { { 0, event<T>::idle, 0, nullptr } };
        vector<shared_ptr<T>> items;
	
	public:
		vec() {}
		

		vec(initializer_list<T> items)
		{
			
			auto i = 0;
			for( auto item : items )
			{
				insert( i++, item );
			}
		}

		vec(const vector<T>& items)
		{
			auto i = 0;
			for( auto item : items )
			{
				insert( i++, item );
			}
		}
		
		size_t size() const { return items.size(); }
	
		uint64_t now() const { return ++generation; }
		
        void insert( size_t index, T item )
        {
            auto box = make_shared<T>( std::move( item ) );
            changes.push_back({ now(), event<T>::insert, index, box });
            items.insert( items.begin() + index, box );
            //roll();
            
            dependency.invalidate();
        }

        void remove( size_t index )
        {
            changes.push_back({ now(), event<T>::remove, index });
            items.erase( items.begin() + index );
            //roll();
            
            dependency.invalidate();
        }

        event<T> poll( iterator& it ) const
        {
            dependency.visit();
            
            if( !it.data )
            {
				it.data = make_shared<uint64_t>( 0 );
			}
			
            uint64_t& itt = *std::static_pointer_cast<uint64_t>(it.data);
            for( auto i = changes.size(); i-- > 0; )
            {
                if( changes[i].timestamp <= itt )
                {
					if( i+1 >= changes.size() )
					{
						return { itt, event<T>::idle, 0, nullptr };
					}
					else
					{
						itt = changes[i+1].timestamp;
						return changes[i+1];
					}
                }
            }
			itt = changes[0].timestamp;
			return changes[0];
        }

        void roll()
        {
			auto window_size = 20;
            if( changes.size() > window_size )
            {
                changes.erase( changes.begin() );
            }
        }
    };
    
	
}
