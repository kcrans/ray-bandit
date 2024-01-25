SOURCE = ./src/
SRC := $(wildcard $(SOURCE)/*)
#BUILD = ./src/
raytracer: $(SRC) 
	g++ -std=c++11 -Werror -o raytracer $(SOURCE)main.cpp
clean:
	rm -f raytracer
