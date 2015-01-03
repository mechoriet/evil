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

#include <winsock2.h>
#include <windows.h>
#include <process.h>

//#include <random>

#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

#include "Q:/jsoncpp-master/include/json/json.h"
#include <libircclient.h>
#include <libirc_rfcnumeric.h>


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

/*
 * We store data in IRC session context.
 */
typedef struct
{
	char 	* channel;
	char 	* nick;

} irc_ctx_t;

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
	
	void tts(string &u, string &q);
	void getmp3url(string &q, string &h, string&c);
	void test();
	void writemp3(string &, sf::Http::Response &);
	void plusspaces(string &);
	void sanitizequote(string &);
	void stripuser(string &);
	//extern validArgs
	
	Json::Value midrash;
	bool readjsonconfig();
	
	bool connecttoirc();
	
	irc_ctx_t ctx;
	irc_session_t *ses;
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
			
			evil::readjsonconfig();
			
			evil::connecttoirc();
			
			//EVILLOG( EVILBAR << "The input is sufficient and syntactically correct. Starting..." << EVILBAR)
				
			string a = string("You are not a good person");
			
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

bool evil::readjsonconfig() {
	
	const char *name;
	
	if ( config[CONFIG] ) name = config[CONFIG]->c_str();
	else name = "config.json";
	
	char *buf;
	
	if ( ! (access( name, F_OK ) != -1) ) {
		EVILLOG("creating missing " << name)
		ofstream json(name, ios::out);
		json << EVIL_CONFIG_DEFAULT_JSON;
	}
	
	// get config .json in buf
	ifstream json(name, ios::in);
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
		return false;
	}
	
	/*std::string version = midrash.get("version", "unknown").asString();
	
	LOG("Midrash.json versions " << version);
	
	if ( version.compare(ap::VERSION) ) {
		LOG("base/exe version mismatch");
	}*/
	
	return true;
	
}

void dump_event (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char buf[512];
	int cnt;

	buf[0] = '\0';

	for ( cnt = 0; cnt < count; cnt++ )
	{
		if ( cnt )
			strcat (buf, "|");

		strcat (buf, params[cnt]);
	}

	EVILLOG("Event " << event << ", origin: " <<  (origin ? origin : "NULL") << "params: " << cnt << "[" << buf << "]");
	//EVILLOG ("Event \"%s\", origin: \"%s\", params: %d [%s]", event, origin ? origin : "NULL", cnt, buf);
}


void event_connect (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
	dump_event (session, event, origin, params, count);

	irc_cmd_join (session, ctx->channel, 0);
	
	EVILLOG("connected to " << ctx->channel);
}


void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
	char buf[24];
	sprintf (buf, "%d", event);

	dump_event (session, buf, origin, params, count);
}

void event_channel (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	dump_event (session, event, origin, params, count);
	
	EVILLOG("channel event")
		
	char nickbuf[128];

	if ( count != 2 )
		return;

	//const char *user = origin ? origin : "someone";
	
	string user( origin );
	evil::stripuser(user);
	
	string quote( params[1] );
	
	EVILLOG("'" << user << "' said in channel " << params[0] << ": " << params[1]);
	
	evil::tts(user, quote);
	

}

void event_join (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	dump_event (session, event, origin, params, count);
	irc_cmd_user_mode (session, "+i");
	irc_cmd_msg (session, params[0], "Hi all");
}

