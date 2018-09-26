CC=g++
CFLAGS=-Wall -g `sdl-config --cflags`
SOURCES=main.cpp SDLApp.cpp Workshop2.cpp geometry.cpp ShaderProgram.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
LDFLAGS=`sdl-config --libs` -lGLU -lGLEW  -lGL 

# Get the location of this makefile.
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
# Path where depencencies live.
RUST_LIB:=$(ROOT_DIR)/dependencies/png_decode/target/debug
MANIFEST_DIR:=$(ROOT_DIR)/dependencies/png_decode

main: main.cpp
	$(CC) $(CPPFLAGS) -c $(SOURCES)
	$(CC) $(OBJECTS) $(CPPFLAGS) -L$(RUST_LIB) $(LDFLAGS) -lpng_decode -o mountain

static: main.cpp
	$(CC) $(CPPFLAGS) -c $(SOURCES)
	$(CC) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o mountain_statically_linked -L$(RUST_LIB) -Wl,-Bstatic -lpng_decode -Wl,-Bdynamic 

.PHONY: clean
clean:
	rm -rf $(OBJECTS) mountain mountain_statically_linked
	
.PHONY: run
run:
	LD_LIBRARY_PATH=$(RUST_LIB) ./mountain
	
.PHONY: deps
deps:
	cd $(MANIFEST_DIR); cargo build

.PHONY: cleandeps
cleandeps:
	cd $(MANIFEST_DIR); cargo clean
