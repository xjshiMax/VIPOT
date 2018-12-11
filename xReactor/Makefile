CC=gcc
CFLAGS= -g -lstdc++
cFile=xReactor.hpp xEventDemultiplexer.hpp xtimeheap.hpp xbaseclass.hpp 
all:a.out svrtest
a.out: xReactor.cpp $(cFile)
	$(CC) $(CFLAGS) -o a.out $^
svrtest:reactorServer.hpp $(cFile) singleton.hpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f a.out svrtest
