#ifndef FRC_API_DB_H
#define FRC_API_DB_H

#include<iostream>
#include<optional>
#include<vector>
#include<mutex>
#include<sqlite3.h>

namespace frc_api{

using Cell=std::pair<std::string,std::optional<std::string>>;
using Row=std::vector<Cell>;

class Sqlite{
	sqlite3 *db;

	public:
	explicit Sqlite(const char *filename);
	Sqlite(Sqlite const&)=delete;
	Sqlite& operator=(Sqlite const&)=delete;
	~Sqlite();

	operator sqlite3 *()const;
	std::vector<Row> query(std::string const& s);
};

using HTTP_Date=std::string;
using Data=std::string;
using URL=std::string;

class Nonempty_string{
	std::string s;

	public:
	explicit Nonempty_string(std::string);

	std::string const& str()const;
};

std::string operator+(const char*,Nonempty_string const&);

struct Fetcher{
	Nonempty_string authorization;

	std::pair<std::optional<HTTP_Date>,Data> fetch(URL);
};

class Cache_policy{
	public:
	enum class Type{ANY,DATE,NONE};

	private:
	Type type_;
	HTTP_Date date_;

	Cache_policy(Type);

	public:
	Type type()const;
	HTTP_Date const& date()const;

	static Cache_policy any();
	static Cache_policy date(HTTP_Date);
	static Cache_policy none();
};

std::ostream& operator<<(std::ostream&,Cache_policy const&);

class Cache{
	Sqlite db;
	std::mutex mutex;

	public:
	Cache_policy policy;

	Cache();
	explicit Cache(const char*);

	void add(URL,std::pair<std::optional<HTTP_Date>,Data>);
	std::optional<std::pair<std::optional<HTTP_Date>,Data>> fetch(URL url);
};

struct Cached_fetcher{
	Fetcher fetcher;
	Cache cache;

	std::pair<std::optional<HTTP_Date>,Data> fetch(URL url);
};

}

#endif
