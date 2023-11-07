#include <iostream>

#include "test.h"

#include <functional>

using namespace std;
using namespace rtl;


#include <rtl/log.h>
int main(int argc, const char** argv)
{
	rtl::log_consumer logger = {[](const log_event& e) 
	{
		//cout << e.location.file_name() << ":" << e.location.line() << " " << e.verb << endl;
	}};

	tests::run();

	cout << endl << "All tests completed successfully" << endl;
	std::string str;
	cin >> str;
	
	return 0;
}
