#include <iostream>

#include "test.h"

#include <functional>

using namespace std;
using namespace rtl;

function<bool(const string&)> make_matcher(int patc, const char** patv)
{
	if (patc == 0)
	{
		return [](const string&)
		{ 
			return true;
		};
	}

	return [patc, patv](const string& file)
	{
		for (int i = 0; i < patc; i++)
		{
			if (file.find(patv[i]) != string::npos)
			{
				return true;
			}
		}
		return false;
	};
}

int main(int argc, const char** argv)
{
	auto match = make_matcher(argc - 1, &argv[1]);

	for (auto it = test::tests->begin(); it != test::tests->end(); ++it)
	{
		if (match(it->first))
		{
			cout << "Running " << it->first << "... " << flush;
			it->second->run();
			cout << "Success" << endl;
		}
	}
	return 0;
}
