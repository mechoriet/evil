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
#define EVILBAR "============================================================="

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
		
	string valids[] = {
		string("verbose"),
		string("server"),
		string("channel") };
	string *config[COUNT] = { new string };
		
	std::fstream log;
	
	void parseserver();
	void parsechannel();
	
	int matchvalid();
	
	map<string *, string *> argm;
	void mapargs(int, char **);
	bool hasminimuminfo();
	
	void posttext(string &);
	void getmp3url(string &);
	//extern validArgs
}


int main(int argc, char** argv) {

	evil::log.open("whatwassaid.txt", ios::out);
	
	EVILLOG("          -~=. `Evil` IRC TTS Bot .=~-" << LFCR)
	
	EVILLOG("Hi");
	
	while ( true ) {
		if ( argc < 2 ) {
			EVILLOG(EVIL_HELP)}

		else {
			evil::mapargs(argc, argv);

			if ( ! evil::hasminimuminfo() ) {
				EVILLOG(LFCR<<EVILBAR<<LFCR<<"Not enough arguments to start operating on your IRC"<<LFCR<<EVILBAR)
				break;
			}

			// CONTINUE AS NORMAL ^_^
			
			EVILLOG(LFCR<<EVILBAR<<LFCR<< "The input is sufficient and syntactically correct. Starting..."<<LFCR<<EVILBAR)
				
			string a = string("awesome");
			evil::posttext( a );
			
		}
		
		break;
	}
	
	// end of evil
	EVILLOG(LFCR << "Press any key to hide");
	cin.get();
	
	evil::log.close();
	
	return 0;
}

void evil::mapargs(int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		const char *v = argv[i];
		string s( v+1 );
		
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
			l = new string(s);
		
		{ //
		int i = 0;
		for ( auto x : valids ) {
			if ( 0 == l->compare(x) ) {
				EVILLOG("provided " << v)
				evil::config[i] = r;
				break;
			}
			i ++;
		}

		if ( ! evil::config[i] ) {
			EVILLOG("unrecognized " << l->c_str() )
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

void evil::posttext(string &quote) {

	sf::Http http("http://www.acapela-group.com/");
	sf::Http::Request request;
	request.setMethod(sf::Http::Request::Post);
	request.setUri("/demo-tts/DemoHTML5Form_V2.php?langdemo=Powered+by+Acapela-vaas.com");
	//request.setHttpVersion(1, 1); // HTTP 1.1
	
	request.setField("Host", "www.acapela-group.com");
	request.setField("Connection", "keep-alive");
	request.setField("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setField("Content-Type", "application/x-www-form-urlencoded");
	request.setField("User-Agent", "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
	request.setField("Referer", "http://www.acapela-group.com/demo-tts/DemoHTML5Form_V2.php?langdemo=Powered+by+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-vaas.com%22%3EAcapela+Voice+as+a+Service%3C%2Fa%3E.+For+demo+and+evaluation+purpose+only%2C+for+commercial+use+of+generated+sound+files+please+go+to+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-box.com%22%3Ewww.acapela-box.com%3C%2Fa%3E");
	request.setField("Accept-Encoding", "gzip, deflate");
	request.setField("Accept-Language", "nl-NL,nl;q=0.8,en-US;q=0.6,en;q=0.4");
	request.setField("Cache-Control", "max-age=0");
	request.setBody("MyLanguages=sonid10&MySelectedVoice=WillBadGuy&MyTextForTTS=premonition+suns");
	
	sf::Http::Response response = http.sendRequest(request);
	
	string a = string(response.getBody());
	evil::getmp3url( a );
	
}

void evil::getmp3url(string &html) {
	std::size_t found = html.find_first_of( "var myPhpVar = '" );
	
	if ( string::npos != found ) {
		EVILLOG("No myPhpVar for some reason");
		break;
	}
	
	std::size_t end = html.find(".mp3';", found);
	string mp3url( html.substr(found))
	
	sf::Http http("194.158.21.231:8081");
	sf::Http::Request request;
	request.setMethod(sf::Http::Request::Post);
	request.setUri("/demo-tts/DemoHTML5Form_V2.php?langdemo=Powered+by+Acapela-vaas.com");
	//request.setHttpVersion(1, 1); // HTTP 1.1

	request.setField("Host", "www.acapela-group.com");
	request.setField("Connection", "keep-alive");
	request.setField("Accept", "*/*");
	request.setField("Content-Type", "application/x-www-form-urlencoded");
	request.setField("User-Agent", "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
	request.setField("Referer", "http://www.acapela-group.com/demo-tts/DemoHTML5Form_V2.php?langdemo=Powered+by+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-vaas.com%22%3EAcapela+Voice+as+a+Service%3C%2Fa%3E.+For+demo+and+evaluation+purpose+only%2C+for+commercial+use+of+generated+sound+files+please+go+to+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-box.com%22%3Ewww.acapela-box.com%3C%2Fa%3E");
	request.setField("Accept-Language", "nl-NL,nl;q=0.8,en-US;q=0.6,en;q=0.4");
	request.setBody("MyLanguages=sonid10&MySelectedVoice=WillBadGuy&MyTextForTTS=premonition+suns");
	
	sf::Http::Response response = http.sendRequest(request);
}

void parseserver() {
	
}