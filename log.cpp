#include "log.h"

namespace rtl
{
	log_consumer* log_consumer::first_consumer = nullptr;
	log_consumer* log_consumer::last_consumer = nullptr;
}