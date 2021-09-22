#pragma once

#include <vector>

namespace rtl
{
    class invalidatable;
    class dependency;

    class dependant
    {
        invalidatable* subscriber;

        std::vector<dependency*> dependencies;
    
    public:
		dependant(const dependant&) = delete;
        
		dependant(invalidatable* subscriber);
        ~dependant();

        void prepare_mark_and_sweep();
        void mark_and_subscribe(dependency* dep);
        void sweep_and_unsubscribe();
    };
}
