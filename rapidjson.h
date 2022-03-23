#ifndef FRC_API_RAPID_JSON_H
#define FRC_API_RAPID_JSON_H

#include<iostream>
#include<optional>
#include<vector>
#include<typeinfo>
#include<variant>
#include<map>
#include<sstream>
#include<algorithm>
#include "rapidjson/document.h"

namespace frc_api{

using JSON=rapidjson::GenericValue<rapidjson::UTF8<>>;

struct Decode_error{
	std::vector<std::string> path;
	std::string value;
	std::string description;

	Decode_error(const char *,const char *,std::string);
	Decode_error(const char *,JSON const&,std::string);
	Decode_error(std::vector<std::string>,JSON const&,std::string);
};
std::ostream& operator<<(std::ostream&,Decode_error const&);

std::ostream& operator<<(std::ostream&,rapidjson::GenericValue<rapidjson::UTF8<>> const&);

template<typename A,typename B,typename C,typename D>
std::tuple<A,B,C,D> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::tuple<A,B,C,D> *x);

template<typename T>
std::optional<T> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::optional<T>*);

std::string decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::string*);
bool decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const bool*);
int decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const int*);
unsigned int decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const unsigned int*);
int64_t decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const int64_t *);
double decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const double*);
std::nullptr_t decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::nullptr_t*);

template<typename T>
std::variant<T,Decode_error> maybe_decode(JSON const& in,const T *t){
	try{
		return decode(in,t);
	}catch(Decode_error const& e){
		return e;
	}
}

template<typename A>
std::variant<A> decode(JSON const& in,const std::variant<A> *){
	return decode(in,(A*)nullptr);
}

template<typename A,typename B>
std::variant<A,B> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::variant<A,B> *x){
	auto va=maybe_decode(in,(A*)nullptr);
	auto vb=maybe_decode(in,(B*)nullptr);
	auto a=std::holds_alternative<A>(va);
	auto b=std::holds_alternative<B>(vb);
	if(a && !b) return std::get<A>(va);
	if(!a && b) return std::get<B>(vb);
	if(!a && !b){
		throw Decode_error{
			{typeid(x).name()},
			in,
			[&](){
				std::ostringstream ss;
				ss<<"No valid options: "<<std::get<Decode_error>(va)<<" "<<std::get<Decode_error>(vb);
				return ss.str();
			}()
		};
	}
	throw Decode_error{typeid(x).name(),in,"Multiple valid options"};
}

template<typename A,typename B,typename C>
std::variant<A,B,C> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::variant<A,B,C>*){
	try{
		return decode(in,(A*)nullptr);
	}catch(...){
	}
	try{
		return decode(in,(B*)nullptr);
	}catch(...){
	}
	return decode(in,(C*)nullptr);
}

template<typename A,typename B,typename C,typename D>
std::variant<A,B,C,D> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::variant<A,B,C,D>*){
	try{
		return decode(in,(A*)nullptr);
	}catch(...){
	}
	try{
		return decode(in,(B*)nullptr);
	}catch(...){
	}
	try{
		return decode(in,(C*)nullptr);
	}catch(...){
	}
	return decode(in,(D*)nullptr);
}


template<typename A,typename B,typename C,typename D,typename E>
std::variant<A,B,C,D,E> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::variant<A,B,C,D,E>*){
	try{
		return decode(in,(A*)nullptr);
	}catch(...){
	}
	try{
		return decode(in,(B*)nullptr);
	}catch(...){
	}
	try{
		return decode(in,(C*)nullptr);
	}catch(...){
	}
	try{
		return decode(in,(D*)nullptr);
	}catch(...){
	}
	return decode(in,(E*)nullptr);
}


template<typename A,typename B>
std::map<A,B> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::map<A,B> *x){
	if(!in.IsObject()){
		throw Decode_error{typeid(x).name(),in,"expected object"};
	}
	std::map<A,B> r;

	std::transform(
		in.MemberBegin(),
		in.MemberEnd(),
		std::inserter(r,r.end()),
		[](auto const& item){
			return std::make_pair(
				decode(item.name,(const A*)nullptr),
				decode(item.value,(const B*)nullptr)
			);
		}
	);
	return r;
}

template<typename T>
std::vector<T> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::vector<T> *x){
	if(!in.IsArray()){
		throw Decode_error{typeid(x).name(),in,"expected array"};
	}
	std::vector<T> out;
	std::transform(
		in.Begin(),
		in.End(),
		std::back_inserter(out),
		[](auto const& x){ return decode(x,(T*)nullptr); }
	);
	return out;
}

