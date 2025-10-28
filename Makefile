CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC_DIR = src
INCLUDE_DIR = include

all: main test

test: test.o jeu.o bird.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main: main.o bird.o tuyaux.o jeu.o menu.o
	$(CXX) $(CXXFLAGS) -g -o $@ $^ $(LDFLAGS) 

%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< 

clean:
	rm -f *.o main