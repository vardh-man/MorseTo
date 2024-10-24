morseTo.o: morseTo.cpp toAudio.cpp toMorse.cpp
	g++ -Wall -g -std=c++11 -o morseTo morseTo.cpp toAudio.cpp toMorse.cpp

clean: 
	rm -f morseTo.o
	rmdir -rf morseTo.dSYM