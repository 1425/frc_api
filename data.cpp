#include "data.h"
#include<iostream>
#include<cassert>
#include "util.h"

#define nyi FRC_API_NYI

namespace frc_api{

std::ostream& operator<<(std::ostream&,R0_2)nyi

R0_2::R0_2(int i1):i(i1){
	if(i<0 || i>2){
		throw std::invalid_argument("R0_2");
	}
}

R0_2 example(const R0_2*)nyi
R0_2 rand(const R0_2*)nyi

R0_2 decode(JSON_value in,const R0_2*){
	return R0_2{decode(in,(int*)0)};
}

bool alphanumeric(char c){
	return isalpha(c) || isdigit(c);
}

String2::String2(std::string a):s(std::move(a)){
	if(s.size()<2) throw std::invalid_argument("String2");
	if(!all_of(begin(a),end(a),alphanumeric)){
		throw std::invalid_argument{"String2"};
	}
}

std::string const& String2::get()const{ return s; }

bool operator==(String2 const& a,const char *b){
	return a.get()==b;
}

bool operator!=(String2 const& a,const char *b){
	return a.get()!=b;
}

String2 example(const String2*){
	nyi
}

char rand_char(){
	return 'a'+(::rand()%26);
}

String2 rand(const String2*){
	std::ostringstream ss;
	ss<<rand_char()<<rand_char();
	return String2{ss.str()};
}

std::ostream& operator<<(std::ostream& o,String2 const& a){
	return o<<a.get();
}

String2 decode(JSON_value in,const String2*){
	return String2{decode(in,(std::string*)nullptr)};
}

Team_number decode(JSON_value in,const Team_number*){
	return Team_number{decode(in,(int*)nullptr)};
}

Event_code::Event_code(String2 a):data(std::move(a)){}

Event_code::Event_code(const char *s):data(s){}

Event_code::operator String2()const{
	return data;
}

Event_code example(Event_code const*){
	nyi
}

Event_code rand(Event_code const*){
	nyi
}

Event_code decode(JSON_value in,Event_code const*){
	return Event_code{decode(in,(String2*)0)};
}

District_code::District_code(String2 a):data(std::move(a)){}

District_code::District_code(const char* a):data(a){}

District_code::operator String2()const{
	return data;
}

District_code decode(JSON_value in,District_code const*){
	return District_code{decode(in,(String2*)0)};
}

District_code rand(District_code const*){
	nyi
}

District_code example(District_code const*){
	nyi
}

#define PRINT_ITEM(A) if(a==decltype(a)::A) return o<<""#A;

#define ITEM_LIST(A) NAME::A,

#define DECODE(A) if(s==""#A) return NAME::A;

#define STR(A) (""#A)

#define IMPL_ENUM(ITEMS)\
	std::ostream& operator<<(std::ostream& o,NAME a){\
		ITEMS(PRINT_ITEM)\
		assert(0);\
	}\
	NAME example(const NAME*){\
		std::vector<NAME> options{ITEMS(ITEM_LIST)};\
		return options.at(0);\
	}\
	NAME rand(const NAME*){\
		std::vector<NAME> options{ITEMS(ITEM_LIST)};\
		return options[::rand()%options.size()];\
	}\
	NAME decode(JSON_value in,NAME const* x){\
		auto s=decode(in,(std::string*)nullptr);\
		ITEMS(DECODE)\
		throw Decode_error{typeid(*x).name(),as_string(in),"unexpected value"};\
	}\
	NAME decode(JSON_array,NAME const*){\
		std::cout<<"e2\n";\
		FRC_API_NYI\
	}\
	NAME decode(JSON_object,NAME const*){\
		std::cout<<"e3\n";\
		FRC_API_NYI\
	}\

#define NAME Tournament_level
IMPL_ENUM(FRC_API_TOURNAMENT_LEVEL)
#undef NAME

#define NAME Award_Event_type
IMPL_ENUM(FRC_API_AWARD_EVENT_TYPES)
#undef NAME

#define NAME Station
IMPL_ENUM(FRC_API_STATION)
#undef NAME

#define NAME Field
IMPL_ENUM(FRC_API_FIELD)
#undef NAME

#define NAME Event_Event_type
IMPL_ENUM(FRC_API_EVENT_EVENT_TYPES)
#undef NAME

#define NAME Alliance_color
IMPL_ENUM(FRC_API_ALLIANCE_COLOR)
#undef NAME

#define NAME Auto_2017
IMPL_ENUM(FRC_API_AUTO_2017)
#undef NAME

#define NAME Touchpad
IMPL_ENUM(FRC_API_TOUCHPAD)
#undef NAME

#define NAME Auto_2016
IMPL_ENUM(FRC_API_AUTO_2016)
#undef NAME

#define NAME Tower_face
IMPL_ENUM(FRC_API_TOWER_FACE)
#undef NAME

}
