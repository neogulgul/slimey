make:
	g++ ./src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system -o main

win:
	x86_64-w64-mingw32-g++ ./src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system -o main.exe
