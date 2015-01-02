# build with winbuilds mingw-w64

FLAGS = -DSFML_STATIC
SFML_DEPS = -lwinmm -lws2_32 -lsndfile -lopenal32
SFML = -lsfml-system-s -lsfml-network-s -lsfml-audio-s
FLAGS = -std=c++0x

O = o/main.o
INCL = -IQ:/SFML-2.2/include -IQ:/libircclient-1.8/include
	
##LIRC = -LQ:\libircclient-1.8\bin\
#lIRC = -lircclient

all: evil.exe


#$(LIRC) $(lIRC)
evil.exe: $(O)
	g++ $(O)  $(SFML) $(SFML_DEPS) -LQ:/SFML-2.2/lib -static-libgcc -static-libstdc++ -o evil -Wall

o/main.o: main.cpp
	g++ $(INCL) -c main.cpp -o o/main.o $(FLAGS)
