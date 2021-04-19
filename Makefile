CXX=g++

all: server client

debug: server-debug client-debug

client: clean-client client.cpp
	$(CXX) client.cpp -o client -pthread

server: clean-server server.cpp
	$(CXX) server.cpp -o server -pthread

client-debug: client.cpp
	$(CXX) client.cpp -o client -pthread -g

server-debug: server.cpp
	$(CXX) server.cpp -o server -pthread -g

clean-client:
	rm -f client

clean-server:
	rm -f server

clean: clean-client clean-server