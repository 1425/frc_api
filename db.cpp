#include "db.h"
#include<sstream>
#include<cassert>
#include<algorithm>
#include "curl.h"
#include "util.h"

namespace frc_api{

static int callback2(void *data,int arg,char **argv,char **azColname);

Sqlite::Sqlite(const char *filename){
	int rc=sqlite3_open(filename,&db);
	if(rc){
		std::ostringstream ss;
		ss<<"Could not open database: "<<filename;
		throw std::runtime_error{ss.str()};
	}
}

Sqlite::~Sqlite(){
	sqlite3_close(db);
}

Sqlite::operator sqlite3 *()const{ return db; }

std::vector<Row> Sqlite::query(std::string const& s){
	std::vector<Row> r;
	char *err_msg;
	int rc=sqlite3_exec(db,s.c_str(),callback2,&r,&err_msg);
	if(rc!=SQLITE_OK){
		std::ostringstream ss;
		ss<<"SQL error:"<<err_msg<<" tried running: "<<s;
		sqlite3_free(err_msg);
		throw std::runtime_error{ss.str()};
	}
	return r;
}

static int callback2(void *data,int argc,char **argv,char **azColname){
	std::vector<Row> &r=*(std::vector<Row>*)data;
	Row row;
	for(int i=0;i<argc;i++){
		row.push_back(std::make_pair(azColname[i],argv[i]));
	}
	r.push_back(row);
	return 0;
}

using Date=std::string;

std::pair<std::optional<Date>,Data> Fetcher::fetch(URL url){
	auto g=get_url(
		url,
		{
			"Authorization: Basic "+authorization,
			"Accept: application/json",
			"If-Modified-Since;"//: Mon, 1 Jan 1900 2:00:00 GMT"
		}
	);

	auto f=std::find_if(g.headers.begin(),g.headers.end(),[](auto const& p){ return p.first=="Last-Modified"; });

	if(f==g.headers.end()){
		/*PRINT(g);
		std::ostringstream ss;
		ss<<"fetching:"<<url<<"\n";
		ss<<"got:"<<g.headers<<"\n";
		ss<<"Error: Did not find Last-Modified header";
		throw std::runtime_error{ss.str()};*/
		return make_pair(std::optional<HTTP_Date>{},g.data);
	}

	return make_pair(f->second,g.data);
}

Cache_policy::Cache_policy(Type type1):type_(type1){}

Cache_policy::Type Cache_policy::type()const{ return type_; }

HTTP_Date const& Cache_policy::date()const{
	assert(type()==Type::DATE);
	return date_;
}

Cache_policy Cache_policy::any(){ return Cache_policy{Type::ANY}; }

Cache_policy Cache_policy::date(Date date){
	Cache_policy r{Type::DATE};
	r.date_=date;
	return r;
}

Cache_policy Cache_policy::none(){ return Cache_policy{Type::NONE}; }

Cache::Cache(const char *path):db(path),policy(Cache_policy::any()){
	db.query("CREATE TABLE IF NOT EXISTS cache (url STRING UNIQUE NOT NULL,date INT NOT NULL,body VARCHAR NOT NULL);");
}

Cache::Cache():Cache("cache.db"){}

void escape(std::ostream& o,std::string const& s){
	std::for_each(begin(s),end(s),[&](char c){
		if(c=='\''){
			o<<"''";
		}else{
			o<<c;
		}
	});
}
			
void Cache::add(URL url,std::pair<std::optional<HTTP_Date>,Data> p){
	auto date=p.first;
	auto body=p.second;
	std::ostringstream ss;
	ss<<"INSERT INTO cache VALUES (\'"<<url<<"\',\'"<<date<<"\',\'";
	escape(ss,body);
	ss<<"\')";
	db.query(ss.str());
}

struct Mutex_lock{
	std::mutex &mutex;

	Mutex_lock(std::mutex &m):mutex(m){
		m.lock();
	}

	~Mutex_lock(){
		mutex.unlock();
	}
};

std::optional<std::pair<std::optional<HTTP_Date>,Data>> Cache::fetch(URL url){
	switch(policy.type()){
		case Cache_policy::Type::NONE:
			return {};
		case Cache_policy::Type::ANY:
			break;
		default:
			FRC_API_NYI
	}
	Mutex_lock lock(mutex);
	std::ostringstream ss;
	ss<<"SELECT date,body FROM cache WHERE url=\'"<<url<<"\'";
	auto r=db.query(ss.str());
	if(r.size()==0){
		return {};
	}
	if(r.size()==1){
		auto x=r[0];
		assert(x.size()==2);
		auto date=x[0].second;
		auto body=x[1].second;
		//cout<<"date:"<<date<<"\n";
		//cout<<"body:"<<body<<"\n";
		assert(date);
		assert(body);
		return make_pair(*date,*body);
	}
	assert(0);FRC_API_NYI
}

std::pair<std::optional<HTTP_Date>,Data> Cached_fetcher::fetch(URL url){
	auto c=cache.fetch(url);
	if(c) return *c;
	auto f=fetcher.fetch(url);
	cache.add(url,f);
	return f;
}

Nonempty_string::Nonempty_string(std::string s1):s(move(s1)){}

std::string const& Nonempty_string::str()const{
	return s;
}

std::string operator+(const char *s1,Nonempty_string const& s2){
	return s1+s2.str();
}

}

