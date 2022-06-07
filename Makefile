make:
	g++ ./src/*.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system

win:
	x86_64-w64-mingw32-g++ ./src/*.cpp  -o main.exe -lsfml-graphics -lsfml-window -lsfml-system -mwindows
