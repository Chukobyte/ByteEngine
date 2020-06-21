build:
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*.cpp ./src/core/*.cpp ./src/core/managers/*.cpp ./src/core/handlers/*.cpp ./src/core/components/*.cpp ./lib/json/* \
	-o byte_engine.exe \
	-I"${SDL2_HOME}\\include" \
	-L"${SDL2_HOME}\\lib" \
	-I"C:\\Users\\chuko\\AppData\\Local\\Programs\\Python\\Python37-32\\include" \
	-L"C:\\Users\\chuko\\AppData\\Local\\Programs\\Python\\Python37-32\\libs" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer \
	-lSDL2 \
	-lpython37

clean:
	del ./byte_engine

run:
	./byte_engine