template<typename T,size_t LEN>
std::array<T,LEN> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::array<T,LEN> *x){
	if(!in.IsArray()) throw Decode_error{typeid(x).name(),in,"expected array"};
	if(in.Size()!=LEN){
		std::ostringstream ss;
		ss<<"wrong length: expected "<<LEN<<" but found "<<in.Size();
		throw Decode_error{typeid(x).name(),in,ss.str()};
	}

	std::array<T,LEN> r;
	std::transform(in.Begin(),in.End(),std::begin(r),[](auto const& x){
		try{
			return decode(x,(T*)nullptr);
		}catch(Decode_error e){
			e.path.push_back(typeid(x).name());
			throw e;
		}
	});
	std::cout<<r<<"\n";
	return r;
}

template<typename T>
std::optional<T> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::optional<T>*){
	if(in.IsNull()){
		return {};
	}
	return decode(in,(const T*)nullptr);
}

template<typename A,typename B>
std::tuple<A,B> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::tuple<A,B> *x){
	if(!in.IsArray()){
		throw Decode_error{typeid(x).name(),in,"expected array"};
	}
	if(in.Size()!=2){
		std::ostringstream ss;
		ss<<"Decoding 2-tuple but found length "<<in.Size();
		throw Decode_error{typeid(x).name(),in,ss.str()};
	}
	return {
		decode(in[0],(A*)nullptr),
		decode(in[1],(B*)nullptr)
	};
}

template<typename A,typename B,typename C>
std::tuple<A,B,C> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::tuple<A,B,C> *x){
	if(!in.IsArray()){
		throw Decode_error{typeid(x).name(),in,"expected array"};
	}
	if(in.Size()!=3){
		std::ostringstream ss;
		ss<<"Decoding 3-tuple but found length "<<in.Size();
		throw Decode_error{typeid(x).name(),in,ss.str()};
	}
	return {
		decode(in[0],(A*)nullptr),
		decode(in[1],(B*)nullptr),
		decode(in[2],(C*)nullptr)
	};
}

template<typename A,typename B,typename C,typename D>
std::tuple<A,B,C,D> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::tuple<A,B,C,D> *x){
	if(!in.IsArray()){
		throw Decode_error{typeid(x).name(),in,"expected array"};
	}
	if(in.Size()!=4){
		std::ostringstream ss;
		ss<<"Decoding 4-tuple but found length "<<in.Size();
		throw Decode_error{typeid(x).name(),in,ss.str()};
	}
	return {
		decode(in[0],(A*)nullptr),
		decode(in[1],(B*)nullptr),
		decode(in[2],(C*)nullptr),
		decode(in[3],(D*)nullptr)
	};
}

template<typename A,typename B,typename C,typename D,typename E>
std::tuple<A,B,C,D,E> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::tuple<A,B,C,D,E> *x){
	if(!in.IsArray()){
		throw Decode_error{typeid(x).name(),in,"expected array"};
	}
	if(in.Size()!=5){
		std::ostringstream ss;
		ss<<"Decoding 5-tuple but found length "<<in.Size();
		throw Decode_error{typeid(x).name(),in,ss.str()};
	}
	return {
		decode(in[0],(A*)nullptr),
		decode(in[1],(B*)nullptr),
		decode(in[2],(C*)nullptr),
		decode(in[3],(D*)nullptr),
		decode(in[4],(E*)nullptr)
	};
}

template<typename A,typename B,typename C,typename D,typename E,typename F,typename G>
std::tuple<A,B,C,D,E,F,G> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::tuple<A,B,C,D,E,F,G> *x){
	if(!in.IsArray()){
		throw Decode_error{typeid(x).name(),in,"expected array"};
	}
	if(in.Size()!=7){
		std::ostringstream ss;
		ss<<"Decoding 7-tuple but found length "<<in.Size();
		throw Decode_error{typeid(x).name(),in,ss.str()};
	}
	return {
		decode(in[0],(A*)nullptr),
		decode(in[1],(B*)nullptr),
		decode(in[2],(C*)nullptr),
		decode(in[3],(D*)nullptr),
		decode(in[4],(E*)nullptr),
		decode(in[5],(F*)nullptr),
		decode(in[6],(G*)nullptr)
	};
}

template<typename A,typename B>
std::pair<A,B> decode(rapidjson::GenericValue<rapidjson::UTF8<>> const& in,const std::pair<A,B> *x){
	if(!in.IsArray()){
		throw Decode_error{typeid(x).name(),in,"expected array"};
	}
	if(in.Size()!=2){
		std::ostringstream ss;
		ss<<"Decoding pair but found length "<<in.Size();
		throw Decode_error{typeid(x).name(),in,ss.str()};
	}
	return {
		decode(in[0],(A*)nullptr),
		decode(in[1],(B*)nullptr)
	};
}

template<typename T>
std::string to_string(T const& a){
	std::ostringstream ss;
	ss<<a;
	return ss.str();
}

}

#endif
