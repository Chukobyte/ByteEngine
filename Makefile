build:
	g++ -w -std=c++14 -Wfatal-errors -static-libgcc -static-libstdc++ \
	./src/*.cpp ./src/core/*.cpp ./src/core/managers/*.cpp ./src/core/handlers/*.cpp \
	./src/core/components/*.cpp ./src/core/rendering/*.cpp ./src/core/rendering/glad.c \
	./lib/json/* \
	-o byte_engine.exe \
	-I"./include" \
	-L"./include/lib" \
	-I"${SDL2_HOME}\\include" \
	-L"${SDL2_HOME}\\lib" \
	-I"${PYTHON_HOME}\\include" \
	-L"${PYTHON_HOME}\\libs" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2_mixer \
	-lSDL2 \
	-lpython37 \
	-lfreetype \

clean:
	del ./byte_engine

run:
	./byte_engine
