CXX=g++

all: server client

client: client.cpp
	$(CXX) client.cpp -o client -pthread

server: server.cpp
	$(CXX) server.cpp -o server -pthread

clean-client:
	rm -f client

clean-server:
	rm -f server

clean: clean-client clean-server