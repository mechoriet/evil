/* 
 * File:   main.cpp
 * Author: Petrus
 *
 * Created on January 2, 2015, 2:46 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
//#include <random>

#include <SFML/Network.hpp>

using namespace std;

#define LFCR "\r\n"
#define LOG(QUOTE) \
	std::cout << QUOTE << std::endl; \
	evil::log << QUOTE << LFCR; \
	evil::log.flush();

namespace evil {
	std::fstream log;
}

using namespace evil;

int main(int argc, char** argv) {

	// connect to irc
	
	LOG("hi")
	
	return 0;
}

