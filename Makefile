FLAGS = -DSFML_STATIC
PREQS = -winmm -lws2_32 -sndfile -openal32
SFML = -sfml-system-s -sfml-network-s -sfml-audio-s
O = o/main.o
INCL = -IQ:/SFML-2.2/include

all: evil.exe


	
evil.exe: $(O)
	g++ $(O) -LQ:/built -LQ:/SFML-2.2/lib-lwinmm -static-libgcc -static-libstdc++ -o ../bin/apolune -Wall

o/main.o: main.cpp
	g++ $(INC) -c main.cpp -o o/main.o $(FLAGS)
