#ifndef FRC_API_SIMDJSON_H
#define FRC_API_SIMDJSON_H

#include<simdjson.h>
#include<map>
#include<variant>
#include "util.h"

namespace frc_api{

struct Decode_error{
	std::vector<std::string> path;
	std::string value;
	std::string description;

	Decode_error(const char *,const char *,std::string);
	Decode_error(const char *,std::string,std::string);
};
std::ostream& operator<<(std::ostream&,Decode_error const&);

using JSON_value=simdjson::dom::element;
using JSON_object=simdjson::simdjson_result<simdjson::dom::object>;
using JSON_array=simdjson::simdjson_result<simdjson::dom::array>;

template<typename T>
std::vector<T> decode(JSON_object,std::vector<T> const*){
	throw Decode_error{"vector","object","wrong type"};
}

std::string decode2(std::string_view,std::string const*);

template<typename T>
T decode2(std::string_view a,T const*){
	//for perf, could go make all the things take string_view
	return T{std::string{a}};
}

int decode(JSON_value,int const*);
int decode(JSON_object,int const*);

bool decode(JSON_value,bool const*);
unsigned decode(JSON_value,unsigned const*);
long decode(JSON_value,long const*);
double decode(JSON_value,double const*);
std::string decode(JSON_value,std::string const*);

std::nullptr_t decode(JSON_object,std::nullptr_t const*);
std::nullptr_t decode(JSON_value,std::nullptr_t const*);
std::nullptr_t decode(std::nullptr_t,std::nullptr_t const*);

template<typename K,typename V>
std::map<K,V> decode(JSON_object,std::map<K,V> const*);

template<
	typename A,
	typename B,
	typename C,
	typename D,
	typename E,
	typename F,
	typename G
>
std::variant<A,B,C,D,E,F,G> decode(JSON_object in,std::variant<A,B,C,D,E,F,G> const*);

template<typename A,typename B,typename C,typename D,typename E,typename F,typename G>
auto decode(JSON_value in,std::tuple<A,B,C,D,E,F,G> const*);

auto as_string(auto const& x){
	std::stringstream ss;
	ss<<x;
	return ss.str();
}

template<typename K,typename V>
std::map<K,V> decode(JSON_value in,std::map<K,V> const*){
	if(in.type()!=simdjson::dom::element_type::OBJECT){
		throw Decode_error{"std::map",as_string(in),"wrong type"};
	}
	auto obj=in.get_object();
	std::map<K,V> r;
	for(auto [k,v]:obj){
		auto k1=decode2(k,(K*)nullptr);
		auto v1=decode(v,(V*)nullptr);
		r[k1]=v1;
	}
	return r;
}

template<typename A,typename B>
std::pair<A,B> decode(JSON_value in,std::pair<A,B> const*){
	if(in.type()!=simdjson::dom::element_type::ARRAY){
		throw Decode_error{"std::pair",as_string(in),"wrong type, expected array"};
	}
	auto a=in.get_array();
	return std::make_pair(
		decode(a.at(0),(A*)0),
		decode(a.at(1),(B*)0)
	);
}

template<
	typename A,
	typename B,
	typename C,
	typename D,
	typename E
>
std::variant<A,B,C,D,E> decode(JSON_value in,std::variant<A,B,C,D,E> const*){
	#define X(NAME) try{\
		return decode(in,(NAME*)nullptr);\
	}catch(...){\
	}
	X(A) X(B) X(C) X(D) 
	#undef X
	return decode(in,(E*)nullptr);
}

template<
	typename A,
	typename B,
	typename C,
	typename D,
	typename E,
	typename F
>
std::variant<A,B,C,D,E,F> decode(JSON_value in,std::variant<A,B,C,D,E,F> const*){
	#define X(NAME) try{\
		return decode(in,(NAME*)nullptr);\
	}catch(...){\
	}
	X(A) X(B) X(C) X(D) X(E) 
	#undef X
	return decode(in,(F*)nullptr);
}

template<
	typename A,
	typename B,
	typename C,
	typename D,
	typename E,
	typename F,
	typename G
>
std::variant<A,B,C,D,E,F,G> decode(JSON_object in,std::variant<A,B,C,D,E,F,G> const*){
	#define X(NAME) try{\
		return decode(in,(NAME*)nullptr);\
	}catch(...){\
	}
	X(A) X(B) X(C) X(D) X(E) X(F) 
	#undef X
	return decode(in,(G*)nullptr);
}

template<
	typename A,
	typename B,
	typename C,
	typename D,
	typename E,
	typename F,
	typename G,
	typename H
>
std::variant<A,B,C,D,E,F,G,H> decode(JSON_object in,std::variant<A,B,C,D,E,F,G,H> const*){
	#define X(NAME) try{\
		return decode(in,(NAME*)nullptr);\
	}catch(...){\
	}
	X(A) X(B) X(C) X(D) X(E) X(F) X(G)
	#undef X
	return decode(in,(H*)nullptr);
}

/*template<typename ...Ts>
std::variant<Ts...> decode(JSON_object,std::variant<Ts...> const*){
	TBA_PRINT(sizeof...(Ts));
	TBA_NYI
}*/

