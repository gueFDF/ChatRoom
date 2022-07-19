client:client.cc
			g++ client.cc -o client -lhiredis -ljson -lpthread
			g++ server.cc -o server -lhiredis -ljson -lpthread
.PHONY:client
clean:
			rm -f client
			rm -f client