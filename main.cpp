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
#include <cstring>
#include <map>
//#include <random>

#include <SFML/Network.hpp>

using namespace std;

#define LFCR "\r\n"
#define EVIL_HELP \
	"Running evil.exe without arguments :(" LFCR \
	"Use the following arguments:" LFCR \
	"-server=ip or dns" LFCR \
	"--------------------------------------"

#define EVILLOG(QUOTE) \
	std::cout << QUOTE << std::endl; \
	evil::log << QUOTE << LFCR; \
	evil::log.flush();

#include "auxiliary.cpp"


namespace evil {
	enum Config {
		VERBOSE,
		SERVER,
		CHANNEL,
		COUNT };
	map<Config, const char*> configmatch;
	string *config[COUNT] = { new string };
		
	std::fstream log;
	void parseserver();
	void parsechannel();
	//static const char *valids[] = {"-server", "-channel"};
	int matchvalid();
	map<string *, string *> argm;
	void mapargs(int, char **);
	bool hasminimuminfo();
	//extern validArgs
}


int main(int argc, char** argv) {
	
	map_init( evil::configmatch )
		(evil::VERBOSE, "verbose")
		(evil::SERVER, "server")
		(evil::CHANNEL, "channel")
	;

	evil::log.open("whatwassaid.txt", ios::out);
	
	EVILLOG("-~=. Evil TTS Bot .=~-")
	
	//EVILLOG("Hi");
	
	while ( true ) {
		if ( argc < 2 ) {
			EVILLOG(EVIL_HELP)}

		else {
			evil::mapargs(argc, argv);

			if ( ! evil::hasminimuminfo() ) {
				EVILLOG("Evil does not have enough information to start listening")
				break;
			}

			
		}
		
		break;
	}
	
	// end of evil
	EVILLOG(LFCR << "press key to die");
	cin.get();
	
	evil::log.close();
	
	return 0;
}

void evil::mapargs(int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		const char *v = argv[i];
		string s(v+1);

		EVILLOG("parsing arg: " << v)

		if ( '-' != v[0] ) {
			EVILLOG("argument should start with -, ignored: " << v)
			continue;
		}

		std::size_t found = s.find_first_of( "=", 1 );
		
		string *l = nullptr;
		string *r = nullptr;
		if ( string::npos != found ) {
			l = new string( s.substr(0, found) );
			r = new string( s.substr(found + 1, string::npos) );
		}
		else
			l = new string(s.c_str() + 1);
		
		if ( configmatch.find(l->c_str()) )
			evil::argm.insert( pair<string *, string *>(l, r) );
	}
}

bool evil::hasminimuminfo() {
	if ( nullptr != config[SERVER]  
		
		)
		return false;
		
	//string *server = config.find( SERVER );
	//EVILLOG()
	return true;
}

//int 

void parseserver() {
	
}