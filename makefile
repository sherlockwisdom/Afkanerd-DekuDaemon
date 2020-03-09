all:
	g++ daemon.cxx -o daemon -pthread -g -std=c++1z
helpers:
	g++ helpers.cxx -o helpers -g -std=c++1z
tester:
	g++ tester.cxx -o tester -g -std=c++1z -pthread -g
start:
	make -C src/ start
	doxygen Doxyfile
