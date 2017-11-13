CXX = g++-7
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LIBS = -lcurl -lsqlite3

all: fetch

query: data.o

fetch: curl.o query.o data.o fetch.o db.o rapidjson.o util.o
	$(CXX) $^ $(LIBS) -o $@

.PHONY:clean
clean:
	rm -f query *.o fetch
