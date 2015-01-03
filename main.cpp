/* 
 * File:   main.cpp
 * Author: Petrus
 *
 * Created on January 2, 2015, 2:46 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <algorithm> // for string remove
#include <io.h> // for F_OK file exists
//#include <random>

#include <SFML/Network.hpp>
#include "Q:/jsoncpp-master/include/json/json.h"

using namespace std;

#define LFCR "\r\n"
#define EVIL_HELP \
	"Running evil.exe without arguments :(" LFCR \
	"Use the following arguments:" LFCR \
	"-server=ip or dns" LFCR \
	"--------------------------------------"
//#define EVILBAR LFCR << "=============================================================" << LFCR
#define EVILBAR ""

#define EVIL_CONFIG_DEFAULT_JSON "\
	{\
	\"IRC_Settings\": {\
		\"channel\": \"cool\",\
		\"server\": \"1239123\"\
	},\
	\
	\"Voice By User\": {\
		\"steerlat\": \"WillBadGuy\"\
	}\
}"

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
		CONFIG,
		COUNT };
		
	string valids[] = {
		string("verbose"),
		string("server"),
		string("channel"),
		string("config") };
	string *config[COUNT] = { new string };
		
	std::fstream log;
	
	void parseserver();
	void parsechannel();
	
	int matchvalid();
	
	map<string *, string *> argm;
	void mapargs(int, char **);
	bool hasminimuminfo();
	
	void posttext(string &);
	void getmp3url(string &, string &, string&);
	void test();
	void writemp3(string &, sf::Http::Response &);
	void plusspaces(string &);
	void sanitizequote(string &);
	//extern validArgs
	
	Json::Value midrash;
	bool readjsonconfig();
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

			if ( ! evil::hasminimuminfo() )
				break;
			
			// CONTINUE AS NORMAL ^_^
			
			evil::readjsonconfig();
			
			//EVILLOG( EVILBAR << "The input is sufficient and syntactically correct. Starting..." << EVILBAR)
				
			string a = string("You are a beautiful soul inmate");
			evil::posttext( a );
			
			//evil::test();
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

		std::size_t found = s.find( "=", 1 );
		
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
	
	/*if ( ! config[SERVER] ) {
		EVILLOG( "missing -server argument: " );
		
		return false;
	}
	
	else if ( ! config[CHANNEL] ) {
		EVILLOG( "missing -channel argument" );
		return false;
	}

	else*/
		return true;
	
}

bool evil::readjsonconfig() {
	fstream json;
	
	const char *name;
	
	if ( config[CONFIG] ) name = config[CONFIG]->c_str();
	else name = "config.json";
	
	char *buf;
	
	// specified config .json doesn't exist...
	if ( ! (access( name, F_OK ) != -1) ) {
		EVILLOG("creating missing " << name)
		json.open(name, ios::out);
		json << EVIL_CONFIG_DEFAULT_JSON;
	}
	
	// get config .json in buf
	json.open(name, ios::in);
	json.seekg (0, json.end);
	int len = json.tellg();
	json.seekg (0, json.beg);
	buf = new char [len];
	json.read (buf, len);
	json.close();
	
	using namespace Json;
	Reader reader;
	bool good = reader.parse(buf, midrash);
	
	if ( ! good) {
		EVILLOG(name << " is syntactically broken. Delete it if you want a new one.")
	}
	
	/*std::string version = midrash.get("version", "unknown").asString();
	
	LOG("Midrash.json versions " << version);
	
	if ( version.compare(ap::VERSION) ) {
		LOG("base/exe version mismatch");
		return false;
	}*/
}

//int 

void evil::sanitizequote(string &quote) {
	const char illegals[] = {
		/* Windows filename: */
		'\\', '/', ':', '*', '?', '"', '<', '>', '|',
		/* extra: */
		'&', '=', '[', ']', '{', '}'};
	
	for (char x : illegals ) {
		quote.erase ( std::remove(quote.begin(), quote.end(), x), quote.end());
	}
	
	if ( config[VERBOSE] ) {
	EVILLOG("sanitized quote: " << quote) }
}

