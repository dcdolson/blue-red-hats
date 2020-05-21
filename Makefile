main: main.cpp hat_color.h
	g++ -O2 -g --std=c++11 -o $@ $< -lpthread

