# build with winbuilds mingw-w64

FLAGS = -DSFML_STATIC -std=c++0x
SFML_DEPS = -lwinmm -lws2_32 -lsndfile -lopenal32
SFML = -LQ:/SFML-2.2/lib -lsfml-audio-s -lsfml-network-s -lsfml-system-s 

O = o/main.o o/IRC.o
INCL = -IQ:/SFML-2.2/include -IQ:/libircclient-1.8/include
	

	
LIRC = -LQ:/libircclient-1.8/bin
lIRC = -lircclient

all: evil.exe



evil.exe: $(O)
	g++ $(O) $(SFML) $(SFML_DEPS) $(LIRC) $(lIRC) -LQ:/built -ljsoncpp -static-libgcc -static-libstdc++ -o evil -Wall

o/main.o: main.cpp
	g++ $(INCL) -c main.cpp -o o/main.o $(FLAGS)

o/ircbot.o: ircbot.cpp
	g++ -c ircbot.cpp -o o/ircbot.o

o/IRC.o: IRC.cc
	gcc -c IRC.cc -o o/IRC.o
	
clean:
	rm $(O)