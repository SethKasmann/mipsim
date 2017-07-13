exe:	src/main.cpp
	g++ src/*.cpp -std=c++14

debug:	main.cpp
	g++ src/*.cpp -std=c++14 -fsanitize=undefined

run:
	./a.out

r:
	./a.out

clean:
	rm a.out

c:
	rm a.out