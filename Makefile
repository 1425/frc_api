CXXFLAGS = -std=c++20 -Wall -Wextra -O2
LIBS = -lcurl -lsqlite3 -lsimdjson

all: fetch

fetch: curl.o query.o data.o fetch.o db.o util.o simdjson.o
	$(CXX) $^ $(LIBS) -o $@

.PHONY:clean
clean:
	rm -f *.o fetch
