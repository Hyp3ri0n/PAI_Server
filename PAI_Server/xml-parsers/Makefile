CC=g++
PP=g++
CFLAGS= -Wall -Wextra
INCLUDEFLAGS=-I/usr/include/libxml2 -I/usr/include/glibmm-2.4 -I/usr/lib/x86_64-linux-gnu/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/libxml++-2.6 -I/usr/lib/libxml++-2.6/include -I/usr/include/curl 
LDLIBS=-L/opt/local/lib  -lcurl -lxml++-2.6 -lxml2 -lglibmm-2.4 -lgobject-2.0 -lsigc-2.0 -lglib-2.0 -lxslt

all: server

server: server.o client.o LwSaxParser.o FromXMLToGoogleMapHTTPRequest.o FromGoogleMapXMLToDistanceTable.o SortVisits.o Address.o tsp.o triBulle.o kruskal.o
	$(CC) Address.o SortVisits.o FromGoogleMapXMLToDistanceTable.o FromXMLToGoogleMapHTTPRequest.o LwSaxParser.o server.o client.o tsp.o triBulle.o kruskal.o -o server-metier $(LDLIBS)

LwSaxParser.o : LwSaxParser.h LwSaxParser.cpp
	$(PP) -c $(CFLAGS) $(INCLUDEFLAGS) LwSaxParser.cpp -o LwSaxParser.o

FromXMLToGoogleMapHTTPRequest.o :  Address.h FromXMLToGoogleMapHTTPRequest.h FromXMLToGoogleMapHTTPRequest.cpp LwSaxParser.h LwSaxParser.cpp
	$(PP) -c $(CFLAGS) $(INCLUDEFLAGS) FromXMLToGoogleMapHTTPRequest.cpp -o FromXMLToGoogleMapHTTPRequest.o

FromGoogleMapXMLToDistanceTable.o : FromGoogleMapXMLToDistanceTable.h FromGoogleMapXMLToDistanceTable.cpp LwSaxParser.h LwSaxParser.cpp
	$(PP) -c $(CFLAGS) $(INCLUDEFLAGS) FromGoogleMapXMLToDistanceTable.cpp -o FromGoogleMapXMLToDistanceTable.o

SortVisits.o : SortVisits.h SortVisits.cpp Address.h
	$(PP) -c $(CFLAGS) $(INCLUDEFLAGS) SortVisits.cpp -o SortVisits.o	

Address.o : Address.h Address.cpp
	$(PP) -c $(CFLAGS) $(INCLUDEFLAGS) Address.cpp -o Address.o

triBulle.o : triBulle.h triBulle.cpp
	$(PP) -c $(CFLAGS) $(INCLUDEFLAGS) triBulle.cpp -o triBulle.o

tsp.o : tsp.h tsp.cpp
	$(PP) -c $(CFLAGS) $(INCLIDEFLAGS) tsp.cpp -o tsp.o

kruskal.o : kruskal.h kruskal.cpp
	$(PP) -c $(CFLAGS) $(INCLUDEFLAGS) kruskal.cpp -o kruskal.o

client.o : client.h client.c
	$(CC) -c $(FLAGS) $(INCLUDEFLAGS) client.c -o client.o
	
server.o : server.c FromXMLToGoogleMapHTTPRequest.h
	$(CC) -c $(FLAGS) $(INCLUDEFLAGS) server.c -o server.o
	
clean:
	rm -rf *o 
	rm -rf *~
	rm -rf #*

realclean: clean
	rm -rf testParsers
