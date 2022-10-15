name = Slimey
libs = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
srcs = $(wildcard ./src/*.cpp)

all:
	g++ $(srcs) $(libs) -o $(name)
