# C opengl hobbyproject MAKEFILE
# mortargoblin@github

# Compilers
CC  = gcc
CXX = g++

# Include paths
INCLUDES = \
	-I./include \
	-I./include/fast_obj-1.3 \

# Flags
CFLAGS   = -Wall -Wextra -std=c11 $(INCLUDES)
CXXFLAGS = -Wall -Wextra -std=c++17 $(INCLUDES)

# Libraries
LIBS = -lGL -lGLEW -lEGL -lGLU -lOpenGL -lglfw -lstdc++ -lm

# Target
game: main.o shader.o shader_c.o
	$(CC) $^ -o $@ $(LIBS)

# C object
main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

# C++ objects
shader.o: include/shader.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

shader_c.o: include/shader_c.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f *.o game

