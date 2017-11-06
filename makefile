# TC 2 - Computação Gráfica - 2017/2
## Marcela Freitas Vieira

# Libraries
LIBS        = -lglut -lGLU -lGL -lm

# Main targets
all: trabalhocg

trabalhocg:	main.cpp player.cpp circle.cpp tinyxml2/tinyxml2.cpp
	g++ -o trabalhocg main.cpp player.cpp circle.cpp tinyxml2/tinyxml2.cpp $(LIBS)

clean:
	rm -i trabalhocg
