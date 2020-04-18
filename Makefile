all:
	/usr/local/gcc-10/bin/g++-10 -std=c++2a main.cpp -o ranges
	
clean:
	rm -f ranges
	