bool evil::connecttoirc() {
	
	const Json::Value IRCSetup = midrash["IRC Setup"];
	
	if ( ! IRCSetup ) {
		EVILLOG("Missing IRC Setup Object in config .json")
	}
	
	string server = IRCSetup.get("server", "none sir").asString();
	int port = IRCSetup.get("port", 6667).asInt();
	string channel = IRCSetup.get("channel", "N/A").asString();
	string username = IRCSetup.get("username", "not provided").asString();
	string password = IRCSetup.get("password", "0").asString();
	
	WSADATA wsaData;

	if ( WSAStartup ( MAKEWORD (2, 2), &wsaData) != 0 ) {
		EVILLOG("error")
		return false;
	}
	
	// The IRC callbacks structure
	irc_callbacks_t callbacks;

	// Init it
	memset ( &callbacks, 0, sizeof(callbacks) );

	// Set up the mandatory events
	callbacks.event_connect = event_connect;
	callbacks.event_join = event_join;
	callbacks.event_numeric = event_numeric;
	callbacks.event_channel = event_channel;
	
	//callbacks.event_connect = event_connect;
	//callbacks.event_join = event_join;
	/*callbacks.event_nick = dump_event;
	callbacks.event_quit = dump_event;
	callbacks.event_part = dump_event;
	callbacks.event_mode = dump_event;
	callbacks.event_topic = dump_event;
	callbacks.event_kick = dump_event;
	//callbacks.event_channel = event_channel;
	callbacks.event_privmsg = dump_event;
	callbacks.event_notice = dump_event;
	callbacks.event_invite = dump_event;
	callbacks.event_umode = dump_event;
	callbacks.event_ctcp_rep = dump_event;
	callbacks.event_ctcp_action = dump_event;
	callbacks.event_unknown = dump_event;*/
	//callbacks.event_numeric = event_numeric;

	// Set up the rest of events

	// Now create the session
	ses = irc_create_session( &callbacks );

	if ( !ses ) {
		// Handle the error
		EVILLOG("no session")
		return false;
	}
	
	ctx.channel = strdup(channel.c_str());
	ctx.nick = strdup(username.c_str());
	
	irc_set_ctx (ses, &ctx);
	
	if ( irc_connect (ses, strdup(server.c_str()), 6667, strdup(password.c_str()), strdup(username.c_str()), strdup(username.c_str()), 0 ) ) {
		EVILLOG("irc connect problem: " << irc_strerror(irc_errno(ses)) << " code " << irc_errno(ses) )
		return false;
	}
	
	if ( irc_run (ses) ) {
		EVILLOG("irc_run problem: " << irc_strerror(irc_errno(ses)) << " code " << irc_errno(ses) )
		return false;
	}
	
	/*if ( irc_cmd_join(ses, ctx.channel, 0) ) {
		EVILLOG("irc_cmd_join error " << irc_strerror(irc_errno(ses)) << " code " << irc_errno(ses) )
		return false;
	}*/
	
	//irc_cmd_msg(ses, "steerlat", "Running cool text to speech service try it.");
	
}


void evil::stripuser(string &user) {
	std::size_t found = user.find_first_of("!");
	if ( string::npos != found ) {
		user.assign( user.substr(0, found) );
	}
}

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

void evil::tts(string &user, string &quote) {

	stringstream all;
	all << user << " said: " << quote;
	quote.assign(all.str());
	
	EVILLOG("going to fetch: " << quote)
	
	return;
		
	sanitizequote(quote);
	plusspaces(quote);
	
	stringstream post;
	post << "langdemo:Powered+by+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-vaas.com%22%3EAcapela+Voice+as+a+Service%3C%2Fa%3E.+For+demo+and+evaluation+purpose+only%2C+for+commercial+use+of+generated+sound+files+please+go+to+%3Ca+href%3D%22http%3A%2F%2Fwww.acapela-box.com%22%3Ewww.acapela-box.com%3C%2Fa%3E";
	post << "&MyLanguages=sonid10";
	post << "&0=Leila&1=Laia&2=Eliska&3=Mette&4=Jeroen&5=Daan&6=Liam&7=Deepa&8=Rhona&9=Graham";
	post << "&MySelectedVoice=Laura";
	post << "&11=Sanna&12=Justine&13=Louise&14=Manon&15=Andreas&16=Dimitris&17=chiara&18=Sakura&19=Minji&20=Lulu&21=Bente&22=Ania&23=Marcia&24=Celia&25=Alyona&26=Antonio&27=Emilio&28=Elin&29=Samuel&30=Kal&31=Mia&32=Ipek";
	post << "&MySelectedVoice=WillBadGuy";
	post << "&MyTextForTTS=" << quote;
	post << "&t=1&SendToVaaS=";
	
	if ( config[VERBOSE] ) {
		EVILLOG("produced get body: " << post.str()) }
		
	
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