template<typename ...Ts>
std::variant<Ts...> decode(JSON_array,std::variant<Ts...> const*){
	assert(0);//TBA_NYI
}

template<typename ...Ts>
std::variant<Ts...> decode(JSON_value,std::variant<Ts...> const*){
	FRC_API_PRINT(sizeof...(Ts))
	FRC_API_NYI
}

template<typename A,typename B>
std::variant<A,B> decode(JSON_value in,std::variant<A,B> const*){
	try{
		return decode(in,(A*)0);
	}catch(...){}
	return decode(in,(B*)0);
}

template<typename T>
std::vector<T> decode(JSON_value in,std::vector<T> const*);

template<typename A,typename B,typename C>
auto decode(JSON_value in,std::tuple<A,B,C> const*){
	if(in.type()!=simdjson::dom::element_type::ARRAY){
		throw Decode_error{"tuple<A,B,C>",as_string(in),"expected array"};
	}
	auto a=in.get_array();
	assert(a.size()==3);
	return std::tuple<A,B,C>{
		decode(a.at(0),(A*)nullptr),
		decode(a.at(1),(B*)nullptr),
		decode(a.at(2),(C*)nullptr)
	};
}

template<typename A,typename B,typename C,typename D,typename E,typename F,typename G>
auto decode(JSON_value in,std::tuple<A,B,C,D,E,F,G> const*){
	if(in.type()!=simdjson::dom::element_type::ARRAY){
		throw Decode_error{"tuple",as_string(in),"expected array"};
	}
	auto a=in.get_array();
	assert(a.size()==7);
	return std::tuple<A,B,C,D,E,F,G>{
		decode(a.at(0),(A*)nullptr),
		decode(a.at(1),(B*)nullptr),
		decode(a.at(2),(C*)nullptr),
		decode(a.at(2),(D*)nullptr),
		decode(a.at(2),(E*)nullptr),
		decode(a.at(2),(F*)nullptr),
		decode(a.at(2),(G*)nullptr)
	};
}

template<typename ...Ts>
std::tuple<Ts...> decode(JSON_value,std::tuple<Ts...> const*){
	assert(0);
	/*TBA_PRINT(sizeof...(Ts));
	TBA_NYI*/
}

template<typename T,size_t N>
std::array<T,N> decode(JSON_value,std::array<T,N> const*){
	assert(0);
}

template<typename T>
std::optional<T> decode(JSON_value in,std::optional<T> const*);

template<typename T>
std::optional<T> decode(std::nullptr_t,std::optional<T> const*){
	return std::nullopt;
}

template<typename T>
std::vector<T> decode(std::nullptr_t,std::vector<T> const*){
	throw Decode_error{"vector","null","wrong type"};
}

template<
	typename A,typename B,typename C,typename D,
	typename E,typename F,typename G,typename H
>
std::variant<A,B,C,D,E,F,G,H> decode(std::nullptr_t x,std::variant<A,B,C,D, E,F,G,H> const*){
	#define X(N) try{ return decode(x,(N*)nullptr); }catch(...){}
	X(A) X(B) X(C) X(D)
	X(E) X(F) X(G)
	#undef X
	return decode(x,(H*)nullptr);
}

/*template<typename ...Ts>
std::variant<Ts...> decode(std::nullptr_t,std::variant<Ts...> const*){
	TBA_PRINT(sizeof...(Ts));
	//throw Decode_error{"variant<...>","nullptr","wrong type
	TBA_NYI
}*/

template<typename T>
std::optional<T> decode(JSON_object in,std::optional<T> const*){
	try{
		return decode(in,(T*)nullptr);
	}catch(...){
		if(in.size()==0){
			return std::nullopt;
		}
		throw;
	}
}

template<typename T>
std::optional<std::vector<T>> decode(JSON_array in,std::optional<std::vector<T>> const*){
	return decode(in,(std::vector<T> const*)nullptr);
}

template<typename T>
std::optional<T> decode(JSON_array in,std::optional<T> const*){
	//if we got an array, then it is not null!
	return decode(in,(T*)0);
}

template<typename T>
std::optional<T> decode(JSON_value in,std::optional<T> const*){
	if(in.is_null()){
		return std::nullopt;
	}
	try{
		return decode(in,(T*)nullptr);
	}catch(...){
		if(in.type()==simdjson::dom::element_type::OBJECT){
			auto obj=in.get_object();
			if(obj.size()==0){
				return std::nullopt;
			}
		}
		throw;
	}
}

template<typename T>
std::vector<T> decode(JSON_array a,std::vector<T> const*){
	std::vector<T> r;
	for(auto elem:a){
		r.push_back(decode(elem,(T*)nullptr));
	}
	return r;
}

template<typename T>
std::vector<T> decode(JSON_value in,std::vector<T> const*){
	if(in.type()!=simdjson::dom::element_type::ARRAY){
		std::stringstream ss;
		ss<<in;
		throw Decode_error("std::vector<T>",ss.str(),"not an array");
	}
	return decode(in.get_array(),(std::vector<T> const*)nullptr);
}

}

#endif
