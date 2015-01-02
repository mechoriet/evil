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
	map<Config, string> configmap;
	vector<string *> matchvector;
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
	
	map_init( evil::configmap )
		(evil::VERBOSE, string("verbose") )
		(evil::SERVER, string("server") )
		(evil::CHANNEL, string("channel") )	;
	
	for( map<evil::Config, string>::iterator it = evil::configmap.begin();
		it != evil::configmap.end(); ++it ) {
    	evil::matchvector.push_back( &it->second );
    }

	evil::log.open("whatwassaid.txt", ios::out);
	
	EVILLOG("          -~=. `Evil` IRC TTS Bot .=~-" << LFCR)
	
	EVILLOG("Hi");
	
	while ( true ) {
		if ( argc < 2 ) {
			EVILLOG(EVIL_HELP)}

		else {
			evil::mapargs(argc, argv);

			if ( ! evil::hasminimuminfo() ) {
				EVILLOG("")
				EVILLOG("=============================================================")
				EVILLOG("Not enough arguments to start operating on your IRC")
				EVILLOG("=============================================================")
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
		
		if ( '-' != v[0] ) {
			EVILLOG("ignored argument: " << v << " (missing dash -)")
			continue;
		}

		std::size_t found = s.find_first_of( "=", 1 );
		
		string *l = new string();
		string *r = new string();
		if ( string::npos != found ) {
			l->assign( s.substr(0, found) );
			r = new string( s.substr(found + 1, string::npos) );
		}
		else
			l = new string(v + 1);
		
		{ //
		bool match = false;
		int i = 0;
		for ( auto x : matchvector ) {
			//EVILLOG("comparing " << l->c_str() << " to " << x->c_str() )
			if ( 0 == l->compare(*x) ) {
				match = true;
				EVILLOG("provided " << v)
				evil::config[i] = r;
				break;
			}
			i ++;
		}
		

		if ( ! match ) {
			EVILLOG("unrecognized " << v)
			continue;
		}
		} //
	}
}

bool evil::hasminimuminfo() {
	
	if ( ! config[SERVER] ) {
		EVILLOG( "missing -server argument: " );
		return false;
	}
	
	else if ( ! config[CHANNEL] ) {
		EVILLOG( "missing -channel argument" );
		return false;
	}

	else
		return true;
	
}

//int 

void parseserver() {
	
}