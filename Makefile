# build with winbuilds mingw-w64

FLAGS = -DSFML_STATIC
PREQS = -winmm -lws2_32 -sndfile -openal32
SFML = -sfml-system-s -sfml-network-s -sfml-audio-s
FLAGS = -std=c++0x
O = o/main.o
INCL = -IQ:/SFML-2.2/include

all: evil.exe


	
evil.exe: $(O)
	g++ $(O) -LQ:/SFML-2.2/lib -lwinmm -static-libgcc -static-libstdc++ -o evil -Wall

o/main.o: main.cpp
	g++ $(INCL) -c main.cpp -o o/main.o $(FLAGS)