void evil::plusspaces(string &quote) {
	for ( char &x : quote )
		if ( ' ' == x ) x = '+';
	
	if ( config[VERBOSE] ) {
	EVILLOG("plussed spaces: " << quote) }
}

void evil::posttext(string &quote) {

	sanitizequote(quote);
	plusspaces(quote);
	
	stringstream post;
	post << "langdemo:Powered+by+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-vaas.com%22%3EAcapela+Voice+as+a+Service%3C%2Fa%3E.+For+demo+and+evaluation+purpose+only%2C+for+commercial+use+of+generated+sound+files+please+go+to+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-box.com%22%3Ewww.acapela-box.com%3C%2Fa%3E";
	post << "&MyLanguages=sonid10";
	post << "&0=Leila&1=Laia&2=Eliska&3=Mette&4=Jeroen&5=Daan&6=Liam&7=Deepa&8=Rhona&9=Graham";
	post << "&MySelectedVoice=Laura";
	post << "&11=Sanna&12=Justine&13=Louise&14=Manon&15=Andreas&16=Dimitris&17=chiara&18=Sakura&19=Minji&20=Lulu&21=Bente&22=Ania&23=Marcia&24=Celia&25=Alyona&26=Antonio&27=Emilio&28=Elin&29=Samuel&30=Kal&31=Mia&32=Ipek";
	post << "&MySelectedVoice=WillBadGuy";
	post << "&MyTextForTTS=" << quote;;
	post << "&t=1&SendToVaaS=";
	EVILLOG("produced get body: " << post.str() )
		
	sf::Http http("http://www.acapela-group.com");
	
	sf::Http::Request request;
	request.setMethod(sf::Http::Request::Post);
	request.setUri("/demo-tts/DemoHTML5Form_V2.php?langdemo=Powered+by+Acapela-vaas.com");
	
	request.setField("Host", "www.acapela-group.com");
	request.setField("Connection", "keep-alive");
	request.setField("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	request.setField("Content-Type", "application/x-www-form-urlencoded");
	request.setField("User-Agent", "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
	request.setField("Referer", "http://www.acapela-group.com/demo-tts/DemoHTML5Form_V2.php?langdemo=Powered+by+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-vaas.com%22%3EAcapela+Voice+as+a+Service%3C%2Fa%3E.+For+demo+and+evaluation+purpose+only%2C+for+commercial+use+of+generated+sound+files+please+go+to+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-box.com%22%3Ewww.acapela-box.com%3C%2Fa%3E");
	request.setField("Accept-Encoding", "gzip, deflate");
	request.setField("Accept-Language", "nl-NL,nl;q=0.8,en-US;q=0.6,en;q=0.4");
	request.setField("Cache-Control", "max-age=0");
	request.setBody( post.str() );
	
	if ( config[VERBOSE] ) {
	EVILLOG("getting .html ...") }
	sf::Http::Response response = http.sendRequest(request);
	
	string html = string(response.getBody());
	string cookie = response.getField("Cookie");
	
	if ( config[VERBOSE] ) {
	EVILLOG("cookie is " << cookie) }
	
	evil::getmp3url( quote, html, cookie );
	
}

void evil::getmp3url(string &quote, string &html, string &cookie) {
	//evil::log << html.c_str();
	
	string startjs("var myPhpVar = '");
	std::size_t start = html.find( startjs );
	
	if ( string::npos == start ) {
		EVILLOG("No myPhpVar for some reason");
		return;
	}
	
	string endjs(".mp3';");
	std::size_t end = html.find(endjs, start);
	
	// produce qualified .mp3 URL from raw HTML/JavaScript
	int pos = start + startjs.length();
	int len = (end + endjs.length()) - (start + startjs.length()+2);
	string url( html.substr(pos, len) );
	string file( html.substr(pos, len) );
	EVILLOG("mp3 url is: " << url.c_str() );
	
	// get host of .mp3 URL
	std::size_t slash = url.find("/", 7);
	string host( url.substr(7, slash-7) );
	string hostport( host );
	//EVILLOG("host is " << host)
	string server( url.substr(0, slash) );
	//EVILLOG("server is " << server)
	
	// generate port
	std::size_t colon = host.find(":");
	//EVILLOG("removed port from host: " << host.c_str() )
	string port( host.substr(colon+1, slash-colon) );
	host.assign( host.substr(0, colon) );
	//EVILLOG("produced port: " << port.c_str() )
	
	// remove 'http://xx' from mp3 URL
	file.assign( url.substr(server.length(), string::npos) );
	
	sf::Http http( host.c_str(), stoi(port.c_str()) );
	
	EVILLOG("getting file: " << url << " from " << hostport)
		
	sf::Http::Request request;
	request.setMethod(sf::Http::Request::Get);
	request.setUri( file.c_str() );

	request.setField("Accept", "*/*");
	request.setField("Accept-Encoding", "identity;q=1, *;q=0");
	request.setField("Accept-Language", "nl-NL,nl;q=0.8,en-US;q=0.6,en;q=0.4");
	request.setField("Cache-Control", "max-age=0");
	request.setField("Connection", "keep-alive");
	request.setField("Host", hostport.c_str() );
	request.setField("Pragma", "no-cache");
	//request.setField("Range", "bytes=0-");
	// range gives partial response so no-go
	
	request.setField("Referer", "http://www.acapela-group.com/demo-tts/DemoHTML5Form_V2.php?langdemo=Powered+by+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-vaas.com%22%3EAcapela+Voice+as+a+Service%3C%2Fa%3E.+For+demo+and+evaluation+purpose+only%2C+for+commercial+use+of+generated+sound+files+please+go+to+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-box.com%22%3Ewww.acapela-box.com%3C%2Fa%3E");
	request.setField("User-Agent", "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
	request.setField("Cookie", cookie.c_str() );
	
	
	EVILLOG("getting .mp3 ...")
	sf::Http::Response response = http.sendRequest(request);
	
	EVILLOG("status: " << response.getStatus())
		
	evil::writemp3(quote, response);
	
}

void evil::writemp3(string &quote, sf::Http::Response &get) {
	fstream mp3;
	
	string filename(quote);
	
	stringstream ss;
	ss << "gets/";
	ss << filename << ".mp3";
	
	EVILLOG("writing .mp3: " << ss.str())
		
	mp3.open(ss.str(), ios::out | std::ios::binary);
	mp3 << get.getBody();
	mp3.close();
}

/*
void evil::test() {
	sf::Http http( "194.158.21.231", 8081 );
	sf::Http::Request request;
	
	request.setMethod(sf::Http::Request::Get);
	request.setUri( "/MESSAGES/012099097112101108097071114111117112/AcapelaGroup_WebDemo_HTML/sounds/98576538_d8b66814dca07.mp3" );
	
	request.setHttpVersion(1, 1);
	request.setField("Accept-Encoding", "identity;q=1, *;q=0");
	request.setField("Accept-Language", "nl-NL,nl;q=0.8,en-US;q=0.6,en;q=0.4");
	request.setField("Cache-Control", "max-age=0");
	request.setField("Connection", "keep-alive");
	request.setField("Host", "194.158.21.231:8081");
	request.setField("Pragma", "no-cache");
	//request.setField("Range", "bytes=0-");
	// range gives partial response
	
	request.setField("Referer", "http://www.acapela-group.com/demo-tts/DemoHTML5Form_V2.php?langdemo=Powered+by+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-vaas.com%22%3EAcapela+Voice+as+a+Service%3C%2Fa%3E.+For+demo+and+evaluation+purpose+only%2C+for+commercial+use+of+generated+sound+files+please+go+to+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-box.com%22%3Ewww.acapela-box.com%3C%2Fa%3E");
	request.setField("User-Agent", "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
	request.setField("Cookie", "PHPSESSID=1n67esiaifl8m2pj2n6khq2be3; qtrans_cookie_test=qTranslate+Cookie+Test; __utma=57132138.719638552.1420242028.1420242028.1420242028.1; __utmc=57132138; __utmz=57132138.1420242028.1.1.utmccn=(direct)|utmcsr=(direct)|utmcmd=(none)");
	//request.setBody("");
	
	EVILLOG("getting .mp3 ...")
	sf::Http::Response response = http.sendRequest(request);
	
	EVILLOG("status: " << response.getStatus())
	
	evil::writemp3(response);
}*/