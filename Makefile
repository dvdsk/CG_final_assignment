CC=g++
CFLAGS=-Wall -g `sdl-config --cflags`
SOURCES=main.cpp SDLApp.cpp Workshop2.cpp geometry.cpp ShaderProgram.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LDFLAGS=`sdl-config --libs` -lGLU -lGLEW  -lGL 

main: main.cpp
	$(CC) $(CPPFLAGS) -c $(SOURCES) dependencies/lodepng.cpp png_decode/png_decode.cpp
	$(CC) $(OBJECTS) lodepng.o png_decode.o $(CPPFLAGS) -L$(RUST_LIB) $(LDFLAGS) -o landscape

.PHONY: clean
clean:
	rm -rf $(OBJECTS) landscape
