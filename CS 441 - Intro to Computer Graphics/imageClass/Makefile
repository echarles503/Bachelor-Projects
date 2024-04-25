# Makefile for Image Class
#

INC = -I ./include

out: main.o pixel.o image.o
	g++ main.o pixel.o image.o -o out

main.o: main.cpp ./include/pixel.h ./include/image.h
	g++ $(INC) -c main.cpp

pixel.o: pixel.cpp ./include/pixel.h
	g++ $(INC) -c pixel.cpp

image.o: image.cpp ./include/pixel.h ./include/image.h
	g++ $(INC) -c image.cpp

clean: 
	rm -f *.o

