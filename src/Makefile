CC = gcc
CXX = g++
CXXFLAGS = -Wall -g 
LDFLAGS = -lGL -lGLU -lglfw -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor -lasound

OBJECTS = glad.o emulator.o executor.o parser.o screen.o audiohandler.o util.o

INCLUDES = shader.h screen.h

chip8: %: %.o ${OBJECTS}
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

%.o: %.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $<

glad.o:
	$(CC) $(CXXFLAGS) -c ./Lib/glad.c -o $@

clean:
	rm -f chip8 *.o
