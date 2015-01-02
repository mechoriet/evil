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
	/*enum Valids {
		VERBOSE,
		SERVER,
		CHANNEL };
	map<Valids, const char*> valids;*/
		
	std::fstream log;
	void parseserver();
	void parsechannel();
	//static const char *valids[] = {"-server", "-channel"};
	int matchvalid();
	map<string *, string *> argm;
	//extern validArgs
}


    //(CHANNEL, "B")
    //(VERBOSE, "C")
;

//const vector<evil::validArgs> = {"Cool", "Cool"};

int main(int argc, char** argv) {
	
	/*map_init( evil::valids )
		(evil::Valids::VERBOSE, "-verbose")
		(evil::Valids::SERVER, "-server=")
		(evil::Valids::SERVER, "-channel=")
	;*/

	evil::log.open("whatwassaid.txt", ios::out);
	
	//EVILLOG("Hi");
	
	if ( argc < 2 ) {
		EVILLOG(EVIL_HELP)}
	
	else {
		
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
			else {
				l = new string(s.c_str() + 1);
			}
			
			evil::argm.insert( pair<string *, string *>(l, r) );
			
			//evil::argm.insert( pair<const char *, string *>("server", nullptr) );
			//else {
			//	string k();
			//}
			
			//evil::argm[k->c_str()];
			//if (  )
			//if ( )
			/*
#define SAME 0
			if
			( SAME == strcmp("-server", v) ) {
				//parseserver(v);
			}
			else if 
			( SAME == strcmp("-channel", v) ) {
				
			}
			else {
				EVILLOG("!! Unrecognized Argument: " << v);
			}*/
		}
	}
	
	// end of evil
	EVILLOG("press key to die");
	cin.get();
	
	evil::log.close();
	
	return 0;
}

//int 

void parseserver() {
	
}