#include "main.h"

#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/random.hpp"

int main(int argc, char *argv[])
{
	#ifndef _WIN32
	daemonize();
	#endif

	return 0;
}

