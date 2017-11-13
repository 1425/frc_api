CXX = g++-7
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LIBS = -lcurl -lsqlite3

all: fetch

fetch: curl.o query.o data.o fetch.o db.o rapidjson.o util.o
	$(CXX) $^ $(LIBS) -o $@

.PHONY:clean
clean:
	rm -f *.o fetch
